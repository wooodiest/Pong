#pragma once

#include <vector>

#include "Ball.h"
#include "Paddle.h"
#include "Obstacle.h"
#include "Player.h"
#include "HumanPlayer.h"
#include "ComputerPlayer.h"

namespace Pong {

	constexpr inline int max_rounds           = 9;
	constexpr inline int max_ball_count       = 10;
	constexpr inline int max_static_obstacles = 5;
	constexpr inline int max_moving_obstacles = 5;

	struct PongSetting
	{
		bool LeftPlayerComputer  = false;
		bool RightPlayerComputer = false;
		ComputerPlayerDifficulty LeftPlayerDifficulty  = ComputerPlayerDifficulty::NORMAL;
		ComputerPlayerDifficulty RightPlayerDifficulty = ComputerPlayerDifficulty::NORMAL;

		int	Rounds          = 3;
		int BallCount       = 1;
		int StaticObstacles = 0;
		int MovingObstacles = 0;
	};

	class Pong
	{
	public:
		Pong(const PongSetting& settings);
		~Pong();

		void OnUpdate(float dt);
		void Render() const;

		bool IsGameOver()    const { return m_GameOver;   }
		bool GetQuitStatus() const { return m_QuitStatus; }

	private:
		void SetUpPlayers();
		void Reset();

	private:
		PongSetting       m_Settings;
		bool m_GameOver   = false;
		bool m_QuitStatus = false;
		int  m_Rounds     = 0;

		std::vector<Ball>     m_Balls;
		std::vector<Obstacle> m_Obstacles;
		Player* m_LeftPlayer  = nullptr;
		Player* m_RightPlayer = nullptr;

		
	};

}