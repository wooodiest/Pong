#pragma once

#include <raylib.h>
#include <raymath.h>
#include "Obstacle.h"

namespace Pong {

	inline constexpr float ball_size_screen_percentage = 0.01f;
	inline constexpr float min_speed = 0.2f;

	enum class BallStatus
	{
		InBounds,
		OutOfBoudsLeft,
		OutOfBoudsRight,
		OutOfBoudsTop,
		OutOfBoudsBottom,
	};

	class Ball
	{
	public:
		Ball() = default;
		Ball(Vector2 position, Vector2 speed, Color color, float radius);
		~Ball();

		void OnUpdate(float dt);
		void Render() const;

		void SetPosition(Vector2 position) { m_Position = position; }
		void SetSpeed   (Vector2 speed   ) { m_Speed    = speed;    }
		void SetColor   (Color   color   ) { m_Color    = color;    }
		void SetRadius  (float   radius  ) { m_Radius   = radius;   }

		BallStatus GetStatus()   const { return m_Status;   }
		Vector2    GetPosition() const { return m_Position; }
		Vector2    GetSpeed()    const { return m_Speed;    }
		Color      GetColor()    const { return m_Color;    }
		float      GetRadius()   const { return m_Radius;   }

		void ReflectOnRectangle(Rectangle rect);
		void CheckCollision(Ball& other);
		void CheckOutOfBounds();

		void ResetSize();

	private:
		BallStatus m_Status   = BallStatus::InBounds;
		Vector2    m_Position = { 0.5f, 0.5f };
		Vector2    m_Speed    = { 0.0f, 0.0f };
		Color      m_Color    = ORANGE;
		float      m_Radius   = ball_size_screen_percentage;
	};

}