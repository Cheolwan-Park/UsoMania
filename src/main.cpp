#include "Application.h"
#include "TextureStorage.h"
#include "ShaderProgram.h"
#include "OsuFile.h"
#include "RhythmScene.h"
#include "Camera.h"
#include "VisualEffect.h"

using namespace GamesEngine;

void test();
int32 AppSetting();
void Release();

int main(int argc, const char * argv[]) {
    
    GLFW::Init(GLFW::DefaultErrCallback, GLFW::DefaultSettingFun);

    Application &app = Application::Get();
    GLFW::Window window;

    window.Create(1600, 900, "Uso Mania");
//    window.Create(1280, 720, "Uso Mania");
    app.Create(window, DefaultGLSettingFun, AppSetting);

    test();

    DEBUG_LOG(main(), LOG_INFO, ("Start Application.."));
    app.Run();

    app.Release(Release);

    return 0;
}

void test()
{
    auto &app = Application::Get();
    auto &allocator = app.GetAllocator();

    // read osu file
    
//    FILE *osu_file = OpenFile("Songs/251365 Halozy - Kanshou no Matenrou/Halozy - Kanshou no Matenrou (Feerum) [World's End].osu", "r");
//    FILE *osu_file = OpenFile("Songs/381334 aaaa - Bokutachi no Tabi to Epilogue[Long ver]/aaaa - Bokutachi no Tabi to Epilogue.[Long ver.] (Daikyi) [Final Voyage].osu", "r");
    FILE *osu_file = OpenFile("Songs/309328 xi - Hesperides/xi - Hesperides (Ichigaki) [Standard].osu", "r");
//    FILE *osu_file = OpenFile("Songs/171880 xi - Happy End of the World/xi - Happy End of the World (Blocko) [4K Hard].osu", "r");
    
    size_t osu_file_len = GetFileSize(osu_file);
    StackAllocator::Marker mark = allocator.GetTopMarker();
    void *mem = allocator.Alloc(osu_file_len+1);
    FileIO osu_io;
    osu_io.Open(osu_file, mem, osu_file_len);
    CloseFile(osu_file);
    mem = allocator.Alloc(sizeof(UsoMania::OsuFile));
    UsoMania::OsuFile *osu = new (mem) UsoMania::OsuFile();
    
//    osu->Open(osu_io, "Songs/251365 Halozy - Kanshou no Matenrou/", allocator);
//    osu->Open(osu_io, "Songs/381334 aaaa - Bokutachi no Tabi to Epilogue[Long ver]/", allocator);
    osu->Open(osu_io, "Songs/309328 xi - Hesperides/", allocator);
    //    osu->Open(osu_io, "Songs/171880 xi - Happy End of the World/", allocator);
    
    UsoMania::RhythmScene *scene = new UsoMania::RhythmScene();
    scene->SetRhythmMap(osu);
    app.SetScene(scene, mark);
}

int32 AppSetting()
{
    auto &app = Application::Get();
    
    auto &allocator = app.GetAllocator();
    allocator.ReAllocBuffer(1024*1024*30);

    // alloc memory for storages
    auto &global_textures = TextureStorage::GetGlobal();
    void *mem = allocator.Alloc(sizeof(TextureStorage::Type)*2048);
    global_textures.AssignMemory(mem, 2048);

    auto &global_shaders = ShaderStorage::GetGlobal();
    mem = allocator.Alloc(sizeof(ShaderStorage::Type)*2048);
    global_shaders.AssignMemory(mem, 2048);
    
    // set camera
    GLint w=0, h=0;
    app.GetWindow().GetSize(&w, &h);
    mem = allocator.Alloc(sizeof(OrthoCamera));
    OrthoCamera *maincam = new (mem) OrthoCamera();
    maincam->SetView(glm::vec3(0.0f, 0.0f, 1.0f),
                     glm::vec3(0.0f, 0.0f, 0.0f),
                     glm::vec3(0.0f, 1.0f, 0.0f));
    maincam->SetProjection(-0.5f*w, 0.5f*w,
                           -0.5f*h, 0.5f*h,
                           0.1f, 8.0f);
    Camera::SetMain(maincam);
    
    // init SDL_Mixer
    if(SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        DEBUG_LOG(AppSetting(), LOG_ERROR, ("failed to init SDL : %s", SDL_GetError()));
        return RET_FAILED;
    }
    if(Mix_Init(MIX_INIT_MP3|MIX_INIT_OGG) < 0)
    {
        DEBUG_LOG(AppSetting(), LOG_ERROR, ("failed to init SDL_Mix : %s", Mix_GetError()));
        return RET_FAILED;
    }
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        DEBUG_LOG(AppSetting(), LOG_ERROR, ("failed to open audio : %s", Mix_GetError()));
        return RET_FAILED;
    }
    
    GLFW::ClearGLError();

    return DefaultAppSettingFun();
}

void Release()
{
    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();
    DefaultReleaseFun();
}
















