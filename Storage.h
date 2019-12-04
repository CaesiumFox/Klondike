#ifndef _STORAGE_H_
#define _STORAGE_H_

#include "Common.h"
#include "Font.h"

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
	SDL_Texture* particle;
	SDL_Texture* particle_overlay;

	SDL_Texture* new_one_image;
	SDL_Texture* new_three_image;

	SDL_Texture* preview_border_inactive;
	SDL_Texture* preview_border_active;

	Font* font;

	// settings

	int settingBackground;

	Storage(SDL_Renderer *ren);
	~Storage();
};

#endif