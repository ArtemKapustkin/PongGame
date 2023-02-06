#include"Score.hpp"

Score::Score(Coordinates position, SDL_Renderer* renderer, TTF_Font* font) : renderer(renderer), font(font)
{
	this->surface = TTF_RenderText_Solid(font, "0", { 0xFF, 0xFF, 0xFF, 0xFF });
	this->texture = SDL_CreateTextureFromSurface(renderer, this->surface);

	int width, height;
	SDL_QueryTexture(this->texture, nullptr, nullptr, &width, &height);

	rect.x = static_cast<int>(position.getX());
	rect.y = static_cast<int>(position.getY());
	rect.w = width;
	rect.h = height;
}

Score::~Score()
{
	SDL_FreeSurface(this->surface);
	SDL_DestroyTexture(this->texture);
}

void Score::SetScore(int score)
{
	SDL_FreeSurface(this->surface);
	SDL_DestroyTexture(this->texture);

	this->surface = TTF_RenderText_Solid(this->font, std::to_string(score).c_str(), { 0xFF, 0xFF, 0xFF, 0xFF });
	this->texture = SDL_CreateTextureFromSurface(this->renderer, this->surface);

	int width, height;
	SDL_QueryTexture(this->texture, nullptr, nullptr, &width, &height);
	rect.w = width;
	rect.h = height;
}

void Score::Draw()
{
	SDL_RenderCopy(this->renderer, this->texture, nullptr, &rect);
}