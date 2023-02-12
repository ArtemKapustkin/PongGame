#include "Game.hpp"

Game::Game(const char* title, int x, int y, int w, int h)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0 && TTF_Init() == 0 && SDL_Init(SDL_INIT_VIDEO) == 0)
	{
		std::cout << "Subsystem initialized." << std::endl;
		this->scoreFont = TTF_OpenFont("DejaVuSansMono.ttf", 40);
	    this->menuFont = TTF_OpenFont("OpenSans-Regular.ttf", 15);
		this->rect = new SDL_Rect();

		if (this->scoreFont && this->menuFont)
		{
			std::cout << "Fonts loaded successfully." << std::endl;
		}
		else
			std::cout << "Error: " << SDL_GetError() << std::endl;

		this->window = SDL_CreateWindow(title, x, y, w, h, 0);
		if (this->window)
		{
			std::cout << "Window created successfully." << std::endl;
		}

		this->renderer = SDL_CreateRenderer(window, -1, 0);
		if (this->renderer)
		{
			std::cout << "Renderer created successfully." << std::endl;
		}

		this->SetIsRunning(true);
	}
	else
	{
		this->SetIsRunning(false);
	}
}

Game::~Game()
{
	delete this->rect;

	SDL_DestroyWindow(window);
	std::cout << "Window destroyed." << std::endl;
	SDL_DestroyRenderer(renderer);
	std::cout << "Renderer destoyed." << std::endl;
	SDL_Quit();
	std::cout << "Game cleaned." << std::endl;
}

Contact CheckPaddleCollision(Ball* const& ball, Paddle* const& paddle)
{
	float ballLeft = ball->position.getX();
	float ballRight = ball->position.getX() + B_WIDTH;
	float ballTop = ball->position.getY();
	float ballBottom = ball->position.getY() + B_HEIGHT;

	float paddleLeft = paddle->position.getX();
	float paddleRight = paddle->position.getX() + P_WIDTH;
	float paddleTop = paddle->position.getY();
	float paddleBottom = paddle->position.getY() + P_HEIGHT;

	Contact contact{};

	if (ballLeft >= paddleRight)
	{
		return contact;
	}

	if (ballRight <= paddleLeft)
	{
		return contact;
	}

	if (ballTop >= paddleBottom)
	{
		return contact;
	}

	if (ballBottom <= paddleTop)
	{
		return contact;
	}

	float paddleRangeUpper = paddleBottom - (2.0f * P_HEIGHT / 3.0f);
	float paddleRangeMiddle = paddleBottom - (P_HEIGHT / 3.0f);

	if (ball->velocity.getX() < 0)
	{
		// Left paddle
		contact.penetration = paddleRight - ballLeft;
	}
	else if (ball->velocity.getX() > 0)
	{
		// Right paddle
		contact.penetration = paddleLeft - ballRight;
	}

	if ((ballBottom > paddleTop) && (ballBottom < paddleRangeUpper))
	{
		contact.type = CollisionType::Top;
	}
	else if ((ballBottom > paddleRangeUpper) && (ballBottom < paddleRangeMiddle))
	{
		contact.type = CollisionType::Middle;
	}
	else
	{
		contact.type = CollisionType::Bottom;
	}

	return contact;
}

Contact CheckWallCollision(Ball* const& ball)
{
	float ballLeft = ball->position.getX();
	float ballRight = ball->position.getX() + B_WIDTH;
	float ballTop = ball->position.getY();
	float ballBottom = ball->position.getY() + B_HEIGHT;

	Contact contact{};

	if (ballLeft < 0.0f)
	{
		contact.type = CollisionType::Left;
	}
	else if (ballRight > W_WIDTH)
	{
		contact.type = CollisionType::Right;
	}
	else if (ballTop < 0.0f)
	{
		contact.type = CollisionType::Top;
		contact.penetration = -ballTop;
	}
	else if (ballBottom > W_HEIGHT)
	{
		contact.type = CollisionType::Bottom;
		contact.penetration = W_HEIGHT - ballBottom;
	}

	return contact;
}


