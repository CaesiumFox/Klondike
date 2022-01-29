#ifndef _FIREWORK_H_
#define _FIREWORK_H_

#include <complex>
#include "Common.h"
#include "Storage.h"

class Firework
{
private:
	int color;
	int particles_count;
	complex<double> center;
	double dist;
	vector<complex<double>> particles_location;
	double velocity;
public:
	Firework(complex<double> _center, int _count, int _color);
	~Firework();
	void Draw(SDL_Renderer*, Storage*);
	void Update();
	bool Faded();
	void Recharge(complex<double> _center, int _count, int _color);
	static SDL_Point ComplexToPoint(complex<double>);
	static SDL_Rect RectAroundPoint(SDL_Point, int w, int h);
	double GetVelocity();
};

#endif