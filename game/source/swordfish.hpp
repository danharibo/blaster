#ifndef SWORDFISH_HPP
#define SWORDFISH_HPP
#include "ship.hpp"

class Swordfish : public Ship
{
	bool _fireSwitch;
public:
	Swordfish();

	void fire();
};

#endif // SWORDFISH_HPP
