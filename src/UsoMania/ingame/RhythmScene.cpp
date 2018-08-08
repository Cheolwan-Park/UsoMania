#include "RhythmScene.h"
#include "ShaderProgram.h"
#include "Note.h"
#include "VisualEffect.h"
#include "RhythmScore.h"
#include <cstring>

#include "Text.h"

namespace  UsoMania
{
    RhythmScene::RhythmScene()
    :ObjectScene(), m_map(nullptr)
    {
        ;
    }
    
    RhythmScene::~RhythmScene()
    {
        if(nullptr != m_map)
        {
            m_map = nullptr;
        }
        
        SoundPlayer::GetGlobal().Release();
    }
    
    int32 RhythmScene::Init()
    {
        if(nullptr == m_map)
        {
            DEBUG_LOG(RhythmScene::Init(), LOG_ERROR, ("rhythm map is null"));
            return RET_FAILED;
        }
        
        auto &app = Application::Get();
        auto &storage = TextureStorage::GetGlobal();
        auto &shaders = ShaderStorage::GetGlobal();
        auto *scene = (ObjectScene*)Application::Get().GetScene();
        
        if(RET_SUCC != ObjectScene::Init())
            return RET_FAILED;
        
        // set shaders
        ObjectStorage *global = GetObjectStorage("global"_hash);
        DrawAbleStorage *unlit = GetDrawableStorage("unlit"_hash);
        DrawAbleStorage *transparent = GetDrawableStorage("transparent"_hash);
        
        // set background
        GLint w=0, h=0;
        app.GetWindow().GetSize(&w, &h);
        auto &allocator = app.GetAllocator();
        void *mem = nullptr;
        
        mem = allocator.Alloc<Sprite>();
        Sprite *background = new (mem) Sprite("background"_hash, true);
        
        auto *background_tex = storage["background"_hash];
        background->SetTexture(background_tex);
        background->SetScale(1.0f, (float32)background_tex->h/background_tex->w);
        background->Scale((float32)w);
        global->Register(background);
        unlit->Register(background);
        
        
        mem = allocator.Alloc<Sprite>();
        Sprite *background_mask = new (mem) Sprite("background_mask"_hash, true);
        background_mask->SetTexture(storage["atlas_ingame"_hash]);
        background_mask->SetScale((GLfloat)w, (GLfloat)h);
        background_mask->SetUV(Math::IRect({200,664,640,360}));
        global->Register(background_mask);
        transparent->Register(background_mask);
        
        // add player
        SoundPlayer::GetGlobal().Init(m_map);
        
        // add text storage
        mem = allocator.Alloc<DrawAbleStorage>();
        DrawAbleStorage *text_storage = new (mem) DrawAbleStorage("texts"_hash, 16);
        mem = allocator.Alloc(512*sizeof(DrawAbleStorage::Type));
        text_storage->AssignMemory(mem, 512);
        text_storage->SetShader(shaders["text"_hash]);
        text_storage->SetRenderSettingFun([](void) {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        });
        scene->AddDrawableStorage(text_storage);
        
        // add ui
        mem = allocator.Alloc(sizeof(RhythmUI));
        RhythmUI *rhythm_ui = new (mem) RhythmUI("rhythmUI"_hash, true);
        rhythm_ui->Init(m_map, 1000);
        global->Register(rhythm_ui);
        
        // reset score
        RhythmScore::GetGlobal().Reset();
        
        return RET_SUCC;
    }
    
    void RhythmScene::Update()
    {
        ObjectScene::Update();
    }
    
    void RhythmScene::Render()
    {
        ObjectScene::Render();
    }
    
    void RhythmScene::CheckDeletedObjects()
    {
        ObjectScene::CheckDeletedObjects();
    }
    
    void RhythmScene::SetRhythmMap(const RhythmMap *map)
    {
        m_map = map;
    }
    
    int32 RhythmScene::LoadShaders()
    {
        auto &allocator = Application::Get().GetAllocator();
        auto &global_shaders = ShaderStorage::GetGlobal();
        
        // load text shader
        void *mem_vert = nullptr, *mem_frag = nullptr;
        
        FILE *vert = OpenFile("res/shader/default_vert.glsl", "r");
        FILE *frag = OpenFile("res/shader/text_frag.glsl", "r");
        size_t size_vert = GetFileSize(vert);
        size_t size_frag = GetFileSize(frag);
        FileIO vert_io, frag_io;
        mem_vert = malloc(size_vert+1);
        mem_frag = malloc(size_frag+1);
        memset(mem_vert, 0, size_vert+1);
        memset(mem_frag, 0, size_frag+1);
        
        if(RET_SUCC != vert_io.Open(vert, mem_vert, size_vert))
        {
            free(mem_vert);
            free(mem_frag);
            return RET_FAILED;
        }
        
        if(RET_SUCC != frag_io.Open(frag, mem_frag, size_frag))
        {
            free(mem_vert);
            free(mem_frag);
            return RET_FAILED;
        }
        
        void *mem = allocator.Alloc(sizeof(ShaderProgram));
        ShaderProgram *program = new (mem) ShaderProgram();
        
        if(RET_SUCC != program->Init(vert_io, frag_io))
        {
            free(mem_vert);
            free(mem_frag);
            return RET_FAILED;
        }
        
        global_shaders.Register(program, "text"_hash);
        
        free(mem_vert);
        free(mem_frag);
        
        // load default shader
        return ObjectScene::LoadShaders();
    }
    
