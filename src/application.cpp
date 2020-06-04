#include "application.h"
#include <iostream>

Application& Application::instance() {
	static Application* instance = new Application();
	return *instance;
}

Application::Application() {
    controller = std::make_unique<Controller>(&model);
}

void Application::run() {
	std::cout << "Application::run()" << std::endl;
}

Controller& Application::get_controller() {
    return *controller;
}
