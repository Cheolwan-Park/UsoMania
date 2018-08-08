#include "GLFWWindow.h"

namespace GLFW
{
    // Window class
    Window::Window()
    :m_window(nullptr)
    {
        ;
    }
    
    Window::Window(const Window &other)
    :m_window(other.m_window)
    {
        ;
    }
    
    Window::~Window()
    {
        ;
    }
    
    Window &Window::operator=(const Window &other)
    {
        this->m_window = other.m_window;
        return (*this);
    }
    
    GLint Window::Create(GLuint w, GLuint h,
                         const GLchar *title)
    {
        m_window = glfwCreateWindow(w, h, title, nullptr, nullptr);
        if(nullptr == m_window)
        {
            return GLFW_FALSE;
        }
        return GLFW_TRUE;
    }
    
    void Window::Destroy()
    {
        if(nullptr != m_window)
        {
            glfwDestroyWindow(m_window);
            m_window = nullptr;
        }
    }
    
    void Window::Close()
    {
        if(isAvailable())
        {
            glfwSetWindowShouldClose(m_window, GLFW_TRUE);
        }
    }
    
    void Window::Swap()
    {
        if(isAvailable())
        {
            glfwSwapBuffers(m_window);
        }
    }
    
    void Window::SetCurrentContext()
    {
        if(isAvailable())
        {
            glfwMakeContextCurrent(m_window);
        }
    }
    
    void Window::SetSize(GLuint width, GLuint height)
    {
        if(isAvailable())
        {
            glfwSetWindowSize(m_window, width, height);
        }
    }
    
    void Window::SetKeyCallback(GLFWkeyfun func)
    {
        if(isAvailable())
        {
            glfwSetKeyCallback(m_window, func);
        }
    }
    
    void Window::SetMousePosCallback(GLFWcursorposfun fun)
    {
        if(isAvailable())
        {
            glfwSetCursorPosCallback(m_window, fun);
        }
    }
    
    void Window::SetMouseButtonCallback(GLFWmousebuttonfun fun)
    {
        if(isAvailable())
        {
            glfwSetMouseButtonCallback(m_window, fun);
        }
    }
    
    void Window::SetScrollCallback(GLFWscrollfun fun)
    {
        if(isAvailable())
        {
            glfwSetScrollCallback(m_window, fun);
        }
    }
    
    void Window::SetMouseEnterCallback(GLFWcursorenterfun fun)
    {
        if(isAvailable())
        {
            glfwSetCursorEnterCallback(m_window, fun);
        }
    }
    
    void Window::SetCloseCallback(GLFWwindowclosefun func)
    {
        if(isAvailable())
        {
            glfwSetWindowCloseCallback(m_window, func);
        }
    }
    
    GLint Window::isShouldClose()const
    {
        if(isAvailable())
        {
            return glfwWindowShouldClose(m_window);
        }
        else
        {
            return GLFW_TRUE;
        }
    }
    
    GLint Window::isAvailable()const
    {
        return (m_window != nullptr);
    }
    
    void Window::GetSize(GLint *width, GLint *height)const
    {
        if(isAvailable())
        {
            glfwGetWindowSize(m_window, width, height);
        }
    }
}





