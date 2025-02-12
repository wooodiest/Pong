#include "Utils.h"

#include <raymath.h>

namespace Pong::Utils {

	void DrawCenteredText(const std::string& text, int fontSize, Color color)
	{
		int textWidth  = MeasureText(text.c_str(), fontSize);
		int textHeight = fontSize;

		int posX = (GetScreenWidth() - textWidth) / 2;
		int posY = (GetScreenHeight() - textHeight) / 2;

		DrawText(text.c_str(), posX, posY, fontSize, color);
	}

	void DrawCenteredTextHorizontal(const std::string& text, int y, int fontSize, Color color)
	{
		int textWidth = MeasureText(text.c_str(), fontSize);
		int posX      = (GetScreenWidth() - textWidth) / 2;

		DrawText(text.c_str(), posX, y, fontSize, color);
	}

	void DrawCenteredTextVertical(const std::string& text, int x, int fontSize, Color color)
	{
		int textHeight = fontSize;
		int posY       = (GetScreenHeight() - textHeight) / 2;

		DrawText(text.c_str(), x, posY, fontSize, color);
	}

	float RandomFloat(float min, float max)
	{
		return min + (max - min) * (static_cast<float>(GetRandomValue(0, RAND_MAX)) / static_cast<float>(RAND_MAX));
	}

	Vector2 RandomVec2(Vector2 min, Vector2 max)
	{
		return { 
			RandomFloat(min.x, max.x), 
			RandomFloat(min.y, max.y)
		};
	}

	Color RandomColor()
	{
		return Color(
			GetRandomValue(0, 255),
			GetRandomValue(0, 255),
			GetRandomValue(0, 255),
			255                  );
	}

}
