#ifndef _GAME_STATE_H_
#define _GAME_STATE_H_

enum class GameState {
	Playing = 0,
	Paused = 1,
	Finished = 2
};

enum class FormShown {
	GamePlay = 0,
	NewGameDialog = 1,
	SettingsDialog = 2
};

#endif