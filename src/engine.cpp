#include "engine.h"

#include <iostream>

#include "shader.h"
#include "material.h"

Engine *Engine::instance = nullptr;

Engine::Engine(){
	// Empty
}

void Engine::postContextCreation(){
	glfwSetFramebufferSizeCallback(window, Engine::resizeWindow);
	glEnable(GL_DEPTH_TEST);
	Shader::DEFAULT_SHADER.loadSources("data/shaders/default.vs", "data/shaders/default.fs");
	Material::DEFAULT_MATERIAL = Material();
}

Engine::~Engine(){
	glfwDestroyWindow(window);
}

Engine* Engine::getInstance(){
	if(Engine::instance == nullptr){
		Engine::instance = new Engine();
	}
	return Engine::instance;
}

GLFWwindow* Engine::createWindow(int width, int height, const char *name){
	glfwDestroyWindow(window);

	if(!glfwInit()){
		std::cout << "Failed to initialize GLFW" << std::endl;
		return nullptr;
	}

	this->window = glfwCreateWindow(width, height, name, NULL, NULL);
	this->width = width;
	this->height = height;

	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	postContextCreation();

	return window;
}

void Engine::playScene(Scene &scene){
	while(!glfwWindowShouldClose(window)){
		float newTime = glfwGetTime();
		deltaTime = newTime - lastTime;
		lastTime = newTime;

		glfwPollEvents();
		processInputs();

		scene.draw();
		glfwSwapBuffers(window);
	}
	glfwTerminate();
}

void Engine::processInputs(){
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
		glfwSetWindowShouldClose(window, true);
	}
	for(auto it = inputCallbacks.begin(); it != inputCallbacks.end(); ++it){
		if(glfwGetKey(window, it->first) == GLFW_PRESS){
			for(auto fp = it->second.begin(); fp != it->second.end(); ++fp){
				(*fp)(deltaTime);
			}
		}
	}
}

void Engine::registerKeyEvent(int key, std::function<void(float)> func){
	inputCallbacks[key].push_back(func);
}

GLFWwindow* Engine::getWindow(){
	return window;
}

int Engine::getWidth(){
	return width;
}

int Engine::getHeight(){
	return height;
}

void Engine::resizeWindow(GLFWwindow *window, int newWidth, int newHeight){
	Engine *e = Engine::getInstance();
	glViewport(0, 0, newWidth, newHeight);
	e->width = newWidth;
	e->height = newHeight;
}
