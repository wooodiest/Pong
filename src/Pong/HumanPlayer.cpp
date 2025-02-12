#include "HumanPlayer.h"

#include <raylib.h>

namespace Pong {

	HumanPlayer::HumanPlayer(PlayerType type)
		: Player(type)
	{
	}

	HumanPlayer::~HumanPlayer()
	{
	}

	void HumanPlayer::OnUpdate(float dt)
	{
		if (IsKeyDown(m_Type == PlayerType::Left ? KEY_W : KEY_UP))
			m_Paddle.OnUpdate(dt, PaddleMove::Up);

		if (IsKeyDown(m_Type == PlayerType::Left ? KEY_S : KEY_DOWN))
			m_Paddle.OnUpdate(dt, PaddleMove::Down);
	}
}