void Game::InitFigures(GameMode gamemode)
{
	this->playerOneScore = new Score(Coordinates(W_WIDTH / 4, 20), renderer, scoreFont);

	this->playerTwoScore = new Score(Coordinates(3 * W_WIDTH / 4, 20), renderer, scoreFont);

	this->ball = new Ball(Coordinates(W_WIDTH / 2.0f, W_HEIGHT / 2.0f), Coordinates(BALL_SPEED, 0.0f));

	this->paddleOne = new Paddle(Coordinates(50.0f, W_HEIGHT / 2.0f), Coordinates(0.0f, 0.0f));

	if (gamemode == GameMode::PlayerVsPlayer)
		this->paddleTwo = new Paddle(Coordinates(W_WIDTH - 50.0f, W_HEIGHT / 2.0f), Coordinates(0.0f, 0.0f));
	else
		this->paddleTwo = new BotPaddle(Coordinates(W_WIDTH - 50.0f, W_HEIGHT / 2.0f), Coordinates(0.0f, 0.0f));
}

void Game::UpdateFigures(GameMode gamemode, float dt)
{
	// Update the paddle positions
	this->paddleOne->Update(dt);
	this->paddleTwo->Update(dt);

	// Update the ball position
	this->ball->Update(dt);

	// If gamemode is "vs Bot" bot paddle start tracking ball position
	if (gamemode == GameMode::PlayerVsBot)
		this->paddleTwo->Tracking(this->ball->position);
}

bool Game::CheckingCollision()
{
	bool running = true;
	if (Contact contact = CheckPaddleCollision(this->ball, this->paddleOne); contact.type != CollisionType::None)
	{
		this->ball->CollideWithPaddle(contact);
	}
	else if (contact = CheckPaddleCollision(this->ball, this->paddleTwo); contact.type != CollisionType::None)
	{
		this->ball->CollideWithPaddle(contact);
	}
	else if (contact = CheckWallCollision(this->ball); contact.type != CollisionType::None)
	{
		ball->CollideWithWall(contact);
		if (contact.type == CollisionType::Left)
		{
			int newScore = this->playerTwoScore->GetScoreValue() + 1;
			if (newScore >= 3)
			{
				SetWinner(Winner::RightPlayer);
				running = false;
			}
			this->playerTwoScore->SetScoreValue(newScore);
		}
		else if (contact.type == CollisionType::Right)
		{
			int newScore = playerOneScore->GetScoreValue() + 1;
			if (newScore >= 3)
			{
				SetWinner(Winner::LeftPlayer);
				running = false;
			}
			this->playerOneScore->SetScoreValue(newScore);
		}
	}
	return running;
}

void Game::DrawingNetAndFigures()
{
	for (int y = 0; y < W_HEIGHT; ++y)
	{
		if (y % 5)
		{
			SDL_RenderDrawPoint(this->renderer, W_WIDTH / 2, y);
		}
	}

	this->ball->Draw(renderer);

	this->paddleOne->Draw(renderer);
	this->paddleTwo->Draw(renderer);

	this->playerOneScore->Draw();
	this->playerTwoScore->Draw();
}


void Game::ControlButtonHandler(SDL_Event* event, bool running, bool buttons[4], GameMode gamemode)
{
	while (SDL_PollEvent(event))
	{
		if (event->type == SDL_QUIT)
		{
			running = false;
		}
		else if (event->type == SDL_KEYDOWN)
		{
			if (event->key.keysym.sym == SDLK_ESCAPE)
			{
				running = false;
			}
			else if (event->key.keysym.sym == SDLK_w)
			{
				buttons[Buttons::PaddleOneUp] = true;
			}
			else if (event->key.keysym.sym == SDLK_s)
			{
				buttons[Buttons::PaddleOneDown] = true;
			}
			else if (event->key.keysym.sym == SDLK_UP)
			{
				buttons[Buttons::PaddleTwoUp] = true;
			}
			else if (event->key.keysym.sym == SDLK_DOWN)
			{
				buttons[Buttons::PaddleTwoDown] = true;
			}
		}
		else if (event->type == SDL_KEYUP)
		{
			if (event->key.keysym.sym == SDLK_w)
			{
				buttons[Buttons::PaddleOneUp] = false;
			}
			else if (event->key.keysym.sym == SDLK_s)
			{
				buttons[Buttons::PaddleOneDown] = false;
			}
			else if (event->key.keysym.sym == SDLK_UP)
			{
				buttons[Buttons::PaddleTwoUp] = false;
			}
			else if (event->key.keysym.sym == SDLK_DOWN)
			{
				buttons[Buttons::PaddleTwoDown] = false;
			}
		}
	}

	if (buttons[Buttons::PaddleOneUp])
	{
		this->paddleOne->velocity.setY(-PADDLE_SPEED);
	}
	else if (buttons[Buttons::PaddleOneDown])
	{
		this->paddleOne->velocity.setY(PADDLE_SPEED);
	}
	else
	{
		this->paddleOne->velocity.setY(0.0f);
	}

	if (gamemode == GameMode::PlayerVsPlayer)
	{
		if (buttons[Buttons::PaddleTwoUp])
		{
			this->paddleTwo->velocity.setY(-PADDLE_SPEED);
		}
		else if (buttons[Buttons::PaddleTwoDown])
		{
			this->paddleTwo->velocity.setY(PADDLE_SPEED);
		}
		else
		{
			this->paddleTwo->velocity.setY(0.0f);
		}
	}
}

