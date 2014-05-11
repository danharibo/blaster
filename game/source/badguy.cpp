#include "badguy.hpp"

BadGuy::BadGuy()
	: polygon({
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
			  })
{
	polygon.setColor(fea::Color(255, 0, 0));
	polygon.setScale({10.f, 10.f});

	polygon.setVertexColors({
								1.f,1.f,1.f, 1.f,
								1.f,1.f,1.f, 1.f,
								1.f,1.f,1.f, 1.f,

								1.f,1.f,1.f, 1.f,
								1.f,1.f,1.f, 1.f,
								1.f,1.f,1.f, 1.f,

								1.f,1.f,1.f, 1.f,
								1.f,1.f,1.f, 1.f,
								1.f,1.f,1.f, 1.f,

								1.f,1.f,1.f, 1.f,
								1.f,1.f,1.f, 1.f,
								1.f,1.f,1.f, 1.f,

								1.f,1.f,1.f, 1.f,
								1.f,1.f,1.f, 1.f,
								1.f,1.f,1.f, 1.f,

								1.f,1.f,1.f, 1.f,
								1.f,1.f,1.f, 1.f,
								1.f,1.f,1.f, 1.f,

								1.f,1.f,1.f, 1.f,
								1.f,1.f,1.f, 1.f,
								1.f,1.f,1.f, 1.f,

								1.f,1.f,1.f, 1.f,
								1.f,1.f,1.f, 1.f,
								1.f,1.f,1.f, 1.f,
							});
	polygon.setTexCoords({
							 0.f, 0.f,
							 0.f, 0.f,
							 0.f, 0.f,

							 0.f, 0.f,
							 0.f, 0.f,
							 0.f, 0.f,

							 0.f, 0.f,
							 0.f, 0.f,
							 0.f, 0.f,

							 0.f, 0.f,
							 0.f, 0.f,
							 0.f, 0.f,

							 0.f, 0.f,
							 0.f, 0.f,
							 0.f, 0.f,

							 0.f, 0.f,
							 0.f, 0.f,
							 0.f, 0.f,

							 0.f, 0.f,
							 0.f, 0.f,
							 0.f, 0.f,

							 0.f, 0.f,
							 0.f, 0.f,
							 0.f, 0.f,
						 });
}

void BadGuy::setPosition(const glm::vec2 &p)
{
	polygon.setPosition(p);
}

void BadGuy::setRotation(float angle)
{
	polygon.setRotation(angle);
}

void BadGuy::draw(fea::Renderer2D &renderer)
{
	renderer.queue(polygon);
}
