#include "explosion.hpp"
#include "ship.hpp"

Explosion::Explosion(float radius, float time)
	: _time(time), _radius(radius), _currentRadius(0.f)
{
}

void Explosion::tick(float dt, const std::vector<Ship *> ships)
{
	float radiusPerSecond = _radius / _time;
	float prevR = _currentRadius;
	_currentRadius += radiusPerSecond * dt;

	for(auto s : ships) {
		float l = glm::length(s->getPosition() - getPosition());
		if(l > prevR && l <= _currentRadius) {
			s->damage(std::min(_radius / l, 1.0f) * 25.f);
		}
	};
}

int segments = 16;

void Explosion::draw(fea::Renderer2D &renderer)
{
	float outerRadius = _currentRadius;
	float innerRadius = _currentRadius - 2.5f;

	std::vector<float> verts;

	float radsPerSegment = (2.f * 3.1415f) / segments;
	for(int s = 0; s < segments; s++) {
		float a = radsPerSegment * s;
		float b = radsPerSegment * (s+1);

		verts.push_back(sin(a) * innerRadius);
		verts.push_back(cos(a) * innerRadius);

		verts.push_back(sin(a) * outerRadius);
		verts.push_back(cos(a) * outerRadius);

		verts.push_back(sin(b) * innerRadius);
		verts.push_back(cos(b) * innerRadius);


		verts.push_back(sin(a) * outerRadius);
		verts.push_back(cos(a) * outerRadius);

		verts.push_back(sin(b) * outerRadius);
		verts.push_back(cos(b) * outerRadius);

		verts.push_back(sin(b) * innerRadius);
		verts.push_back(cos(b) * innerRadius);
	}

	setVertices(verts);

	float radFraction = _currentRadius / _radius;
	std::vector<float> colours;
	for(size_t i = 0; i < verts.size(); ++i) {
		colours.push_back(1.f);
		colours.push_back(1.f);
		colours.push_back(1.f);
		colours.push_back(1.f);
	}
	setColor({1.f, 1.f, 1.f, cos(radFraction * 0.5f * 3.1415f)});

	setVertexColors(colours);

	std::vector<float> texc;
	for(size_t i = 0; i < verts.size(); ++i) {
		texc.push_back(1.f);
		texc.push_back(1.f);
	}
	setTexCoords(texc);

	renderer.queue(*this);
}

bool Explosion::isOver() const
{
	return _currentRadius >= _radius;
}
