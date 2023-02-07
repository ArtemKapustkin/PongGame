#include"Score.hpp"

Score::Score(Coordinates position, SDL_Renderer* renderer, TTF_Font* font) : renderer(renderer), font(font)
{
	this->scoreValue = 0;
	this->rect = new SDL_Rect();
	this->surface = TTF_RenderText_Solid(font, "0", { 0xFF, 0xFF, 0xFF, 0xFF });
	this->texture = SDL_CreateTextureFromSurface(renderer, this->surface);

	int width, height;
	SDL_QueryTexture(this->texture, nullptr, nullptr, &width, &height);

	this->rect->x = static_cast<int>(position.getX());
	this->rect->y = static_cast<int>(position.getY());
	this->rect->w = width;
	this->rect->h = height;
}

Score::~Score()
{
	delete this->rect; //?
	SDL_FreeSurface(this->surface);
	SDL_DestroyTexture(this->texture);
}

void Score::SetScoreValue(int score)
{
	SDL_FreeSurface(this->surface);
	SDL_DestroyTexture(this->texture);

	this->surface = TTF_RenderText_Solid(this->font, std::to_string(score).c_str(), { 0xFF, 0xFF, 0xFF, 0xFF });
	this->texture = SDL_CreateTextureFromSurface(this->renderer, this->surface);

	int width, height;
	SDL_QueryTexture(this->texture, nullptr, nullptr, &width, &height);
	this->rect->w = width;
	this->rect->h = height;

	this->scoreValue = score;
}

int Score::GetScoreValue()
{
	return this->scoreValue;
}

void Score::Draw()
{
	SDL_RenderCopy(this->renderer, this->texture, nullptr, this->rect);
}