void Game::Gameplay(GameMode gamemode, SDL_Event* event)
{
	InitFigures(gamemode);
	bool running = true;
	bool buttons[4] = {};
	float dt = 0.0f;

	while (running)
	{
		auto startTime = std::chrono::high_resolution_clock::now();
		while (SDL_PollEvent(event))
		{
			if (event->type == SDL_QUIT)
			{
				running = false;
			}
			else if (event->type == SDL_KEYDOWN)
			{
				if (event->key.keysym.sym == SDLK_ESCAPE)
				{
					running = false;
				}
				else if (event->key.keysym.sym == SDLK_w)
				{
					buttons[Buttons::PaddleOneUp] = true;
				}
				else if (event->key.keysym.sym == SDLK_s)
				{
					buttons[Buttons::PaddleOneDown] = true;
				}
				else if (event->key.keysym.sym == SDLK_UP)
				{
					buttons[Buttons::PaddleTwoUp] = true;
				}
				else if (event->key.keysym.sym == SDLK_DOWN)
				{
					buttons[Buttons::PaddleTwoDown] = true;
				}
			}
			else if (event->type == SDL_KEYUP)
			{
				if (event->key.keysym.sym == SDLK_w)
				{
					buttons[Buttons::PaddleOneUp] = false;
				}
				else if (event->key.keysym.sym == SDLK_s)
				{
					buttons[Buttons::PaddleOneDown] = false;
				}
				else if (event->key.keysym.sym == SDLK_UP)
				{
					buttons[Buttons::PaddleTwoUp] = false;
				}
				else if (event->key.keysym.sym == SDLK_DOWN)
				{
					buttons[Buttons::PaddleTwoDown] = false;
				}
			}
		}

		if (buttons[Buttons::PaddleOneUp])
		{
			paddleOne->velocity.setY(-PADDLE_SPEED);
		}
		else if (buttons[Buttons::PaddleOneDown])
		{
			paddleOne->velocity.setY(PADDLE_SPEED);
		}
		else
		{
			paddleOne->velocity.setY(0.0f);
		}

		if (gamemode == GameMode::PlayerVsPlayer)
		{
			if (buttons[Buttons::PaddleTwoUp])
			{
				paddleTwo->velocity.setY(- PADDLE_SPEED);
			}
			else if (buttons[Buttons::PaddleTwoDown])
			{
				paddleTwo->velocity.setY(PADDLE_SPEED);
			}
			else
			{
				paddleTwo->velocity.setY(0.0f);
			}
		}

		UpdateFigures(gamemode, dt);

		// Check collisions
		running = CheckingCollision();

		// Clear the window to black
		SDL_SetRenderDrawColor(this->renderer, 0x0, 0x0, 0x0, 0xFF);
		SDL_RenderClear(this->renderer);

		// Set the draw color to be white
		SDL_SetRenderDrawColor(this->renderer, 0xFF, 0xFF, 0xFF, 0xFF);

		// Draw Net, Figures and Score
		DrawingNetAndFigures();

		// Present the backbuffer
		SDL_RenderPresent(this->renderer);

		// Calculate frame time
		auto stopTime = std::chrono::high_resolution_clock::now();
		dt = std::chrono::duration<float, std::chrono::milliseconds::period>(stopTime - startTime).count();
	}

	// Cleaning screen after ending "while(running)" loop (ESC button)
	SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 0);
	
	SDL_RenderClear(this->renderer);
}

