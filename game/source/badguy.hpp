#ifndef BADGUY_HPP
#define BADGUY_HPP

#include "feapolygon.hpp"

class BadGuy
{
	feaPolygon polygon;
public:
	BadGuy();

	void setPosition(const glm::vec2& p);
	void setRotation(float angle);

	void draw(fea::Renderer2D& renderer);
};

#endif // BADGUY_HPP
