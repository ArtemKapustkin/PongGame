#include "Game.hpp"

Game::Game(const char* title, int x, int y, int w, int h, bool fullscreen)
{
	int flags = 0;

	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0 && TTF_Init() == 0 && SDL_Init(SDL_INIT_VIDEO) == 0)
	{
		std::cout << "Subsystem initialized." << std::endl;

		window = SDL_CreateWindow(title, x, y, w, h, flags);
		if (window)
		{
			std::cout << "Window created successfully." << std::endl;
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			std::cout << "Renderer created successfully." << std::endl;
		}
		isRunning = true;
	}
	else
	{
		isRunning = false;
	}
}
Game::~Game()
{

}

Contact CheckPaddleCollision(Ball const& ball, Paddle const& paddle)
{
	float ballLeft = ball.position.x;
	float ballRight = ball.position.x + B_WIDTH;
	float ballTop = ball.position.y;
	float ballBottom = ball.position.y + B_HEIGHT;

	float paddleLeft = paddle.position.x;
	float paddleRight = paddle.position.x + P_WIDTH;
	float paddleTop = paddle.position.y;
	float paddleBottom = paddle.position.y + P_HEIGHT;

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

	if (ball.velocity.x < 0)
	{
		// Left paddle
		contact.penetration = paddleRight - ballLeft;
	}
	else if (ball.velocity.x > 0)
	{
		// Right paddle
		contact.penetration = paddleLeft - ballRight;
	}

	if ((ballBottom > paddleTop)
		&& (ballBottom < paddleRangeUpper))
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

Contact CheckWallCollision(Ball const& ball)
{
	float ballLeft = ball.position.x;
	float ballRight = ball.position.x + B_WIDTH;
	float ballTop = ball.position.y;
	float ballBottom = ball.position.y + B_HEIGHT;

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

void gameplay(bool b, SDL_Renderer* renderer, TTF_Font* scoreFont)
{
	Score playerOneScoreText(Coordinates(W_WIDTH / 4, 20), renderer, scoreFont);

	Score playerTwoScoreText(Coordinates(3 * W_WIDTH / 4, 20), renderer, scoreFont);

	Ball ball(Coordinates(W_WIDTH / 2.0f, W_HEIGHT / 2.0f), Coordinates(BALL_SPEED, 0.0f));

	Paddle paddleOne(Coordinates(50.0f, W_HEIGHT / 2.0f), Coordinates(0.0f, 0.0f));

	Paddle paddleTwo(Coordinates(W_WIDTH - 50.0f, W_HEIGHT / 2.0f), Coordinates(0.0f, 0.0f));
	BotPaddle botPaddle(Coordinates(W_WIDTH - 50.0f, W_HEIGHT / 2.0f), Coordinates(0.0f, 0.0f));
	if (b == true)
		botPaddle.setStartCoordinates(Coordinates(-50.0f, -50.0f), Coordinates(0.0f, 0.0f));
	else
		paddleTwo.setStartCoordinates(Coordinates(-50.0f, -50.0f), Coordinates(0.0f, 0.0f));

	int playerOneScore = 0;
	int playerTwoScore = 0;


	bool running = true;
	bool buttons[4] = {};

	float dt = 0.0f;

	while (running)
	{
		auto startTime = std::chrono::high_resolution_clock::now();
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				running = false;
			}
			else if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					running = false;
				}
				else if (event.key.keysym.sym == SDLK_w)
				{
					buttons[Buttons::PaddleOneUp] = true;
				}
				else if (event.key.keysym.sym == SDLK_s)
				{
					buttons[Buttons::PaddleOneDown] = true;
				}
				else if (event.key.keysym.sym == SDLK_UP)
				{
					buttons[Buttons::PaddleTwoUp] = true;
				}
				else if (event.key.keysym.sym == SDLK_DOWN)
				{
					buttons[Buttons::PaddleTwoDown] = true;
				}
			}
			else if (event.type == SDL_KEYUP)
			{
				if (event.key.keysym.sym == SDLK_w)
				{
					buttons[Buttons::PaddleOneUp] = false;
				}
				else if (event.key.keysym.sym == SDLK_s)
				{
					buttons[Buttons::PaddleOneDown] = false;
				}
				else if (event.key.keysym.sym == SDLK_UP)
				{
					buttons[Buttons::PaddleTwoUp] = false;
				}
				else if (event.key.keysym.sym == SDLK_DOWN)
				{
					buttons[Buttons::PaddleTwoDown] = false;
				}
			}
		}


		if (buttons[Buttons::PaddleOneUp])
		{
			paddleOne.velocity.y = -PADDLE_SPEED;
		}
		else if (buttons[Buttons::PaddleOneDown])
		{
			paddleOne.velocity.y = PADDLE_SPEED;
		}
		else
		{
			paddleOne.velocity.y = 0.0f;
		}

		if (buttons[Buttons::PaddleTwoUp])
		{
			paddleTwo.velocity.y = -PADDLE_SPEED;
		}
		else if (buttons[Buttons::PaddleTwoDown])
		{
			paddleTwo.velocity.y = PADDLE_SPEED;
		}
		else
		{
			paddleTwo.velocity.y = 0.0f;
		}


		// Update the paddle positions
		paddleOne.Update(dt);
		paddleTwo.Update(dt);
		botPaddle.Update(dt);

		// Update the ball position
		ball.Update(dt);

		if (b == false)
			botPaddle.Tracking(ball.position);

		// Check collisions
		if (Contact contact = CheckPaddleCollision(ball, paddleOne); contact.type != CollisionType::None)
		{
			ball.CollideWithPaddle(contact);
		}
		else if (contact = CheckPaddleCollision(ball, paddleTwo); contact.type != CollisionType::None && b == true)
		{
			ball.CollideWithPaddle(contact);
		}
		else if (contact = CheckPaddleCollision(ball, botPaddle); contact.type != CollisionType::None && b == false)
		{
			ball.CollideWithPaddle(contact);
		}
		else if (contact = CheckWallCollision(ball); contact.type != CollisionType::None)
		{
			ball.CollideWithWall(contact);
			if (contact.type == CollisionType::Left)
			{
				++playerTwoScore;

				playerTwoScoreText.SetScore(playerTwoScore);
			}
			else if (contact.type == CollisionType::Right)
			{
				++playerOneScore;

				playerOneScoreText.SetScore(playerOneScore);
			}
		}



		// Clear the window to black
		SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, 0xFF);
		SDL_RenderClear(renderer);

		// Set the draw color to be white
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);


		// Draw the net
		for (int y = 0; y < W_HEIGHT; ++y)
		{
			if (y % 5)
			{
				SDL_RenderDrawPoint(renderer, W_WIDTH / 2, y);
			}
		}

		// Draw the ball
		ball.Draw(renderer);

		// Draw the paddles
		paddleOne.Draw(renderer);
		if (b == true)
			paddleTwo.Draw(renderer);
		else
			botPaddle.Draw(renderer);

		playerOneScoreText.Draw();
		playerTwoScoreText.Draw();


		// Present the backbuffer
		SDL_RenderPresent(renderer);


		// Calculate frame time
		auto stopTime = std::chrono::high_resolution_clock::now();
		dt = std::chrono::duration<float, std::chrono::milliseconds::period>(stopTime - startTime).count();
	}

}

