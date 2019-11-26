#include "Card.h"

Card::Card(CardSuit _suit, CardRank _rank, bool _opened) {
	suit = _suit;
	rank = _rank;
	opened = _opened;
}

void Card::Turn() {
	opened = !opened;
}

void Card::Open() {
	opened = true;
}

void Card::Close() {
	opened = false;
}

void Card::Draw(SDL_Renderer* renderer, Storage* storage, int x, int y) {
	SDL_Rect dest = { x, y, 96, 128 };
	if(opened)
		SDL_RenderCopy(renderer, storage->opened_card_images[int(suit)][int(rank)], NULL, &dest);
	else
		SDL_RenderCopy(renderer, storage->closed_card_image, NULL, &dest);
}

bool Card::IsOpened() {
	return opened;
}

CardSuit Card::GetSuit() {
	return suit;
}
CardRank Card::GetRank() {
	return rank;
}