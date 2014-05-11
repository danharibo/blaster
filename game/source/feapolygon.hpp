#ifndef FEAPOLYGON_HPP
#define FEAPOLYGON_HPP

#include <featherkit/render2d.hpp>

class feaPolygon : public fea::Drawable2D
{
public:

	feaPolygon();
	feaPolygon(const std::vector<float>& vertices);

	void setVertices(const std::vector<float>& vertices);

	void setTexCoords(const std::vector<float>& texcoords);

	void setVertexColors(const std::vector<float>& vertexcolours);

	fea::RenderInfo getRenderInfo() const;
};

#endif // FEAPOLYGON_HPP
