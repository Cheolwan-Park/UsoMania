#ifndef UsoManiaRhythmScene_hpp
#define UsoManiaRhythmScene_hpp

#include "RhythmMap.h"
#include "Scene.h"
#include <SDL2/SDL.h>
#include <SDL2_mixer/SDL_mixer.h>

namespace UsoMania
{
    class Note;
    class NoteGenerator;
    class SoundPlayer;
    class RhythmUI;
    
    class RhythmScene : public ObjectScene
    {
    public:
        RhythmScene();
        
        RhythmScene(const RhythmScene &other) = delete;
        
        virtual ~RhythmScene();
        
        RhythmScene &operator=(const RhythmScene &other) = delete;
        
        virtual int32 Init();
        
        virtual void Update();
        
        virtual void Render();
        
        virtual void CheckDeletedObjects();
        
        void SetRhythmMap(const RhythmMap *map);
        
    protected:
        virtual int32 LoadShaders();
        
        virtual int32 LoadTextures();
        
    private:
        const RhythmMap *m_map;
    };
    
    class RhythmUI : public GameObject
    {
    public:
        RhythmUI() = delete;
        
        RhythmUI(Uint32 id, Bool isStatic = false);
        
        RhythmUI(Uint32 id, const GameObject *parent, Bool isStatic = false);
        
        RhythmUI(const RhythmUI &other) = delete;
        
        virtual ~RhythmUI();
        
        RhythmUI &operator=(const RhythmUI &other) = delete;
        
        virtual void Start();
        
        virtual void Update();
        
        void Pause();
        
        void Resume();
        
        void Init(const RhythmMap *map, Uint32 speed, Uint32 wait=0);
        
    private:
        void GenSongInfoTexts();
        
    private:
        Uint32 m_music_wait;
        Uint32 m_linenum;
        const RhythmMap *m_map;
        Sprite *m_lines;
        NoteGenerator *m_notegenerator;
    };
    
    class SoundPlayer
    {
    private:
        static constexpr Uint32 HITSOUND_KINDS = 4;
        static const char8 *hitsounds_filename[HITSOUND_KINDS];
        
    public:
        SoundPlayer();
        
        SoundPlayer(const SoundPlayer &other) = delete;
        
        virtual ~SoundPlayer();
        
        SoundPlayer &operator=(const SoundPlayer &other) = delete;
        
        void PlayHitSound(Uint32 idx);
        
        void StartPlayMusic();
        
        void PauseMusic();
        
        void ResumeMusic();
        
        void Init(const RhythmMap *map);
        
        void Release();
        
        static SoundPlayer &GetGlobal();
        
    private:
        Mix_Music *m_music;
        Mix_Chunk **m_hitsounds;
    };
}

#endif
