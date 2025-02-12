#pragma once

#include <vector>
#include <string>

#include "Pong.h"
#include "ComputerPlayer.h"

namespace Pong
{
	inline constexpr int   target_fps         = 60;
	inline constexpr int   window_width       = 1600;
	inline constexpr int   window_height      = 900;
	inline constexpr int   window_min_width   = 1280;
	inline constexpr int   window_min_height  = 720;
	inline constexpr float logo_display_time  = 2.0f;

	inline constexpr float board_left_bound   = 0.0f;
	inline constexpr float board_right_bound  = 1.0f;
	inline constexpr float board_top_bound    = 0.0f;
	inline constexpr float board_bottom_bound = 1.0f;
	inline constexpr float board_center_X = (board_right_bound  - board_left_bound) / 2.0f;
	inline constexpr float board_center_Y = (board_bottom_bound - board_top_bound)  / 2.0f;

	inline constexpr const char* app_data_path = "game_data.json";

	enum class ApplicationState
	{
		LOGO = 0,
		MENU,
		GAME
	};

	enum class MenuOption 
	{
		CONTINUE = 0,
		NEW_GAME,
		SETTINGS,
		EXIT
	};

	class Application
	{
	public:
		Application();
		~Application();

	private:
		void Init();
		void Run();
		void Shutdown();

		void OnUpdate(float dt);
		void UpdateMainMenu();
		void UpdateSettings();
		void UpdatePauseMenu();

		void Render() const;
		void RenderMainMenu() const;
		void RenderSettings() const;
		void RenderPauseMenu() const;
		void RenderControls() const;

	private:
		std::string GetPlayerTypeString(bool isComputer, ComputerPlayerDifficulty difficulty) const;
		void CyclePlayerType(bool& isComputer, ComputerPlayerDifficulty& difficulty);

		void Serialize() const;
		void Deserialize();

	private:
		bool             m_Running = true;
		ApplicationState m_State   = ApplicationState::LOGO;
		Pong*            m_Game    = nullptr;
		PongSetting      m_GameSettings;

		MenuOption m_CurrentMenuSelection    = MenuOption::CONTINUE;
		bool       m_InSettings              = false;
		int        m_CurrentSettingSelection = 0;

		bool m_IsPaused           = false; 
		int  m_PauseMenuSelection = 0;
	};
}
