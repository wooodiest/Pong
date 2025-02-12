#pragma once

#include "Paddle.h"

namespace Pong {

	enum class PlayerType
	{
		Left = 0,
		Right
	};

	class Player
	{
	public:
		Player(PlayerType type);

		virtual ~Player() = default;

		virtual void OnUpdate(float dt) = 0;

		void Render() const;

		void Scored();

		Paddle&    GetPaddle()      { return m_Paddle; }
		PlayerType GetType()  const { return m_Type;   }
		int        GetScore() const { return m_Score;  }

	protected:
		Paddle     m_Paddle;
		PlayerType m_Type;
		int        m_Score = 0;

	};

}