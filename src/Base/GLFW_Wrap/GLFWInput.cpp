#include "GLFWInput.h"
#include "GLFWWindow.h"
#include <string.h>

namespace GLFW
{
    // Input class
    
    Input::Input()
    :m_data()
    {
        memset(&m_data, 0, sizeof(Data));
    }
    
    Input::~Input()
    {
        ;
    }
    
    void Input::Clear()
    {
        memset(m_data.keypressed_buf, 0, sizeof(m_data.keypressed_buf));
        memset(m_data.mouseinfo.buttonpressed_buf, 0,
               sizeof(m_data.mouseinfo.buttonpressed_buf));
    }
    
    void Input::PollEvents()
    {
        glfwPollEvents();
    }
    
    GLint Input::isKeyDown(GLint key)const
    {
        return m_data.keydown_buf[key];
    }
    
    GLint Input::isKeyPressed(GLint key)const
    {
        return m_data.keypressed_buf[key];
    }
    
    GLint Input::isButtonDown(GLuint button)const
    {
        return m_data.mouseinfo.buttondown_buf[button];
    }
    
    GLint Input::isButtonPressed(GLint button)const
    {
        return m_data.mouseinfo.buttonpressed_buf[button];
    }
    
    void Input::GetMousePos(GLdouble *x, GLdouble *y)const
    {
        *x = m_data.mouseinfo.x;
        *y = m_data.mouseinfo.y;
    }
    
    const Input::Mouse &Input::GetMouseInfo()const
    {
        return m_data.mouseinfo;
    }
    
    void Input::KeyboardCallback(GLFWwindow *window, GLint key, GLint scancode,
                                 GLint action, GLint mods)
    {
        if(key < 0)
        {
            return;
        }
        
        Input &input = Input::Get();
        
        if(GLFW_PRESS == action)
        {
            input.m_data.keydown_buf[key] = GLFW_TRUE;
            input.m_data.keypressed_buf[key] = GLFW_TRUE;
        }
        else if(GLFW_RELEASE == action)
        {
            input.m_data.keydown_buf[key] = GLFW_FALSE;
        }
    }
    
    void Input::MousePosCallback(GLFWwindow *window, GLdouble x, GLdouble y)
    {
        Input &input = Input::Get();
        input.m_data.mouseinfo.x = x;
        input.m_data.mouseinfo.y = y;
    }
    
    void Input::MouseButtonCallback(GLFWwindow *window, GLint button,
                                    GLint action, GLint mods)
    {
        Input &input = Input::Get();
        if(GLFW_PRESS == action)
        {
            input.m_data.mouseinfo.buttondown_buf[button] = GLFW_TRUE;
            input.m_data.mouseinfo.buttonpressed_buf[button] = GLFW_TRUE;
        }
        else if(GLFW_RELEASE == action)
        {
            input.m_data.mouseinfo.buttondown_buf[button] = GLFW_FALSE;
        }
    }
    
    Input &Input::Get()
    {
        static Input instance;
        return instance;
    }
    
    void Input::Init(Window window)
    {
        // set callbacks
        window.SetKeyCallback(KeyboardCallback);
        window.SetMousePosCallback(MousePosCallback);
        window.SetMouseButtonCallback(MouseButtonCallback);
    }
}






