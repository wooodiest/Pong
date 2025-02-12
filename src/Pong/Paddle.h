#pragma once

#include <raylib.h>
#include <raymath.h>

namespace Pong {

	inline constexpr float paddle_size_screen_percentage_x = 0.03f;
	inline constexpr float paddle_size_screen_percentage_y = 0.15f;
	inline constexpr float paddle_travel_time = 0.75f;
	inline constexpr float paddle_offset      = 0.05f;

	enum class PaddleMove
	{
		None = 0,
		Up,
		Down
	};

	enum class PaddleType
	{
		None = 0,
		Left,
		Right
	};

	class Paddle
	{
	public:
		Paddle() = default;
		Paddle(PaddleType type);
		~Paddle();

		void OnUpdate(float dt, PaddleMove move);
		void Render() const;

		void SetPosition  (Vector2 position)   { m_Position   = position;   }
		void SetSize      (Vector2 size)       { m_Size       = size;       }
		void SetTravelTime(float   travelTime) { m_TravelTime = travelTime; }
		void SetColor     (Color   color)      { m_Color      = color;      }

		Rectangle GetBoundingBox() const { 
			return { m_Position.x, m_Position.y,
				     m_Size.x,     m_Size.y     };
		}
		Vector2    GetPosition()   const { return m_Position;   }
		Vector2    GetSize()       const { return m_Size;       }
		float      GetTravelTime() const { return m_TravelTime; }
		Color      GetColor()      const { return m_Color;      }
		PaddleType GetType()       const { return m_Type;       }

		void Reset();
		void ResetSize();
		void ResetPosition();

	private:
		PaddleType m_Type       = PaddleType::None;
		Vector2    m_Position   = { 0.0f,  0.0f   };
		Vector2    m_Size       = { paddle_size_screen_percentage_x, paddle_size_screen_percentage_y };
		Color      m_Color      = ORANGE;
		float      m_TravelTime = paddle_travel_time;
	};

}