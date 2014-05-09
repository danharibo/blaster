#include "blastgame.hpp"
#include "blastshaders.hpp"

u_int8_t tex_beamData8[] = {
	0xFF, 0xFF, 0xFF, 0x88,
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0x88
};

u_int8_t tex_beamData4[] = {
	0x55, 0x55, 0x55, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,
	0x55, 0x55, 0x55, 0xFF,
};

u_int8_t tex_circle[] = {
	0x00, 0x00, 0x00, 0x00,
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,
	0x00, 0x00, 0x00, 0x00,

	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0x00, 0x00, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,

	0x55, 0x55, 0x55, 0xFF,
	0x55, 0x55, 0x55, 0xFF,
	0x55, 0x00, 0x00, 0xFF,
	0x55, 0x55, 0x55, 0xFF,

	0x00, 0x00, 0x00, 0x00,
	0x55, 0x55, 0x55, 0xFF,
	0x55, 0x55, 0x55, 0xFF,
	0x00, 0x00, 0x00, 0x00,
};

float gausian_matrix[] = {
	1.f, 4.f, 6.f, 4.f, 1.f,
	4.f,16.f,24.f,16.f, 4.f,
	6.f,24.f,36.f,24.f, 6.f,
	4.f,16.f,24.f,16.f, 4.f,
	1.f, 4.f, 6.f, 4.f, 1.f
};
// 96
float gausian_matrix_norm[] = {
	0.0625f,
	0.25f,
	0.375f,
	0.25f,
	0.0625f,
};

BlastGame::BlastGame() : window(new fea::SDL2WindowBackend()),
	input(new fea::SDL2InputBackend()),
	renderer(fea::Viewport({800.0f, 600.0f}, {0, 0}, fea::Camera({800.0f / 2.0f, 600.0f / 2.0f}))),
	quad({10.f, 600.f})
{
}

void BlastGame::setup(const std::vector<std::string> &args)
{
	window.create(fea::VideoMode(800,600, 32), "Blaster");
	renderer.setup();
	beam.create(4, 1, tex_beamData4);
	circle.create(4, 4, tex_circle);

	quad.setTexture(beam);

	framebufferShader.setSource(
				Shaders::Framebuffer::VertexSource,
				Shaders::Framebuffer::FragmentSource);
	framebufferShader.compile();
	framebufferShader.activate();
	framebufferShader.setUniform("gaussianValues", fea::FLOAT, 5, gausian_matrix_norm);

	brightnessShader.setSource(
				Shaders::Framebuffer::VertexSource,
				Shaders::BrightnessExtract::FragmentSource);
	brightnessShader.compile();

	framebufferQuad.setPosition({0.f, 0.f});
	framebufferQuad.setVFlip(true);
}

glm::vec2 dir(1.f, 0.f);

void BlastGame::loop()
{
	fea::Event eve;

	while(input.pollEvent(eve)) {
		if (eve.type == fea::Event::CLOSED) {
			quit();
		}
	}

	fea::RenderTarget screenTarget;
	screenTarget.create(window.getSize().x, window.getSize().y);

	renderer.clear(screenTarget, fea::Color(0.f, 0.f, 0.f));

	renderer.setBlendMode(fea::ALPHA);

	drawBeamY(150.f);
	drawBeamY(350.f);
	drawBeamX(125.f);

	fea::Quad q{{50.f, 50.f}};
	q.setPosition({300.f, 300.f});
	q.setTexture(circle);
	renderer.queue(q);

	renderer.render(screenTarget);

	renderer.setBlendMode(fea::ALPHA);
	framebufferQuad.setTexture(screenTarget.getTexture());
	framebufferQuad.setSize({window.getSize().x, window.getSize().y});
	renderer.queue(framebufferQuad);

	// Perform the final post-process.
	fea::RenderTarget ppTarget;
	ppTarget.create(screenTarget.getSize().x, screenTarget.getSize().y);
	fea::RenderTarget ppTarget2;
	ppTarget2.create(screenTarget.getSize().x, screenTarget.getSize().y);

	renderer.render(ppTarget, brightnessShader);

	dir.x = 1.f; dir.y = 0.f;
	framebufferShader.activate();
	framebufferShader.setUniform("blurDir", fea::VEC2, &dir);
	framebufferQuad.setTexture(ppTarget.getTexture());
	renderer.queue(framebufferQuad);
	renderer.render(ppTarget2, framebufferShader);

	dir.x = 0.f; dir.y = 1.f;
	framebufferShader.activate();
	framebufferShader.setUniform("blurDir", fea::VEC2, &dir);
	framebufferQuad.setTexture(ppTarget2.getTexture());
	renderer.queue(framebufferQuad);

	renderer.clear();
	renderer.render(framebufferShader);

	window.swapBuffers();
}

void BlastGame::destroy()
{

}

void BlastGame::drawBeamY(const float X)
{
	quad.setRotation(0.f);
	quad.setPosition({X, 0.f});
	quad.setSize({10.f, window.getSize().y});
	renderer.queue(quad);
}

void BlastGame::drawBeamX(const float Y)
{
	quad.setRotation((3.1415f/2.f));
	quad.setPosition({0.f, Y});
	quad.setSize({10.f, window.getSize().x});
	renderer.queue(quad);
}
