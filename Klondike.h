#ifndef _KLONDIKE_H_
#define _KLONDIKE_H_

#include "Common.h"
#include "Storage.h"
#include "Card.h"
#include "Layout.h"
#include "GameState.h"
#include "GameMode.h"
#include "Firework.h"

class Klondike {
private:
	int WinWidth, WinHeight;

	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;

	Storage* storage;

	GameState state;
	FormShown shown;

	Layout layout;
	vector<Layout> history;

	SDL_Rect stock_pos;
	SDL_Rect waste_pos;
	SDL_Rect waste_hitbox;
	vector<SDL_Rect> foundations_pos;
	vector<SDL_Rect> tableau_pos; // the uppest card

	SDL_Rect youwin_pos;

	SDL_Rect pb_pos;
	SDL_Rect ngb_pos;
	SDL_Rect udb_pos;
	SDL_Rect qb_pos;
	SDL_Rect set_pos;

	SDL_Rect scorelabel_pos;
	SDL_Rect timelabel_pos;
	SDL_Rect nowlabel_pos;
	SDL_Point scorevalue_pos;
	SDL_Point timevalue_pos;
	SDL_Point nowvalue_pos;

	SDL_Rect newgame_window_pos;
	SDL_Rect newgame_btn1_pos;
	SDL_Rect newgame_btn3_pos;
	SDL_Rect newgame_cancel_pos;

	SDL_Rect settings_window_pos;
	SDL_Rect settings_bg1_pos;
	SDL_Rect settings_bg2_pos;
	SDL_Rect settings_bg3_pos;
	SDL_Rect settings_cancel_pos;

	bool running;
	bool win;

	// gameplay
	struct CardSelection {
		// '0' = none
		// 'w' = waste
		// '1' - '7' = tableau
		// 'a' - 'd' = foundations
		char where;
		// only for tableau (top card = 0)
		int pos;
	} selected;
	time_t delay;
	vector<time_t> time_points;
	GameMode gamemode;

	Firework* final_firework_1;
	Firework* final_firework_2;
	Firework* final_firework_3;
	Firework* final_firework_4;

	void Render();
	void Input();
	void Update();

	void Loop();

	// rendering
	void DrawStock();
	void DrawWaste();
	void DrawFoundations();
	void DrawFannedPiles();
	
	void NewGame(GameMode);
	bool Build();
	void Undo();
	void Record();
	void Reset();
	void SaveFile();
	void LoadFile();

	bool PossibleToFoundation(int index, Card*);
	bool PossibleToPile(int index, Card*);
	int WhichPile(int x);
	int WhichCardInPile(int index, int y);
	int CountOpenedInPile(int index);

	void InputEverywhere();
	void InputPlaying();
	void InputPaused();
	void InputFinished();
	void InputNewGame();
	void InputSettings();

	void RenderEverywhereBelow();
	void RenderPlaying();
	void RenderPaused();
	void RenderFinished();
	void RenderNewGame();
	void RenderSettings();
	void RenderEverywhereAbove();

	void UpdateEverywhere();
	void UpdatePlaying();
	void UpdatePaused();
	void UpdateFinished();
	void UpdateNewGame();
	void UpdateSettings();
	
public:
	Klondike();
	~Klondike();
};

#endif