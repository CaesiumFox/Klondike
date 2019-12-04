#include "Klondike.h"

Klondike::Klondike() {
	srand((uint32_t)time(0));
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("Klondike", 100, 100, 0, 0, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN);
	SDL_GetWindowSize(window, &WinWidth, &WinHeight);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	//SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	event = SDL_Event();
	storage = new Storage(renderer);
	state = GameState::Playing;

	selected = { '0', 0 };

	foundations_pos = vector<SDL_Rect>(4);
	tableau_pos = vector<SDL_Rect>(7);
	for(auto &e : layout.foundations)
		e = deque<Card>(0);
	for (auto& e : layout.tableau)
		e = deque<Card>(0);

	stock_pos = { (WinWidth / 2 - 464) + 32, 32, 96, 128 };
	waste_pos = { (WinWidth / 2 - 464) + 160, 32, 96, 128 };
	waste_hitbox = { (WinWidth / 2 - 464) + 160, 32, 160, 128 };
	for (int i = 0; i < 4; i++) foundations_pos[i] = { (WinWidth / 2 - 464) + 416 + 128 * i, 32, 96, 128 };
	for (int i = 0; i < 7; i++) tableau_pos[i] = { (WinWidth / 2 - 464) + 32 + 128 * i, 192, 96, 128 };
	youwin_pos = { WinWidth / 2 - 128, WinHeight / 2 - 32, 256, 64 };

	scorelabel_pos = { (WinWidth / 2 - 464) + 32, 0, 128, 32 };
	timelabel_pos = { (WinWidth / 2 - 464) + 288, 0, 128, 32 };
	nowlabel_pos = { WinWidth - 256, WinHeight - 32, 128, 32 };
	scorevalue_pos = { (WinWidth / 2 - 464) + 160 , 0};
	timevalue_pos = { (WinWidth / 2 - 464) + 416 , 0 };
	nowvalue_pos = { WinWidth - 128 , WinHeight - 32 };

	pb_pos = { 0, 0, 32, 32 };
	ngb_pos = { 0, 32, 32, 32 };
	udb_pos = {0, 64, 32, 32};
	set_pos = {0, 96, 32, 32};
	qb_pos = { WinWidth - 32, 0, 32, 32 };

	newgame_window_pos = { WinWidth / 2 - 192, WinHeight / 2 - 160, 384, 320 };
	newgame_btn1_pos = { newgame_window_pos.x + 64, newgame_window_pos.y + 64, 128, 128 };
	newgame_btn3_pos = { newgame_window_pos.x + 192, newgame_window_pos.y + 64, 128, 128 };
	newgame_cancel_pos = { newgame_window_pos.x + 128, newgame_window_pos.y + 224, 128, 64 };

	settings_window_pos = { WinWidth / 2 - 320, WinHeight / 2 - 200, 640, 400 };
	settings_bg_pos = vector<SDL_Rect>(3);
	settings_bg_pos[0] = { settings_window_pos.x + 64, settings_window_pos.y + 64, 128, 128 };
	settings_bg_pos[1] = {settings_window_pos.x + 256, settings_window_pos.y + 64, 128, 128};
	settings_bg_pos[2] = {settings_window_pos.x + 448, settings_window_pos.y + 64, 128, 128};
	settings_cancel_pos = { settings_window_pos.x + 256, settings_window_pos.y + 272, 128, 64 };

	final_firework_1 = nullptr;
	final_firework_2 = nullptr;
	final_firework_3 = nullptr;
	final_firework_4 = nullptr;

	delay = 0;
	running = true;
	win = false;
	gamemode = GameMode::OneCard;
	shown = FormShown::GamePlay;
	LoadFile();
	Loop();
}
Klondike::~Klondike() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	storage->~Storage();
	layout.stock.~deque();
	layout.waste.~deque();
	for (auto& e : layout.foundations)
		e.~deque();
	for (auto& e : layout.tableau)
		e.~deque();
	SDL_Quit();
}

void Klondike::Loop() {
	while (running) {
		Input();
		Update();
		Render();
	}
}

