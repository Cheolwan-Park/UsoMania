#ifndef GLWrapperGLFW_hpp
#define GLWrapperGLFW_hpp

#include <GL/glew.h>
//#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace GLFW
{
    // class definition
    class Window;
    class Input;
    
    // general functions
    typedef GLint(*GLFWsettingfun)(void);
    GLint Init(GLFWerrorfun error_callback, GLFWsettingfun setting_fun);
    
    void Terminate();
    
    void ClearGLError();
    
    // default functions
    void DefaultErrCallback(GLint err_code, const GLchar *err_description);
    
    GLint DefaultSettingFun();
}


#endif
