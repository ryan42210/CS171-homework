#include <iostream>
#include <string>
#include <chrono>
#include "config.hpp"
#include "camera.hpp"
#include "light.hpp"
#include "implicit_geom.hpp"
#include "volume_renderer.hpp"
#include "bbox.hpp"



Config conf;


int main(int argc, char *argv[])
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
        conf.parseConfigFile("../../configs/fw_porous_surf.txt");
		conf.printConfig();
		// std::cout << "Please specify a configuration file." << std::endl;
		// return -1;
	}

    /////////////////////////////////////////////////////////////////////////
    // Implicit geometry setting
    /////////////////////////////////////////////////////////////////////////
    
    ImplicitGeometry *geom, *geom1, *geom2, *geom3;

    GenusTwoSurface genus2_geom(Eigen::Vector3f(0, 0, 0), Eigen::Vector3f(5.0, 5.0, 5.0));
    WineGlassSurface wineglass_geom(Eigen::Vector3f(0, 0, 0), Eigen::Vector3f(5.0, 5.0, 6.0));
    PorousSurface porous_surf_geom(Eigen::Vector3f(0, 0, 0), Eigen::Vector3f(2.0, 2.0, 2.0));
    if (conf.scene_prefab == 1)
    {
        geom = &genus2_geom;
    }
    else if (conf.scene_prefab == 2)
    {
        geom = &wineglass_geom;
    }
    else if (conf.scene_prefab == 3)
    {
        geom = &porous_surf_geom;
    }
    geom1 = &genus2_geom;
    geom2 = &wineglass_geom;
    geom3 = &porous_surf_geom;
    /////////////////////////////////////////////////////////////////////////
    // Camera setting
    /////////////////////////////////////////////////////////////////////////

    AABB geom_bbox = geom->getBBox();
    Eigen::Vector3f camera_pos;
    Eigen::Vector3f camera_look_at = geom_bbox.getCenter();
    Eigen::Vector2i film_res(conf.output_resolution, conf.output_resolution);
    
    if (conf.scene_prefab == 1)
    {
        camera_pos = geom_bbox.getCenter() + Eigen::Vector3f(0.5, -5.5, 2.0);
    }
    else if (conf.scene_prefab == 2)
    {
        camera_pos = geom_bbox.getCenter() + Eigen::Vector3f(0.5, -9.5, 4.0);
    }
    else if (conf.scene_prefab == 3)
    {
        camera_pos = geom_bbox.getCenter() + Eigen::Vector3f(2.5, -2.1, 5.0);
    }
        
    Camera camera(camera_pos, 45.0, film_res);
    camera.lookAt(camera_look_at, Eigen::Vector3f(0, 0, 1));

    /////////////////////////////////////////////////////////////////////////
    // Light setting
    /////////////////////////////////////////////////////////////////////////

    Light light1(Eigen::Vector3f(0.5, -7.5, 1.0), Eigen::Vector3f(0.9, 0.9, 0.9));
    Light light2(Eigen::Vector3f(0, 0, 0), Eigen::Vector3f(0.9, 0.9, 0.9));

    /////////////////////////////////////////////////////////////////////////
    // Classifier setting
    /////////////////////////////////////////////////////////////////////////
   
    IsosurfaceClassifier classifer(0.0);

    /////////////////////////////////////////////////////////////////////////
    // Initialize and run a volume renderer
    /////////////////////////////////////////////////////////////////////////

    VolumeRenderer renderer;
    renderer.addGeometry(geom);
    // renderer.addGeometry(geom2);
    renderer.setCamera(&camera);
    renderer.addLight(&light1);
    renderer.addLight(&light2);
    renderer.setClassifier(&classifer);

    std::cout << "\n\nRendering...\n" << std::endl;

	auto start_time = std::chrono::steady_clock::now();
    
    renderer.renderFrontToBack();

    auto end_time = std::chrono::steady_clock::now();
	double time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

    /////////////////////////////////////////////////////////////////////////
    // Save the rendering result
    /////////////////////////////////////////////////////////////////////////

    camera.getFilm().writeToFile(conf.output_file);

    return 0;
}