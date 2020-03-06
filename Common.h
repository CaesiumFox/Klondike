#ifndef _COMMON_H_
#define _COMMON_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <string>
#include <ctime>
#include <cmath>


#ifdef _OS_LINUX_
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define FILE_SEPARATOR '/'
#else
#include <SDL.h>
#include <SDL_image.h>
//#include <SDL_ttf.h>

#define FILE_SEPARATOR '\\'
#endif

using namespace std;

const double tau = 6.283185307179586476925286766559;

class ToolKit {
public:
	static string GetFileParentFolder(string filename) {
		int i = filename.length() - 1;
		for(; filename[i] != FILE_SEPARATOR && i >= 0; i--);
		if(i == -1) return "";
		else return filename.substr(0, i) + FILE_SEPARATOR;
	}

	static void Encode4B(ofstream& out, unsigned long v) {
		out << (char)(v & 0xff);
		out << (char)((v >> 8) & 0xff);
		out << (char)((v >> 16) & 0xff);
		out << (char)((v >> 24) & 0xff);
	}

	static void Encode8B(ofstream& out, unsigned long long v) {
		out << (char)(v & 0xff);
		out << (char)((v >> 8) & 0xff);
		out << (char)((v >> 16) & 0xff);
		out << (char)((v >> 24) & 0xff);
		out << (char)((v >> 32) & 0xff);
		out << (char)((v >> 40) & 0xff);
		out << (char)((v >> 48) & 0xff);
		out << (char)((v >> 56) & 0xff);
	}

	static uint8_t Decode_u8(ifstream &in) {
		char c0 = in.get();
		return *(uint8_t*)(&c0);
	}

	static int8_t Decode_s8(ifstream& in) {
		return (int8_t)in.get();
	}

	static uint16_t Decode_u16_BE(ifstream& in) {
		char c0 = in.get();
		char c1 = in.get();
		return (uint16_t(*(uint8_t*)(&c0)) << 8) | uint16_t(*(uint8_t*)(&c1));
	}

	static uint16_t Decode_u16_LE(ifstream& in) {
		char c0 = in.get();
		char c1 = in.get();
		return (uint16_t(*(uint8_t*)(&c1)) << 8) | uint16_t(*(uint8_t*)(&c0));
	}

	static int16_t Decode_s16_BE(ifstream& in) {
		uint16_t n = Decode_u16_BE(in);
		return *(int16_t*)(&n);
	}

	static int16_t Decode_s16_LE(ifstream & in) {
		uint16_t n = Decode_u16_LE(in);
		return *(int16_t*)(&n);
	}

	static uint32_t Decode_u32_BE(ifstream& in) {
		char c0 = in.get();
		char c1 = in.get();
		char c2 = in.get();
		char c3 = in.get();
		return
			(uint32_t(*(uint8_t *)(&c0)) << 24) |
			(uint32_t(*(uint8_t *)(&c1)) << 16) |
			(uint32_t(*(uint8_t *)(&c2)) << 8) |
			uint32_t(*(uint8_t *)(&c3));
	}

	static uint32_t Decode_u32_LE(ifstream& in) {
		char c0 = in.get();
		char c1 = in.get();
		char c2 = in.get();
		char c3 = in.get();
		return
			(uint32_t(*(uint8_t *)(&c3)) << 24) |
			(uint32_t(*(uint8_t *)(&c2)) << 16) |
			(uint32_t(*(uint8_t *)(&c1)) << 8) |
			uint32_t(*(uint8_t *)(&c0));
	}

	static int32_t Decode_s32_BE(ifstream& in) {
		uint32_t n = Decode_u32_BE(in);
		return *(int32_t*)(&n);
	}

	static int32_t Decode_s32_LE(ifstream& in) {
		uint32_t n = Decode_u32_LE(in);
		return *(int32_t*)(&n);
	}

	static uint64_t Decode_u64_BE(ifstream& in) {
		char c0 = in.get();
		char c1 = in.get();
		char c2 = in.get();
		char c3 = in.get();
		char c4 = in.get();
		char c5 = in.get();
		char c6 = in.get();
		char c7 = in.get();
		return
			(uint64_t(*(uint8_t *)(&c0)) << 56) |
			(uint64_t(*(uint8_t *)(&c1)) << 48) |
			(uint64_t(*(uint8_t *)(&c2)) << 40) |
			(uint64_t(*(uint8_t *)(&c3)) << 32) |
			(uint64_t(*(uint8_t *)(&c4)) << 24) |
			(uint64_t(*(uint8_t *)(&c5)) << 16) |
			(uint64_t(*(uint8_t *)(&c6)) << 8) |
			uint64_t(*(uint8_t *)(&c7));
	}

	static uint64_t Decode_u64_LE(ifstream& in) {
		char c0 = in.get();
		char c1 = in.get();
		char c2 = in.get();
		char c3 = in.get();
		char c4 = in.get();
		char c5 = in.get();
		char c6 = in.get();
		char c7 = in.get();
		return
			(uint64_t(*(uint8_t *)(&c7)) << 56) |
			(uint64_t(*(uint8_t *)(&c6)) << 48) |
			(uint64_t(*(uint8_t *)(&c5)) << 40) |
			(uint64_t(*(uint8_t *)(&c4)) << 32) |
			(uint64_t(*(uint8_t *)(&c3)) << 24) |
			(uint64_t(*(uint8_t *)(&c2)) << 16) |
			(uint64_t(*(uint8_t *)(&c1)) << 8) |
			uint64_t(*(uint8_t *)(&c0));
	}

	static int64_t Decode_s64_BE(ifstream& in) {
		uint64_t n = Decode_u64_BE(in);
		return *(int64_t*)(&n);
	}

	static int64_t Decode_s64_LE(ifstream& in) {
		uint64_t n = Decode_u64_LE(in);
		return *(int64_t*)(&n);
	}

	static uint8_t  HexToDecDigit(char d) {
		if(d >= '0' && d <= '9') {
			return d - '0';
		}
		if(d >= 'A' && d <= 'F') {
			return d - 'A' + 10;
		}
		if(d >= 'a' && d <= 'f') {
			return d - 'a' + 10;
		}
		return 0;
	}

	static time_t TimeCut(vector<time_t>& points) {
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

	static string GetStrTime(time_t time) {
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

	static string GetCurrentStrTime() {
		time_t t = time(0);
		#ifdef _OS_LINUX_
		tm* lt = localtime(&t);
		#else
		tm* lt = new tm();
		localtime_s(lt, &t);
		#endif
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
};

#endif
