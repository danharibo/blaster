#include "badguy.hpp"

std::vector<float> bg_verts = {
	0.5f, 0.5f,
	0.0f,-1.0f,
	0.0f, 0.5f,

	0.5f, 0.5f,
	0.5f,-1.0f,
	0.0f,-1.0f,

	0.5f, 0.5f,
	1.5f, 0.0f,
	0.5f,-1.0f,

	0.5f, 0.5f,
	1.5f, 1.5f,
	1.5f, 0.0f,

   -0.5f, 0.5f,
	0.0f,-1.0f,
	0.0f, 0.5f,

   -0.5f, 0.5f,
   -0.5f,-1.0f,
	0.0f,-1.0f,

   -0.5f, 0.5f,
   -1.5f, 0.0f,
   -0.5f,-1.0f,

   -0.5f, 0.5f,
   -1.5f, 1.5f,
   -1.5f, 0.0f,
	};

BadGuy::BadGuy()
{
	polygon.setColor(fea::Color(255, 0, 0));
	polygon.setScale({10.f, 10.f});

	polygon.setVertices(bg_verts);

	std::vector<float> vc;
	for(size_t i = 0; i < bg_verts.size()*4; i++)
		vc.push_back(1.f);
	polygon.setVertexColors(vc);

	std::vector<float> tc;
	for(size_t i = 0; i < bg_verts.size()*2; i++)
		tc.push_back(0.f);
	polygon.setTexCoords(tc);
	calculateBoundingRadius();
}

void BadGuy::fire()
{

}
