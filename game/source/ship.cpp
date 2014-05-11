#include "ship.hpp"
#include <glm/gtx/quaternion.hpp>

Ship::Ship()
{
}

void Ship::setPosition(const glm::vec2 &p)
{
	polygon.setPosition(p);
}

void Ship::setRotation(float angle)
{
	polygon.setRotation(angle);
}

#include <iostream>
glm::vec2 Ship::getForwardVector() const
{
	glm::quat q(glm::vec3{0.f, 0.f, -polygon.getRotation()});
	auto tmp = q * glm::vec3{0.f, 1.f, 0.f};
	return {tmp.x, tmp.y};
}

glm::vec2 Ship::getRightVector() const
{
	glm::quat q(glm::vec3{0.f, 0.f, -polygon.getRotation()});
	auto tmp = q * glm::vec3{1.f, 0.f, 0.f};
	return {tmp.x, tmp.y};
}

void Ship::setVelocity(const glm::vec2 &v)
{
	_velocity = v;
}

void Ship::setAngularVeocity(float av)
{
	_angularVelocity = av;
}

void Ship::draw(fea::Renderer2D &renderer)
{
	renderer.queue(polygon);
}

void Ship::tick(float dt)
{
	auto p = polygon.getPosition();
	setPosition(p + (_velocity * dt));
	auto a = polygon.getRotation();
	setRotation(a + (_angularVelocity * dt));
}
