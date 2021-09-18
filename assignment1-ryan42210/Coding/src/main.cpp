#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

#include "../inc/camera.h"
#include "../inc/loadmesh.h"
#include "../inc/loadmesh_m.h"
#include "../inc/shader_m.h"

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 700;
// camera
glm::vec3 pos(0.0f, 0.0f, 8.0f);
Camera cam(pos);

// mouse control
float prev_x = SCREEN_WIDTH/2;
float prev_y = SCREEN_HEIGHT/2;
bool first_load = true;

// time control
float lastframe = 0.0;
float delt = 0.0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double x, double y);
void processInput(GLFWwindow* window);


int main(){

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// MSAA
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT,"helloworld",NULL,NULL);
	if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
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
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	// configure global opengl state
	// -----------------------------
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);

	// Mesh Bunny = loadmesh("../../resources/cube.object");
	Mesh_m Bunny = loadmesh_m("../../resources/bunny_simlified.object");
	// Mesh Pumpkin = loadmesh("../../resources/pumpkin_simlified.object");

	Shader shader("../../src/shader_vtx.vs", "../../src/shader_frag.fs");

	unsigned int VBO_Bunny, VAO_Bunny, EBO_Bunny;
	glGenBuffers(1, &EBO_Bunny);
	glGenBuffers(1, &VBO_Bunny);
	// -------------------VAO of Bunny--------------------//
	glGenVertexArrays(1, &VAO_Bunny);
	glBindVertexArray(VAO_Bunny);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Bunny);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*Bunny.vertexAttrib.size(),
				Bunny.vertexAttrib.data(), GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_Bunny);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Bunny.indices.size(),
				Bunny.indices.data(), GL_STATIC_DRAW);

	// vertex attribute: position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
	glEnableVertexAttribArray(0);
	// vertex attribute: normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(sizeof(glm::vec3)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glBindVertexArray(0);
	// ------------------VAO of Bunny---------------------//

	// ------------------VAO of Pumpkin-------------------//
	// unsigned int VBO_Pumpkin, VAO_Pumpkin, EBO_Pumpkin;
	// glGenBuffers(1, &EBO_Pumpkin);
	// glGenBuffers(1, &VBO_Pumpkin);

	// glGenVertexArrays(1, &VAO_Pumpkin);
	// glBindVertexArray(VAO_Pumpkin);

	// glBindBuffer(GL_ARRAY_BUFFER, VBO_Pumpkin);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(float) * Pumpkin.vertexAttrib.size(),
	// 			&Pumpkin.vertexAttrib.front(), GL_STATIC_DRAW);
	
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_Pumpkin);
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Pumpkin.indices.size(),
	// 			&Pumpkin.indices.front(), GL_STATIC_DRAW);

	// // vertex attribute: position
	// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void *)0);
	// glEnableVertexAttribArray(0);
	// // vertex attribute: normal
	// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void *)(3*sizeof(float)));
	// glEnableVertexAttribArray(1);

	// glBindBuffer(GL_ARRAY_BUFFER, 0);

	// glBindVertexArray(0);
	// ------------------VAO of Pumpkin-------------------//

	//polygon mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window)) {
		// time data
		float currframe = glfwGetTime();
		delt = currframe - lastframe;
		lastframe = currframe;
		
		// input
		// -----
		processInput(window);

		// rendering command
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// activate shader program
		shader.use();

		glm::mat4 view;
		view = cam.getViewMat();
		// view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));
		glm::mat4 proj = glm::perspective(glm::radians(50.0f), (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT, 0.1f, 100.0f);

		glm::vec3 lightPos1_world = glm::vec3(5.0f, 7.0f, 8.0f);
		glm::vec3 lightPos2_world = glm::vec3(-3.0f, 2.0f, -3.0f);
		
		// set 'uniform'
		shader.setMat4("view", view);
		shader.setMat4("proj", proj);
		shader.setVec3("lightPos1_world", lightPos1_world);
		shader.setVec3("lightPos2_world", lightPos2_world);
		// draw Bunny
		glm::mat4 model_bunny;
		shader.setMat4("model", model_bunny);
		glBindVertexArray(VAO_Bunny);
		glDrawElements(GL_TRIANGLES, Bunny.f_num*3, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		// draw Pumpkin
		// glm::mat4 model_pumpkin = glm::mat4(1.0f);
		// model_pumpkin = glm::translate(model_pumpkin, glm::vec3(0.0, -1.5, 0.7));
		// model_pumpkin = glm::scale(model_pumpkin, glm::vec3(0.8, 0.8, 0.8));
		// shader.setMat4("model", model_pumpkin);
		// glBindVertexArray(VAO_Pumpkin);
		// glDrawElements(GL_TRIANGLES, Pumpkin.f_num*3, GL_UNSIGNED_INT, 0);
		// glBindVertexArray(0);

		// check evens and swap buffer
		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window) {
	float camSpeed = 3;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cam.moveUp(camSpeed * delt);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cam.moveDown(camSpeed * delt);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cam.moveLeft(camSpeed * delt);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cam.moveRight(camSpeed * delt);
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		cam.moveFront(camSpeed * delt);
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		cam.moveBack(camSpeed * delt);
	// if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	// 	cam.yawLeft(camSpeed * delt);
	// if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	// 	cam.yawRight(camSpeed * delt);
	
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void mouse_callback(GLFWwindow* window, double x, double y) {
	
	if (first_load) {
		prev_x = x;
		prev_y = y;
		first_load = false;
	}

	double speed = 0.05;

	double delt_x = (x - prev_x)*speed;
	double delt_y = (prev_y - y)*speed;
	prev_x = x, prev_y = y;

	cam.rotate(delt_x, delt_y);

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}