    int32 RhythmScene::LoadTextures()
    {
        // load textures
        auto &allocator = Application::Get().GetAllocator();
        auto &storage = TextureStorage::GetGlobal();
        void *mem = nullptr;
        
        auto &meta = m_map->GetMetaData();
        char bgfile[sizeof(meta.folder)+sizeof(meta.bgname)] = {0,};
        strcpy(bgfile, meta.folder);
        mem = allocator.Alloc(sizeof(TextureStorage::Val));
        TextureStorage::Val *background_tex = new (mem) TextureStorage::Val();
        background_tex->id = LoadTexture(strcat(bgfile, meta.bgname),
                                         &(background_tex->w), &(background_tex->h));
        storage.Register(background_tex, "background"_hash);
        
        
        mem = allocator.Alloc(sizeof(TextureStorage::Val));
        TextureStorage::Val *atlas = new (mem) TextureStorage::Val();
        atlas->id = LoadTexture("res/atlas_ingame.png", &(atlas->w), &(atlas->h));
        storage.Register(atlas, "atlas_ingame"_hash);
        
        
        mem = allocator.Alloc(sizeof(TextureStorage::Val));
        TextureStorage::Val *text_atlas = new (mem) TextureStorage::Val();
        text_atlas->id = LoadTexture("res/atlas_text.png", &(text_atlas->w), &(text_atlas->h));
        storage.Register(text_atlas, "atlas_text"_hash);
        
        return RET_SUCC;
    }
    
    // RhythmUI
    RhythmUI::RhythmUI(Uint32 id, Bool isStatic)
    :GamesEngine::GameObject(id, isStatic),
    m_music_wait(0), m_linenum(0), m_map(nullptr), m_lines(nullptr), m_notegenerator(nullptr)
    {
        ;
    }
    
    RhythmUI::RhythmUI(Uint32 id, const GameObject *parent, Bool isStatic)
    :GamesEngine::GameObject(id, isStatic),
    m_music_wait(0), m_linenum(0), m_map(nullptr), m_lines(nullptr), m_notegenerator(nullptr)
    {
        ;
    }
    
    RhythmUI::~RhythmUI()
    {
        if(nullptr != m_lines)
        {
            for(Uint32 i=0; i<m_linenum; ++i)
                m_lines[i].Delete();
        }
        if(nullptr != m_notegenerator)
            m_notegenerator->Delete();
    }
    
