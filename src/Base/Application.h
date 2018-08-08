//
//  Application.h
//  GamesEngine
//
//  Created by Park Cheol Wan on 2017. 7. 19..
//  Copyright © 2017년 Games. All rights reserved.
//

#ifndef GamesEngineApplication_h
#define GamesEngineApplication_h

#include "types.h"
#include "GLFW.h"
#include "GLFWWindow.h"
#include "Allocator.h"
#include "BitFlag.h"
#include <thread>

namespace GamesEngine
{
    class Application;
    
    typedef GLint(*GLSettingFun)(void);
    typedef int32(*AppSettingFun)(void);
    typedef void(*AppReleaseFun)(void);
    
    GLint DefaultGLSettingFun();
    int32 DefaultAppSettingFun();
    void DefaultReleaseFun();
    
    class Scene;
    
    class Application
    {
    private:
        Application(const Application &other) = delete;
        Application &operator=(const Application &other) = delete;
        
        Application();
        
    public:
        ~Application();
        
        int32 Create(GLFW::Window window,
                     GLSettingFun gl_setting = DefaultGLSettingFun,
                     AppSettingFun app_setting = DefaultAppSettingFun);
        
        void Run();
        
        void Quit();
        
        void Release(AppReleaseFun fun);
        
        // mark is pointer to release scene
        int32 SetScene(Scene *scene, StackAllocator::Marker mark = 0);
        
        GLFW::Window GetWindow();
        
        StackAllocator &GetAllocator();
        
        Scene *GetScene();
        
        int32 isQuit();
        
        static Application &Get();
        
        
    private:
        void CheckDeleteObjects();
        
        void Update();
        
        void Render();
        
        void ClearDletedObjects();
        
    private:
        GLFW::Window m_window;
        StackAllocator m_allocator;
        Scene *m_scene;
        StackAllocator::Marker m_scene_release;
        int32 m_quit;
    };
}


#endif