void Game::printtext(std::string name, int x, int y, int w, int h, SDL_Renderer* renderer, TTF_Font* font)
{
	const char* label = name.c_str();
	//TTF_Font* menuFont = TTF_OpenFont("OpenSans-Regular.ttf", 15);
	//if (auto c = TTF_OpenFont("OpenSans-Regular.ttf", 15); c == NULL)
	//s	std::cout << c << std::endl << TTF_GetError() << std::endl;
	tmpSurface = TTF_RenderText_Solid(font, label, White);
	//if (auto z = TTF_RenderText_Solid(menuFont, "label", White); z == NULL)
	//	std::cout << z << TTF_GetError() << std::endl;
	tmpTexture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	SDL_RenderCopy(renderer, tmpTexture, NULL, &rect);
	SDL_RenderPresent(renderer);
}

void Game::init(const char* title, int x, int y, int w, int h, bool fullscreen)
{
	int flags = 0;
	
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0 && TTF_Init() == 0 && SDL_Init(SDL_INIT_VIDEO) == 0)
	{
		std::cout << "Subsystem initialized." << std::endl;

		window = SDL_CreateWindow(title, x, y, w, h, flags);
		if (window)
		{
			std::cout << "Window created successfully." << std::endl;
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			std::cout << "Renderer created successfully." << std::endl;
		}
		isRunning = true;
	}
	else
	{
		isRunning = false;
	}

	

}

void Game::handleEvents()
{
	SDL_Event event;
	TTF_Font* scoreFont = TTF_OpenFont("DejaVuSansMono.ttf", 40);
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			isRunning = false;
		}
		else if (event.type == SDL_KEYDOWN)
		{
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				isRunning = false;
			}
			else if (event.key.keysym.sym == SDLK_1)
			{
				//isRunning = false;
				gameplay(true, renderer, scoreFont);
			}
			else if (event.key.keysym.sym == SDLK_2)
			{
				//isRunning = false;
				gameplay(false, renderer, scoreFont);
			}
		}
	}
}

void Game::updateMenu()
{
	count++;
	std::cout << count << std::endl;
}
void Game::renderMenu()
{
	TTF_Font* menuFont = TTF_OpenFont("OpenSans-Regular.ttf", 15);
	//SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, 0xFF);
	//SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	printtext("Pong by Artem Kapustkin", W_WIDTH / 2.0f - 500 / 2.0f, W_HEIGHT / 2.0f - 50 - 100, 500, 100, renderer, menuFont);
	printtext("1) Player vs Player", W_WIDTH / 2.0f - 250 / 2.0f, W_HEIGHT / 2.0f - 50, 250, 50, renderer, menuFont);
	printtext("2) Player vs Bot", W_WIDTH / 2.0f - 250 / 2.0f, W_HEIGHT / 2.0f, 250, 50, renderer, menuFont);
	
	//int u = SDL_RenderCopy(renderer, tmpTexture, NULL, &rect);
	//std::cout << u << "ERROR: " << SDL_GetError() << std::endl;
	SDL_RenderPresent(renderer);
}

void Game::updateGame()
{
	count++;
	std::cout << count << std::endl;
}

void Game::renderGame()
{
	TTF_Font* scoreFont = TTF_OpenFont("DejaVuSansMono.ttf", 40);

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);




	SDL_RenderPresent(renderer);
}


void Game::clear()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_FreeSurface(tmpSurface);
	SDL_DestroyTexture(tmpTexture);
	SDL_Quit();
	std::cout << "Game cleaned." << std::endl;
}