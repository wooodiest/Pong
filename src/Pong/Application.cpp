#include "Application.h"

#include <raylib.h>
#include <fstream>
#include <json.hpp>

#include "Utils.h"

namespace Pong {

	static std::vector<std::string> s_MenuItems = { "Continue Game", "New Game", "Settings", "Exit" };
	static std::vector<std::string> s_SettingsItems = { "Left Player: ", "Right Player: ", "Rounds: ", "Ball Count: " , "Static Obstacles: ", "Moving Obstacles: ", "Back" };

	Application::Application()
	{
		Init();
		Run();
	}

	Application::~Application()
	{
		Shutdown();
	}

	void Application::Init()
	{
		InitWindow(window_width, window_height, "Pong");
		SetWindowState(FLAG_WINDOW_RESIZABLE); 
		SetWindowMinSize(window_min_width, window_min_height);
		SetTargetFPS(target_fps);

		Deserialize();
	}

	void Application::Run()
	{
		while (m_Running && !WindowShouldClose())
		{
			OnUpdate(GetFrameTime());
			BeginDrawing();
			ClearBackground(DARKGRAY);
			Render();
			EndDrawing();
		}
	}

	void Application::Shutdown()
	{
		Serialize();
		CloseWindow();
	}

	void Application::OnUpdate(float dt)
	{
		switch (m_State)
		{
			case ApplicationState::LOGO:
			{
				if (IsKeyPressed(KEY_ENTER) || GetTime() >= logo_display_time)
					m_State = ApplicationState::MENU;  
				
				break;
			}

			case ApplicationState::MENU:
			{
				if (m_InSettings)
					UpdateSettings();
				else
					UpdateMainMenu();
				break;
			}

			case ApplicationState::GAME:
			{
				if (m_Game)
				{
					if (IsKeyPressed(KEY_SPACE) && !m_Game->IsGameOver())
						m_IsPaused = !m_IsPaused;

					if (m_IsPaused)
						UpdatePauseMenu();
					else
						m_Game->OnUpdate(dt);

					if (m_Game->GetQuitStatus())
					{
						delete m_Game;
						m_Game  = nullptr;
						m_State = ApplicationState::MENU;
					}
				}
				break;
			}

		}
	}

	void Application::Render() const
	{
		switch (m_State)
		{
			case ApplicationState::LOGO:
			{
				{
					int   fontSize = 100;
					Color color    = LIGHTGRAY;
					Utils::DrawCenteredText("PONG", fontSize, color);
				}
				{
					int   fontSize = 40;
					Color color    = LIGHTGRAY;
					int   padding  = 7;
					DrawText("Michal Kuchnicki", padding, GetScreenHeight() - fontSize, fontSize, color);
				}				

				break;
			}

			case ApplicationState::MENU: 
			{
				if (m_InSettings)
					RenderSettings();
				else
					RenderMainMenu();

				break;
			}

			case ApplicationState::GAME:
			{
				if (m_Game)
					m_Game->Render();

				if (m_IsPaused)
					RenderPauseMenu();

				break;
			}

		}
	}

	void Application::RenderMainMenu() const
	{
		int fontSize = 30;
		int padding  = 50;
		int startY   = (GetScreenHeight() - (s_MenuItems.size() * (fontSize + padding))) / 2;

		for (size_t i = (m_Game ? 0 : 1); i < s_MenuItems.size(); i++)
		{
			Color color = (static_cast<MenuOption>(i) == m_CurrentMenuSelection) ? YELLOW : LIGHTGRAY;
			Utils::DrawCenteredTextHorizontal(s_MenuItems[i], startY + i * (fontSize + padding), fontSize, color);
		}

		RenderControls();
	}

	void Application::RenderSettings() const
	{
		auto settingItems = s_SettingsItems;
		settingItems[0] += GetPlayerTypeString(m_GameSettings.LeftPlayerComputer, m_GameSettings.LeftPlayerDifficulty);
		settingItems[1] += GetPlayerTypeString(m_GameSettings.RightPlayerComputer, m_GameSettings.RightPlayerDifficulty);
		settingItems[2] += std::to_string(m_GameSettings.Rounds);
		settingItems[3] += std::to_string(m_GameSettings.BallCount);
		settingItems[4] += std::to_string(m_GameSettings.StaticObstacles);
		settingItems[5] += std::to_string(m_GameSettings.MovingObstacles);

		int fontSize = 30;
		int padding  = 40;
		int startY   = (GetScreenHeight() - s_SettingsItems.size() * (fontSize + padding)) / 2;

		for (size_t i = 0; i < settingItems.size(); i++)
		{
			Color color = (m_CurrentSettingSelection == i) ? YELLOW : LIGHTGRAY;
			Utils::DrawCenteredTextHorizontal(settingItems[i], startY + i * (fontSize + padding), fontSize, color);
		}

	}

