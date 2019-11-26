#ifndef _CARD_H_
#define _CARD_H_

#include "Common.h"
#include "CardRank.h"
#include "CardSuit.h"
#include "Storage.h"

class Card {
private:
	CardSuit suit;
	CardRank rank;
	bool opened;

public:
	Card(CardSuit _suit = CardSuit::Spades, CardRank _rank = CardRank::Ace, bool _opened = false);
	void Turn();
	void Open();
	void Close();
	void Draw(SDL_Renderer* renderer, Storage* storage, int x, int y);

	bool IsOpened();
	CardSuit GetSuit();
	CardRank GetRank();
};

#endif