void Klondike::Render() {
	SDL_RenderClear(renderer);
	
	RenderEverywhereBelow();
	switch (shown) {
	case FormShown::GamePlay:
		switch (state) {
		case GameState::Playing:
			RenderPlaying();
			break;
		case GameState::Paused:
			RenderPaused();
			break;
		case GameState::Finished:
			RenderFinished();
			break;
		}
		break;
	case FormShown::NewGameDialog:
		RenderNewGame();
		break;
	case FormShown::SettingsDialog:
		RenderSettings();
		break;
	}
	RenderEverywhereAbove();

	SDL_RenderPresent(renderer);
}
void Klondike::RenderEverywhereBelow() {
	SDL_RenderCopy(renderer, storage->backgrounds[storage->settingBackground], NULL, NULL);
}
void Klondike::RenderPlaying() {
	DrawStock();
	DrawWaste();
	DrawFoundations();
	DrawFannedPiles();
	SDL_RenderCopy(renderer, storage->undobutton_img, NULL, &udb_pos);
	SDL_RenderCopy(renderer, storage->pausebutton_img, NULL, &pb_pos);
	SDL_RenderCopy(renderer, storage->newgamebutton_img, NULL, &ngb_pos);
}
void Klondike::RenderPaused() {
	SDL_RenderCopy(renderer, storage->empty_place_image, NULL, &stock_pos);
	SDL_RenderCopy(renderer, storage->empty_place_image, NULL, &waste_pos);
	for (int i = 0; i < 4; i++)
		SDL_RenderCopy(renderer, storage->empty_place_image, NULL, &foundations_pos[i]);
	for (int i = 0; i < 7; i++)
		SDL_RenderCopy(renderer, storage->empty_place_image, NULL, &tableau_pos[i]);
	SDL_RenderCopy(renderer, storage->undobutton_img, NULL, &udb_pos);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 64);
	SDL_RenderFillRect(renderer, NULL);
	SDL_RenderCopy(renderer, storage->playbutton_img, NULL, &pb_pos);
	SDL_RenderCopy(renderer, storage->newgamebutton_img, NULL, &ngb_pos);
	SDL_RenderCopy(renderer, storage->setbutton_img, NULL, &set_pos);
	SDL_RenderCopy(renderer, storage->pausedlabel_image, NULL, &youwin_pos);
}
void Klondike::RenderFinished() {
	DrawStock();
	DrawWaste();
	DrawFoundations();
	DrawFannedPiles();
	SDL_RenderCopy(renderer, storage->pausebutton_img, NULL, &pb_pos);
	SDL_RenderCopy(renderer, storage->undobutton_img, NULL, &udb_pos);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 64);
	SDL_RenderFillRect(renderer, NULL);
	SDL_RenderCopy(renderer, storage->newgamebutton_img, NULL, &ngb_pos);
	if (win) {
		if (final_firework_1) final_firework_1->Draw(renderer, storage);
		if (final_firework_2) final_firework_2->Draw(renderer, storage);
		if (final_firework_3) final_firework_3->Draw(renderer, storage);
		if (final_firework_4) final_firework_4->Draw(renderer, storage);
		SDL_RenderCopy(renderer, storage->win_image, NULL, &youwin_pos);
	}
}
void Klondike::RenderNewGame() {
	SDL_RenderCopy(renderer, storage->empty_place_image, NULL, &stock_pos);
	SDL_RenderCopy(renderer, storage->empty_place_image, NULL, &waste_pos);
	for (int i = 0; i < 4; i++)
		SDL_RenderCopy(renderer, storage->empty_place_image, NULL, &foundations_pos[i]);
	for (int i = 0; i < 7; i++)
		SDL_RenderCopy(renderer, storage->empty_place_image, NULL, &tableau_pos[i]);
	SDL_RenderCopy(renderer, storage->undobutton_img, NULL, &udb_pos);
	SDL_RenderCopy(renderer, storage->pausebutton_img, NULL, &pb_pos);
	SDL_RenderCopy(renderer, storage->newgamebutton_img, NULL, &ngb_pos);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 64);
	SDL_RenderFillRect(renderer, NULL);

	SDL_RenderCopy(renderer, storage->newgame_window_image, NULL, &newgame_window_pos);
	SDL_RenderCopy(renderer, storage->new_one_image, NULL, &newgame_btn1_pos);
	SDL_RenderCopy(renderer, storage->new_three_image, NULL, &newgame_btn3_pos);
	SDL_RenderCopy(renderer, storage->cancel_big_button_image, NULL, &newgame_cancel_pos);
}
void Klondike::RenderSettings() {
	SDL_RenderCopy(renderer, storage->empty_place_image, NULL, &stock_pos);
	SDL_RenderCopy(renderer, storage->empty_place_image, NULL, &waste_pos);
	for (int i = 0; i < 4; i++)
		SDL_RenderCopy(renderer, storage->empty_place_image, NULL, &foundations_pos[i]);
	for (int i = 0; i < 7; i++)
		SDL_RenderCopy(renderer, storage->empty_place_image, NULL, &tableau_pos[i]);
	SDL_RenderCopy(renderer, storage->undobutton_img, NULL, &udb_pos);
	SDL_RenderCopy(renderer, storage->pausebutton_img, NULL, &pb_pos);
	SDL_RenderCopy(renderer, storage->newgamebutton_img, NULL, &ngb_pos);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 64);
	SDL_RenderFillRect(renderer, NULL);

	SDL_RenderCopy(renderer, storage->settings_window_image, NULL, &settings_window_pos);
	for (int i = 0; i < storage->bg_count; i++) {
		SDL_RenderCopy(renderer, storage->backgrounds[i], NULL, &settings_bg_pos[i]);
	}
	SDL_RenderCopy(renderer, storage->cancel_big_button_image, NULL, &settings_cancel_pos);
}
void Klondike::RenderEverywhereAbove() {
	SDL_RenderCopy(renderer, storage->score_label_img, NULL, &scorelabel_pos);
	SDL_RenderCopy(renderer, storage->time_label_img, NULL, &timelabel_pos);
	SDL_RenderCopy(renderer, storage->now_label_img, NULL, &nowlabel_pos);
	Storage::DrawString(renderer, storage, to_string(layout.score), scorevalue_pos);
	Storage::DrawString(renderer, storage, Storage::GetStrTime(delay + Storage::TimeCut(time_points)), timevalue_pos);
	Storage::DrawString(renderer, storage, Storage::GetCurrentStrTime(), nowvalue_pos);
	SDL_RenderCopy(renderer, storage->quitbutton_img, NULL, &qb_pos);
}

