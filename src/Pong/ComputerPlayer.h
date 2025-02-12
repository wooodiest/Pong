#pragma once

#include "Player.h"
#include "Ball.h"

#include <vector>

namespace Pong {

    constexpr float computer_player_dead_zone              = 0.07f;
    constexpr float computer_player_easy_speed_factor      = 0.7f;
    constexpr float computer_player_normal_speed_factor    = 1.0f;
    constexpr float computer_player_hard_speed_factor      = 1.2f;
    constexpr float computer_player_easy_error_margin      = 0.7f;
    constexpr float computer_player_normal_error_margin    = 0.4f;
    constexpr float computer_player_hard_prediction_factor = 0.2f;

    enum class ComputerPlayerDifficulty
    {
        EASY,
        NORMAL,
        HARD
    };

    class ComputerPlayer : public Player
    {
    public:
        ComputerPlayer(PlayerType type, ComputerPlayerDifficulty difficulty);

        virtual ~ComputerPlayer(); 

        virtual void OnUpdate(float dt) override;

        void TrackBalls(const std::vector<Ball>& balls);

    private:
        void UpdatePaddlePosition(float ball_y, float paddle_center_y, float speed_factor, float offset);

    private:
        ComputerPlayerDifficulty m_Difficulty = ComputerPlayerDifficulty::NORMAL;
        const Ball* m_TargetBall = nullptr;

    };

}