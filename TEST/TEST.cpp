// TEST.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <raylib.h>
#include <queue>
#define MAX_COLORS_COUNT    23     

bool igual(Vector2 uno, Vector2 dos) {
	return (uno.x == dos.x) && (uno.y == dos.y);
}

int main()
{
	float gameScreenWidth = 800, gameScreenHeight = 660;

	float UIScreenWidth = 800, UIScreenHeight = 60;

	float scale = 1;
	InitWindow(800,660,"PAINT CURSE");
	int radius = 2;
	RenderTexture2D Lienzo = LoadRenderTexture(gameScreenWidth,gameScreenHeight);
	RenderTexture2D UI= LoadRenderTexture(UIScreenWidth, UIScreenHeight);
	Vector2 MousePos = {};
	std::queue<Vector2> MouseBuffer;
	Vector2 previus = GetMousePosition();
	int framecounter = 0;
	Color colors[MAX_COLORS_COUNT] = {RAYWHITE, YELLOW, GOLD, ORANGE, PINK, RED, MAROON, GREEN, LIME, DARKGREEN,
		SKYBLUE, BLUE, DARKBLUE, PURPLE, VIOLET, DARKPURPLE, BEIGE, BROWN, DARKBROWN,
		LIGHTGRAY, GRAY, DARKGRAY, BLACK };
	Rectangle colorsRecs[MAX_COLORS_COUNT] = { 0 };

	for (int i = 0; i < MAX_COLORS_COUNT; i++)
	{
		colorsRecs[i].x = 10 + 30.0f * i + 2 * i;
		colorsRecs[i].y = 10;
		colorsRecs[i].width = 30;
		colorsRecs[i].height = 30;
	}
	int colorSelected = 0;
	int colorSelectedPrev = colorSelected;
	int colorMouseHover = 0;

	Color CURRENTCOLOR = BLACK;

	while (!WindowShouldClose()) {

		framecounter++;
		MousePos = GetMousePosition();

		CURRENTCOLOR = colors[colorSelected];

		if (IsKeyPressed(KEY_RIGHT)) colorSelected++;
		else if (IsKeyPressed(KEY_LEFT)) colorSelected--;

		if (colorSelected >= MAX_COLORS_COUNT) colorSelected = MAX_COLORS_COUNT - 1;
		else if (colorSelected < 0) colorSelected = 0;

		// Choose color with mouse
		for (int i = 0; i < MAX_COLORS_COUNT; i++)
		{
			if (CheckCollisionPointRec(MousePos, colorsRecs[i]))
			{
				colorMouseHover = i;
				break;
			}
			else colorMouseHover = -1;
		}

		if ((colorMouseHover >= 0) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			colorSelected = colorMouseHover;
			colorSelectedPrev = colorSelected;
		}

		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {

			MouseBuffer.push(MousePos);

		}else
		if (framecounter%3==0) {
			previus = GetMousePosition();
		}
		radius += GetMouseWheelMove() * 5;
		if (radius < 2) radius = 2;
		if (radius > 50) radius = 50;
		if ( IsKeyPressed(KEY_S))
		{
			Image image = LoadImageFromTexture(Lienzo.texture);
			ImageFlipVertical(&image);
			ExportImage(image, "my_amazing_texture_painting.png");
			UnloadImage(image);
		}
		BeginTextureMode(Lienzo);
			if (IsKeyPressed(KEY_R)) {
				ClearBackground(WHITE);
			}
			while (!MouseBuffer.empty()) {
				Vector2 aux = MouseBuffer.front();
				DrawCircleV(aux,radius, CURRENTCOLOR);
				DrawLineEx(previus, aux, radius*2, CURRENTCOLOR);

				std::cout << "{" << aux.x << "}{" << aux.y << "}<-dibuje\n ";
				std::cout << "{" << previus.x << "}{" << previus.y << "} prebius\n ";

				previus = aux;
				MouseBuffer.pop();
				//std::cout << "\nlinea\n";

			}
		EndTextureMode();

		BeginTextureMode(UI);
		ClearBackground(WHITE);
			for (int i = 0; i < MAX_COLORS_COUNT; i++) DrawRectangleRec(colorsRecs[i], colors[i]);
			DrawRectangleLines(10, 10, 30, 30, BLACK);
			if (colorMouseHover >= 0) DrawRectangleRec(colorsRecs[colorMouseHover], Fade(WHITE, 0.6f));
			
		EndTextureMode();
			
		BeginDrawing();

			ClearBackground(WHITE);

			DrawTexturePro(Lienzo.texture,  { 0.0f, 0.0f, float(Lienzo.texture.width), float( - Lienzo.texture.height)},
				 {(GetScreenWidth() - (gameScreenWidth * scale)) * 0.5f, (GetScreenHeight() - (gameScreenHeight * scale)) * 0.5f, gameScreenWidth* scale, gameScreenHeight* scale}, { 0, 0 }, 0.0f, WHITE);
			
			DrawTexturePro(UI.texture, { 0.0f, 0.0f, float(UI.texture.width), float(-UI.texture.height) },
				{ (GetScreenWidth() - (UIScreenWidth * scale)), (GetScreenHeight() - (UIScreenHeight * scale))*0.f, UIScreenWidth * scale, UIScreenHeight * scale }, { 0, 0 }, 0.0f, WHITE);
			DrawCircleV(GetMousePosition(), radius, CURRENTCOLOR);
			DrawFPS(0,600);
			
		EndDrawing();
		
	}
	UnloadRenderTexture(Lienzo);
	CloseWindow();

	return 0;

}