    void RhythmUI::Start()
    {
        if(nullptr == m_notegenerator)
        {
            DEBUG_LOG(RhythmUI::Start(), LOG_ERROR, ("RhythmUI::Init() not called yet"));
            return;
        }
        
        auto &app = Application::Get();
        auto &allocator = app.GetAllocator();
        auto &texstorage = TextureStorage::GetGlobal();
        auto *scene = (ObjectScene*)app.GetScene();
        auto &shaders = ShaderStorage::GetGlobal();
        GLint window_w = 0, window_h;
        app.GetWindow().GetSize(&window_w, &window_h);
        void *mem = nullptr;
        
        auto *global_objs = scene->GetObjectStorage("global"_hash);
        auto *transparent = scene->GetDrawableStorage("transparent"_hash);
        
        // add score line
        mem = allocator.Alloc<Sprite>();
        Sprite *scoreline = new (mem) Sprite("scoreline"_hash, true);
        scoreline->SetTexture(texstorage["atlas_ingame"_hash]);
        scoreline->SetUV(Math::IRect({120,0,60,1024}));
        scoreline->SetLocalPosition(0.0f, -0.4f*window_h);
        scoreline->SetScale(30, window_w);
        scoreline->SetRotation(Math::Deg2Rad(90.0f));
        global_objs->Register(scoreline);
        transparent->Register(scoreline);
        
        // add noteline storage
        mem = allocator.Alloc<DrawAbleStorage>();
        DrawAbleStorage *notelines = new (mem) DrawAbleStorage("notelines"_hash, 200);
        mem = allocator.Alloc(sizeof(DrawAbleStorage::Type)*m_linenum);
        notelines->AssignMemory(mem, m_linenum);
        notelines->SetShader(shaders["default"_hash]);
        scene->AddDrawableStorage(notelines);
        
        // add note line
        m_lines = allocator.Alloc<Sprite>(m_linenum);
        Sprite *line = nullptr;
        float start = 0.0f;
        if(0 == m_linenum%2)
            start -= 50.0f + 100.0f*(m_linenum/2-1);
        else
            start -= 100.0f + 100.0f*(m_linenum/2);
        
        for(Uint32 i=0; i<m_linenum; ++i)
        {
            line = new (m_lines + i) Sprite(i, true);
            line->SetTexture(texstorage["atlas_ingame"_hash]);
            line->SetUV(Math::IRect({0,0,100,1024}));
            line->SetScale(90,window_h);
            line->SetLocalPosition(start+i*100.0f, 0.0f);
            global_objs->Register(line);
            notelines->Register(line);
        }
        
        // add score effect
        mem = allocator.Alloc<ScoreEffect>();
        ScoreEffect *score_effect = new (mem) ScoreEffect("ScoreEffect"_hash);
        score_effect->SetLocalPosition(0.0f, 0.0f);
        global_objs->Register(score_effect);
        transparent->Register(score_effect);
        
        // add hiteffects
        mem = allocator.Alloc<HitEffects>();
        HitEffects *effects = new (mem) HitEffects("HitEffects"_hash);
        effects->Init(m_lines, m_linenum);
        global_objs->Register(effects);
        HitEffects::SetGlobal(effects);
        
        // gen texts
        GenSongInfoTexts();
        
        // add generator
        m_notegenerator->SetLineSprites(m_lines);
        global_objs->Register(m_notegenerator);
    }
    
    void RhythmUI::Update()
    {
        Uint32 t = m_notegenerator->GetTime();
        if(0 != m_music_wait && m_music_wait <= t)
        {
            SoundPlayer::GetGlobal().StartPlayMusic();
            m_music_wait = 0;
        }
    }
    
    void RhythmUI::Pause()
    {
        m_notegenerator->Pause();
        SoundPlayer::GetGlobal().PauseMusic();
        SetAvailable(false);
    }
    
    void RhythmUI::Resume()
    {
        m_notegenerator->Resume();
        SoundPlayer::GetGlobal().ResumeMusic();
        SetAvailable(true);
    }
    
    void RhythmUI::Init(const RhythmMap *map, Uint32 speed, Uint32 wait)
    {
        auto &allocator = Application::Get().GetAllocator();
        void *mem = allocator.Alloc(sizeof(NoteGenerator));
        m_notegenerator = new (mem) NoteGenerator("notegenerator"_hash, true);
        m_notegenerator->Init(map, speed, wait+map->GetMetaData().audioleadin);
        
        m_linenum = map->GetNumLines();
        m_map = map;
        m_music_wait = wait < speed ? speed : wait;
        m_music_wait += map->GetMetaData().audioleadin;
    }
    
    void RhythmUI::GenSongInfoTexts()
    {
        // add song info text
        auto &meta = m_map->GetMetaData();
        auto &allocator = Application::Get().GetAllocator();
        auto &storage = TextureStorage::GetGlobal();
        auto *scene = (ObjectScene*)Application::Get().GetScene();
        auto *global = scene->GetObjectStorage("global"_hash);
        auto *text_storage = scene->GetDrawableStorage("texts"_hash);
        void *mem = nullptr;
        
        GLint w=0,h=0;
        Application::Get().GetWindow().GetSize(&w, &h);
        
        Uint32 len = 0;
        
        mem = allocator.Alloc<Text>();
        Text *songname = new (mem) Text("songname"_hash, true);
        songname->SetScale((float32)w*0.03f, (float32)w*0.03f);
        songname->SetLocalPosition(-0.48f*(float32)w, -0.21f*(float32)h);
        len = (Uint32)strlen(meta.title);
        mem = allocator.Alloc<CharSprite>(len);
        songname->AssignMemory(mem, len);
        songname->SetFont({10,10,storage["atlas_text"_hash]});
        songname->SetString(meta.title, len);
        global->Register(songname);
        text_storage->Register(songname);
        
        mem = allocator.Alloc<Text>();
        Text *artistname = new (mem) Text("artistname"_hash, true);
        artistname->SetScale((float32)w*0.03f, (float32)w*0.03f);
        artistname->SetLocalPosition(-0.48f*(float32)w, -0.28f*(float32)h);
        len = (Uint32)strlen(meta.artist);
        mem = allocator.Alloc<CharSprite>(len);
        artistname->AssignMemory(mem, len);
        artistname->SetFont({10,10,storage["atlas_text"_hash]});
        artistname->SetString(meta.artist, len);
        global->Register(artistname);
        text_storage->Register(artistname);
        
        mem = allocator.Alloc<Text>();
        Text *version = new (mem) Text("version"_hash, true);
        version->SetScale((float32)w*0.03f, (float32)w*0.03f);
        version->SetLocalPosition(-0.48f*(float32)w, -0.35f*(float32)h);
        len = (Uint32)strlen(meta.version);
        mem = allocator.Alloc<CharSprite>(len);
        version->AssignMemory(mem, len);
        version->SetFont({10, 10 ,storage["atlas_text"_hash]});
        version->SetString(meta.version, len);
        global->Register(version);
        text_storage->Register(version);
        
        // Banners
        mem = allocator.Alloc<ScoreBanner>();
        ScoreBanner *score_banner = new (mem) ScoreBanner("score_banner"_hash);
        score_banner->SetScale((float32)w*0.03f, (float32)w*0.03f);
        score_banner->SetLocalPosition(0.2f*(float32)w, 0.45f*(float32)h);
        global->Register(score_banner);
        text_storage->Register(score_banner);
        
        mem = allocator.Alloc<ComboBanner>();
        ComboBanner *combo_banner = new (mem) ComboBanner("combo_banner"_hash);
        combo_banner->SetScale((float32)w*0.03f, (float32)w*0.03f);
        combo_banner->SetLocalPosition(0.2f*(float32)w, -0.35f*(float32)h);
        global->Register(combo_banner);
        text_storage->Register(combo_banner);
    }
    
