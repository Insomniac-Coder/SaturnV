#ifndef WINDOW_H
#define WINDOW_H

#include "Renderer.h"
#include "../Log/Log.h"

namespace Saturn {

    class Window {
    private:
        unsigned int& w_Width;
        unsigned int& w_Height;
        unsigned int& w_VirtualWidth;
        unsigned int& w_VirtualHeight;
        GLFWwindow* w_Window;
        Saturn::Renderer* w_Renderer;
        inline GLFWwindow* SendWindowRef() { return w_Window; }

    public:
        Window(unsigned int& windowWidth, unsigned int& windowHeight, unsigned int& virtualWidth, unsigned int& virtualHeight);
        ~Window();

        void Run();
    };

}

#endif WINDOW_H