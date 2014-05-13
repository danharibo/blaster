#ifndef EXPLOSION_HPP
#define EXPLOSION_HPP

class Ship;

#include "feapolygon.hpp"

class Explosion : public feaPolygon
{
	float _time;
	float _radius;
	float _currentRadius;
public:
	Explosion(float radius, float time);

	void tick(float dt, const std::vector<Ship*> ships);

	void draw(fea::Renderer2D& renderer);

	bool isOver() const;
};

#endif // EXPLOSION_HPP
