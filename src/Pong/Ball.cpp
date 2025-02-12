#include "Ball.h"

#include "Application.h"

namespace Pong {

	Ball::Ball(Vector2 position, Vector2 speed, Color color, float radius)
		: m_Position(position), m_Speed(speed), m_Color(color), m_Radius(radius)
	{

	}

	Ball::~Ball()
	{

	}

	void Ball::OnUpdate(float dt)
	{
		if (fabs(m_Speed.x) < min_speed)
			m_Speed.x = (m_Speed.x < 0) ? -min_speed : min_speed;

		if (fabs(m_Speed.y) < min_speed)
			m_Speed.y = (m_Speed.y < 0) ? -min_speed : min_speed;

		m_Position += m_Speed * dt;	
	}

	void Ball::Render() const
	{
		Vector2 pixelPosition = {
			m_Position.x * GetScreenWidth(),
			m_Position.y * GetScreenHeight()
		};
		float pixelRadius = m_Radius * GetScreenWidth();

		DrawCircle(pixelPosition.x, pixelPosition.y, pixelRadius, m_Color);
	}

	void Ball::ReflectOnRectangle(Rectangle rect)
	{
		if (!CheckCollisionCircleRec(m_Position, m_Radius, rect))
			return;

		float overlapLeft   = (rect.x - (m_Position.x + m_Radius));
		float overlapRight  = ((rect.x + rect.width) - (m_Position.x - m_Radius));
		float overlapTop    = (rect.y - (m_Position.y + m_Radius));
		float overlapBottom = ((rect.y + rect.height) - (m_Position.y - m_Radius));

		float minOverlapX = (fabs(overlapLeft) < fabs(overlapRight)) ? overlapLeft : overlapRight;
		float minOverlapY = (fabs(overlapTop) < fabs(overlapBottom)) ? overlapTop : overlapBottom;

		if (fabs(minOverlapX) < fabs(minOverlapY))
		{
			m_Speed.x    *= -1.0f;
			m_Position.x += minOverlapX;
		}
		else
		{
			m_Speed.y    *= -1.0f;
			m_Position.y += minOverlapY;
		}
	}

	void Ball::CheckCollision(Ball& other)
	{
		Vector2 diff   = m_Position - other.m_Position;
		float distance = Vector2Length(diff);

		if (distance <= m_Radius + other.m_Radius)
		{
			Vector2 normal = diff / distance;

			m_Speed       = m_Speed       - normal * 2.0f * Vector2DotProduct(m_Speed,       normal);
			other.m_Speed = other.m_Speed - normal * 2.0f * Vector2DotProduct(other.m_Speed, normal);

			Vector2 correction = normal * (m_Radius + other.m_Radius - distance) / 2.0f;
			m_Position        += correction;
			other.m_Position  -= correction;
		}
	}

	void Ball::CheckOutOfBounds()
	{
		if (m_Position.y + m_Radius >= board_bottom_bound)
		{
			m_Position.y = board_bottom_bound - m_Radius;
			m_Speed.y   *= -1.0f;

			m_Status = BallStatus::OutOfBoudsBottom;
		}
		else if (m_Position.y - m_Radius <= board_top_bound)
		{
			m_Position.y = board_top_bound + m_Radius;
			m_Speed.y   *= -1.0f;

			m_Status = BallStatus::OutOfBoudsTop;
		}
		else if (m_Position.x + m_Radius >= board_right_bound)
		{
			m_Position.x = board_right_bound - m_Radius;
			m_Speed.x   *= -1.0f;

			m_Status = BallStatus::OutOfBoudsRight;
		}
		else if (m_Position.x - m_Radius <= board_left_bound)
		{
			m_Position.x = board_left_bound + m_Radius;
			m_Speed.x   *= -1.0f;

			m_Status = BallStatus::OutOfBoudsLeft;
		}
		else
			m_Status = BallStatus::InBounds;
	}

	void Ball::ResetSize()
	{
		m_Radius = ball_size_screen_percentage;
	}

}