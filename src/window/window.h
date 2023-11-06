#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window{
public:
    Window(int width, int height);
    ~Window();

    void close();
    bool isOpen() const;
    struct WindowSize{int w;int h;};
    WindowSize getSize() const;
    void poll();
    void swapBuffer();

    void keyCallback(GLFWkeyfun fun) const;
    void mouseCallback(GLFWmousebuttonfun fun) const;
    void mouseMoveCallback(GLFWcursorposfun fun) const;

    void mousePos(double& x, double& y) const;
private:
    static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
    static void mousePosCallback(GLFWwindow* window, double xpos, double ypos);

    GLFWwindow *window;
};

#endif