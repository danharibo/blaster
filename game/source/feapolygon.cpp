#include "feapolygon.hpp"

feaPolygon::feaPolygon()
{
}

feaPolygon::feaPolygon(const std::vector<float> &vertices)
{
	setVertices(vertices);
}

void feaPolygon::setVertices(const std::vector<float> &vertices)
{
	mVertices = vertices;
}

void feaPolygon::setTexCoords(const std::vector<float> &texcoords)
{
	mTexCoords = texcoords;
}

void feaPolygon::setVertexColors(const std::vector<float> &vertexcolours)
{
	mVertexColors = vertexcolours;
}

fea::RenderInfo feaPolygon::getRenderInfo() const
{
	fea::RenderInfo temp;

	temp.mDrawMode = GL_TRIANGLES;
	temp.mElementAmount = mVertices.size() / 2;

	glm::vec3 colorInfo = glm::vec3(mColor.r(), mColor.g(), mColor.b());
	float opacity = mColor.a();

	temp.mVertexAttributes.push_back(fea::VertexAttribute("vertex", 2, &mVertices[0]));
	temp.mVertexAttributes.push_back(fea::VertexAttribute("texCoords", 2, &mTexCoords[0]));
	temp.mVertexAttributes.push_back(fea::VertexAttribute("colors", 4, &mVertexColors[0]));

	temp.mUniforms.push_back(fea::Uniform("position", fea::VEC2, mPosition));
	temp.mUniforms.push_back(fea::Uniform("origin",   fea::VEC2, mOrigin));
	temp.mUniforms.push_back(fea::Uniform("rotation", fea::FLOAT, mRotation));
	temp.mUniforms.push_back(fea::Uniform("scaling",  fea::VEC2, mScaling));
	temp.mUniforms.push_back(fea::Uniform("parallax", fea::VEC2, mParallax));
	temp.mUniforms.push_back(fea::Uniform("constraints", fea::VEC4, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)));
	temp.mUniforms.push_back(fea::Uniform("color",    fea::VEC3, colorInfo));
	temp.mUniforms.push_back(fea::Uniform("opacity",  fea::FLOAT, opacity));

	return temp;
}
