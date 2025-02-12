#include "Pong.h"

#include "Utils.h"

namespace Pong {

	Pong::Pong(const PongSetting& settings)
		: m_Settings(settings)
	{
		SetUpPlayers();
		Reset();
	}

	Pong::~Pong()
	{
	}

	void Pong::OnUpdate(float dt)
	{
		for (auto& obstacle : m_Obstacles)
			obstacle.OnUpdate(dt);

		if (m_GameOver)
		{
			if (IsKeyPressed(KEY_ENTER))
				m_QuitStatus = true;

			return;
		}

		if (auto* computerPlayer = dynamic_cast<ComputerPlayer*>(m_LeftPlayer))
			computerPlayer->TrackBalls(m_Balls);

		if (auto* computerPlayer = dynamic_cast<ComputerPlayer*>(m_RightPlayer))
			computerPlayer->TrackBalls(m_Balls);

		m_LeftPlayer->OnUpdate(dt);
		m_RightPlayer->OnUpdate(dt);

		for (auto it = m_Balls.begin(); it != m_Balls.end(); )
		{
			auto& ball = *it;
			ball.OnUpdate(dt);

			ball.ReflectOnRectangle(m_LeftPlayer->GetPaddle().GetBoundingBox());
			ball.ReflectOnRectangle(m_RightPlayer->GetPaddle().GetBoundingBox());

			for (size_t j = 0; j < m_Obstacles.size(); j++)
				ball.ReflectOnRectangle(m_Obstacles[j].GetBoundingBox());

			for (auto jt = it + 1; jt != m_Balls.end(); ++jt)
				ball.CheckCollision(*jt);

			ball.CheckOutOfBounds();

			if (ball.GetStatus() != BallStatus::InBounds)
			{
				switch (ball.GetStatus())
				{
					case BallStatus::OutOfBoudsLeft:
					{
						m_RightPlayer->Scored();
						it = m_Balls.erase(it);
						break;
					}
					case BallStatus::OutOfBoudsRight:
					{
						m_LeftPlayer->Scored();
						it = m_Balls.erase(it);
						break;
					}
					default: {
						++it;
					}
				}
			}
			else
				++it;

			if (m_Balls.empty())
			{
				m_Rounds++;

				if (m_Rounds >= m_Settings.Rounds)
					m_GameOver = true;
				else
					Reset();
				
			}
		}	
	}

	void Pong::Render() const
	{
		for (const auto& ball : m_Balls)
			ball.Render();

		for (const auto& obstacle : m_Obstacles)
			obstacle.Render();

		m_LeftPlayer->Render();
		m_RightPlayer->Render();

		int padding  = 20;
		int fontSize = 30;

		std::string player1Info = "Player 1: " + std::to_string(m_LeftPlayer->GetScore());
		DrawText(player1Info.c_str(), padding, padding, fontSize, LIGHTGRAY);

		std::string player2Info = "Player 2: " + std::to_string(m_RightPlayer->GetScore());
		DrawText(player2Info.c_str(), GetScreenWidth() - MeasureText(player2Info.c_str(), fontSize) - padding, padding, fontSize, LIGHTGRAY);

		if (m_GameOver)
		{
			int offset  = -150;
			int spacing = 60; 

			Utils::DrawCenteredTextHorizontal("GAME OVER", (GetScreenHeight() / 2) + offset, 100, LIGHTGRAY);

			std::string winnerInfo;
			if (m_LeftPlayer->GetScore() > m_RightPlayer->GetScore())
				winnerInfo = "Player 1 Wins!";
			else if (m_RightPlayer->GetScore() > m_LeftPlayer->GetScore())
				winnerInfo = "Player 2 Wins!";
			else
				winnerInfo = "It's a Draw!";
			Utils::DrawCenteredTextHorizontal(winnerInfo.c_str(), (GetScreenHeight() / 2) + spacing + offset / 1.5f, 50, LIGHTGRAY);

			std::string scoreInfo = "Final Score: " +
				std::to_string(m_LeftPlayer->GetScore()) + " - " +
				std::to_string(m_RightPlayer->GetScore());
			Utils::DrawCenteredTextHorizontal(scoreInfo.c_str(), (GetScreenHeight() / 2) + spacing * 2 + offset / 1.5f, 40, LIGHTGRAY);

			Utils::DrawCenteredTextHorizontal("Press ENTER to return to menu", (GetScreenHeight() / 2) + spacing * 4 + offset / 2, 30, LIGHTGRAY);
		}
	}

	void Pong::SetUpPlayers()
	{
		if (m_Settings.LeftPlayerComputer)
			m_LeftPlayer = new ComputerPlayer(PlayerType::Left, m_Settings.LeftPlayerDifficulty);
		else
			m_LeftPlayer = new HumanPlayer(PlayerType::Left);


		if (m_Settings.RightPlayerComputer)
			m_RightPlayer = new ComputerPlayer(PlayerType::Right, m_Settings.RightPlayerDifficulty);
		else
			m_RightPlayer = new HumanPlayer(PlayerType::Right);
	}

	void Pong::Reset()
	{
		m_Balls.clear();
		m_Obstacles.clear();

		for (int i = 0; i < m_Settings.BallCount; i++)
		{
			Ball ball;
			ball.SetPosition(Utils::RandomVec2({ 0.45f, 0.1f }, { 0.55f, 0.9f }));
			ball.SetColor(Utils::RandomColor());

			Vector2 direction = { 1.0f, Utils::RandomFloat(-1.0f, 1.0f) };
			ball.SetSpeed(Vector2Normalize(direction) * Utils::RandomFloat(0.45f, 0.5f));

			m_Balls.push_back(ball);
		}

		for (int i = 0; i < m_Settings.StaticObstacles; i++)
		{
			Obstacle obstacle;
			obstacle.SetPosition(Utils::RandomVec2({ 0.15f, 0.1f }, { 0.85f, 0.9f }));
			obstacle.SetSize(Utils::RandomVec2({ 0.05f, 0.05f }, { 0.1, 0.1f }));
			obstacle.SetColor(Utils::RandomColor());

			m_Obstacles.push_back(obstacle);
		}

		for (int i = 0; i < m_Settings.MovingObstacles; i++)
		{
			Obstacle obstacle;
			obstacle.SetPosition(Utils::RandomVec2({ 0.15f, 0.1f }, { 0.85f, 0.9f }));
			obstacle.SetSize(Utils::RandomVec2({ 0.05f, 0.05f }, { 0.1f, 0.1f }));
			obstacle.SetColor(Utils::RandomColor());

			int     randomNumer = GetRandomValue(0, 2);
			Vector2 minSpeed = { -0.3f, -0.3f };
			Vector2 maxSpeed = {  0.3f,  0.3f };
			switch (randomNumer)
			{
				case 0: obstacle.SetSpeed({ 0.0f, Utils::RandomFloat(minSpeed.y, maxSpeed.y) }); break;
				case 1: obstacle.SetSpeed({ Utils::RandomFloat(minSpeed.x, maxSpeed.x), 0.0f }); break;
				case 2: obstacle.SetSpeed(Utils::RandomVec2(minSpeed, maxSpeed));              break;
			}

			m_Obstacles.push_back(obstacle);
		}
	}

}
