#include "SurfaceRender.hpp"
#include "Texture.hpp"
#include "header.hpp"
#include "../inc/camera.h"
#include "../inc/shader_m.h"
#include "../inc/nurbs_data.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 800

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double x, double y);
void processInput(GLFWwindow* window);

// camera
glm::vec3 pos(0.0f, 0.0f, 6.0f);
Camera cam(pos);

// mouse control
float prev_x = SCREEN_WIDTH/2;
float prev_y = SCREEN_HEIGHT/2;
bool first_load = true;
// time control
float lastframe = 0.0;
float delta = 0.0;

int drawChoose = 1;

// set light source position
glm::vec3 lightPos1_world = glm::vec3(10.0f, 10.0f, 8.0f);

int main() {
    glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// MSAA
	glfwWindowHint(GLFW_SAMPLES, 4);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT,"B_spline",NULL,NULL);
	if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}
	// configure global open_gl state
	// -----------------------------
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);

    Shader surface_shader("../../shader/surface.vs", "../../shader/surface.fs");

    //read given control point.(We already provide one piece of control points in dat file)
    //you can also define your own control point.
    ControlPoint m_controlPoint;
    m_controlPoint.readBinary("../../resources/controlPoint_0.dat");
    vector<vector<glm::vec3>> controlpoint = m_controlPoint.getControlPoint();

    vector<float> knots_u = {0, 0, 0, 0, 0.33, 0.66, 1, 1, 1, 1};
    vector<float> knots_v = {0, 0, 0, 0, 0.33, 0.66, 1, 1, 1, 1};
	vector<vector<float>> weight = {
		{1.0f,1.0f,1.0f,1.0f,1.0f,1.0f},
		{1.0f,1.0f,1.0f,1.0f,1.0f,1.0f},
		{1.0f,1.0f,1.0f,1.0f,1.0f,1.0f},
		{1.0f,1.0f,1.0f,1.0f,1.0f,1.0f},
		{1.0f,1.0f,1.0f,1.0f,1.0f,1.0f},
		{1.0f,1.0f,1.0f,1.0f,1.0f,1.0f}
	};
    float sampleStep = 0.01;
    MeshSur mymeshsur = MeshSur(knots_u, knots_v, sampleStep);
    mymeshsur.initialize(controlpoint, weight, 3);

    SurfaceRender myrender;
    myrender.loadSurface(mymeshsur);
    myrender.loadEdgeSurface(mymeshsur);

	MeshSur nurbs_sphere = MeshSur(kts_u, kts_v, sampleStep);
	nurbs_sphere.initialize(nurbs_cnPoint, nurbs_weight, 2);

	SurfaceRender nurbsRenderer;
	nurbsRenderer.loadSurface(nurbs_sphere);

	Texture check_tex("../../resources/checkerBox.png");
	check_tex.genTexture();
	//polygon mode
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window)) {
		// time data
		float currframe = glfwGetTime();
		delta = currframe - lastframe;
		lastframe = currframe;
		// input
		// -----
		processInput(window);

		// rendering command
		glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// enable shader

		glm::mat4 view;
		view = cam.getViewMat();
		// view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));
		glm::mat4 proj = glm::perspective(glm::radians(50.0f), (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT, 0.1f, 100.0f);

		// set 'uniform'
		surface_shader.setMat4("view", view);
		surface_shader.setMat4("proj", proj);
		surface_shader.setVec3("lightPos1_world", lightPos1_world);
		// draw 
		glm::mat4 model_surface = glm::mat4(1.0f);
		surface_shader.setMat4("model", model_surface);
		surface_shader.setInt("texture1", 0);

		if (drawChoose == 1) 
			myrender.drawEdge(surface_shader, check_tex.getTexture());
		else if (drawChoose == 2)
			myrender.draw(surface_shader, check_tex.getTexture());
		else if (drawChoose == 3)
			nurbsRenderer.draw(surface_shader, check_tex.getTexture());

		// check evens and swap buffer
		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	glfwTerminate();
	return 0;
}


void processInput(GLFWwindow* window) {
	double camSpeed = 3.0;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cam.moveUp(camSpeed * delta);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cam.moveDown(camSpeed * delta);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cam.moveLeft(camSpeed * delta);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cam.moveRight(camSpeed * delta);
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		cam.moveFront(camSpeed * delta);
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		cam.moveBack(camSpeed * delta);
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		drawChoose = 1;
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		drawChoose = 2;
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		drawChoose = 3;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void mouse_callback(GLFWwindow* window, double x, double y) {

	if (first_load) {
		prev_x = x;
		prev_y = y;
		first_load = false;
	}

	float speed = 0.05;

	float delta_x = (x - prev_x)*speed;
	float delta_y = (prev_y - y)*speed;
	prev_x = x, prev_y = y;

	cam.rotate(delta_x, delta_y);

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}