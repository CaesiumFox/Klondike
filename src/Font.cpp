#include "Font.h"

Font::Font(SDL_Renderer* ren, string filename) {
	string foldername = ToolKit::GetFileParentFolder(filename);
	ifstream file(filename, ios::binary);

	uint8_t type = ToolKit::Decode_u8(file);
	if(type != 0x00) {
		cout << "Error: font loading failed, invalid font type for class Font." << endl;
		cout << "type = " << type << endl;
		throw 0;
	}

	original_height = ToolKit::Decode_u8(file);

	// Loading textures
	SDL_Surface* surf;
	uint16_t number_of_textures = ToolKit::Decode_u16_BE(file);
	char_lists = vector<SDL_Texture*>(number_of_textures);
	for(uint16_t texture_index = 0; texture_index < number_of_textures; texture_index++) {
		uint8_t len = ToolKit::Decode_u8(file);
		string tex_filename;
		for(uint8_t index = 0; index < len; index++) {
			char c;
			file >> c;
			tex_filename += c;
		}
		surf = IMG_Load((foldername + tex_filename).c_str());
		char_lists[texture_index] = SDL_CreateTextureFromSurface(ren, surf);
		if(char_lists[texture_index] == nullptr) {
			cout << "Failed to load image: \"" << tex_filename << "\"" << endl;
		}
	}

	// Loading characters' data
	char_data = vector<CharData>(0x10000);
	uint16_t number_of_chunks = ToolKit::Decode_u16_BE(file);
	for(uint16_t chunk_index = 0; chunk_index < number_of_chunks; chunk_index++) {
		char16_t start = ToolKit::Decode_s16_BE(file), end = ToolKit::Decode_s16_BE(file);
		for(char16_t char_pos = start; char_pos <= end; char_pos++) {
			char_data[char_pos].width = ToolKit::Decode_u8(file);
			char_data[char_pos].n_img = ToolKit::Decode_u8(file);
			char_data[char_pos].g_pos = vector<GraphemePosition>(char_data[char_pos].n_img);
			for(uint8_t grapheme_index = 0; grapheme_index < char_data[char_pos].n_img; grapheme_index++) {
				char_data[char_pos].g_pos[grapheme_index].tn = ToolKit::Decode_u16_BE(file);

				char_data[char_pos].g_pos[grapheme_index].x = ToolKit::Decode_u8(file);
				char_data[char_pos].g_pos[grapheme_index].y = ToolKit::Decode_u8(file);
				char_data[char_pos].g_pos[grapheme_index].w = ToolKit::Decode_u8(file);
				char_data[char_pos].g_pos[grapheme_index].h = ToolKit::Decode_u8(file);

				char_data[char_pos].g_pos[grapheme_index].offset_x = ToolKit::Decode_s8(file);
				char_data[char_pos].g_pos[grapheme_index].offset_y = ToolKit::Decode_s8(file);
			}

			char_data[char_pos].dpa_x = ToolKit::Decode_s8(file);
			char_data[char_pos].dpa_y = ToolKit::Decode_s8(file);
			char_data[char_pos].dpb_x = ToolKit::Decode_s8(file);
			char_data[char_pos].dpb_y = ToolKit::Decode_s8(file);
		}
	}

	Height = original_height;
	Spacing = 1;
	Interval = 1;
	Color = { 0xff, 0xff, 0xff, 0xff };
}

Font::~Font() {
	for(auto &char_list : char_lists)
		SDL_DestroyTexture(char_list);
	char_lists.~vector();
	char_data.~vector();
}

void Font::DrawString(string str, SDL_Renderer* ren, int x, int y) {
	u16string str16;
	for(char& c : str) {
		str16 += char16_t(c);
	}
	DrawString(str16, ren, x, y);
}

