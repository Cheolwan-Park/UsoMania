#ifndef GamesEngineVector_hpp
#define GamesEngineVector_hpp

#include "types.h"

namespace GamesEngine
{
    namespace Math
    {
        class Vector2
        {
        public:
            Vector2();
            
            Vector2(float32 x, float32 y);
            
            Vector2(const Vector2 &other);
            
            ~Vector2();
            
            void Rotate(float32 angle);
            
            void Normalize();
            
            Vector2 GetRotated(float32 angle)const;
            
            Vector2 GetNormalized()const;
            
            Vector2 &operator=(const Vector2 &other);
            
            Vector2 &operator+=(const Vector2 &other);
            
            Vector2 &operator-=(const Vector2 &other);
            
            Vector2 &operator*=(float32 scala);
            
            Vector2 &operator/=(float32 scala);
            
            friend bool operator==(const Vector2 &lhs, const Vector2 &rhs);
            
            friend bool operator!=(const Vector2 &lhs, const Vector2 &rhs);
            
            friend Vector2 operator+(Vector2 lhs, const Vector2 &rhs);
            
            friend Vector2 operator-(Vector2 lhs, const Vector2 &rhs);
            
            friend Vector2 operator*(Vector2 vec, float32 scala);
            
            friend float operator*(const Vector2 &lhs, const Vector2 &rhs);
            
            friend Vector2 operator/(Vector2 vec, float32 scala);
            
            // members
            float32 x;
            float32 y;
        };
        
        bool operator==(const Vector2 &lhs, const Vector2 &rhs);
        
        bool operator!=(const Vector2 &lhs, const Vector2 &rhs);
        
        Vector2 operator+(Vector2 lhs, const Vector2 &rhs);
        
        Vector2 operator-(Vector2 lhs, const Vector2 &rhs);
        
        Vector2 operator*(Vector2 vec, float32 scala);
        
        Vector2 operator*(Vector2 lhs, const Vector2 &rhs);
        
        Vector2 operator/(Vector2 vec, float32 scala);
        
    }
}

#endif


