#include <iostream>
#include <algorithm>
#include <string>
#include <map>
#include <chrono>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "geometry.hpp"
#include "material.hpp"
#include "scene.hpp"
#include "camera.hpp"
#include "integrator.hpp"
#include "utils.hpp"
#include "config.hpp"

// #define clock_t std::chrono::time_point<std::chrono::system_clock>


Config conf;


int main(int argc, char* argv[])
{
	/////////////////////////////////////////////////////////////////////////
	// Parse input arguments
	/////////////////////////////////////////////////////////////////////////

	if (argc >= 2)
	{
		if (!conf.parseConfigFile(std::string(argv[1])))
		{
			return -1;
		}
		conf.printConfig();
	}
	else
	{
		std::cout << "Please specify a configuration file." << std::endl;
		return -1;
	}

	// temp setting
	// conf.parseConfigFile(std::string("../../configs/config_default.txt"));
	// conf.printConfig();

	/////////////////////////////////////////////////////////////////////////
	// Camera settings
	/////////////////////////////////////////////////////////////////////////

	Eigen::Vector3f camera_pos;
	Eigen::Vector3f camera_look_at;
	if (conf.camera_id == 1)
	{
		// Camera setting 1
		camera_pos = Eigen::Vector3f(0, 4, 10);
		camera_look_at = Eigen::Vector3f(0, 0, 0);
	}
	else if (conf.camera_id == 2)
	{
		// Camera setting 2
		camera_pos = Eigen::Vector3f(5, 3, 12);
		camera_look_at = Eigen::Vector3f(0, 0, 0);
	}
	else if (conf.camera_id == 3)
	{
		// Camera setting 3
		camera_pos = Eigen::Vector3f (-5, -2, 15);
		camera_look_at = Eigen::Vector3f(0, 0, 0);
	}
	float focal_len = 1.0f;
	float vertical_fov = 45.0f;
	Eigen::Vector2i film_res(conf.output_resolution, conf.output_resolution);
	Camera camera(camera_pos, focal_len, vertical_fov, film_res);
	camera.lookAt(camera_look_at, Eigen::Vector3f(0, 1, 0));


	/////////////////////////////////////////////////////////////////////////
	// Material settings
	/////////////////////////////////////////////////////////////////////////

	ColorMaterial mat_grey(Eigen::Vector3f(0.7, 0.7, 0.7), 16.0);
	ColorMaterial mat_cus(Eigen::Vector3f(0.6, 0.6, 0.9), 16.0);
	ColorMaterial mat_red(Eigen::Vector3f(1.0, 0, 0), 16.0);
	ColorMaterial mat_green(Eigen::Vector3f(0, 1.0, 0), 16.0);

	Texture tex_cb1(Eigen::Vector3f(0, 0, 1.0), Eigen::Vector3f(1.0, 1.0, 1.0));
	Texture tex_cb2(Eigen::Vector3f(0.7, 0, 1.0), Eigen::Vector3f(0.9, 0.0, 0.4), 5);
	Texture tex_cb3(Eigen::Vector3f(1.0, 1.0, 1.0), Eigen::Vector3f(0, 0, 0), 8);
	Texture tex_cb4(Eigen::Vector3f(0.3, 1.0, 0.4), Eigen::Vector3f(0.3, 0.7, 0.2));

	Texture tex_cbmm(Eigen::Vector3f(1.0, 1.0, 1.0), Eigen::Vector3f(0, 0, 0),2, true);
	tex_cbmm.genMipMap();

	Texture tex_front("../../env_map/f.png");
	Texture tex_back("../../env_map/bk.png");
	Texture tex_left("../../env_map/l.png");
	Texture tex_right("../../env_map/r.png");
	Texture tex_top("../../env_map/t.png");
	Texture tex_bot("../../env_map/bt.png");
	
	TextureMaterial mat_cb1(&tex_cb1, 22.0);
	TextureMaterial mat_cb2(&tex_cb2, 40.0);
	TextureMaterial mat_cb3(&tex_cb3, 120.0);
	TextureMaterial mat_cb4(&tex_cb4, 16.0);
	TextureMaterial mat_cbmm(&tex_cbmm, 16.0);

	TextureMaterial mat_f(&tex_front, 1.0);
	TextureMaterial mat_bk(&tex_back, 1.0);
	TextureMaterial mat_l(&tex_left, 1.0);
	TextureMaterial mat_r(&tex_right, 1.0);
	TextureMaterial mat_t(&tex_top, 1.0);
	TextureMaterial mat_bt(&tex_bot, 1.0);

	/////////////////////////////////////////////////////////////////////////
	// Setting the Cornell box.
	/////////////////////////////////////////////////////////////////////////

	Eigen::Vector3f p_p0(-10, -10, -10);
	Eigen::Vector3f p_s0(1, 0, 0);
	Eigen::Vector3f p_s1(0, 1, 0);
	Eigen::Vector3f p_normal(0, 0, 1);
	Parallelogram back_wall(p_p0, p_s0 * 20, p_s1 * 20, p_normal, &mat_grey);

	p_p0 = Eigen::Vector3f(-6, -7, -11);
	p_s0 = Eigen::Vector3f(0, 0, 1);
	p_s1 = Eigen::Vector3f(0, 1, 0);
	p_normal = Eigen::Vector3f(1, 0, 0);
	Parallelogram left_wall(p_p0, p_s0 * 20, p_s1 * 20, p_normal, &mat_red);

	p_p0 = Eigen::Vector3f(6, -7, -11);
	p_s0 = Eigen::Vector3f(0, 0, 1);
	p_s1 = Eigen::Vector3f(0, 1, 0);
	p_normal = Eigen::Vector3f(-1, 0, 0);
	Parallelogram right_wall(p_p0, p_s0 * 20, p_s1 * 20, p_normal, &mat_green);

	p_p0 = Eigen::Vector3f(-7, -6, -11);
	p_s0 = Eigen::Vector3f(1, 0, 0);
	p_s1 = Eigen::Vector3f(0, 0, 1);
	p_normal = Eigen::Vector3f(0, 1, 0);
	Parallelogram floor(p_p0, p_s0 * 20, p_s1 * 20, p_normal, &mat_grey);

	p_p0 = Eigen::Vector3f(-7, 6, -11);
	p_s0 = Eigen::Vector3f(1, 0, 0);
	p_s1 = Eigen::Vector3f(0, 0, 1);
	p_normal = Eigen::Vector3f(0, -1, 0);
	Parallelogram ceiling(p_p0, p_s0 * 20, p_s1 * 20, p_normal, &mat_grey);

	/////////////////////////////////////////////////////////////////////////
	// Setting geometries
	/////////////////////////////////////////////////////////////////////////

	Sphere sphere1(Eigen::Vector3f(3, 1, -7), 1.5f, &mat_cb1);
	Sphere sphere2(Eigen::Vector3f(2, -3.2, -5), 1.0f, &mat_cb2);
	Sphere sphere3(Eigen::Vector3f(-4, 2, -5), 1.8f, &mat_cb3);
	Sphere sphere4(Eigen::Vector3f(-2, -4.5, -8), 2.0f, &mat_cb4);
	// sphere for environment cube
	Sphere sphere5(Eigen::Vector3f(0, -2, -7), 2.0f, &mat_grey);
	// sphere for environment cube
	Ground ground(-10, &mat_cbmm);
	/////////////////////////////////////////////////////////////////////////
	// Light setting
	/////////////////////////////////////////////////////////////////////////

	Eigen::Vector3f light_pos;
	Eigen::Vector3f light_color;

	if (conf.light_id == 1)
	{
		// Light setting 1
		light_pos = Eigen::Vector3f(0, 5.9, -6);
		light_color = Eigen::Vector3f(1.0, 1.0, 1.0);
	}
	else if (conf.light_id == 2)
	{
		// Light setting 2
		light_pos = Eigen::Vector3f(3, 5.9, -6);
		light_color = Eigen::Vector3f(0.9, 0.8, 0);
	}
	else if (conf.light_id == 3)
	{
		// Light setting 3
		light_pos = Eigen::Vector3f(-3, 5.9, -6);
		light_color = Eigen::Vector3f(0, 0.9, 0.7);
	}

	AreaLight light2(light_pos, Eigen::Vector3f(0,0,0), Eigen::Vector2f(2, 2));
	AreaLight light(light_pos, light_color, Eigen::Vector2f(2, 2));
	EnvironmentLight env_cube(20.0f, &mat_f, &mat_bk, &mat_l, &mat_r, &mat_bt, &mat_t);


	/////////////////////////////////////////////////////////////////////////
	// Scene setup
	/////////////////////////////////////////////////////////////////////////

	Scene scene(&light);
	if (conf.scene_id == 1)
	{
		scene.setAmbientLight(Eigen::Vector3f(0.1, 0.1, 0.1));
		scene.addGeometry(&back_wall);
		scene.addGeometry(&left_wall);
		scene.addGeometry(&right_wall);
		scene.addGeometry(&floor);
		scene.addGeometry(&ceiling);

		scene.addGeometry(&sphere1);
		scene.addGeometry(&sphere2);
		scene.addGeometry(&sphere3);
		scene.addGeometry(&sphere4);
	}
	else if (conf.scene_id == 2)
	{
		scene.setAmbientLight(Eigen::Vector3f(0, 0, 0));
		scene.setLight(&env_cube);
		scene.addGeometry(&sphere5);
	}
	else if (conf.scene_id == 3)
	{
		scene.setLight(&light2);
		scene.setAmbientLight(Eigen::Vector3f(0.8, 0.8, 0.8));
		scene.addGeometry(&ground);
	}
	/////////////////////////////////////////////////////////////////////////
	// Perform Phong lighting integrator
	/////////////////////////////////////////////////////////////////////////

	std::cout << "Rendering..." << std::endl;

	auto start_time = std::chrono::steady_clock::now();

	PhongLightingIntegrator integrator(&scene, &camera);
	integrator.render(conf.scene_id);

	auto end_time = std::chrono::steady_clock::now();
	double time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();


	/////////////////////////////////////////////////////////////////////////
	// Output the image to a file
	/////////////////////////////////////////////////////////////////////////

	std::string output_path = conf.output_file;
	std::vector<unsigned char> output_data;
	output_data.reserve(film_res.x() * film_res.y() * 3);

	for (Eigen::Vector3f v : camera.getFilm().pixel_array)
	{
		for (int i = 0; i < 3; i++)
		{
			output_data.push_back(gamma_correction(v[i]));
		}
	}
	stbi_flip_vertically_on_write(true);
	stbi_write_png(output_path.c_str(), film_res.x(), film_res.y(), 3, output_data.data(), 0);

	std::cout << "Time elapsed: " << time_elapsed/1000 << " s" << std::endl;
	return 0;
}