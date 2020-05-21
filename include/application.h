#pragma once

#include <vector>

#include "model.h"
#include "controller.h"

class Application {
public:
	static Application* instance();

	void run();

	Controller& get_controller();

private:
	Application();
	Application(const Application&) = default;
	Application& operator=(const Application&) = default;
	Application(Application&&) = default;
	Application& operator=(Application&&) = default;

	~Application() = default;

private:
	Model model;
	Controller controller;
};
