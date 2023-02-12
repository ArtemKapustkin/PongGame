#pragma once
#include "SDL.h"
#include "SDL_ttf.h"
#include <iostream>
#include "Constants.hpp"
#include "Figures.hpp"
#include "Score.hpp"
#include <chrono>

class Game
{
public:
	Game(const char* title, int x, int y, int w, int h);

	~Game();
	
	void InitFigures(GameMode gamemode);

	void UpdateFigures(GameMode gamemode, float dt);

	bool CheckingCollision();

	void DrawingNetAndFigures();

	void ControlButtonHandler(SDL_Event* event, bool running, bool buttons[4], GameMode gamemode);

	void PrintText(std::string name, int x, int y, int w, int h, TTF_Font* font);

	void PrintWinScreen(std::string winScreenText);

	void HandleEvents(SDL_Event* event);

	void HandleWinScreenEvents(SDL_Event* event);

	void RenderMenu();

	void RenderWinScreen();

	void Gameplay(GameMode gamemode, SDL_Event* event);

	void CleanScreen();

	bool IsRunning() const
	{
		return this->isRunning;
	}

	void SetIsRunning(bool b)
	{
		this->isRunning = b;
	}

	void SetGameMode(GameMode gameMode)
	{
		this->gameMode = gameMode;
	}

	GameMode GetGameMode()
	{
		return this->gameMode;
	}
	
	void SetWinner(Winner winner)
	{
		this->winner = winner;
	}

	Winner GetWinner()
	{
		return this->winner;
	}
	
private:
	bool isRunning;
	GameMode gameMode = GameMode::Unselected;
	Winner winner = Winner::NoWinner;
	Ball* ball;
	Paddle* paddleOne;
	Paddle* paddleTwo;
	SDL_Window* window;
	SDL_Renderer* renderer;
	TTF_Font* scoreFont;
	TTF_Font* menuFont;
	SDL_Color white = { 255, 255, 255 };
	SDL_Surface* tmpSurface;
	SDL_Texture* tmpTexture;
	Score* playerOneScore;
	Score* playerTwoScore;
	SDL_Rect* rect;
};