	void Application::RenderPauseMenu() const
	{
		int rectWidth  = GetScreenWidth()  / 2;
		int rectHeight = GetScreenHeight() / 3;
		int rectX = (GetScreenWidth()  - rectWidth)  / 2;
		int rectY = (GetScreenHeight() - rectHeight) / 2;

		DrawRectangle(rectX, rectY, rectWidth, rectHeight, Fade(BLACK, 0.55f));

		std::vector<std::string> pauseMenuOptions = { "Continue", "Back to Menu" };

		int fontSize = 40;
		int padding = 20;
		int startY = rectY + (rectHeight - (pauseMenuOptions.size() * (fontSize + padding))) / 2;

		for (size_t i = 0; i < pauseMenuOptions.size(); i++)
		{
			Color color = (m_PauseMenuSelection == i) ? YELLOW : LIGHTGRAY;
			Utils::DrawCenteredTextHorizontal(pauseMenuOptions[i], startY + i * (fontSize + padding), fontSize, color);
		}

		RenderControls();
	}

	void Application::RenderControls() const
	{
		int padding = 70;
		int controlFontSize = 20;
		std::string controls =
			"Controls:\n"
			"Player 1: W - Up, S - Down\n"
			"Player 2: Arrow Up - Up, Arrow Down - Down\n"
			"Space - Pause\n"
			"Arrow Keys - Navigate Menu\n"
			"Enter - Select";

		DrawText(controls.c_str(), padding, padding, controlFontSize, LIGHTGRAY);
	}

	void Application::UpdateMainMenu()
	{
		if (IsKeyPressed(KEY_DOWN))
			m_CurrentMenuSelection = static_cast<MenuOption>((static_cast<int>(m_CurrentMenuSelection) + 1) % s_MenuItems.size());
		if (IsKeyPressed(KEY_UP))
			m_CurrentMenuSelection = static_cast<MenuOption>((static_cast<int>(m_CurrentMenuSelection) - 1 + s_MenuItems.size()) % s_MenuItems.size());

		if (!m_Game && m_CurrentMenuSelection == MenuOption::CONTINUE)
			m_CurrentMenuSelection = MenuOption::NEW_GAME;

		if (IsKeyPressed(KEY_ENTER))
		{
			switch (m_CurrentMenuSelection)
			{
				case MenuOption::CONTINUE:
				{
					if (m_Game)
						m_State = ApplicationState::GAME;

					break;
				}
				case MenuOption::NEW_GAME:
				{
					m_Game  = new Pong(m_GameSettings);
					m_State = ApplicationState::GAME;
					break;
				}
				case MenuOption::SETTINGS:
				{
					m_InSettings = true;
					m_CurrentSettingSelection = 0;
					break;
				}
				case MenuOption::EXIT:
				{
					m_Running = false;
					break;
				}
			}
		}
	}

	void Application::UpdateSettings()
	{
		if (IsKeyPressed(KEY_DOWN))
			m_CurrentSettingSelection = (m_CurrentSettingSelection + 1) % s_SettingsItems.size();
		if (IsKeyPressed(KEY_UP))
			m_CurrentSettingSelection = (m_CurrentSettingSelection - 1 + s_SettingsItems.size()) % s_SettingsItems.size();

		if (IsKeyPressed(KEY_ENTER))
		{
			switch (m_CurrentSettingSelection) 
			{
				case 0: CyclePlayerType(m_GameSettings.LeftPlayerComputer,  m_GameSettings.LeftPlayerDifficulty);           break;
				case 1: CyclePlayerType(m_GameSettings.RightPlayerComputer, m_GameSettings.RightPlayerDifficulty);          break;
				case 2: m_GameSettings.Rounds          = (m_GameSettings.Rounds % max_rounds) + 1;                          break;
				case 3: m_GameSettings.BallCount       = (m_GameSettings.BallCount  % max_ball_count)   + 1;                break;
				case 4: m_GameSettings.StaticObstacles = (m_GameSettings.StaticObstacles + 1) % (max_static_obstacles + 1); break;
				case 5: m_GameSettings.MovingObstacles = (m_GameSettings.MovingObstacles + 1) % (max_moving_obstacles + 1); break;
				case 6: m_InSettings = false; break;
			}
		}
	}

