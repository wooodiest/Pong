#include "ComputerPlayer.h"

#include "Utils.h"

namespace Pong {

	ComputerPlayer::ComputerPlayer(PlayerType type, ComputerPlayerDifficulty difficulty)
		: Player(type), m_Difficulty(difficulty)
	{

	}

	ComputerPlayer::~ComputerPlayer()
	{

	}

	void ComputerPlayer::OnUpdate(float dt)
	{
        if (!m_TargetBall)
            return;

        float paddle_center_y = m_Paddle.GetPosition().y + m_Paddle.GetSize().y / 2.0f;
        float ball_y          = m_TargetBall->GetPosition().y;

        if (fabs(ball_y - paddle_center_y) < computer_player_dead_zone)
            return;

        switch (m_Difficulty)
        {
            case ComputerPlayerDifficulty::EASY:
            {
                float error_margin = m_Paddle.GetSize().y * computer_player_easy_error_margin;
                ball_y += Utils::RandomFloat(-error_margin, error_margin);
                UpdatePaddlePosition(ball_y, paddle_center_y, dt * computer_player_easy_speed_factor, 0.2f);
                break;
            }
            case ComputerPlayerDifficulty::NORMAL:
            {
                float error_margin = m_Paddle.GetSize().y * computer_player_normal_error_margin;
                ball_y += Utils::RandomFloat(-error_margin, error_margin);
                UpdatePaddlePosition(ball_y, paddle_center_y, dt * computer_player_normal_speed_factor, 0.1f);
                break;
            }
            case ComputerPlayerDifficulty::HARD:
            {
                float predicted_ball_y = ball_y + m_TargetBall->GetSpeed().y * computer_player_hard_prediction_factor;
                UpdatePaddlePosition(predicted_ball_y, paddle_center_y, dt * computer_player_hard_speed_factor, 0.0f);
                break;
            }
        }
	}

	void ComputerPlayer::TrackBalls(const std::vector<Ball>& balls)
	{
        if (balls.empty())
        {
            m_TargetBall = nullptr;
            return;
        }

        float closestDistance = FLT_MAX;
        float trackingDistanceThreshold = 0.0f;

        switch (m_Difficulty)
        {
            case ComputerPlayerDifficulty::EASY:   trackingDistanceThreshold = 0.8f; break;
            case ComputerPlayerDifficulty::NORMAL: trackingDistanceThreshold = 0.5f; break;
            case ComputerPlayerDifficulty::HARD:   trackingDistanceThreshold = 0.3f; break;
        }

        const Ball* bestTarget = nullptr;
        for (const Ball& ball : balls)
        {
            bool isMovingTowardsPaddle = (m_Type == PlayerType::Left && ball.GetSpeed().x < 0) ||
                (m_Type == PlayerType::Right && ball.GetSpeed().x > 0);

            if (isMovingTowardsPaddle)
            {
                float distance = fabs(m_Paddle.GetPosition().x - ball.GetPosition().x);
                if (distance <= trackingDistanceThreshold && distance < closestDistance)
                {
                    closestDistance = distance;
                    bestTarget = &ball;
                }
            }
        }

        m_TargetBall = bestTarget;
	}

    void ComputerPlayer::UpdatePaddlePosition(float ball_y, float paddle_center_y, float speed_factor, float offset)
    {
        if (ball_y < paddle_center_y - m_Paddle.GetSize().y * offset)
            m_Paddle.OnUpdate(speed_factor, PaddleMove::Up);
        else if (ball_y > paddle_center_y + m_Paddle.GetSize().y * offset)
            m_Paddle.OnUpdate(speed_factor, PaddleMove::Down);
    }

}