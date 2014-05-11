#include "swordfish.hpp"

std::vector<float> sf_verts = {
	0.0f, 3.0f,
	0.2f, 3.0f,
	0.0f, 0.3f,

	0.2f, 3.0f,
	0.5f, 0.3f,
	0.0f, 0.3f,

	0.0f, 0.3f,
	0.5f, 0.3f,
	0.0f,-0.7f,

	0.5f, 0.3f,
	0.5f,-0.7f,
	0.0f,-0.7f,

	0.0f,-0.7f,
	0.5f,-0.7f,
	0.0f,-1.4f,

	0.5f,-0.7f,
	0.4f,-1.4f,
	0.0f,-1.4f,

	0.5f, 0.3f,
	1.5f, 0.3f,
	0.5f,-0.7f,

	1.5f, 0.3f,
	1.5f,-0.7f,
	0.5f,-0.7f,

	1.5f, 0.8f,
	1.7f, 0.3f,
	1.5f,-1.2f,

	1.7f, 0.3f,
	1.7f,-0.7f,
	1.5f,-1.2f,


	-0.0f, 3.0f,
	-0.2f, 3.0f,
	-0.0f, 0.3f,

	-0.2f, 3.0f,
	-0.5f, 0.3f,
	-0.0f, 0.3f,

	-0.0f, 0.3f,
	-0.5f, 0.3f,
	-0.0f,-0.7f,

	-0.5f, 0.3f,
	-0.5f,-0.7f,
	-0.0f,-0.7f,

	-0.0f,-0.7f,
	-0.5f,-0.7f,
	-0.0f,-1.4f,

	-0.5f,-0.7f,
	-0.4f,-1.4f,
	-0.0f,-1.4f,

	-0.5f, 0.3f,
	-1.5f, 0.3f,
	-0.5f,-0.7f,

	-1.5f, 0.3f,
	-1.5f,-0.7f,
	-0.5f,-0.7f,

	-1.5f, 0.8f,
	-1.7f, 0.3f,
	-1.5f,-1.2f,

	-1.7f, 0.3f,
	-1.7f,-0.7f,
	-1.5f,-1.2f,
	};

Swordfish::Swordfish()
{
	polygon.setColor(fea::Color(0, 255, 255));
	polygon.setScale({10.f, 10.f});

	polygon.setVertices(sf_verts);

	std::vector<float> vc;
	for(size_t i = 0; i < sf_verts.size()*4; i++)
		vc.push_back(1.f);
	polygon.setVertexColors(vc);

	std::vector<float> tc;
	for(size_t i = 0; i < sf_verts.size()*2; i++)
		tc.push_back(0.f);
	polygon.setTexCoords(tc);
}
