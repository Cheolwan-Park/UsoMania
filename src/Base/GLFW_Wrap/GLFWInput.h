#ifndef GLWrapperGLFWInput_hpp
#define GLWrapperGLFWInput_hpp

#include "GLFW.h"

namespace GLFW
{
    class Input
    {
    public:
        static constexpr GLuint NUM_MOUSE_BUTTONS = 8;
        struct Mouse
        {
            GLdouble x;
            GLdouble y;
            GLchar buttondown_buf[NUM_MOUSE_BUTTONS];
            GLchar buttonpressed_buf[NUM_MOUSE_BUTTONS];
        };
        
    private:
        static constexpr GLuint KEYBOARD_BUFFER_SIZE = 352;
        struct Data
        {
            GLchar keydown_buf[KEYBOARD_BUFFER_SIZE];
            GLchar keypressed_buf[KEYBOARD_BUFFER_SIZE];
            Mouse mouseinfo;
        };
        
        Input();
        
        Input(const Input &other) = delete;
        
        Input &operator=(const Input &other) = delete;
        
    public:
        ~Input();
        
        void Clear();
        
        void PollEvents();
        
        GLint isKeyDown(GLint key)const;
        
        GLint isKeyPressed(GLint key)const;
        
        GLint isButtonDown(GLuint button)const;
        
        GLint isButtonPressed(GLint button)const;
        
        void GetMousePos(GLdouble *x, GLdouble *y)const;
        
        const Mouse &GetMouseInfo()const;
        
        // callbacks
        
        static void KeyboardCallback(GLFWwindow *window, GLint key, GLint scancode,
                                     GLint action, GLint mods);
        
        static void MousePosCallback(GLFWwindow *window, GLdouble x, GLdouble y);
        
        static void MouseButtonCallback(GLFWwindow *window, GLint button,
                                        GLint action, GLint mods);
        
    public:
        static Input &Get();
        
        static void Init(Window window);
        
    private:
        Data m_data;
    };
}

#endif
