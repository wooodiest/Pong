#pragma once

#include "Player.h"

namespace Pong {

	class HumanPlayer : public Player
	{
	public:
		HumanPlayer(PlayerType type);

		virtual ~HumanPlayer();

		virtual void OnUpdate(float dt) override;
	};

}