	void Application::UpdatePauseMenu()
	{
		if (IsKeyPressed(KEY_DOWN))
			m_PauseMenuSelection = (m_PauseMenuSelection + 1) % 2;

		if (IsKeyPressed(KEY_UP))
			m_PauseMenuSelection = (m_PauseMenuSelection - 1 + 2) % 2;

		if (IsKeyPressed(KEY_ENTER))
		{
			switch (m_PauseMenuSelection)
			{
				case 0: m_IsPaused = false; break;
				case 1: m_IsPaused = false; m_State = ApplicationState::MENU; break;
			}
		}
	}

	std::string Application::GetPlayerTypeString(bool isComputer, ComputerPlayerDifficulty difficulty) const
	{
		if (!isComputer)
			return "Human";

		switch (difficulty)
		{
			case ComputerPlayerDifficulty::EASY:   return "Computer (Easy)";
			case ComputerPlayerDifficulty::NORMAL: return "Computer (Normal)";
			case ComputerPlayerDifficulty::HARD:   return "Computer (Hard)";
			default: return "Unknown";
		}
	}
	void Application::CyclePlayerType(bool& isComputer, ComputerPlayerDifficulty& difficulty)
	{
		if (!isComputer)
		{
			isComputer = true;
			difficulty = ComputerPlayerDifficulty::EASY;
		}
		else if (difficulty == ComputerPlayerDifficulty::EASY)
			difficulty = ComputerPlayerDifficulty::NORMAL;
		else if (difficulty == ComputerPlayerDifficulty::NORMAL)
			difficulty = ComputerPlayerDifficulty::HARD;
		else
			isComputer = false;
	}

	void Application::Serialize() const
	{
		nlohmann::json json;

		json["GameSettings"] = {
			{ "LeftPlayerComputer",         m_GameSettings.LeftPlayerComputer    },
			{ "RightPlayerComputer",        m_GameSettings.RightPlayerComputer   },
			{ "LeftPlayerDifficulty",  (int)m_GameSettings.LeftPlayerDifficulty  },
			{ "RightPlayerDifficulty", (int)m_GameSettings.RightPlayerDifficulty },
			{ "Rounds",                     m_GameSettings.Rounds                },
			{ "BallCount",                  m_GameSettings.BallCount             },
			{ "StaticObstacles",            m_GameSettings.StaticObstacles       },
			{ "MovingObstacles",            m_GameSettings.MovingObstacles       }
		};

		std::ofstream file(app_data_path);
		if (file.is_open())
		{
			file << json.dump(4);
			file.close();
		}
	}

	void Application::Deserialize()
	{
		std::ifstream file(app_data_path);
		if (file.is_open())
		{
			nlohmann::json json;
			file >> json;

			try
			{
				PongSetting gameSettings;
				gameSettings.LeftPlayerComputer    = json["GameSettings"]["LeftPlayerComputer"].get<bool>();
				gameSettings.RightPlayerComputer   = json["GameSettings"]["RightPlayerComputer"].get<bool>();
				gameSettings.LeftPlayerDifficulty  = (ComputerPlayerDifficulty)json["GameSettings"]["LeftPlayerDifficulty"].get<int>();
				gameSettings.RightPlayerDifficulty = (ComputerPlayerDifficulty)json["GameSettings"]["RightPlayerDifficulty"].get<int>();
				gameSettings.Rounds                = json["GameSettings"]["Rounds"].get<int>();
				gameSettings.BallCount             = json["GameSettings"]["BallCount"].get<int>();
				gameSettings.StaticObstacles       = json["GameSettings"]["StaticObstacles"].get<int>();
				gameSettings.MovingObstacles       = json["GameSettings"]["MovingObstacles"].get<int>();
				
				m_GameSettings = gameSettings;
			}
			catch (...)
			{

			}
		}

	}

}


