#pragma once

#include <vector>

#include "document.h"


class Application {
public:
	static Application* instance();

	void run();

	DocumentPtr get_current_document() const;

	Controller& get_controller() const;

private:
	Application() = default;
	Application(const Application&) = default;
	Application& operator=(const Application&) = default;
	Application(Application&&) = default;
	Application& operator=(Application&&) = default;

	~Application() = default;

private:
	std::vector<DocumentPtr> documents;
	DocumentPtr current_document;

	Controller controller;
};