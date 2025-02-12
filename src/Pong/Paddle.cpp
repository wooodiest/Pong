#include "Paddle.h"

#include "Application.h"

namespace Pong {

	Paddle::~Paddle()
	{
	}

    Paddle::Paddle(PaddleType type)
        : m_Type(type)
    {
    }

    void Paddle::OnUpdate(float dt, PaddleMove move)
    {
        float speed = (board_bottom_bound - board_top_bound) / m_TravelTime;

        float targetY = m_Position.y;
        switch (move)
        {
            case PaddleMove::Up:
                targetY -= speed * dt;
                break;
            case PaddleMove::Down:
                targetY += speed * dt;
                break;
        }

        m_Position.y = Lerp(m_Position.y, targetY, 1.0f - powf(0.1f, dt * target_fps));

        if (m_Position.y < board_top_bound)
            m_Position.y = board_top_bound;
        else if (m_Position.y + m_Size.y > board_bottom_bound)
            m_Position.y = board_bottom_bound - m_Size.y;
    }

	void Paddle::Render() const
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

    void Paddle::ResetSize()
    {
        m_Size = { paddle_size_screen_percentage_x, paddle_size_screen_percentage_y };
    }

    void Paddle::ResetPosition()
    {
        switch (m_Type)
        {
            case PaddleType::None: 
            {
                m_Position = { board_center_X, board_center_Y };
                break;
            }
            case PaddleType::Left:
            {
                m_Position.x = paddle_offset;
                m_Position.y = board_center_Y - (m_Size.y / 2.0f);
                break;
            }
            case PaddleType::Right:
            {
                m_Position.x = board_right_bound - paddle_offset - m_Size.x;
                m_Position.y = board_center_Y - (m_Size.y / 2.0f);
                break;
            }
        }
    }

    void Paddle::Reset()
    {
        ResetSize();
        ResetPosition();
    }

}

