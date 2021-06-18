#include "Entrypoint.h"
#include "Application.h"

int main(int argc, char** argv) {
	Saturn::Application* app = new Saturn::Application();

	app->AppRun();

	delete app;	
}