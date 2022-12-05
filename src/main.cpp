#include <utils.h>
#include <mesh.h>
#include "shader.h"
#include "controls.h"

const int WIDTH = 1920;
const int HEIGHT = 1080;

GLFWwindow* window;

#define GLDebug(expre) GLClearError();(expre);GLCheckError(#expre,__FILE__,__LINE__);
static void GLClearError()
{
	while (glGetError()!=GL_NO_ERROR);
}
static void GLCheckError(const char* functionName, const char* file,int line) {
	if (GLenum error=glGetError())
	{
		std::cout << "[OpenGL ERROR] " << error << std::endl; //返回错误代码，请去glew.h找十六进制编号
		std::cout << "Function: " << functionName << std::endl;
		std::cout << "File: " << file << std::endl;
		std::cout << "Line: " << line << std::endl;
		std::cout << "Go glew.h to find the hex of the error code!" << line << std::endl;
		__debugbreak();//MSBC编译器特有函数
	}
}
int main() {
	WindowGuard windowGuard(window, WIDTH, HEIGHT, "CS171 hw1");

	vec3 sunPosition = vec3(0, 2.0f, 0);
	
	Mesh rabbitMesh("assets/bunny.obj");
	rabbitMesh.setScale(vec3(1));
	Mesh sunmesh("assets/sphere.obj");
	sunmesh.setPosition(sunPosition);
	sunmesh.setScale(vec3(0.05));
	Mesh planemesh("assets/plane.obj");

	Shader shader("shaders/vertex/SimpleVertexShader.vertexshader", "shaders/fragment/MultiLights.glsl");
	Shader simpleshader("shaders/vertex/onlyMVP.glsl", "shaders/fragment/simplestFS.glsl");
	Shader geshader("shaders/vertex/onlyMVP.glsl", "shaders/fragment/simplestFS.glsl", "shaders/geometry/explode.glsl");

	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#pragma region 主循环流程

		shader.use();
		shader.setVec3("viewPos", getCameraPosition());
		shader.setVec3("OBJOcolor", vec3(.498, 0.149, 0.0156));
		
		
		//Point light settings
		shader.setVec3("pointLight.lightPosition", sunPosition);
		shader.setVec3("pointLight.LightColor", vec3(1));
		shader.setFloat("pointLight.ambientLightStrength", .5f);
		shader.setFloat("pointLight.specularStrength", 0.8);
		//Direct Light settings
		shader.setVec3("dirLight.direction", vec3(-1,-1,.3));
		shader.setVec3("dirLight.LightColor", vec3(0,.979,0));
		shader.setFloat("dirLight.ambientLightStrength", 2);
		shader.setFloat("dirLight.specularStrength", 0.8);
		//Spot with camera
		shader.setVec3("spotLight.position", getCameraPosition());
		shader.setVec3("spotLight.direction", getCameraFront());
		shader.setFloat("spotLight.lightPower", 5.f);
		shader.setVec3("spotLight.LightColor", vec3(1,0,0));
		shader.setFloat("spotLight.ambientLightStrength", .5f);
		shader.setFloat("spotLight.specularStrength", 0.8);
		shader.setFloat("spotLight.constant", 1.0f);
		shader.setFloat("spotLight.linear", 0.09f);
		shader.setFloat("spotLight.quadratic", 0.032f);
		shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(2.f)));
		
		//MVP with mouse and keyboard
		shader.setMat4("MVP", getMVPCM(rabbitMesh.getModelMatrix()));
		//shader.setMat4("modelP", rabbitMesh.getPositionMatrix()); 
		shader.setInt("pointLight.isEnable", 1);
		shader.setInt("spotLight.isEnable", 1);
		shader.setInt("dirLight.isEnable", 1);
		GLDebug(rabbitMesh.draw());

		geshader.use();
		geshader.setMat4("MVP", getMVPCM(sunmesh.getModelMatrix()));
		geshader.setFloat("time", (float)glfwGetTime());
		GLDebug(sunmesh.draw());




#pragma endregion
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}