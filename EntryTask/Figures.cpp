#include"Figures.hpp"
#include <chrono>

Figure::Figure(Coordinates position, Coordinates velocity) : position(position), velocity(velocity)
{
	rect.x = static_cast<int>(position.getX());
	rect.y = static_cast<int>(position.getY());
	rect.w = 10;
	rect.h = 10;
}

void Figure::Update(float dt)
{
	position += velocity * dt;
}

void Figure::Draw(SDL_Renderer* renderer)
{
	rect.x = static_cast<int>(position.getX());
	rect.y = static_cast<int>(position.getY());

	SDL_RenderFillRect(renderer, &rect);
}

void Paddle::Update(float dt)
{
	position += velocity * dt;

	if (position.getY() < 0)
	{
		// Restrict to top of the screen
		position.setY(0.f);
	}
	else if (position.getY() > (W_HEIGHT - P_HEIGHT))
	{
		// Restrict to bottom of the screen
		position.setY(W_HEIGHT - P_HEIGHT);
	}
}

void Paddle::Draw(SDL_Renderer* renderer)
{
	rect.y = static_cast<int>(position.getY());

	SDL_RenderFillRect(renderer, &rect);
}

void Paddle::setStartCoordinates(Coordinates position, Coordinates velocity)
{
	rect.x = static_cast<int>(position.getX());
	rect.y = static_cast<int>(position.getY());
}

Paddle::Paddle(Coordinates position, Coordinates velocity) : Figure(position, velocity)
{
	rect.w = P_WIDTH;
	rect.h = P_HEIGHT;
}

void Ball::CollideWithPaddle(Contact const& contact)
{
	position.setX(position.getX() + contact.penetration);
	velocity.setX(-velocity.getX());

	if (contact.type == CollisionType::Top)
	{
		velocity.setY(- 0.75f * BALL_SPEED);
	}
	else if (contact.type == CollisionType::Bottom)
	{
		velocity.setY(0.75f * BALL_SPEED);
	}
}

void Ball::CollideWithWall(Contact const& contact)
{
	if ((contact.type == CollisionType::Top) || (contact.type == CollisionType::Bottom))
	{
		position.setY(position.getY() + contact.penetration);
		velocity.setY(-velocity.getY());
	}
	else if (contact.type == CollisionType::Left)
	{
		position.setX(W_WIDTH / 2.0f);
		position.setY(W_HEIGHT / 2.0f);
		velocity.setX(BALL_SPEED);
		velocity.setY(((rand() / (RAND_MAX + 0.5)) - 0.5)* BALL_SPEED);
	}
	else if (contact.type == CollisionType::Right)
	{
		position.setX(W_WIDTH / 2.0f);
		position.setY(W_HEIGHT / 2.0f);
		velocity.setX(-BALL_SPEED);
		velocity.setY(((rand() / (RAND_MAX + 0.5)) - 0.5) * BALL_SPEED);
	}
}

Ball::Ball(Coordinates position, Coordinates velocity) : Figure(position, velocity)
{
	rect.w = B_WIDTH;
	rect.h = B_HEIGHT;
}

BotPaddle::BotPaddle(Coordinates position, Coordinates velocity) : Paddle(position, velocity)
{
}

void BotPaddle::Tracking(Coordinates ball)
{
	//std::cout << "BEFORE" << std::endl;
	//std::cout << "x: " << position.x << std::endl;
	//std::cout << "y: " << position.y << std::endl << std::endl;
	position.setX(W_WIDTH - 50);
	if (ball.getY() > position.getY())
	{
		//std::cout << "Going down " << std::endl;
		this->velocity.setY(P_AI_SPEED);
	}

	else if (ball.getY() <= position.getY())
	{
		//std::cout << "Going up " << std::endl;
		this->velocity.setY(-P_AI_SPEED);
	}
	//std::cout << "AFTER" << std::endl;
	//std::cout << "x: " << position.x << std::endl;
	//std::cout << "y: " << position.y << std::endl;
}