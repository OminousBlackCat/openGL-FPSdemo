#ifdef _WIN32
    #define APIENTRY __stdcall
#endif

#define STB_IMAGE_IMPLEMENTATION

#include<iostream>
#include"stb_image.h"
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<cmath>
#include"mesh.h"
#include"shader.h"
#include"camera.h"
#include"texture.h"
#include "sky_box.h"
#include <ctime>
#include"asset.hpp"
#include"shape.h"
#include "object.h"
#include "light.h"


// force to use amd graphics card
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

Camera myCamera = Camera(glm::vec3(0.0f, 1.0f, 5.0f));

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

    // 测试OBJ
    Object test_spider(MODEL_DIR"/spider/Only_Spider_with_Animations_Export.obj");
    test_spider.scale(0.01f);
    test_spider.translation(glm::vec3(0.0f, 0.0f, -1.5f));

	// 初始化天空盒所需的纹理图片
    // 注册天空盒
    vector<string> tex_urls;
    tex_urls.emplace_back(IMAGE_DIR"/skyBox-night/posx.jpg");
    tex_urls.emplace_back(IMAGE_DIR"/skyBox-night/negx.jpg");
    tex_urls.emplace_back(IMAGE_DIR"/skyBox-night/posy.jpg");
    tex_urls.emplace_back(IMAGE_DIR"/skyBox-night/negy.jpg");
    tex_urls.emplace_back(IMAGE_DIR"/skyBox-night/posz.jpg");
    tex_urls.emplace_back(IMAGE_DIR"/skyBox-night/negz.jpg");
    SkyBox mySkyBox(tex_urls);

    // 注册地板
    Floor myFloor(TEXTURE_DIR"/floor.jpg", TEXTURE_DIR"/floor_spec.jpg");



	// 注册cube
    vector<CubeWithTex> cubeVector;
    CubeWithTex cube_rice( IMAGE_DIR"/rice.png", IMAGE_DIR"/rice_spec.png", 32.f);
    CubeWithTex cube_chen( IMAGE_DIR"/chen.jpg", IMAGE_DIR"/chen_spec.png", 16.f);
    CubeWithTex cube_jjz( IMAGE_DIR"/jjz.jpg", IMAGE_DIR"/jjz_spec.jpg", 128.f);
    CubeWithTex cube_hutao(IMAGE_DIR"/hutao.png", IMAGE_DIR"/hutao_spec.png", 4.f);
    cubeVector.push_back(cube_rice);
    cubeVector.push_back(cube_chen);
    cubeVector.push_back(cube_jjz);
    cubeVector.push_back(cube_hutao);
    for(int i = 0;i<cubeVector.size();i++){
        cubeVector[i].translation(glm::vec3(0.0f, 0.5f, (float)i * 2 + 0.0f));
    }

    // 注册灯光
    PointLight pointLight(glm::vec3(.0f, 1.f, .0f), glm::vec3(.1f),
                          glm::vec3(.8f), glm::vec3(1.f),
                          1.0f, 0.14f, 0.07f);
    FlashLight flashLight( glm::vec3(0.f),glm::vec3(.7f), glm::vec3(1.f),
                          0.98f, 0.94f, 1.0f, 0.14f, 0.07f);

    glm::vec3 origin_position(0.0f, 0.5f, 0.0f);


    Shader objShader(SHADER_DIR"/vertexShader.vs", SHADER_DIR"/frgshader.fs");
    Shader shader_skyBox(SHADER_DIR"/skyVertexShader.vs", SHADER_DIR"/skyFragShader.fs");


	float gree_axis_value = 1.0f;

    int coldDown = 100;

	while (!glfwWindowShouldClose(mainWindow)) {
        // --------------------重构游戏循环(渲染循环)-------------------------
        // --------------------渲染循环流程如下:----------------------------
        // 1. 初始化gl的背景 计算一些参数与循环内计数所需要的变量
        // 2. 处理key与mouse的消息, 将消息放入消息数组
        // 3. 对物体的位置信息进行更新, 判断是否有相交, 判断是否碰撞
        // 4. 对camera进行操作(包括camera物理实体的运动计算以及view矩阵和perspective矩阵的计算)
        // 5. 绘制物体

        // 1. 初始化gl的背景颜色 计算一些参数与循环内计数所需要的变量

		glClearColor(0.3f, 0.3f, 0.3f, 1.0f); // 设置背景颜色为浅灰色
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if(coldDown > 0)
            // 计算冷却 对于一些切换模式的按键 需要有冷却计算
            coldDown --;
        // 记录当前的摄像机位置 以便后续判断碰撞时使用
        glm::vec3 currentCameraPosition(myCamera.position.x, myCamera.position.y, myCamera.position.z);

        // 2. 处理key与mouse的消息, 将消息放入消息数组
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
			pointLight.position.y += 1.0f * deltaTime;
		}
		if (keys[GLFW_KEY_DOWN]) {
            pointLight.position.y -= 1.0f * deltaTime;
		}
		if (keys[GLFW_KEY_LEFT]) {
            pointLight.position.x -= 1.0f * deltaTime;
		}
		if (keys[GLFW_KEY_RIGHT]) {
            pointLight.position.x += 1.0f * deltaTime;
		}
		if (keys[GLFW_KEY_G]){
            if(coldDown == 0){
                myCamera.ifFpsMode = !myCamera.ifFpsMode;
                coldDown = int(0.3 / deltaTime);
            }
		}
        if(keys[GLFW_KEY_F]){
            if(coldDown == 0) {
                flashLight.flashSwitch = !flashLight.flashSwitch;
                coldDown = int(0.3 / deltaTime);
            }
        }
		if (keys[GLFW_KEY_SPACE]){
            myCamera.jumpVelocity = 5.0f;
			myCamera.isJumping = true;
		}



        // 3. 对物体的位置信息进行更新, 判断是否有相交, 判断是否碰撞
        bool ifCollision = false;
        for(auto object: cubeVector){
            ifCollision = ifCollision || myCamera.ifCollision(object);
        }
        if(ifCollision){
            myCamera.position.x = currentCameraPosition.x;
            myCamera.position.y = currentCameraPosition.y;
            myCamera.position.z = currentCameraPosition.z;
        }else
            currentCameraPosition = glm::vec3(myCamera.position);

        // 应用重力加速度
        ifCollision = false;
        myCamera.down(deltaTime);
        for(auto object: cubeVector){
            ifCollision = ifCollision || myCamera.ifCollision(object);
        }
        if(ifCollision){
            myCamera.position = currentCameraPosition;
            myCamera.jumpVelocity = 0.0f;
        }



        // 4. 对camera进行操作(包括camera物理实体的运动计算以及view矩阵和perspective矩阵的计算)

        // 将聚光的位置和朝向更新
        flashLight.position = myCamera.position;
        flashLight.direction = myCamera.frontVec;

		// define matrix
		glm::mat4 view = glm::mat4(1.0f);
		view = myCamera.getLookAtMat();

		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(myCamera.zoomAngle, (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);

        // 5. 绘制物体
        // 首先绘制天空盒
        glm::mat4 skyView = glm::mat4(glm::mat3(view));
        mySkyBox.draw(shader_skyBox, projection, skyView);

		// 使用objShader(带光照的 物体所使用的着色器)
		objShader.use();

        // 注册摄像机朝向, 环境光, 漫反射光与镜面反射光的属性
        objShader.uniform_vec3(myCamera.position, "viewPos");
        objShader.uniform_flashLight(flashLight, "flashLight");
        objShader.uniform_pointLight(pointLight, "pointLight");

        test_spider.draw(objShader, projection, view);

        myFloor.draw(objShader, projection, view);



        glm::vec3 current_position(origin_position.x, origin_position.y, origin_position.z);
        for (auto & i : cubeVector) {
            i.draw(objShader, projection, view);
        }

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