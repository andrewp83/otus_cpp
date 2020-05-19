#include <application.h>

Application* Application::instance() {
	static Application* instance = new Application();
	return instance;
}

void Application::run() {
	std::cout << "Application::run()" std::endl;
}