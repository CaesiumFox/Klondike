#include "Storage.h"

Storage::Storage(SDL_Renderer *renderer) {
	string suits[] = { "H", "C", "D", "S" };
	string ranks[] = { "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };
	opened_card_images = vector<vector<SDL_Texture*>>(4, vector<SDL_Texture*>(13));
	SDL_Surface* surface;
	for (int suit = 0; suit < 4; suit++) {
		for (int rank = 0; rank < 13; rank++) {
			surface = IMG_Load(("Textures"s + FILE_SEPARATOR + "Cards"s + FILE_SEPARATOR + suits[suit] + ranks[rank] + ".png"s).c_str());
			opened_card_images[suit][rank] = SDL_CreateTextureFromSurface(renderer, surface);
			if (opened_card_images[suit][rank] == nullptr) {
				cout << "Failed to load \"" << "Textures"s + FILE_SEPARATOR + "Cards"s + FILE_SEPARATOR + ""s + suits[suit] + ranks[rank] + ".png"s << "\"!" << endl;
				throw 1;
			}
		}
	}
	surface = IMG_Load(("Textures"s + FILE_SEPARATOR + "Cards"s + FILE_SEPARATOR + "I.png").c_str());
	closed_card_image = SDL_CreateTextureFromSurface(renderer, surface);
	if (closed_card_image == nullptr) {
		cout << "Failed to load \"Textures"s + FILE_SEPARATOR + "Cards"s + FILE_SEPARATOR + "I.png\"!" << endl;
		throw 1;
	}
	surface = IMG_Load(("Textures"s + FILE_SEPARATOR + "Cards"s + FILE_SEPARATOR + "E.png").c_str());
	empty_place_image = SDL_CreateTextureFromSurface(renderer, surface);
	if (empty_place_image == nullptr) {
		cout << "Failed to load \"Textures"s + FILE_SEPARATOR + "Cards"s + FILE_SEPARATOR + "E.png\"!" << endl;
		throw 1;
	}
	bg_count = 3;
	settingBackground = 0;
	backgrounds = vector<SDL_Texture*>(bg_count);
	for(int i = 0; i < bg_count; i++) {
		surface = IMG_Load(("Textures"s + FILE_SEPARATOR + "BG" + to_string(i + 1) + ".png").c_str());
		backgrounds[i] = SDL_CreateTextureFromSurface(renderer, surface);
		if (backgrounds[i] == nullptr) {
			cout << "Failed to load \"Textures"s + FILE_SEPARATOR + "BG" << i + 1 << ".png\"!" << endl;
			throw 1;
		}
	}
	surface = IMG_Load(("Textures"s + FILE_SEPARATOR + "NG.png").c_str());
	newgamebutton_img = SDL_CreateTextureFromSurface(renderer, surface);
	if (newgamebutton_img == nullptr) {
		cout << "Failed to load \"Textures"s + FILE_SEPARATOR + "NG.png\"!" << endl;
		throw 1;
	}
	surface = IMG_Load(("Textures"s + FILE_SEPARATOR + "UD.png").c_str());
	undobutton_img = SDL_CreateTextureFromSurface(renderer, surface);
	if (undobutton_img == nullptr) {
		cout << "Failed to load \"Textures"s + FILE_SEPARATOR + "UD.png\"!" << endl;
		throw 1;
	}
	surface = IMG_Load(("Textures"s + FILE_SEPARATOR + "Quit.png").c_str());
	quitbutton_img = SDL_CreateTextureFromSurface(renderer, surface);
	if (quitbutton_img == nullptr) {
		cout << "Failed to load \"Textures"s + FILE_SEPARATOR + "Quit.png\"!" << endl;
		throw 1;
	}
	surface = IMG_Load(("Textures"s + FILE_SEPARATOR + "Pause.png").c_str());
	pausebutton_img = SDL_CreateTextureFromSurface(renderer, surface);
	if (pausebutton_img == nullptr) {
		cout << "Failed to load \"Textures"s + FILE_SEPARATOR + "Pause.png\"!" << endl;
		throw 1;
	}
	surface = IMG_Load(("Textures"s + FILE_SEPARATOR + "Play.png").c_str());
	playbutton_img = SDL_CreateTextureFromSurface(renderer, surface);
	if (playbutton_img == nullptr) {
		cout << "Failed to load \"Textures"s + FILE_SEPARATOR + "Play.png\"!" << endl;
		throw 1;
	}
	surface = IMG_Load(("Textures"s + FILE_SEPARATOR + "Statistics.png").c_str());
	statsbutton_img = SDL_CreateTextureFromSurface(renderer, surface);
	if (statsbutton_img == nullptr) {
		cout << "Failed to load \"Textures"s + FILE_SEPARATOR + "Statistics.png\"!" << endl;
		throw 1;
	}
	surface = IMG_Load(("Textures"s + FILE_SEPARATOR + "Settings.png").c_str());
	setbutton_img = SDL_CreateTextureFromSurface(renderer, surface);
	if (setbutton_img == nullptr)
	{
		cout << "Failed to load \"Textures"s + FILE_SEPARATOR + "Settings.png\"!" << endl;
		throw 1;
	}
	surface = IMG_Load(("Textures"s + FILE_SEPARATOR + "Selected.png").c_str());
	select_bound = SDL_CreateTextureFromSurface(renderer, surface);
	if (select_bound == nullptr) {
		cout << "Failed to load \"Textures"s + FILE_SEPARATOR + "Selected.png\"!" << endl;
		throw 1;
	}
	surface = IMG_Load(("Textures"s + FILE_SEPARATOR + "Particle.png").c_str());
	particle = SDL_CreateTextureFromSurface(renderer, surface);
	if (particle == nullptr) {
		cout << "Failed to load \"Textures"s + FILE_SEPARATOR + "Particle.png\"!" << endl;
		throw 1;
	}
	surface = IMG_Load(("Textures"s + FILE_SEPARATOR + "ParticleOverlay.png").c_str());
	particle_overlay = SDL_CreateTextureFromSurface(renderer, surface);
	if (particle_overlay == nullptr) {
		cout << "Failed to load \"Textures"s + FILE_SEPARATOR + "ParticleOverlay.png\"!" << endl;
		throw 1;
	}
	surface = IMG_Load(("Textures"s + FILE_SEPARATOR + "NG_One.png").c_str());
	new_one_image = SDL_CreateTextureFromSurface(renderer, surface);
	if (new_one_image == nullptr) {
		cout << "Failed to load \"Textures"s + FILE_SEPARATOR + "NG_One.png\"!" << endl;
		throw 1;
	}
	surface = IMG_Load(("Textures"s + FILE_SEPARATOR + "NG_Three.png").c_str());
	new_three_image = SDL_CreateTextureFromSurface(renderer, surface);
	if (new_three_image == nullptr) {
		cout << "Failed to load \"Textures"s + FILE_SEPARATOR + "NG_Three.png\"!" << endl;
		throw 1;
	}
	SDL_FreeSurface(surface);
	font = new Font(renderer, "Fonts"s + FILE_SEPARATOR + "main_font.fl"s);
	font->Height = 32;
	font->Interval = 2;
	font->Spacing = 2;
}

Storage::~Storage() {
	for (auto& e1 : opened_card_images) {
		for (auto& e2 : e1) {
			SDL_DestroyTexture(e2);
		}
		e1.~vector();
	}
	font->~Font();
	opened_card_images.~vector();
	SDL_DestroyTexture(closed_card_image);
	SDL_DestroyTexture(empty_place_image);
	for(int i = 0; i < bg_count; i++) {
		SDL_DestroyTexture(backgrounds[i]);
	}
	backgrounds.~vector();
	SDL_DestroyTexture(newgamebutton_img);
	SDL_DestroyTexture(undobutton_img);
	SDL_DestroyTexture(quitbutton_img);
	SDL_DestroyTexture(pausebutton_img);
	SDL_DestroyTexture(playbutton_img);
	SDL_DestroyTexture(statsbutton_img);
	SDL_DestroyTexture(setbutton_img);
	SDL_DestroyTexture(select_bound);
	SDL_DestroyTexture(particle);
	SDL_DestroyTexture(particle_overlay);
	SDL_DestroyTexture(new_one_image);
	SDL_DestroyTexture(new_three_image);
}