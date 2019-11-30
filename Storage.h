#ifndef _STORAGE_H_
#define _STORAGE_H_

#include "Common.h"

class Storage {
public:
	vector<vector<SDL_Texture*>> opened_card_images;
	SDL_Texture* closed_card_image;
	SDL_Texture* empty_place_image;
	int bg_count;
	vector<SDL_Texture*> backgrounds;
	SDL_Texture* newgamebutton_img;
	SDL_Texture* undobutton_img;
	SDL_Texture* quitbutton_img;
	SDL_Texture* pausebutton_img;
	SDL_Texture* playbutton_img;
	SDL_Texture* statsbutton_img;
	SDL_Texture* setbutton_img;
	SDL_Texture* select_bound;
	SDL_Texture* win_image;
	SDL_Texture* pausedlabel_image;
	SDL_Texture* score_label_img;
	SDL_Texture* time_label_img;
	SDL_Texture* now_label_img;
	SDL_Texture* digits_tile;
	SDL_Texture* particle;
	SDL_Texture* particle_overlay;

	SDL_Texture* newgame_window_image;
	SDL_Texture* cancel_big_button_image;
	SDL_Texture* new_one_image;
	SDL_Texture* new_three_image;

	SDL_Texture* settings_window_image;
	SDL_Texture* preview_border_inactive;
	SDL_Texture* preview_border_active;

	//TTF_Font* font;

	vector<SDL_Rect> digits_pos_on_tile;

	// settings

	int settingBackground;

	static void DrawString(SDL_Renderer*, Storage*, string, SDL_Point &);
	// the first - when the count is started
	// even - when the count is paused
	// odd - when the count is continued
	static time_t TimeCut(vector<time_t> &time_points);
	static string GetStrTime(time_t);
	static string GetCurrentStrTime();

	static void Encode4B(ofstream&, unsigned long);
	static void Encode8B(ofstream&, unsigned long long);
	static unsigned long Decode4B(ifstream&);
	static unsigned long long Decode8B(ifstream&);

	Storage(SDL_Renderer *ren);
	~Storage();
};

#endif