    // SoundPlayer class
    const char8 *SoundPlayer::hitsounds_filename[4] =
    {
        "normal.wav",
        "whistle.wav",
        "finish.wav",
        "clap.wav"
    };
    
    SoundPlayer::SoundPlayer()
    :m_music(nullptr), m_hitsounds(nullptr)
    {
        ;
    }
    
    SoundPlayer::~SoundPlayer()
    {
        Release();
    }
    
    void SoundPlayer::PlayHitSound(Uint32 idx)
    {
        Mix_PlayChannel(-1, m_hitsounds[idx], 0);
    }
    
    void SoundPlayer::StartPlayMusic()
    {
        Mix_PlayMusic(m_music, 0);
    }
    
    void SoundPlayer::PauseMusic()
    {
        Mix_PauseMusic();
    }
    
    void SoundPlayer::ResumeMusic()
    {
        Mix_ResumeMusic();
    }
    
    void SoundPlayer::Init(const RhythmMap *map)
    {
        if(nullptr == map)
        {
            DEBUG_LOG(SoundPlayer::Init(), LOG_ERROR, ("RhythmMap is null"));
            return;
        }
        
        Release();
        
        // load music
        auto &meta = map->GetMetaData();
        char8 filename[sizeof(meta.folder)+sizeof(meta.audiofile)] = {0,};
        strcpy(filename, meta.folder);
        m_music = Mix_LoadMUS(strcat(filename, meta.audiofile));
        if(nullptr == m_music)
        {
            DEBUG_LOG(SoundPlayer::Init(), LOG_ERROR,
                      ("failed load music : %s", Mix_GetError()));
            return;
        }
        
        // load hitsounds
        auto &allocator = Application::Get().GetAllocator();
        m_hitsounds = allocator.Alloc<Mix_Chunk*>(HITSOUND_KINDS);
        for(Uint32 i=0; i<HITSOUND_KINDS; ++i)
        {
            memset(filename, 0, sizeof(filename));
            sprintf(filename, "res/hitsounds/%s", hitsounds_filename[i]);
            m_hitsounds[i] = Mix_LoadWAV(filename);
            if(nullptr == m_hitsounds[i])
            {
                DEBUG_LOG(SoundPlayer::Init(), LOG_ERROR,
                          ("failed load hitsound : %s", Mix_GetError()));
                return;
            }
            Mix_VolumeChunk(m_hitsounds[i], 32);
        }
    }
    
    void SoundPlayer::Release()
    {
        if(nullptr != m_music)
        {
            Mix_FreeMusic(m_music);
            m_music = nullptr;
        }
        if(nullptr != m_hitsounds)
        {
            for(Uint32 i=0; i<HITSOUND_KINDS; ++i)
            {
                if(nullptr != m_hitsounds[i])
                    Mix_FreeChunk(m_hitsounds[i]);
                m_hitsounds[i] = nullptr;
            }
            m_hitsounds = nullptr;
        }
    }
    
    SoundPlayer &SoundPlayer::GetGlobal()
    {
        static SoundPlayer instance;
        return instance;
    }
}







