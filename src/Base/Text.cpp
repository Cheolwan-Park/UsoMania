#include "Text.h"

namespace GamesEngine
{
    // CharSprite class
    CharSprite::CharSprite(Uint32 id, Bool isStatic)
    :Sprite(id, isStatic), m_font({1, 1, nullptr})
    {
        ;
    }
    
    CharSprite::CharSprite(Uint32 id, const GameObject *parent, Bool isStatic)
    :Sprite(id, parent, isStatic), m_font({1, 1, nullptr})
    {
        ;
    }
    
    CharSprite::CharSprite(const CharSprite &other)
    :Sprite(other), m_font(other.m_font)
    {
        ;
    }
    
    CharSprite::~CharSprite()
    {
        ;
    }
    
    CharSprite &CharSprite::operator=(const CharSprite &other)
    {
        Sprite::operator=(other);
        this->m_font = other.m_font;
        return (*this);
    }
    
    void CharSprite::SetFont(const CharSprite::Font &font)
    {
        m_font = font;
        SetTexture(m_font.tex);
        if(nullptr != m_font.tex)
        {
            float32 w = (m_font.tex->w)/(m_font.col);
            float32 h = (m_font.tex->h)/(m_font.row);
            SetScale(1.0f, (float32)h/w);
        }
    }
    
    void CharSprite::SetChar(char8 c)
    {
        if(nullptr != m_font.tex)
        {
            c = c - CharSprite::Font::ASCII_DELTA;
            auto *tex = m_font.tex;
            Uint16 x = c%(m_font.col);
            Uint16 y = c/(m_font.col);
            Math::IRect uv = {0, 0, (tex->w)/(m_font.col), (tex->h)/(m_font.row)};
            uv.x = uv.w*x;
            uv.y = (m_font.tex->h) - uv.h*(y+1);
            uv.h -= Font::HEIGHT_DELTA;
            uv.y += Font::HEIGHT_DELTA;
            SetUV(uv);
        }
    }
    
    
    // Text class
    
    Text::Text(Uint32 id, Bool isStatic)
    :DrawAble(id, isStatic), m_len(0), m_font({1,1,nullptr}), m_chars(nullptr)
    {
        ;
    }
    
    Text::Text(Uint32 id, const GameObject *parent, Bool isStatic)
    :DrawAble(id, parent, isStatic), m_len(0), m_font({1,1,nullptr}), m_chars(nullptr)
    {
        ;
    }
    
    Text::~Text()
    {
        for(Uint32 i=0; i<m_len; ++i)
            m_chars[i].~CharSprite();
    }
    
    void Text::Start()
    {
        for(Uint32 i=0; i<m_len; ++i)
            m_chars[i].Start();
    }
    
    void Text::Update()
    {
        for(Uint32 i=0; i<m_len; ++i)
            m_chars[i].Update();
    }
    
    void Text::Draw()
    {
        for(Uint32 i=0; i<m_len; ++i)
            m_chars[i].Draw();
    }
    
    void Text::UpdateVBO()
    {
        for(Uint32 i=0; i<m_len; ++i)
            m_chars[i].UpdateVBO();
    }
    
    void Text::AssignMemory(void *mem, Uint32 len)
    {
        m_len = len;
        m_chars = (CharSprite*)mem;
        
        CharSprite *chr = nullptr;
        for(Uint32 i=0; i<m_len; ++i)
        {
            chr = new (m_chars + i) CharSprite("char"_hash + i, this, isStatic());
            chr->SetFont(m_font);
            chr->SetChar(' ');
        }
    }
    
    void Text::SetFont(const CharSprite::Font &font)
    {
        m_font = font;
        for(Uint32 i=0; i<m_len; ++i)
        {
            m_chars[i].SetFont(m_font);
            m_chars[i].SetChar(' ');
        }
        UpdateCharsPos();
    }
    
    void Text::SetString(const char *str, Uint32 len)
    {
        len = len < m_len ? len : m_len;
        for(Uint32 i=0; i<len; ++i)
        {
            m_chars[i].SetAvailable(true);
            if(31 < str[i])
                m_chars[i].SetChar(str[i]);
            else
                m_chars[i].SetAvailable(false);
        }
        for(Uint32 i=len; i<m_len; ++i)
        {
            m_chars[i].SetAvailable(false);
        }
    }
    
    void Text::UpdateCharsPos()
    {
        const float32 scale = GetScale().x;
        glm::vec3 pos({0.0f, 0.0f, 0.0f});
        for(Uint32 i=0; i<m_len; ++i)
        {
            m_chars[i].Scale(scale);
            m_chars[i].SetLocalPosition(pos);
            pos.x += scale;
        }
    }
}








