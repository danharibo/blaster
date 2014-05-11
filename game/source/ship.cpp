#include "ship.hpp"
#include <glm/gtx/quaternion.hpp>

void Ship::calculateBoundingRadius()
{
	auto& vs = polygon.getVertices();
	float accum;
	for(size_t i = 0; i < vs.size() / 3; i+=3) {
		accum += glm::length(glm::vec3{vs[i+0], vs[i+1], vs[i+2]});
	}
	_boundingRadius = (accum / (vs.size()/3)) * 10.f;
}

Ship::Ship()
	: _angularVelocity(0.f),
	  _boundingRadius(0.f),
	  _hp(10.f),
	  _enabledThrusters(0),
	  _gunTimer(0.f)
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

void Ship::enableThruster(Ship::Thruster t)
{
	_enabledThrusters |= t;
}

void Ship::disableThruster(Ship::Thruster t)
{
	_enabledThrusters &= ~t;
}

void Ship::setThruster(Ship::Thruster t, bool enabled)
{
	if(enabled) enableThruster(t);
	else disableThruster(t);
}

bool Ship::isThrusterEnabled(Ship::Thruster t)
{
	return (_enabledThrusters & t) == t;
}

void Ship::draw(fea::Renderer2D &renderer)
{
	renderer.queue(polygon);
}

void Ship::tick(float dt)
{
	if(isThrusterEnabled(T_YAWL)) {
		setAngularVeocity(getAngularVelocity() + 0.05f);
	}
	if(isThrusterEnabled(T_YAWR)) {
		setAngularVeocity(getAngularVelocity() - 0.05f);
	}
	if(isThrusterEnabled(T_MAIN)) {
		setVelocity(getVelocity() +
					   getForwardVector() * 1.0f);
	}
	if(isThrusterEnabled(T_REVERSE)) {
		setVelocity(getVelocity() -
					   getForwardVector() * 1.0f);
	}
	if(isThrusterEnabled(T_MOVER)) {
		setVelocity(getVelocity() +
					   getRightVector() * 0.5f);
	}
	if(isThrusterEnabled(T_MOVEL)) {
		setVelocity(getVelocity() -
					   getRightVector() * 0.5f);
	}
	_gunTimer -= dt;
	if(isThrusterEnabled(T_WEAPON) && _gunTimer <= 0.f) {
		fire();
	}

	auto p = polygon.getPosition();
	setPosition(p + (_velocity * dt));
	auto a = polygon.getRotation();
	setRotation(a + (_angularVelocity * dt));
}
