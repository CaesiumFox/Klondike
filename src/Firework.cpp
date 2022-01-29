#include "Firework.h"

Firework::Firework(complex<double> _center, int count, int _color) {
	Recharge(_center, count, _color);
}

Firework::~Firework() {
	particles_location.~vector();
}

void Firework::Draw(SDL_Renderer* renderer, Storage* storage) {
	SDL_Rect rect;
	SDL_SetTextureAlphaMod(storage->particle, int(velocity / 20.0 * 255));
	SDL_SetTextureAlphaMod(storage->particle_overlay, int(velocity / 20.0 * 255));
	switch (color) {
	case 0:
		SDL_SetTextureColorMod(storage->particle, 255, 255, 255);
		break;
	case 1:
		SDL_SetTextureColorMod(storage->particle, 0, 255, 255);
		break;
	case 2:
		SDL_SetTextureColorMod(storage->particle, 255, 255, 0);
		break;
	case 3:
		SDL_SetTextureColorMod(storage->particle, 0, 255, 255);
		break;
	case 4:
		SDL_SetTextureColorMod(storage->particle, 0, 0, 255);
		break;
	case 5:
		SDL_SetTextureColorMod(storage->particle, 255, 0, 0);
		break;
	case 6:
		SDL_SetTextureColorMod(storage->particle, 0, 255, 0);
		break;
	}
	for (auto& e : particles_location) {
		rect = RectAroundPoint(ComplexToPoint(e), 16, 16);
		SDL_RenderCopy(renderer, storage->particle, NULL, &rect);
		SDL_RenderCopy(renderer, storage->particle_overlay, NULL, &rect);
	}
}

void Firework::Update() {
	dist += velocity;
	for (uint32_t i = 0; i < particles_location.size(); i++) {
		particles_location[i] = center + dist * exp(1.0i * tau * double(i) / double(particles_count));
		particles_location[i] += (20.0 - velocity) * 10.0i;
	}
	velocity-=0.5;
}

bool Firework::Faded() {
	return velocity == 0;
}

void Firework::Recharge(complex<double> _center, int count, int _color) {
	velocity = 20;
	dist = 0;
	center = _center;
	particles_count = count;
	particles_location = vector<complex<double>>(count, _center);
	color = _color;
}

SDL_Point Firework::ComplexToPoint(complex<double> Z) {
	return SDL_Point{int(round(Z.real())), int(round(Z.imag()))};
}

SDL_Rect Firework::RectAroundPoint(SDL_Point P, int w, int h) {
	return { P.x - w / 2, P.y - h / 2, w, h };
}

double Firework::GetVelocity() {
	return velocity;
}