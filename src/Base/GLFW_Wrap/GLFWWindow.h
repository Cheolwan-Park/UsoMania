#ifndef GLWrapperGLFWWindow_hpp
#define GLWrapperGLFWWindow_hpp

#include "GLFW.h"

namespace GLFW
{
    class Window
    {
    public:
        Window();
        
        Window(const Window &other);
        
        ~Window();
        
        Window &operator=(const Window &other);
        
        GLint Create(GLuint width, GLuint height,
                     const GLchar *title);
        
        void Destroy();
        
        void Close();
        
        void Swap();
        
        void SetCurrentContext();
        
        void SetSize(GLuint width, GLuint height);
        
        void SetKeyCallback(GLFWkeyfun fun);
        
        void SetMousePosCallback(GLFWcursorposfun fun);
        
        void SetMouseButtonCallback(GLFWmousebuttonfun fun);
        
        void SetScrollCallback(GLFWscrollfun fun);
        
        void SetMouseEnterCallback(GLFWcursorenterfun fun);
        
        void SetCloseCallback(GLFWwindowclosefun fun);
        
        GLint isShouldClose()const;
        
        GLint isAvailable()const;
        
        void GetSize(GLint *width, GLint *height)const;
        
    private:
        GLFWwindow *m_window;
    };
}

#endif
