#include "Player.h"

namespace Pong {

	Player::Player(PlayerType type)
		: m_Type(type), m_Paddle(type == PlayerType::Left ? PaddleType::Left : PaddleType::Right)
	{
		m_Paddle.Reset();
	}

	void Player::Render() const
	{
		m_Paddle.Render();
	}

	void Player::Scored()
	{
		m_Score++;
	}

}