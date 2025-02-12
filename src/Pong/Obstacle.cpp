#include "Obstacle.h"

#include "Application.h"

namespace Pong {

    Obstacle::Obstacle(Vector2 position, Vector2 speed, Vector2 size, Color color)
		: m_Position(position), m_Speed(speed), m_Size(size), m_Color(color)
	{
	}

    Obstacle::~Obstacle()
    {
    }

    void Obstacle::OnUpdate(float dt)
	{
        m_Position += m_Speed * dt;

        if (m_Position.y < board_top_bound)
        {
            m_Position.y = board_top_bound;
            m_Speed.y   *= -1.0f;
        }
        else if (m_Position.y + m_Size.y > board_bottom_bound)
        {
            m_Position.y = board_bottom_bound - m_Size.y;
            m_Speed.y   *= -1.0f;
        }

        if (m_Position.x < board_left_bound)
        {
            m_Position.x = board_left_bound;
            m_Speed.x   *= -1.0f;
        }
        else if (m_Position.x + m_Size.x > board_right_bound)
        {
            m_Position.x = board_right_bound - m_Size.x;
            m_Speed.x   *= -1.0f;
        }
	}

	void Obstacle::Render() const
	{
        Vector2 pixelPosition = {
               m_Position.x * GetScreenWidth(),
               m_Position.y * GetScreenHeight()
        };

        Vector2 pixelSize = {
            m_Size.x * GetScreenWidth(),
            m_Size.y * GetScreenHeight()
        };

        DrawRectangle(pixelPosition.x, pixelPosition.y, pixelSize.x, pixelSize.y, m_Color);
	}

}
