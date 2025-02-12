#pragma once

#include <raylib.h>
#include <raymath.h>

namespace Pong {

	class Obstacle 
	{
	public:
        Obstacle() = default;
        Obstacle(Vector2 position, Vector2 speed, Vector2 size, Color color);

        ~Obstacle();

        void OnUpdate(float dt);
        void Render() const;

        void SetPosition(Vector2 position) { m_Position = position; }
        void SetSpeed   (Vector2 speed)    { m_Speed    = speed;    }
        void SetSize    (Vector2 size)     { m_Size     = size;     }
        void SetColor   (Color   color)    { m_Color    = color;    }

        Rectangle GetBoundingBox() const  {
            return { m_Position.x, m_Position.y,
                     m_Size.x,     m_Size.y };
        }
        Vector2 GetPosition() const { return m_Position; }
        Vector2 GetSpeed()    const { return m_Speed;    }
        Vector2 GetSize()     const { return m_Size;     }
        Color   GetColor()    const { return m_Color;    }

    protected:
        Vector2 m_Position      = { 0.5f, 0.5f };
        Vector2 m_Speed         = { 0.0f, 0.0f };
        Vector2 m_Size          = { 0.1f, 0.02f };   
        Color   m_Color         = RED;
	};

}