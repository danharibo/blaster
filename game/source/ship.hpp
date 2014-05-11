#ifndef SHIP_HPP
#define SHIP_HPP
#include <functional>

#include "feapolygon.hpp"

typedef std::function<void (const glm::vec2&, const glm::vec2&)> WeaponCallback;

class Ship
{
protected:
	feaPolygon polygon;

	glm::vec2 _velocity;
	float _angularVelocity;

	WeaponCallback _fireCallback;

	float _boundingRadius;

	void calculateBoundingRadius();
public:
	Ship();

	void setPosition(const glm::vec2& p);
	glm::vec2 getPosition() const { return polygon.getPosition(); }
	void setRotation(float angle);
	float getRotation() const { return polygon.getRotation(); }

	float getBoundingRadius() const { return _boundingRadius; }

	glm::vec2 getForwardVector() const;
	glm::vec2 getRightVector() const;

	void setVelocity(const glm::vec2& v);
	glm::vec2 getVelocity() const { return _velocity; }
	void setAngularVeocity(float av);
	float getAngularVelocity() const { return _angularVelocity; }

	void setFireCallback(const WeaponCallback& cb) { _fireCallback = cb; }
	virtual void fire() = 0;

	void draw(fea::Renderer2D& renderer);

	void tick(float dt);
};

#endif // SHIP_HPP
