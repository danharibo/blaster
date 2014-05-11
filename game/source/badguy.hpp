#ifndef BADGUY_HPP
#define BADGUY_HPP
#include "ship.hpp"

class BadGuy : public Ship
{
public:
	BadGuy();

	void fire();
};

#endif // BADGUY_HPP
