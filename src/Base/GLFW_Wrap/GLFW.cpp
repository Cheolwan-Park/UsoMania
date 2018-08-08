#include "GLFW.h"
#include <stdio.h>

namespace GLFW
{
    GLint Init(GLFWerrorfun error_callback, GLFWsettingfun setting_fun)
    {
        glfwSetErrorCallback(error_callback);
        if(GLFW_FALSE == glfwInit())
        {
            return GLFW_FALSE;
        }
        
        return setting_fun();
    }
    
    void Terminate()
    {
        glfwTerminate();
    }
    
    void ClearGLError()
    {
        while(GL_NO_ERROR != glGetError());
    }
    
    void DefaultErrCallback(GLint err_code, const GLchar *err_description)
    {
        fprintf(stderr, "Error: %s\n", err_description);
    }
    
    GLint DefaultSettingFun()
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
//        glfwWindowHint(GLFW_VERSION_MAJOR, 4);
//        glfwWindowHint(GLFW_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        glfwWindowHint(GLFW_SAMPLES, 4);
        return GLFW_TRUE;
    }
}
