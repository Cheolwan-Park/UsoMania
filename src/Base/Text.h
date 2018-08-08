#ifndef UsoManiaText_hpp
#define UsoManiaText_hpp

#include "Sprite.h"

namespace GamesEngine
{
    class CharSprite : public Sprite
    {
    public:
        struct Font
        {
            static constexpr Uint32 ASCII_DELTA = 32;
            static constexpr Uint32 HEIGHT_DELTA = 5;
            Uint16 row;
            Uint16 col;
            const Texture *tex;
        };
        
    public:
        CharSprite() = delete;
        
        CharSprite(Uint32 id, Bool isStatic = false);
        
        CharSprite(Uint32 id, const GameObject *parent, Bool isStatic = false);
        
        CharSprite(const CharSprite &other);
        
        virtual ~CharSprite();
        
        CharSprite &operator=(const CharSprite &other);
        
        void SetFont(const Font &font);
        
        void SetChar(char8 c);
        
    private:
        Font m_font;
    };
    
    class Text : public DrawAble
    {
    public:
        Text() = delete;
        
        Text(Uint32 id, Bool isStatic = false);
        
        Text(Uint32 id, const GameObject *parent, Bool isStatic = false);
        
        Text(const Text &other) = delete;
        
        virtual ~Text();
        
        Text &operator=(const Text &other) = delete;
        
        virtual void Start();
        
        virtual void Update();
        
        virtual void Draw();
        
        virtual void UpdateVBO();
        
        void AssignMemory(void *mem, Uint32 len);
        
        void SetFont(const CharSprite::Font &font);
        
        void SetString(const char *str, Uint32 len);
        
        void UpdateCharsPos();
        
    private:
        Uint32 m_len;
        CharSprite::Font m_font;
        CharSprite *m_chars;
    };
}

#endif
