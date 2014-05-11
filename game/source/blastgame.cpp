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
	0xFF, 0xFF, 0xFF, 0xFF,
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

float gaussian_matrix[] = {
	1.f,
	6.f,
	15.f,
	20.f,
	15.f,
	6.f,
	1.f
};

float gaussian_matrix_norm[] = {
	0.00390625,
	0.03125,
	0.109375,
	0.21875,
	0.2734375,
	0.21875,
	0.109375,
	0.03125,
	0.00390625
};

BlastGame::BlastGame() : window(new fea::SDL2WindowBackend()),
	input(new fea::SDL2InputBackend()),
	renderer(fea::Viewport({800.0f, 600.0f}, {0, 0}, fea::Camera({800.0f / 2.0f, 600.0f / 2.0f}))),
	quad({10.f, 600.f}), projectile({1.f, 4.f})
{
}

void BlastGame::setup(const std::vector<std::string> &args)
{
	window.create(fea::VideoMode(800,600, 32), "Blaster");
	renderer.setup();
	beam.create(1, 1, tex_beamData4);
	circle.create(4, 4, tex_circle);

	quad.setTexture(beam);

	framebufferShader.setSource(
				Shaders::Framebuffer::VertexSource,
				Shaders::Framebuffer::FragmentSource);
	framebufferShader.compile();
	framebufferShader.activate();
	framebufferShader.setUniform("gaussianValues", fea::FLOAT, 9, gaussian_matrix_norm);

	brightnessShader.setSource(
				Shaders::Framebuffer::VertexSource,
				Shaders::BrightnessExtract::FragmentSource);
	brightnessShader.compile();

	framebufferQuad.setPosition({0.f, 0.f});
	framebufferQuad.setVFlip(true);

	bg.setPosition({600.f, 400.f});
	bg.setRotation((3.1415f*1.5f));
	bg.setVelocity({-36.f, 0.f});

	sf.setPosition({200.f, 400.f});
	sf.setRotation((3.1415f*0.5f));
	sf.setVelocity({2.f, 0.f});

	projectile.setTexture(beam);
}

glm::vec2 dir(1.f, 0.f);

void BlastGame::loop()
{
	fea::Event eve;

	while(input.pollEvent(eve)) {
		if (eve.type == fea::Event::CLOSED) {
			quit();
		}
		if(eve.type == fea::Event::KEYPRESSED) {
			if(eve.key.code == fea::Keyboard::Q) {
				sf.setAngularVeocity(sf.getAngularVelocity() + 0.05f);
			}
			if(eve.key.code == fea::Keyboard::E) {
				sf.setAngularVeocity(sf.getAngularVelocity() - 0.05f);
			}
			if(eve.key.code == fea::Keyboard::W) {
				sf.setVelocity(sf.getVelocity() +
							   sf.getForwardVector() * 1.0f);
			}
			if(eve.key.code == fea::Keyboard::S) {
				sf.setVelocity(sf.getVelocity() -
							   sf.getForwardVector() * 1.0f);
			}
			if(eve.key.code == fea::Keyboard::D) {
				sf.setVelocity(sf.getVelocity() +
							   sf.getRightVector() * 0.5f);
			}
			if(eve.key.code == fea::Keyboard::A) {
				sf.setVelocity(sf.getVelocity() -
							   sf.getRightVector() * 0.5f);
			}
			if(eve.key.code == fea::Keyboard::SPACE) {
				projectiles.push_back({
										  sf.getPosition() + sf.getForwardVector() * 1.5f,
										  sf.getForwardVector() * 100.f, fea::Color(0, 0, 255)
									  });
			}
		}
	}

	fea::RenderTarget screenTarget;
	screenTarget.create(window.getSize().x, window.getSize().y);

	renderer.clear(screenTarget, fea::Color(0.f, 0.f, 0.f));

	//renderer.setBlendMode(fea::ALPHA);

	float dt = 0.016f;

	drawBeamY(150.f);
	drawBeamY(350.f);
	drawBeamX(125.f);

	bg.tick(dt);
	bg.draw(renderer);

	sf.tick(dt);
	sf.draw(renderer);

	for(auto it = projectiles.begin();
		it != projectiles.end();)
	{
		if( it->distanceToDeath <= 0.f ) {
			it = projectiles.erase(it);
			continue;
		}
		auto dp = it->velocity * dt;
		it->position += dp;
		it->distanceToDeath -= glm::length(dp);

		drawProjectile(*it);
		it++;
	}

	renderer.clear(screenTarget);
	renderer.render(screenTarget);

	framebufferQuad.setTexture(screenTarget.getTexture());
	framebufferQuad.setSize({window.getSize().x, window.getSize().y});
	renderer.queue(framebufferQuad);

	// Perform the final post-process.
	fea::RenderTarget ppTarget;
	ppTarget.create(screenTarget.getSize().x, screenTarget.getSize().y);
	fea::RenderTarget ppTarget2;
	ppTarget2.create(screenTarget.getSize().x, screenTarget.getSize().y);

	renderer.setBlendMode(fea::NONE);

	renderer.clear(ppTarget);
	renderer.render(ppTarget, brightnessShader);

	dir.x = 1.f; dir.y = 0.f;
	framebufferShader.activate();
	framebufferShader.setUniform("blurDir", fea::VEC2, &dir);
	framebufferQuad.setTexture(ppTarget.getTexture());
	renderer.queue(framebufferQuad);
	renderer.clear(ppTarget2);
	renderer.render(ppTarget2, framebufferShader);

	dir.x = 0.f; dir.y = 1.f;
	framebufferShader.activate();
	framebufferShader.setUniform("blurDir", fea::VEC2, &dir);
	auto t = screenTarget.getTexture().getId();
	framebufferShader.setUniform("sceneTexture", fea::TEXTURE, 1, &t);
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
	quad.setSize({2.f, window.getSize().y});
	renderer.queue(quad);
}

void BlastGame::drawBeamX(const float Y)
{
	quad.setRotation((3.1415f/2.f));
	quad.setPosition({0.f, Y});
	quad.setSize({2.f, window.getSize().x});
	renderer.queue(quad);
}

void BlastGame::drawProjectile(const ProjectileInfo &pi)
{
	projectile.setPosition(pi.position);
	projectile.setColor(pi.color);
	projectile.setRotation(atan2(pi.velocity.x, pi.velocity.y));
	renderer.queue(projectile);
}
