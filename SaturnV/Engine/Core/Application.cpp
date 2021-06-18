#include "Application.h"

Saturn::Application::Application() : a_Width(1280), a_Height(720), a_VirtualWidth(1280), a_VirtualHeight(720)//, a_Window(nullptr)
{
	ApplicationInit();
}

void Saturn::Application::ApplicationInit() {
	LOG_INFO("Initialising Application!");
	if (IsFilePresent("Config.Saturn")) {
		LOG_PASS("Config loaded successfully!");
		LOG_PASS("DEBUG Content:\n" + ReadFile("Config.Saturn"));
		a_Width = std::stoi(GetValue("Config.Saturn", "WindowWidth"));
		a_Height = std::stoi(GetValue("Config.Saturn", "WindowHeight"));
		a_VirtualHeight = std::stoi(GetValue("Config.Saturn", "VirtualRenderHeight"));
		a_VirtualWidth = std::stoi(GetValue("Config.Saturn", "VirtualRenderWidth"));
	}
	else {
		LOG_WARN("Config file not found, using default values");
		LOG_INFO("Creating config file for future use");

		std::string fileContent = "WindowWidth : " + std::to_string(a_Width) + "\n" + "WindowHeight : " + std::to_string(a_Height) + "\n" + "VirtualRenderWidth : " + std::to_string(a_VirtualWidth) + "\n" + "VirtualRenderHeight : " + std::to_string(a_VirtualHeight) + "\n";
		WriteFile("Config.Saturn", fileContent);
		LOG_PASS("Config file created!");
		LOG_PASS("DEBUG Content:\n" + fileContent);

		LOG_PASS("Created config file!");
	}

	LOG_PASS("Application Initalised!");

	a_Window = new Saturn::Window(a_Width, a_Height, a_VirtualWidth, a_VirtualHeight);
}

void Saturn::Application::AppRun() {
	a_Window->Run();
	//while (true);
}

Saturn::Application::~Application() {

}
