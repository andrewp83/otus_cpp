#include "application.h"
#include <iostream>

Application* Application::instance() {
	static Application* instance = new Application();
	return instance;
}

Application::Application() {
	controller.set_model(&model);
}

void Application::run() {
	std::cout << "Application::run()" << std::endl;
}

Controller& Application::get_controller() {
    return std::ref(controller);
}