void Klondike::Input() {
	while (SDL_PollEvent(&event)) {
		InputEverywhere();
		switch (shown) {
		case FormShown::GamePlay:
			switch (state) {
			case GameState::Playing:
				InputPlaying();
				break;
			case GameState::Paused:
				InputPaused();
				break;
			case GameState::Finished:
				InputFinished();
				break;
			default:
				break;
			}
			break;
		case FormShown::NewGameDialog:
			InputNewGame();
			break;
		case FormShown::SettingsDialog:
			InputSettings();
			break;
		default:
			break;
		}
	}
}
void Klondike::InputEverywhere() {
	switch (event.type) {
	case SDL_MOUSEBUTTONDOWN:
		if (event.button.button == SDL_BUTTON_LEFT) {
			SDL_Point m = { event.button.x, event.button.y };
			if (SDL_PointInRect(&m, &qb_pos)) {
				running = false;
				SaveFile();
				return;
			}
		}
		break;
	case SDL_QUIT:
		running = false;
		SaveFile();
		break;
	default:
		break;
	}
}
void Klondike::InputPlaying() {
	switch (event.type) {
	case SDL_MOUSEBUTTONDOWN:
		if (event.button.button == SDL_BUTTON_LEFT) {
			SDL_Point m = { event.button.x, event.button.y };
			if (SDL_PointInRect(&m, &pb_pos)) {
				state = GameState::Paused;
				time_points.push_back(time(NULL));
				return;
			}
			if (SDL_PointInRect(&m, &ngb_pos)) {
				selected = { '0', 0 };
				state = GameState::Paused;
				shown = FormShown::NewGameDialog;
				time_points.push_back(time(NULL));
				return;
			}
			if (SDL_PointInRect(&m, &udb_pos)) {
				selected = { '0', 0 };
				Undo();
				return;
			}
			if (SDL_PointInRect(&m, &stock_pos)) {
				selected = { '0', 0 };
				if (layout.stock.empty()) {
					// don't forget:
					// stock.top = stock.back
					// waste.top = waste.front
					layout.stock = layout.waste;
					layout.waste.clear();
					for (auto& e : layout.stock) {
						e.Turn();
					}
					if (gamemode == GameMode::OneCard) {
						layout.score -= 100;
						if (layout.score < 0) layout.score = 0;
					}
				}
				else {
					for (int i = 0; i < (gamemode == GameMode::OneCard ? 1 : 3) && !layout.stock.empty(); i++) {
						layout.waste.push_front(layout.stock.back());
						layout.waste.front().Turn();
						layout.stock.pop_back();
					}
				}
				Record();
				return;
			}
			if (SDL_PointInRect(&m, &waste_hitbox)) {
				if (!layout.waste.empty()) {
					selected.where = 'w';
				}
				else {
					selected.where = '0';
				}
				return;
			}
			// maybe foundations ?
			for (int f = 0; f < 4; f++) {
				if (SDL_PointInRect(&m, &foundations_pos[f])) {
					if (selected.where == 'w') {
						if (PossibleToFoundation(f, &layout.waste.front())) {
							layout.foundations[f].push_front(layout.waste.front());
							layout.waste.pop_front();
							layout.score += 10;
							selected.where = '0';
							Record();
							return;
						}
						else if (!layout.foundations[f].empty()) {
							selected.where = 'a' + f;
						}
						else {
							selected.where = '0';
						}
					}
					else if (selected.where >= '1' && selected.where <= '7' && selected.pos == 0) {
						if (PossibleToFoundation(f, &layout.tableau[int(selected.where - '1')].front())) {
							layout.foundations[f].push_front(layout.tableau[int(selected.where - '1')].front());
							layout.tableau[int(selected.where - '1')].pop_front();
							layout.score += 10;
							selected.where = '0';
							Record();
							return;
						}
						else if (!layout.foundations[f].empty()) {
							selected.where = 'a' + f;
						}
						else {
							selected.where = '0';
						}
					}
					else if (selected.where >= 'a' && selected.where <= 'd') {
						if (f != int(selected.where - 'a'))
							layout.foundations[f].swap(layout.foundations[int(selected.where - 'a')]);
						selected.where = '0';
						Record();
						return;
					}
					else if (!layout.foundations[f].empty()) {
						selected.where = 'a' + f;
					}
					else {
						selected.where = '0';
					}
					return;
				}
			}
			// maybe tableau ?
			int pile = WhichPile(m.x);
			if (pile != -1) {
				int index = WhichCardInPile(pile, m.y);
				if (index == 0 || index == -2) {
					if (selected.where == 'w') {
						if (PossibleToPile(pile, &layout.waste.front())) {
							layout.tableau[pile].push_front(layout.waste.front());
							layout.waste.pop_front();
							layout.score += 5;
							selected.where = '0';
							Record();
						}
						else if (!layout.tableau[pile].empty()) {
							selected = { char('1' + pile), index };
						}
						else {
							selected.where = '0';
						}
					}
					else if (selected.where >= 'a' && selected.where <= 'd') {
						if (PossibleToPile(pile, &layout.foundations[int(selected.where - 'a')].front())) {
							layout.tableau[pile].push_front(layout.foundations[int(selected.where - 'a')].front());
							layout.foundations[int(selected.where - 'a')].pop_front();
							layout.score -= 15;
							if (layout.score < 0) layout.score = 0;
							selected.where = '0';
							Record();
						}
						else if (!layout.tableau[pile].empty()) {
							selected = { char('1' + pile), index };
						}
						else {
							selected.where = '0';
						}
					}
					else if (selected.where >= '1' && selected.where <= '7') {
						if (PossibleToPile(pile, &layout.tableau[int(selected.where - '1')][selected.pos]) && layout.tableau[int(selected.where - '1')][selected.pos].IsOpened()) {
							for (int i = selected.pos; i >= 0; i--)
								layout.tableau[pile].push_front(layout.tableau[int(selected.where - '1')][i]);
							for (int i = selected.pos; i >= 0; i--)
								layout.tableau[int(selected.where - '1')].pop_front();
							selected.where = '0';
							Record();
						}
						else if (!layout.tableau[pile].empty()) {
							selected = { char('1' + pile), index };
						}
						else {
							selected.where = '0';
						}
					}
					else if (!layout.tableau[pile].empty()) {
						selected = { char('1' + pile), index };
					}
					else {
						selected.where = '0';
					}
					return;
				}
				else if (index != -1) {
					selected = { char('1' + pile), index };
					return;
				}
			}
			// TODO
			selected = { '0', 0 };
		}
		if (event.button.button == SDL_BUTTON_RIGHT) {
			bool builded = Build();
			if(builded) Record();
		}
		break;
	case SDL_KEYDOWN:
		switch (event.key.keysym.scancode) {
		case SDL_Scancode::SDL_SCANCODE_ESCAPE:
			state = GameState::Paused;
			time_points.push_back(time(NULL));
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}
void Klondike::InputPaused() {
	switch (event.type) {
	case SDL_MOUSEBUTTONDOWN:
		if (event.button.button == SDL_BUTTON_LEFT) {
			SDL_Point m = { event.button.x, event.button.y };
			if (SDL_PointInRect(&m, &pb_pos)) {
				state = GameState::Playing;
				time_points.push_back(time(NULL));
				return;
			}
			if (SDL_PointInRect(&m, &ngb_pos)) {
				selected = { '0', 0 };
				shown = FormShown::NewGameDialog;
				return;
			}
			if (SDL_PointInRect(&m, &set_pos)) {
				shown = FormShown::SettingsDialog;
				return;
			}
		}
		break;
	case SDL_KEYDOWN:
		switch (event.key.keysym.scancode) {
		case SDL_Scancode::SDL_SCANCODE_ESCAPE:
			state = GameState::Playing;
			time_points.push_back(time(NULL));
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}
void Klondike::InputFinished() {
	switch (event.type) {
	case SDL_MOUSEBUTTONDOWN:
		if (event.button.button == SDL_BUTTON_LEFT) {
			SDL_Point m = { event.button.x, event.button.y };
			if (SDL_PointInRect(&m, &ngb_pos)) {
				selected = { '0', 0 };
				shown = FormShown::NewGameDialog;
				return;
			}
		}
		break;
	default:
		break;
	}
}
void Klondike::InputNewGame() {
	switch (event.type) {
	case SDL_MOUSEBUTTONDOWN:
		if (event.button.button == SDL_BUTTON_LEFT) {
			SDL_Point m = { event.button.x, event.button.y };
			if (SDL_PointInRect(&m, &newgame_btn1_pos)) {
				selected = { '0', 0 };
				shown = FormShown::GamePlay;
				state = GameState::Playing;
				NewGame(GameMode::OneCard);
				time_points.push_back(time(NULL));
				return;
			}
			if (SDL_PointInRect(&m, &newgame_btn3_pos)) {
				selected = { '0', 0 };
				shown = FormShown::GamePlay;
				state = GameState::Playing;
				NewGame(GameMode::ThreeCards);
				time_points.push_back(time(NULL));
				return;
			}
			if (SDL_PointInRect(&m, &newgame_cancel_pos)) {
				shown = FormShown::GamePlay;
				if(state != GameState::Finished) time_points.push_back(time(NULL));
				return;
			}
		}
		break;
	case SDL_KEYDOWN:
		switch (event.key.keysym.scancode) {
		case SDL_Scancode::SDL_SCANCODE_ESCAPE:
			shown = FormShown::GamePlay;
			time_points.push_back(time(NULL));
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}
void Klondike::InputSettings() {
	switch (event.type) {
	case SDL_MOUSEBUTTONDOWN:
		if (event.button.button == SDL_BUTTON_LEFT) {
			SDL_Point m = { event.button.x, event.button.y };
			for (int i = 0; i < storage->bg_count; i++) {
				if (SDL_PointInRect(&m, &settings_bg_pos[i])) {
					storage->settingBackground = i;
					shown = FormShown::GamePlay;
					state = GameState::Paused;
					return;
				}
			}
			if (SDL_PointInRect(&m, &settings_cancel_pos)) {
				shown = FormShown::GamePlay;
				state = GameState::Paused;
				return;
			}
		}
		break;
	case SDL_KEYDOWN:
		switch (event.key.keysym.scancode) {
		case SDL_Scancode::SDL_SCANCODE_ESCAPE:
			shown = FormShown::GamePlay;
			state = GameState::Paused;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void Klondike::Update() {
	UpdateEverywhere();
	switch (shown) {
	case FormShown::GamePlay:
		switch (state) {
		case GameState::Playing:
			UpdatePlaying();
			break;
		case GameState::Paused:
			UpdatePaused();
			break;
		case GameState::Finished:
			UpdateFinished();
			break;
		}
	case FormShown::NewGameDialog:
		UpdateNewGame();
		break;
	case FormShown::SettingsDialog:
		UpdateSettings();
		break;
	}
}
void Klondike::UpdateEverywhere() {}
void Klondike::UpdatePlaying() {
	for (int p = 0; p < 7; p++) {
		if (!layout.tableau[p].empty()) {
			if (!layout.tableau[p].front().IsOpened()) {
				layout.score += 5;
				layout.tableau[p].front().Open();
			}
		}
	}
	if (!layout.foundations[0].empty() && !layout.foundations[1].empty() && !layout.foundations[2].empty() && !layout.foundations[3].empty())
		win =
		layout.foundations[0].front().GetRank() == CardRank::King &&
		layout.foundations[1].front().GetRank() == CardRank::King &&
		layout.foundations[2].front().GetRank() == CardRank::King &&
		layout.foundations[3].front().GetRank() == CardRank::King;
	if (win) {
		state = GameState::Finished; time_points.push_back(time(NULL));
	}
}
void Klondike::UpdatePaused() {}
void Klondike::UpdateFinished() {
	if (win) {
		if (final_firework_1 == nullptr) {
			final_firework_1 = new Firework(complex<double>(double(rand() % WinWidth), double(rand() % WinHeight)), rand() % 10 + 10, rand() % 7);
		}
		else {
			final_firework_1->Update();
			if (final_firework_1->Faded()) {
				final_firework_1->Recharge(complex<double>(double(rand() % WinWidth), double(rand() % WinHeight)), rand() % 10 + 10, rand() % 7);
			}
		}
		if (final_firework_2 == nullptr) {
			if(final_firework_1->GetVelocity() < 5)
				final_firework_2 = new Firework(complex<double>(double(rand() % WinWidth), double(rand() % WinHeight)), rand() % 10 + 10, rand() % 7);
		}
		else{
			final_firework_2->Update();
			if (final_firework_2->Faded()) {
				final_firework_2->Recharge(complex<double>(double(rand() % WinWidth), double(rand() % WinHeight)), rand() % 10 + 10, rand() % 7);
			}
		}
		if (final_firework_3 == nullptr) {
			if (final_firework_1->GetVelocity() < 10)
				final_firework_3 = new Firework(complex<double>(double(rand() % WinWidth), double(rand() % WinHeight)), rand() % 10 + 10, rand() % 3);
		}
		else {
			final_firework_3->Update();
			if (final_firework_3->Faded()) {
				final_firework_3->Recharge(complex<double>(double(rand() % WinWidth), double(rand() % WinHeight)), rand() % 10 + 10, rand() % 3);
			}
		}
		if (final_firework_4 == nullptr) {
			if (final_firework_1->GetVelocity() < 15)
				final_firework_4 = new Firework(complex<double>(double(rand() % WinWidth), double(rand() % WinHeight)), rand() % 10 + 10, rand() % 3);
		}
		else {
			final_firework_4->Update();
			if (final_firework_4->Faded()) {
				final_firework_4->Recharge(complex<double>(double(rand() % WinWidth), double(rand() % WinHeight)), rand() % 10 + 10, rand() % 3);
			}
		}
	}
}
void Klondike::UpdateNewGame() {}
void Klondike::UpdateSettings() {}

void Klondike::DrawStock() {
	if (layout.stock.empty()) {
		SDL_RenderCopy(renderer, storage->empty_place_image, NULL, &stock_pos);
	}
	else {
		layout.stock.back().Draw(renderer, storage, stock_pos.x, stock_pos.y);
	}
}
void Klondike::DrawWaste() {
	if (gamemode == GameMode::OneCard) {
		if (layout.waste.empty()) {
			SDL_RenderCopy(renderer, storage->empty_place_image, NULL, &waste_pos);
		}
		else {
			if (selected.where == 'w') {
				SDL_Rect bound = { waste_pos.x - 4, waste_pos.y - 4, waste_pos.w + 8, waste_pos.h + 8 };
				SDL_RenderCopy(renderer, storage->select_bound, NULL, &bound);
			}
			layout.waste[0].Draw(renderer, storage, waste_pos.x, waste_pos.y);
		}
	}
	else {
		if (layout.waste.empty()) {
			SDL_RenderCopy(renderer, storage->empty_place_image, NULL, &waste_pos);
		}
		else if (layout.waste.size() == 1) {
			if (selected.where == 'w') {
				SDL_Rect bound = { waste_pos.x - 4, waste_pos.y - 4, waste_pos.w + 8, waste_pos.h + 8 };
				SDL_RenderCopy(renderer, storage->select_bound, NULL, &bound);
			}
			layout.waste[0].Draw(renderer, storage, waste_pos.x, waste_pos.y);
		}
		else if (layout.waste.size() == 2) {
			layout.waste[1].Draw(renderer, storage, waste_pos.x, waste_pos.y);
			if (selected.where == 'w') {
				SDL_Rect bound = { waste_pos.x - 4 + 32, waste_pos.y - 4, waste_pos.w + 8, waste_pos.h + 8 };
				SDL_RenderCopy(renderer, storage->select_bound, NULL, &bound);
			}
			layout.waste[0].Draw(renderer, storage, waste_pos.x + 32, waste_pos.y);
		}
		else {
			layout.waste[2].Draw(renderer, storage, waste_pos.x, waste_pos.y);
			layout.waste[1].Draw(renderer, storage, waste_pos.x + 32, waste_pos.y);
			if (selected.where == 'w') {
				SDL_Rect bound = { waste_pos.x - 4 + 64, waste_pos.y - 4, waste_pos.w + 8, waste_pos.h + 8 };
				SDL_RenderCopy(renderer, storage->select_bound, NULL, &bound);
			}
			layout.waste[0].Draw(renderer, storage, waste_pos.x + 64, waste_pos.y);
		}
	}
}
void Klondike::DrawFoundations() {
	for (int i = 0; i < 4; i++) {
		if (layout.foundations[i].empty()) {
			SDL_RenderCopy(renderer, storage->empty_place_image, NULL, &foundations_pos[i]);
		}
		else {
			if (selected.where == 'a' + i) {
				SDL_Rect bound = { foundations_pos[i].x - 4, foundations_pos[i].y - 4, foundations_pos[i].w + 8, foundations_pos[i].h + 8 };
				SDL_RenderCopy(renderer, storage->select_bound, NULL, &bound);
			}
			layout.foundations[i].front().Draw(renderer, storage, foundations_pos[i].x, foundations_pos[i].y);
		}
	}
}
void Klondike::DrawFannedPiles() {
	for (int i = 0; i < 7; i++) {
		if (layout.tableau[i].empty()) {
			SDL_RenderCopy(renderer, storage->empty_place_image, NULL, &tableau_pos[i]);
		}
		else {
			int offset = 0;
			for (int j = layout.tableau[i].size() - 1; j >= 0; j--) {
				if (selected.where == '1' + i && selected.pos == j) {
					SDL_Rect bound = { tableau_pos[i].x - 4, tableau_pos[i].y - 4 + offset, tableau_pos[i].w + 8, tableau_pos[i].h + 8 };
					SDL_RenderCopy(renderer, storage->select_bound, NULL, &bound);
				}
				layout.tableau[i][j].Draw(renderer, storage, tableau_pos[i].x, tableau_pos[i].y + offset);
				offset += (layout.tableau[i][j].IsOpened() ? (CountOpenedInPile(i) <= 10 ? 32 : 20) : 2);
			}
		}
	}
}

void Klondike::NewGame(GameMode gm = GameMode::OneCard) {
	Reset();
	gamemode = gm;
	// init deck
	for (int suit = 0; suit < 4; suit++) {
		for (int rank = 0; rank < 13; rank++) {
			layout.stock.push_back(Card(CardSuit(suit), CardRank(rank), false));
		}
	}
	// shuffle
	for (int i = 0; i < 1000; i++) {
		int a = rand() % 52, b = rand() % 52;
		auto temp = layout.stock[a];
		layout.stock[a] = layout.stock[b];
		layout.stock[b] = temp;
	}
	// packing
	for (int i = 0; i < 7; i++) {
		for (int p = i; p < 7; p++) {
			layout.tableau[p].push_front(layout.stock.back());
			layout.stock.pop_back();
		}
	}
	for (int p = 0; p < 7; p++) {
		layout.tableau[p].front().Turn();
	}
	Record();
}
bool Klondike::Build() {
	bool flag = true;
	bool res = false;
	while (flag) {
		flag = false;
		for (int s = 0; s < 4; s++) {
			if(!layout.waste.empty())
			if (PossibleToFoundation(s, &layout.waste.front())) {
				flag = true;
				layout.foundations[s].push_front(layout.waste.front());
				layout.waste.pop_front();
				layout.score += 10;
			}
			for (int p = 0; p < 7; p++) {
				if (!layout.tableau[p].empty())
				if (PossibleToFoundation(s, &layout.tableau[p].front())) {
					flag = true;
					layout.foundations[s].push_front(layout.tableau[p].front());
					layout.foundations[s].front().Open();
					layout.tableau[p].pop_front();
					layout.score += 10;
				}
			}
		}
		for (int p = 0; p < 7; p++) {
			if (!layout.tableau[p].empty()) {
				if (!layout.tableau[p].front().IsOpened()) {
					layout.score += 5;
					layout.tableau[p].front().Open();
				}
			}
		}
		if (flag) res = true;
	}
	return res;
}
void Klondike::Undo() {
	if (history.size() > 1) {
		layout.Clear();
		history.back().Clear();
		history.pop_back();
		layout = history.back();
	}
}
void Klondike::Record() {
	history.push_back(layout);
}
void Klondike::Reset() {
	win = false;
	delay = 0;
	time_points.clear();
	layout.Clear();
	for (auto& l : history)
		l.Clear();
	history.clear();
}
void Klondike::SaveFile() {

	ofstream file("save.record", ios::binary);
	auto transform = [](Card * c) -> char { return char(c->GetRank()) | (char(c->GetSuit()) << 4) | (char(c->IsOpened()) << 6); };
	for (auto& e : layout.stock) {
		file << transform(&e);
	}
	file << char(0xFF);
	for (auto& e : layout.waste) {
		file << transform(&e);
	}
	file << char(0xFF);
	for (auto &f : layout.foundations) {
		for (auto& e : f) {
			file << transform(&e);
		}
		file << char(0xFF);
	}
	for (auto& p : layout.tableau) {
		for (auto& e : p) {
			file << transform(&e);
		}
		file << char(0xFF);
	}

	Storage::Encode4B(file, *(unsigned long *)(&layout.score));
	time_t new_delay = delay + Storage::TimeCut(time_points);
	Storage::Encode8B(file, *(unsigned long long*)(&new_delay));
	file << (gamemode == GameMode::OneCard ? '1' : '3');
	file.close();
}
void Klondike::LoadFile() {
	ifstream file("save.record", ios::binary);
	if(file.fail()) {
		NewGame();
		time_points.push_back(time(NULL));
		return;
	}

	Reset();
	auto transform = [](char c) -> Card { return Card(CardSuit((c >> 4) & 3), CardRank(c & 15), bool((c >> 6) & 1)); };

	char c;
	while (1) {
		file >> c;
		if (c == char(0xFF)) break;
		layout.stock.push_back(transform(c));
	}
	while (1) {
		file >> c;
		if (c == char(0xFF)) break;
		layout.waste.push_back(transform(c));
	}
	for (auto& f : layout.foundations) {
		while (1) {
			file >> c;
			if (c == char(0xFF)) break;
			f.push_back(transform(c));
		}
	}
	for (auto& p : layout.tableau) {
		while (1) {
			file >> c;
			if (c == char(0xFF)) break;
			p.push_back(transform(c));
		}
	}
	layout.score = Storage::Decode4B(file);
	delay = Storage::Decode8B(file);
	char gmc;
	file.get(gmc);
	gamemode = (gmc == '1' ? GameMode::OneCard : GameMode::ThreeCards);
	file.close();
	Record();
	time_points.push_back(time(NULL));
}

bool Klondike::PossibleToFoundation(int index, Card* c) {
	if (layout.foundations[index].empty())
		return c->GetRank() == CardRank::Ace;
	if (layout.foundations[index].front().GetRank() == CardRank::King)
		return false;
	return
		(layout.foundations[index].front().GetSuit() == c->GetSuit()) &&
		(CardRank(int(layout.foundations[index].front().GetRank()) + 1) == c->GetRank());
}
bool Klondike::PossibleToPile(int index, Card* c) {
	if (layout.tableau[index].empty())
		return c->GetRank() == CardRank::King;
	if (layout.tableau[index].front().GetRank() == CardRank::Ace)
		return false;
	return
		((int(layout.tableau[index].front().GetSuit()) + int(c->GetSuit())) % 2 == 1) &&
		(CardRank(int(layout.tableau[index].front().GetRank()) - 1) == c->GetRank());
}
int Klondike::WhichPile(int x) {
	for (int p = 0; p < 7; p++) {
		if (x >= (WinWidth / 2 - 464) + p * 128 + 32 && x < (WinWidth / 2 - 464) + (p + 1) * 128)
			return p;
	}
	return -1;
}
int Klondike::WhichCardInPile(int index, int y) {
	if (layout.tableau[index].empty())
		return -2; // special for king
	int offset = 192;
	for (int i = layout.tableau[index].size() - 1; i >= 0; i--) {
		if (y >= offset && y < offset + (i == 0 ? 128 : (layout.tableau[index][i].IsOpened() ? (CountOpenedInPile(index) <= 10 ? 32 : 20) : 2)))
			return i;
		offset += (layout.tableau[index][i].IsOpened() ? (CountOpenedInPile(index) <= 10 ? 32 : 20) : 2);
	}
	return -1;
}
int Klondike::CountOpenedInPile(int index) {
	int count = 0;
	for (auto& e : layout.tableau[index])
		if (e.IsOpened()) count++;
	return count;
}