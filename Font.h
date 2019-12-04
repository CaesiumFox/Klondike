#ifndef _FONT_H_
#define _FONT_H_

#include "Common.h"

enum class Direction {
	LeftToRight = 0, // Latin, Greek, Cyrillic, Armenian, Georgian, Hindi, &c
	RightToLeft = 1, // Arabic, Hebrew, &c
	Both = 2, // Common punctuation
	LTRinRTL = 3 // digits in rtl
};

struct GraphemePosition {
	// texture number
	uint16_t tn = 0;
	// position on the texture
	uint8_t x = 0, y = 0, w = 0, h = 0;
	// position relative to the glyph
	int8_t offset_x = 0, offset_y = 0;
};

struct CharData {
	uint8_t width = 0;
	// the glyph might consist of many graphemes
	// number of graphemes
	uint8_t n_img = 0;
	// the glyph might consist of many graphemes
	// graphemes
	vector<GraphemePosition> g_pos;
	// diacritic anchors (points): above & below
	// for letters: a hint to the programm where to place diactitics
	// for diacritic signs above: dpa - more diacritics, dpb - letter/diacritics below
	// for diacritic signs below: dpb - more diacritics, dpa - letter/diacritics above
	int8_t dpa_x = 0, dpa_y = 0, dpb_x = 0, dpb_y = 0;
};

class Font {
private:
	uint8_t original_height;
	vector<SDL_Texture*> char_lists;
	vector<CharData> char_data;

public:
	Font(SDL_Renderer*, string font_filename);
	~Font();
	
	// additional

	// Drawing height
	int Height;
	// Spacing between characters
	int Spacing;
	// Interval between lines
	int Interval;
	// Drawing color & opacity
	SDL_Color Color;

	// (x, y) from the top left corner from (0, 0)
	void DrawString(string, SDL_Renderer*, int x, int y);
	// (x, y) from the top left corner from (0, 0)
	void DrawString(u16string, SDL_Renderer*, int x, int y);
	int GetWidth(u16string);

	static bool IsDiacritic(char16_t c) {
		return (c >= 0x0300) && (c <= 0x036f);
	}
	static Direction GetDir(char16_t c) {
		// TODO later
		if(0)
			return Direction::RightToLeft;
		if(
			c >= u'0' && c <= u'9'
			)
			return Direction::LTRinRTL;
		if(
			c == u' ' || c == u'!'
			)
			return Direction::Both;
		return Direction::LeftToRight;
	}
};

#endif