void Font::DrawString(u16string str, SDL_Renderer* ren, int x, int y) {
	for(auto& char_list : char_lists) {
		SDL_SetTextureColorMod(char_list, Color.r, Color.g, Color.b);
		SDL_SetTextureAlphaMod(char_list, Color.a);
	}
	SDL_Point current_point{ x, y };
	float size_coefficient = (float)Height / original_height;
	bool first = true;
	SDL_Point current_dpa{ 0, 0 }, current_dpb{ 0, 0 };
	for(uint32_t i = 0; i < str.length(); i++) {
		auto& c = str[i];
		if(c == '&') {
			i++;
			if(str[i] == '#') { // color mod RGB (without A)
				char
					r1 = str[i + 1],
					r0 = str[i + 2],
					g1 = str[i + 3],
					g0 = str[i + 4],
					b1 = str[i + 5],
					b0 = str[i + 6];
				i += 6;
				Color.r = ToolKit::HexToDecDigit(r1) * 16 + ToolKit::HexToDecDigit(r0);
				Color.g = ToolKit::HexToDecDigit(g1) * 16 + ToolKit::HexToDecDigit(g0);
				Color.b = ToolKit::HexToDecDigit(b1) * 16 + ToolKit::HexToDecDigit(b0);
				for(auto& char_list : char_lists) {
					SDL_SetTextureColorMod(char_list, Color.r, Color.g, Color.b);
				}
				continue;
			}
		}

		if(c == '\n') {
			current_point.y += Height + int(Interval * size_coefficient);
			current_point.x = x;
			first = true;
			continue;
		}
		if(c == '\r') {
			current_point.x = x;
			first = true;
			continue;
		}

		bool is_diacritic = IsDiacritic(c);
		// TODO later, now it's always true
		bool is_ltr = true;
		first = first && is_diacritic;

		if(is_diacritic && !first) {
			// TODO later
		}
		else {
			for(auto & gr_pos : char_data[c].g_pos) {
				SDL_Rect src, dest;

				src.x = gr_pos.x;
				src.y = gr_pos.y;
				src.w = gr_pos.w;
				src.h = gr_pos.h;

				dest.x = current_point.x + int(gr_pos.offset_x * size_coefficient);
				dest.y = current_point.y + int(gr_pos.offset_y * size_coefficient);
				dest.w = int(gr_pos.w * size_coefficient);
				dest.h = int(gr_pos.h * size_coefficient);

				SDL_RenderCopy(ren, char_lists[gr_pos.tn], &src, &dest);
			}
			current_dpa.x = current_point.x + int(char_data[c].dpa_x * size_coefficient);
			current_dpa.y = current_point.y + int(char_data[c].dpa_y * size_coefficient);
			current_dpb.x = current_point.x + int(char_data[c].dpb_x * size_coefficient);
			current_dpb.y = current_point.y + int(char_data[c].dpb_y * size_coefficient);

			if(char_data[c].width != 0)
				current_point.x += int((char_data[c].width + Spacing) * size_coefficient);
		}
	}
}

int Font::GetWidth(u16string str) {
	int current_width = 0, max_width = 0;
	float size_coefficient = (float)Height / original_height;
	bool first = true;
	for(int i = 0; i < str.length(); i++) {
		auto& c = str[i];
		if(c == '&') {
			i++;
			if(str[i] == '#') { // color mod RGB (without A)
				i += 6;
				continue;
			}
		}

		if(c == '\n') {
			if(current_width > max_width) {
				max_width = current_width;
			}
			first = true;
			continue;
		}
		if(c == '\r') {
			if(current_width > max_width) {
				max_width = current_width;
			}
			first = true;
			continue;
		}

		bool is_diacritic = IsDiacritic(c);
		// TODO later, now it's always true
		bool is_ltr = true;
		first = first && is_diacritic;

		if(!is_diacritic || first) {
			if(char_data[c].width != 0)
				current_width += int((char_data[c].width + Spacing) * size_coefficient);
		}
	}

	if(current_width > max_width) {
		max_width = current_width;
	}
	return max_width;
}