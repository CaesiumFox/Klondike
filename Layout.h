#ifndef _LAYOUT_H_
#define _LAYOUT_H_

#include "Common.h"
#include "Card.h"

struct Layout {
	int score;
	deque<Card> stock; // back == top
	deque<Card> waste; // front == top
	deque<Card> foundations[4]; // 4 piles, front == top
	deque<Card> tableau[7]; // 7 piles, front == top

	void Clear() {
		score = 0;
		stock.clear();
		waste.clear();
		for (auto& e : foundations) {
			e.clear();
		}
		for (auto& e : tableau) {
			e.clear();
		}
	}
};

#endif