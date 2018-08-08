//
//  Application.cpp
//  GamesEngine
//
//  Created by Park Cheol Wan on 2017. 7. 19..
//  Copyright © 2017년 Games. All rights reserved.
//

#include "Application.h"
#include "Time.h"
#include "types.h"
#include "GLFWInput.h"
#include "Sprite.h"
#include "Scene.h"
#include <FreeImage.h>


namespace GamesEngine
{
    GLint DefaultGLSettingFun()
    {
        glClearColor(1.0, 1.0, 1.0, 1.0);
        return GL_TRUE;
    }
    
    int32 DefaultAppSettingFun()
    {
        FreeImage_Initialise();
        return RET_SUCC;
    }
    
    void DefaultReleaseFun()
    {
        FreeImage_DeInitialise();
        GLFW::Terminate();
    }
    
    // Application class
    
    Application::Application()
    :m_window(), m_allocator(), m_scene(nullptr),
    m_scene_release(0), m_quit(false)
    {
        ;
    }
    
    Application::~Application()
    {
        ;
    }
    
    int32 Application::Create(GLFW::Window window,
                              GLSettingFun gl_setting,
                              AppSettingFun app_setting)
    {
        if(false == window.isAvailable())
            return RET_FAILED;
        
        m_window = window;
        m_window.SetCurrentContext();
        glewExperimental = GL_TRUE;
        GLenum err = glewInit();
        if(GLEW_OK != err)
        {
            DEBUG_LOG(Application::Create(), LOG_ERROR,
                      ("failed glewInit() : %s", glewGetErrorString(err)));
            return RET_FAILED;
        }
        
        if(GL_FALSE == GLEW_VERSION_4_1)
        {
            DEBUG_LOG(Application::Create(), LOG_ERROR, ("OpenGL 4.1 is not available"));
            return RET_FAILED;
        }
        gl_setting();
        
        DEBUG_LOG(Application::Create(), LOG_INFO, ("OpenGL info"));
        printf("OpenGL version : %s\n",   glGetString(GL_VERSION));
        printf("GLSL version   : %s\n",   glGetString(GL_SHADING_LANGUAGE_VERSION));
        printf("Vendor         : %s\n",   glGetString(GL_VENDOR));
        printf("Renderer       : %s\n\n", glGetString(GL_RENDERER));
        
        GLFW::Input &input = GLFW::Input::Get();
        input.Init(m_window);
        
        return app_setting();
    }
    
    void Application::Run()
    {
        Time &time = Time::Get();
        time.SetIdealDeltatime(Time::milliseconds(1000/60));
        
        GLFW::Input &input = GLFW::Input::Get();
        
        while (!m_quit && !m_window.isShouldClose())
        {
            input.PollEvents();
            CheckDeleteObjects();
            Update();
            Render();
            m_window.Swap();
            time.Update();
            input.Clear();
        }
    }
    
    void Application::Quit()
    {
        m_quit = true;
    }
    
    void Application::Release(AppReleaseFun fun)
    {
        fun();
    }
    
    int32 Application::SetScene(Scene *scene, StackAllocator::Marker mark)
    {
        if(nullptr == scene)
        {
            DEBUG_LOG(Application::SetScene(), LOG_ERROR, ("scene is null"));
            return RET_INVALID_PARAMS;
        }
        
        if(nullptr != m_scene)
        {
            delete m_scene;
            m_allocator.FreeWithMarker(m_scene_release);
        }
        m_scene = scene;
        
        if(0 == mark)
            m_scene_release = m_allocator.GetTopMarker();
        else
            m_scene_release = mark;
        return m_scene->Init();
    }
    
    GLFW::Window Application::GetWindow()
    {
        return m_window;
    }
    
    StackAllocator &Application::GetAllocator()
    {
        return m_allocator;
    }
    
    Scene *Application::GetScene()
    {
        return m_scene;
    }
    
    int32 Application::isQuit()
    {
        return m_quit;
    }
    
    Application &Application::Get()
    {
        static Application instance;
        return instance;
    }
    
    void Application::CheckDeleteObjects()
    {
        m_scene->CheckDeletedObjects();
    }
    
    void Application::Update()
    {
        m_scene->Update();
    }
    
    void Application::Render()
    {
        glClear(GL_COLOR_BUFFER_BIT);
        m_scene->Render();
    }
}
