#ifndef SHIP_HPP
#define SHIP_HPP

#include "feapolygon.hpp"

class Ship
{
protected:
	feaPolygon polygon;

	glm::vec2 _velocity;
	float _angularVelocity;

public:
	Ship();

	void setPosition(const glm::vec2& p);
	glm::vec2 getPosition() const { return polygon.getPosition(); }
	void setRotation(float angle);

	glm::vec2 getForwardVector() const;
	glm::vec2 getRightVector() const;

	void setVelocity(const glm::vec2& v);
	glm::vec2 getVelocity() const { return _velocity; }
	void setAngularVeocity(float av);
	float getAngularVelocity() const { return _angularVelocity; }

	void draw(fea::Renderer2D& renderer);

	void tick(float dt);
};

#endif // SHIP_HPP
