#include "Storage.h"

Storage::Storage(SDL_Renderer *renderer) {
	string suits[] = { "H", "C", "D", "S" };
	string ranks[] = { "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };
	opened_card_images = vector<vector<SDL_Texture*>>(4, vector<SDL_Texture*>(13));
	SDL_Surface* surface;
	for (int suit = 0; suit < 4; suit++) {
		for (int rank = 0; rank < 13; rank++) {
			surface = IMG_Load(("Textures\\Cards\\"s + suits[suit] + ranks[rank] + ".png"s).c_str());
			opened_card_images[suit][rank] = SDL_CreateTextureFromSurface(renderer, surface);
			if (opened_card_images[suit][rank] == nullptr) {
				cout << "Failed to load \"" << "Textures\\Cards\\"s + suits[suit] + ranks[rank] + ".png"s << "\"!" << endl;
				throw 1;
			}
		}
	}
	surface = IMG_Load("Textures\\Cards\\I.png");
	closed_card_image = SDL_CreateTextureFromSurface(renderer, surface);
	if (closed_card_image == nullptr) {
		cout << "Failed to load \"Textures\\Cards\\I.png\"!" << endl;
		throw 1;
	}
	surface = IMG_Load("Textures\\Cards\\E.png");
	empty_place_image = SDL_CreateTextureFromSurface(renderer, surface);
	if (empty_place_image == nullptr) {
		cout << "Failed to load \"Textures\\Cards\\E.png\"!" << endl;
		throw 1;
	}
	bg_count = 3;
	settingBackground = 0;
	backgrounds = vector<SDL_Texture*>(bg_count);
	for(int i = 0; i < bg_count; i++) {
		surface = IMG_Load(("Textures\\BG" + to_string(i + 1) + ".png").c_str());
		backgrounds[i] = SDL_CreateTextureFromSurface(renderer, surface);
		if (backgrounds[i] == nullptr) {
			cout << "Failed to load \"Textures\\BG" << i + 1 << ".png\"!" << endl;
			throw 1;
		}
	}
	surface = IMG_Load("Textures\\NG.png");
	newgamebutton_img = SDL_CreateTextureFromSurface(renderer, surface);
	if (newgamebutton_img == nullptr) {
		cout << "Failed to load \"Textures\\NG.png\"!" << endl;
		throw 1;
	}
	surface = IMG_Load("Textures\\UD.png");
	undobutton_img = SDL_CreateTextureFromSurface(renderer, surface);
	if (undobutton_img == nullptr) {
		cout << "Failed to load \"Textures\\UD.png\"!" << endl;
		throw 1;
	}
	surface = IMG_Load("Textures\\Quit.png");
	quitbutton_img = SDL_CreateTextureFromSurface(renderer, surface);
	if (quitbutton_img == nullptr) {
		cout << "Failed to load \"Textures\\Quit.png\"!" << endl;
		throw 1;
	}
	surface = IMG_Load("Textures\\Pause.png");
	pausebutton_img = SDL_CreateTextureFromSurface(renderer, surface);
	if (pausebutton_img == nullptr) {
		cout << "Failed to load \"Textures\\Pause.png\"!" << endl;
		throw 1;
	}
	surface = IMG_Load("Textures\\Play.png");
	playbutton_img = SDL_CreateTextureFromSurface(renderer, surface);
	if (playbutton_img == nullptr) {
		cout << "Failed to load \"Textures\\Play.png\"!" << endl;
		throw 1;
	}
	surface = IMG_Load("Textures\\Statistics.png");
	statsbutton_img = SDL_CreateTextureFromSurface(renderer, surface);
	if (statsbutton_img == nullptr) {
		cout << "Failed to load \"Textures\\Statistics.png\"!" << endl;
		throw 1;
	}
	surface = IMG_Load("Textures\\Settings.png");
	setbutton_img = SDL_CreateTextureFromSurface(renderer, surface);
	if (setbutton_img == nullptr)
	{
		cout << "Failed to load \"Textures\\Settings.png\"!" << endl;
		throw 1;
	}
	surface = IMG_Load("Textures\\Selected.png");
	select_bound = SDL_CreateTextureFromSurface(renderer, surface);
	if (select_bound == nullptr) {
		cout << "Failed to load \"Textures\\Selected.png\"!" << endl;
		throw 1;
	}
	surface = IMG_Load("Textures\\YW.png");
	win_image = SDL_CreateTextureFromSurface(renderer, surface);
	if (win_image == nullptr) {
		cout << "Failed to load \"Textures\\YW.png\"!" << endl;
		throw 1;
	}
	surface = IMG_Load("Textures\\PausedLabel.png");
	pausedlabel_image = SDL_CreateTextureFromSurface(renderer, surface);
	if (pausedlabel_image == nullptr) {
		cout << "Failed to load \"Textures\\PausedLabel.png\"!" << endl;
		throw 1;
	}
	surface = IMG_Load("Textures\\ScoreLabel.png");
	score_label_img = SDL_CreateTextureFromSurface(renderer, surface);
	if (score_label_img == nullptr) {
		cout << "Failed to load \"Textures\\ScoreLabel.png\"!" << endl;
		throw 1;
	}
	surface = IMG_Load("Textures\\TimeLabel.png");
	time_label_img = SDL_CreateTextureFromSurface(renderer, surface);
	if (time_label_img == nullptr) {
		cout << "Failed to load \"Textures\\TimeLabel.png\"!" << endl;
		throw 1;
	}
	surface = IMG_Load("Textures\\NowLabel.png");
	now_label_img = SDL_CreateTextureFromSurface(renderer, surface);
	if (now_label_img == nullptr) {
		cout << "Failed to load \"Textures\\NowLabel.png\"!" << endl;
		throw 1;
	}
	surface = IMG_Load("Textures\\Digits.png");
	digits_tile = SDL_CreateTextureFromSurface(renderer, surface);
	if (digits_tile == nullptr) {
		cout << "Failed to load \"Textures\\Particle.png\"!" << endl;
		throw 1;
	}
	surface = IMG_Load("Textures\\Particle.png");
	particle = SDL_CreateTextureFromSurface(renderer, surface);
	if (particle == nullptr) {
		cout << "Failed to load \"Textures\\Particle.png\"!" << endl;
		throw 1;
	}
	surface = IMG_Load("Textures\\ParticleOverlay.png");
	particle_overlay = SDL_CreateTextureFromSurface(renderer, surface);
	if (particle_overlay == nullptr) {
		cout << "Failed to load \"Textures\\ParticleOverlay.png\"!" << endl;
		throw 1;
	}

	surface = IMG_Load("Textures\\NGWindow.png");
	newgame_window_image = SDL_CreateTextureFromSurface(renderer, surface);
	if (newgame_window_image == nullptr) {
		cout << "Failed to load \"Textures\\NGWindow.png\"!" << endl;
		throw 1;
	}
	surface = IMG_Load("Textures\\SettingsWindow.png");
	settings_window_image = SDL_CreateTextureFromSurface(renderer, surface);
	if (settings_window_image == nullptr) {
		cout << "Failed to load \"Textures\\SettingsWindow.png\"!" << endl;
		throw 1;
	}
	surface = IMG_Load("Textures\\CancelBigButton.png");
	cancel_big_button_image = SDL_CreateTextureFromSurface(renderer, surface);
	if (cancel_big_button_image == nullptr) {
		cout << "Failed to load \"Textures\\CancelBigButton.png\"!" << endl;
		throw 1;
	}
	surface = IMG_Load("Textures\\NG_One.png");
	new_one_image = SDL_CreateTextureFromSurface(renderer, surface);
	if (new_one_image == nullptr) {
		cout << "Failed to load \"Textures\\NG_One.png\"!" << endl;
		throw 1;
	}
	surface = IMG_Load("Textures\\NG_Three.png");
	new_three_image = SDL_CreateTextureFromSurface(renderer, surface);
	if (new_three_image == nullptr) {
		cout << "Failed to load \"Textures\\NG_Three.png\"!" << endl;
		throw 1;
	}
	SDL_FreeSurface(surface);
	//font = TTF_OpenFont("Textures\\KFFont.ttf", 25);
	for (int i = 0; i < 11; i++) {
		digits_pos_on_tile.push_back(SDL_Rect{ i * 16, 0, 16, 32 });
	}
}

Storage::~Storage() {
	for (auto& e1 : opened_card_images) {
		for (auto& e2 : e1) {
			SDL_DestroyTexture(e2);
		}
		e1.~vector();
	}
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
	SDL_DestroyTexture(win_image);
	SDL_DestroyTexture(pausedlabel_image);
	SDL_DestroyTexture(score_label_img);
	SDL_DestroyTexture(time_label_img);
	SDL_DestroyTexture(now_label_img);
	SDL_DestroyTexture(digits_tile);
	SDL_DestroyTexture(particle);
	SDL_DestroyTexture(particle_overlay);
	SDL_DestroyTexture(newgame_window_image);
	SDL_DestroyTexture(cancel_big_button_image);
	SDL_DestroyTexture(new_one_image);
	SDL_DestroyTexture(new_three_image);
	//TTF_CloseFont(font);
}

void Storage::DrawString(SDL_Renderer* renderer, Storage* storage, string str, SDL_Point& p) {
	SDL_Rect dest = { p.x, p.y, 16, 32 };
	SDL_Rect space_rect = { 0, 0, 0, 0 };
	for (auto& c : str) {
		if (c >= '0' && c <= '9') {
			SDL_RenderCopy(renderer, storage->digits_tile, &(storage->digits_pos_on_tile[int(c - '0')]), &dest);
			dest.x += 16;
		}
		else if (c == ':') {
			SDL_RenderCopy(renderer, storage->digits_tile, &(storage->digits_pos_on_tile[10]), &dest);
			dest.x += 16;
		}
		else if (c == ' ') {
			SDL_RenderCopy(renderer, storage->digits_tile, &space_rect, &dest);
			dest.x += 16;
		}
		else if (c == '-') {
		}
		else {
			cout << "Error: Couldnot draw character '" << c << "'!" << endl;
			throw 1;
		}
	}
}

time_t Storage::TimeCut(vector<time_t>& points) {
	if (points.size() % 2 == 0) { // paused or finished
		time_t result = 0;
		for (int i = points.size() - 1; i >= 0; i -= 2) {
			result += points[i];
			result -= points[i - 1];
		}
		return result;
	}
	else { // playing
		time_t result = time(NULL) - points.back();
		for (int i = points.size() - 2; i >= 0; i -= 2) {
			result += points[i];
			result -= points[i - 1];
		}
		return result;
	}
}

string Storage::GetStrTime(time_t time) {
	int seconds = int(time % 60);
	int minutes = int((time / 60) % 60);
	int hours = int(time / 3600);

	string result;
	if (hours) {
		result += to_string(hours) + ":";
	}
	if (minutes < 10) {
		result += "0";
	}
	result += to_string(minutes) + ":";
	if (seconds < 10) {
		result += "0";
	}
	result += to_string(seconds);
	return result;
}

string Storage::GetCurrentStrTime() {
	time_t t = time(0);
	tm* lt = new tm();
	localtime_s(lt, &t);
	string result;
	if (lt->tm_hour < 10) {
		result += " ";
	}
	result += to_string(lt->tm_hour) + ":";
	if (lt->tm_min < 10) {
		result += "0";
	}
	result += to_string(lt->tm_min) + ":";
	if (lt->tm_sec < 10) {
		result += "0";
	}
	result += to_string(lt->tm_sec);
	return result;
}

void Storage::Encode4B(ofstream& out, unsigned long v) {
	out << (char)(v & 0xff);
	out << (char)((v >> 8) & 0xff);
	out << (char)((v >> 16) & 0xff);
	out << (char)((v >> 24) & 0xff);
}

void Storage::Encode8B(ofstream& out, unsigned long long v) {
	out << (char)(v & 0xff);
	out << (char)((v >> 8) & 0xff);
	out << (char)((v >> 16) & 0xff);
	out << (char)((v >> 24) & 0xff);
	out << (char)((v >> 32) & 0xff);
	out << (char)((v >> 40) & 0xff);
	out << (char)((v >> 48) & 0xff);
	out << (char)((v >> 56) & 0xff);
}

unsigned long Storage::Decode4B(ifstream& in) {
	char c;
	unsigned long result = 0;
	for (int i = 0; i < 4; i++) {
		in.get(c);
		result |= ((unsigned long)(*(unsigned char*)(&c)) << (i * 8));
	}
	return result;
}

unsigned long long Storage::Decode8B(ifstream& in) {
	char c;
	unsigned long long result = 0;
	for (int i = 0; i < 8; i++) {
		in.get(c);
		result |= ((unsigned long long)(*(unsigned char*)(&c)) << (i * 8));
	}
	return result;
}