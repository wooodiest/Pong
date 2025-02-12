#pragma once

#include <raylib.h>
#include <json.hpp>
#include <string>

namespace Pong::Utils {

	void DrawCenteredText(const std::string& text, int fontSize, Color color);

	void DrawCenteredTextHorizontal(const std::string& text, int y, int fontSize, Color color);

	void DrawCenteredTextVertical(const std::string& text, int x, int fontSize, Color color);

	float RandomFloat(float min = 0.0f, float max = 1.0f);

	Vector2 RandomVec2(Vector2 min = { 0.0f, 0.0f }, Vector2 max = { 1.0f, 1.0f });

	Color RandomColor();

}