void Game::PrintText(std::string name, int x, int y, int w, int h, TTF_Font* font)
{
	this->tmpSurface = TTF_RenderText_Solid(font, name.c_str(), white);

	this->tmpTexture = SDL_CreateTextureFromSurface(this->renderer, this->tmpSurface);

	this->rect->x = x;
	this->rect->y = y;
	this->rect->w = w;
	this->rect->h = h;

	SDL_RenderCopy(this->renderer, this->tmpTexture, NULL, this->rect);

	SDL_RenderPresent(this->renderer);

	SDL_FreeSurface(this->tmpSurface);

	SDL_DestroyTexture(this->tmpTexture);

}

void Game::HandleEvents(SDL_Event* event)
{
	while (SDL_PollEvent(event))
	{
		if (event->type == SDL_QUIT)
		{
			SetIsRunning(false);
		}
		else if (event->type == SDL_KEYDOWN)
		{
			if (event->key.keysym.sym == SDLK_ESCAPE)
			{
				SetIsRunning(false);
			}
			else if (event->key.keysym.sym == SDLK_1)
			{
				SetGameMode(GameMode::PlayerVsPlayer);
				Gameplay(GetGameMode(), event);
				//SDL_RenderClear(this->renderer);
			}
			else if (event->key.keysym.sym == SDLK_2)
			{
				SetGameMode(GameMode::PlayerVsBot);
				Gameplay(GetGameMode(), event);
				//SDL_RenderClear(this->renderer);
			}
		}
	}
}

void Game::HandleWinScreenEvents(SDL_Event* event)
{
	while (SDL_PollEvent(event))
	{
		if (event->type == SDL_QUIT)
		{
			SetIsRunning(false);
		}
		else if (event->type == SDL_KEYDOWN)
		{
			if (event->key.keysym.sym == SDLK_ESCAPE)
			{
				SetIsRunning(false);
			}
			else if (event->key.keysym.sym == SDLK_1)
			{
				Gameplay(GetGameMode(), event);
			}
			else if (event->key.keysym.sym == SDLK_2)
			{
				SetGameMode(GameMode::Unselected);
				SetWinner(Winner::NoWinner);
				SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 0);

				SDL_RenderClear(this->renderer);
			}
		}
	}
}

void Game::CleanScreen()
{
	SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 0);
	SDL_RenderClear(this->renderer);
}

void Game::RenderMenu()
{
	SDL_SetRenderDrawColor(this->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	PrintText("Pong by Artem Kapustkin", W_WIDTH / 2.0f - 500 / 2.0f, W_HEIGHT / 2.0f - 50 - 100, 500, 100, this->menuFont);
	PrintText("1) Player vs Player", W_WIDTH / 2.0f - 250 / 2.0f, W_HEIGHT / 2.0f - 50, 250, 50, this->menuFont);
	PrintText("2) Player vs Bot", W_WIDTH / 2.0f - 250 / 2.0f, W_HEIGHT / 2.0f, 250, 50, this->menuFont);
	SDL_RenderPresent(this->renderer);
}

void Game::RenderWinScreen()
{
	if(GetWinner() == Winner::LeftPlayer)
	{
		PrintWinScreen("Player 1 won");
	}
	else if(GetWinner() == Winner::RightPlayer)
	{
		if (GetGameMode() == GameMode::PlayerVsPlayer)
		{
			PrintWinScreen("Player 2 won");
		}
		else if (GetGameMode() == GameMode::PlayerVsBot)
		{
			PrintWinScreen("Bot won");
		}
	}
}

void Game::PrintWinScreen(std::string winScreenText)
{
	SDL_SetRenderDrawColor(this->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	PrintText(winScreenText, W_WIDTH / 2.0f - 500 / 2.0f, W_HEIGHT / 2.0f - 50 - 100, 500, 100, this->menuFont);
	PrintText("1) Restart", W_WIDTH / 2.0f - 250 / 2.0f, W_HEIGHT / 2.0f - 50, 250, 50, this->menuFont);
	PrintText("2) Main Menu", W_WIDTH / 2.0f - 250 / 2.0f, W_HEIGHT / 2.0f, 250, 50, this->menuFont);
	SDL_RenderPresent(this->renderer);
}