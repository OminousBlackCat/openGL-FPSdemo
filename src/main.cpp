#ifdef _WIN32
    #define APIENTRY __stdcall
#endif

#define STB_IMAGE_IMPLEMENTATION

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"shader.h"
#include"stb_image.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<math.h>
#include"camera.h"
#include"texture.h"
#include "sky_box.h"
#include <time.h>
#include"asset.hpp"


// force to using amd graphics card
extern "C"
{
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

using namespace std;

const int windowWidth = 800;
const int windowHeight = 800;

bool keys[1024];
// calculate render time cost
float deltaTime = 0.0f; // the render time cost = current frame rendered timestamp - lastFrame
float lastFrame = 0.0f; //the timestamp of rendered last frame

float lastX = windowWidth / 2.0f;
float lastY = windowHeight / 2.0f;
bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;

float viewFov = 45.0f;

Camera myCamera = Camera(glm::vec3(0.0f, 0.0f, 5.0f));

void resize_window(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}


void scrollCallbak(GLFWwindow* window, double xoffset, double yoffset) {
	myCamera.zoom(yoffset);
}

void mouseCallback(GLFWwindow* window, double xposIn, double yposIn) {
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}


	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	myCamera.yawAndPitch(xoffset, yoffset);
}


int main() {
	srand(time(NULL));

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* mainWindow = glfwCreateWindow(windowWidth, windowHeight, "opengl-project2", NULL, NULL);
	if (mainWindow == NULL) {
		return -1;
	}
	glfwMakeContextCurrent(mainWindow);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "failed to initialize GLAD" << endl;
		return -1;
	}
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, windowHeight, windowHeight);

	glfwSetFramebufferSizeCallback(mainWindow, resize_window);

	glfwSetKeyCallback(mainWindow, keyCallback);

	glfwSetCursorPosCallback(mainWindow, mouseCallback);

	glfwSetScrollCallback(mainWindow, scrollCallbak);

	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	
	// 发光cube所使用的顶点数组
	// 只构造了所有的顶点 需要element_index_array来确定绘制顺序
	float vertices[] = {
		 0.0f,  0.0f,  0.0f,	//0
		 1.0f,  0.0f,  0.0f,  	//1
		 0.0f,  1.0f,  0.0f,	//2
		 1.0f,  1.0f,  0.0f,	//3
		 0.0f,  0.0f,  1.0f,	//4
		 1.0f,  0.0f,  1.0f,	//5
		 0.0f,  1.0f,  1.0f,	//6
		 1.0f,  1.0f,  1.0f, 	//7
	};


	// textured cube所使用的顶点数组
	// 包含了所有三角形的顶点 以及纹理坐标 以及每个绘制顶点的法向量
	float vertices_array_mode[] = {
		// ---position--		--texture--  --normal--
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,    0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,    0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,    0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,    0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,    0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,    0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,    0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,     0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,	 0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,	 0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,	 0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	 0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	-1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,	-1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	-1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	-1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	-1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	 1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,	 1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	 1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	 1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	 1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	 1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	 0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,	 0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,	 0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,	 0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	 0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	 0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,	 0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,	 0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	 0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	 0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,	 0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,	 0.0f,  1.0f,  0.0f
	};


	// 对于发光cube的顶点的三角形绘制顺序
	unsigned int elementIndex[] = {	
		0,1,2,
		1,2,3,
		4,5,6,
		5,6,7,
		0,1,4,
		1,4,5,
		2,3,6,
		3,6,7,
		1,3,5,
		3,5,7,
		0,2,4,
		2,4,6
	};


	

	// 初始化所需要的纹理图片
	const int textureCount = 4;

	Texture riceTexture(IMAGE_DIR"/rice.png", GL_RGBA);
	Texture hutaoTexture(IMAGE_DIR"/hutao.png", GL_RGBA);
	Texture jjzTexture(IMAGE_DIR"/jjz.jpg", GL_RGB);
	Texture chenTexture(IMAGE_DIR"/chen.jpg", GL_RGB);

	Texture textureList[textureCount] = { riceTexture, hutaoTexture, jjzTexture, chenTexture };

	// 初始化镜面反射纹理图片
	Texture riceSpec(IMAGE_DIR"/rice_spec.png", GL_RGBA);
	Texture hutaoSpec(IMAGE_DIR"/hutao_spec.png", GL_RGBA);
	Texture jjzSpec(IMAGE_DIR"/jjz_spec.jpg", GL_RGB);
	Texture chenSpec(IMAGE_DIR"/chen_spec.jpg", GL_RGB);

	Texture specList[textureCount] = { riceSpec, hutaoSpec, jjzSpec, chenSpec };


	// 初始化天空盒所需的纹理图片
	int sky_tex_width, sky_tex_height;
	unsigned char* sky_tex_img;


	// 创建VAO baseVAO中用来存放光照cube
	unsigned int baseVAO;
	glGenVertexArrays(1, &baseVAO);
	glBindVertexArray(baseVAO);

	unsigned int tri_vertex;
	unsigned int tri_index;
	glm::vec3 light_postion(2.0f, 2.0f, 0.0f);
	glGenBuffers(1, &tri_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, tri_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glGenBuffers(1, &tri_index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tri_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elementIndex), elementIndex, GL_STATIC_DRAW);
	//map the layout in vertex shader
	//--------------index, size, type, ifNormalize, stride(offset), pointer(the space between first space)-------  
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); //enable location = 0
	glBindVertexArray(0);

	// 创建第二个VAO cube_rice中存放了纹理cube
	unsigned int cube_rice;
	unsigned int rice_vertex;
	glm::vec3 origin_position(0.0f, 0.0f, 0.0f); // 初始位置(世界坐标系中的位置)
	glGenBuffers(1, &rice_vertex);
	glGenVertexArrays(1, &cube_rice);
	glBindVertexArray(cube_rice); 
	glBindBuffer(GL_ARRAY_BUFFER, rice_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_array_mode), vertices_array_mode, GL_STATIC_DRAW);
	// 启用并绑定对应的顶点属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(0); //enable location = 0
	glEnableVertexAttribArray(1); //enable location = 1
	glEnableVertexAttribArray(2); //enable location = 2


	Shader shader1(SHADER_DIR"/vertexShader.vs", SHADER_DIR"/fragmentShader.fs");
	Shader shader2(SHADER_DIR"/lightVertexShader.vs", SHADER_DIR"/lightShader.fs");
	glBindVertexArray(0);

	float gree_axis_value = 1.0f;


	while (!glfwWindowShouldClose(mainWindow)) {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwPollEvents();
		
		//process keyboard events
		float cameraSpeed = 5.0f * deltaTime;
		if (keys[GLFW_KEY_W]) {
			myCamera.movement(Camera::CAMERA_FORWARD, deltaTime);
		}
		if (keys[GLFW_KEY_S]) {
			myCamera.movement(Camera::CAMERA_BACKWARD, deltaTime);
		}
		if (keys[GLFW_KEY_A]) {
			myCamera.movement(Camera::CAMERA_LEFT, deltaTime);
		}
		if (keys[GLFW_KEY_D]) {
			myCamera.movement(Camera::CAMERA_RIGHT, deltaTime);
		}
		if (keys[GLFW_KEY_UP]) {
			light_postion.y += 1.0f * deltaTime;
		}
		if (keys[GLFW_KEY_DOWN]) {
			light_postion.y -= 1.0f * deltaTime;
		}
		if (keys[GLFW_KEY_LEFT]) {
			light_postion.x -= 1.0f * deltaTime;
		}
		if (keys[GLFW_KEY_RIGHT]) {
			light_postion.x += 1.0f * deltaTime;
		}
		

		// define matrix
		glm::mat4 model = glm::mat4(1.0f);
		

		glm::mat4 view = glm::mat4(1.0f);
		view = myCamera.getLookAtMat();

		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(myCamera.zoomAngle, (float)windowWidth / (float)windowHeight, 1.0f, 100.0f);

		// 使用shader1(带光照的 物体所使用的着色器)
		shader1.use();
		unsigned int modelMat = glGetUniformLocation(shader1.ID, "modelMat");
		unsigned int viewMat = glGetUniformLocation(shader1.ID, "viewMat");
		unsigned int projectionMat = glGetUniformLocation(shader1.ID, "projectionMat");
		unsigned int viewPos = glGetUniformLocation(shader1.ID, "viewPos");

		// 注册cube的反射锐利程度
		unsigned int materialShininess = glGetUniformLocation(shader1.ID, "material.shininess");
		glUniform1f(materialShininess, 32.0f);

		// 注册环境光 漫反射光与镜面反射光的属性
		unsigned int lightAmbient = glGetUniformLocation(shader1.ID, "flashLight.ambient");
		unsigned int lightDiffuse = glGetUniformLocation(shader1.ID, "flashLight.diffuse");
		unsigned int lightSpecular = glGetUniformLocation(shader1.ID, "flashLight.specular");
		unsigned int lightPosition = glGetUniformLocation(shader1.ID, "flashLight.position");
		unsigned int lightDirection = glGetUniformLocation(shader1.ID, "flashLight.direction");
		unsigned int flashCutOff = glGetUniformLocation(shader1.ID, "flashLight.cutOff");
		unsigned int flashOutCutOff = glGetUniformLocation(shader1.ID, "flashLight.outerCutOff");
		unsigned int constant = glGetUniformLocation(shader1.ID, "flashLight.attenuation_constant");
		unsigned int liner = glGetUniformLocation(shader1.ID, "flashLight.attenuation_linear");
		unsigned int quadratic = glGetUniformLocation(shader1.ID, "flashLight.attenuation_quadratic");
		glUniform3f(lightAmbient, 0.01f, 0.01f, 0.01f);
		glUniform3f(lightDiffuse, 0.7f, 0.7f, 0.7f);
		glUniform3f(lightSpecular, 1.0f, 1.0f, 1.0f);
		glUniform3f(lightPosition, myCamera.position.x, myCamera.position.y, myCamera.position.z);
		glUniform3f(lightDirection, myCamera.frontVec.x, myCamera.frontVec.y, myCamera.frontVec.z);
		glUniform1f(flashCutOff, 0.98f);
		glUniform1f(flashOutCutOff, 0.94f);
		glUniform1f(constant, 1.0f);
		glUniform1f(liner, 0.14f);
		glUniform1f(quadratic, 0.07f);

		glUniform3f(viewPos, myCamera.position.x, myCamera.position.y, myCamera.position.z);
		glUniformMatrix4fv(viewMat, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionMat, 1, GL_FALSE, glm::value_ptr(projection));
		glBindVertexArray(cube_rice);
		
		 

		glm::vec3 current_postition(origin_position.x, origin_position.y, origin_position.z);
		for (int i = 0; i < 10; i++) {
			current_postition.z = 0.0f;
			for (int j = 0; j < 10; j++) {
				model = glm::translate(model, current_postition);
				glActiveTexture(GL_TEXTURE0);
				glUniformMatrix4fv(modelMat, 1, GL_FALSE, glm::value_ptr(model));
				glBindTexture(GL_TEXTURE_2D, textureList[(i * j) * 5 % 4].textureID);
				glUniform1i(glGetUniformLocation(shader1.ID, "material.objectTexture"), 0);
				glActiveTexture(GL_TEXTURE1); // 启用纹理1 作为镜面反射纹理
				glBindTexture(GL_TEXTURE_2D, specList[(i * j) * 5 % 4].textureID);
				glUniform1i(glGetUniformLocation(shader1.ID, "material.specTexture"), 1);
				glDrawArrays(GL_TRIANGLES, 0, 256);
				
				current_postition.z += 2.0f;
				model = glm::mat4(1.0f); // 重置model矩阵, 不能直接在之前的基础上做变换
			}
			current_postition.x += 2.0f;
		}
		
		

		glBindVertexArray(0);

		// 启用着色器2 光源着色器
		shader2.use();
		model = glm::mat4(1.0f); // 重置模型矩阵

		modelMat = glGetUniformLocation(shader2.ID, "modelMat");
		viewMat = glGetUniformLocation(shader2.ID, "viewMat");
		projectionMat = glGetUniformLocation(shader2.ID, "projectionMat");

		model = glm::translate(model, light_postion);
		glUniformMatrix4fv(modelMat, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewMat, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionMat, 1, GL_FALSE, glm::value_ptr(projection));
		glBindVertexArray(baseVAO);
		glActiveTexture(GL_TEXTURE0);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);


		//calculate the frame render cost
		float currentTimeStamp = glfwGetTime();
		deltaTime = currentTimeStamp - lastFrame;
		lastFrame = currentTimeStamp;

		//check events and swap buffer
		glfwPollEvents();
		glfwSwapBuffers(mainWindow);
		
	}


	glfwTerminate();
	return 0;

}