#include "blastgame.hpp"
#include "blastshaders.hpp"
#include <algorithm>
#include <featherkit/render2dtext.hpp>

u_int8_t tex_beamData4[] = {
	0xFF, 0xFF, 0xFF, 0xFF,
};

// #####
// _###_
// __#__
u_int8_t tex_radarTriangle[] {
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,

	0x00, 0x00, 0x00, 0x00,
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,
	0x00, 0x00, 0x00, 0x00,

	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0xFF, 0xFF, 0xFF, 0xFF,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,

};

#define HALFPI (3.1415f/2.f)
#define QUTRPI (3.1415f/4.f)

std::vector<BackgroundBeam> bgBeams = {
	{{-5000.f,  -5000.f}, 0.f, 0.95f},
	{{-1000.f,  -5000.f}, 0.f, 0.95f},
	{{-250.f,  -5000.f}, 0.f, 0.95f},
	{{0.f,    -5000.f}, 0.f, 0.95f},
	{{250.f,  -5000.f}, 0.f, 0.95f},
	{{500.f,  -5000.f}, 0.f, 0.95f},
	{{1000.f,  -5000.f}, 0.f, 0.95f},
	{{5000.f,  -5000.f}, 0.f, 0.95f},

	{{-5000.f,-500.f}, HALFPI, 0.95f},
	{{-5000.f, 0.f},   HALFPI, 0.95f},
	{{-5000.f, 500.f}, HALFPI, 0.95f},

	{{-5000.f, 500.f}, HALFPI, 0.85f},
	{{-5000.f,-500.f}, HALFPI, 0.85f},
	{{-5000.f, 600.f}, HALFPI, 0.80f},

	{{-5000.f, 570.f}, HALFPI, 0.45f},
	{{-5000.f,-870.f},    0.f, 0.40f},

	{{-5000.f,-5000.f}, QUTRPI, 0.65f},
	{{-4500.f,-5000.f}, QUTRPI, 0.65f},
	{{-4000.f,-5000.f}, QUTRPI, 0.65f},
	{{-3500.f,-5000.f}, QUTRPI, 0.65f},
	{{-3000.f,-5000.f}, QUTRPI, 0.65f},
	{{-2500.f,-5000.f}, QUTRPI, 0.65f},
	{{-2000.f,-5000.f}, QUTRPI, 0.65f},
	{{-1500.f,-5000.f}, QUTRPI, 0.65f},
	{{-1000.f,-5000.f}, QUTRPI, 0.65f},

	{{-5000.f,-4500.f}, QUTRPI, 0.65f},
	{{-5000.f,-4000.f}, QUTRPI, 0.65f},
	{{-5000.f,-3500.f}, QUTRPI, 0.65f},
	{{-5000.f,-3000.f}, QUTRPI, 0.65f},
	{{-5000.f,-2500.f}, QUTRPI, 0.65f},
	{{-5000.f,-2000.f}, QUTRPI, 0.65f},
	{{-5000.f,-1500.f}, QUTRPI, 0.65f},
	{{-5000.f,-1000.f}, QUTRPI, 0.65f},
};

#define GAUSS_BLUR_DIAM (13)

BlastGame::BlastGame() : window(new fea::SDL2WindowBackend()),
	input(new fea::SDL2InputBackend()),
	renderer(fea::Viewport({800.0f, 600.0f}, {0, 0}, fea::Camera({800.0f / 2.0f, 600.0f / 2.0f}))),
	uifont("/usr/share/fonts/truetype/ttf-dejavu/DejaVuSerif.ttf"), /* TODO == TOP KEK */
	quad({10.f, 600.f}), projectile({1.f, 4.f})
{
}

void BlastGame::setup(const std::vector<std::string> &args)
{
	window.create(fea::VideoMode(800,600, 32), "Blaster");
	renderer.setup();
	beam.create(1, 1, tex_beamData4);
	radarTriangle.create(5,3, tex_radarTriangle);

	quad.setTexture(beam);

	// Calculate guassian blur values
	std::vector<float> previousRow = {1,1};
	for(int i = 2; i < GAUSS_BLUR_DIAM; ++i) {
		std::vector<float> pascallRow = {1};
		for(int j = 1; j < previousRow.size(); ++j) {
			pascallRow.push_back({previousRow[j-1] + previousRow[j]});
		}
		pascallRow.push_back({1});
		previousRow = pascallRow;
	}

	float sum = std::accumulate(previousRow.begin(), previousRow.end(), 0.f);
	std::transform(previousRow.begin(), previousRow.end(), previousRow.begin(),
				   [=](float a){ return a / sum; });
	gaussian_values = previousRow;

	framebufferShader.setSource(
				Shaders::Framebuffer::VertexSource,
				Shaders::Framebuffer::FragmentSource);
	framebufferShader.compile();
	framebufferShader.activate();
	framebufferShader.setUniform("gaussianValues", fea::FLOAT, gaussian_values.size(), gaussian_values.data());

	brightnessShader.setSource(
				Shaders::Framebuffer::VertexSource,
				Shaders::BrightnessExtract::FragmentSource);
	brightnessShader.compile();

	framebufferQuad.setPosition({0.f, 0.f});
	framebufferQuad.setVFlip(true);

	auto badguy = new BadGuy;

	badguy->setPosition({600.f, 400.f});
	badguy->setRotation((3.1415f*1.5f));
	badguy->setVelocity({-36.f, 0.f});

	ships.push_back(badguy);

	sf = new Swordfish;

	sf->setPosition({200.f, 400.f});
	sf->setRotation((3.1415f*0.5f));
	sf->setVelocity({2.f, 0.f});

	ships.push_back(sf);

	sf->setFireCallback([&](const glm::vec2& p, const glm::vec2& v) {
				projectiles.push_back(ProjectileInfo{
										  p,
										  v, fea::Color(255, 255, 0),
										  sf
									  });
	});

	projectile.setTexture(beam);
}

glm::vec2 dir(1.f, 0.f);

void BlastGame::loop()
{
	float dt = 0.016f;
	fea::Event eve;

	while(input.pollEvent(eve)) {
		if (eve.type == fea::Event::CLOSED) {
			quit();
		}
		bool pressed = eve.type == fea::Event::KEYPRESSED;
		if(eve.type == fea::Event::KEYRELEASED
				|| pressed) {
			if(eve.key.code == fea::Keyboard::Q) {
				sf->setThruster(Ship::T_YAWL, pressed);
			}
			if(eve.key.code == fea::Keyboard::E) {
				sf->setThruster(Ship::T_YAWR, pressed);
			}
			if(eve.key.code == fea::Keyboard::W) {
				sf->setThruster(Ship::T_MAIN, pressed);
			}
			if(eve.key.code == fea::Keyboard::S) {
				sf->setThruster(Ship::T_REVERSE, pressed);
			}
			if(eve.key.code == fea::Keyboard::D) {
				sf->setThruster(Ship::T_MOVER, pressed);
			}
			if(eve.key.code == fea::Keyboard::A) {
				sf->setThruster(Ship::T_MOVEL, pressed);
			}
			if(eve.key.code == fea::Keyboard::SPACE) {
				sf->setThruster(Ship::T_WEAPON, pressed);
			}
		}
	}

	for(auto it = ships.begin();
		it != ships.end();)
	{
		if((*it)->getHP() <= 0.f) {
			delete *it;
			it = ships.erase(it);
			continue;
		}

		(*it)->tick(dt);
		it++;
	}

	// Make sure there's always someone to shoot.
	if(ships.size() < 2) {
		auto badguy = new BadGuy;

		badguy->setPosition({600.f, 400.f});
		badguy->setRotation((3.1415f*1.5f));
		badguy->setVelocity({-36.f, 0.f});

		ships.push_back(badguy);
	}

	float moveBox = 0.5f;

	auto velocity = glm::length(sf->getVelocity());
	auto lookDir = sf->getForwardVector();
	auto moveDir = lookDir * velocity;
	if(std::abs(moveDir.x) > (window.getSize().x/2.f)*moveBox) {
		moveDir.x = (std::signbit(moveDir.x)?-1.f:1.f) * (window.getSize().x/2.f)*moveBox;
	}
	if(std::abs(moveDir.y) > (window.getSize().y/2.f)*moveBox) {
		moveDir.y = (std::signbit(moveDir.y)?-1.f:1.f) * (window.getSize().y/2.f)*moveBox;
	}
	auto lookBase = sf->getPosition();
	auto lookTarget = lookBase + moveDir;
	renderer.getViewport().getCamera().setPosition(lookTarget);

	fea::RenderTarget screenTarget;
	screenTarget.create(window.getSize().x, window.getSize().y);

	renderer.clear(screenTarget, fea::Color(0.f, 0.f, 0.f));

	for(auto& b : bgBeams) drawBeam(b);

	fea::Quad healthQuad;
	healthQuad.setColor({255, 0, 0});
	fea::Quad radarQuad;
	radarQuad.setTexture(radarTriangle);
	radarQuad.setScale({5.f, 5.f});

	for(auto it = ships.begin();
		it != ships.end(); ++it)
	{
		(*it)->draw(renderer);

		if((*it) != sf) {
			float healthWidth = (*it)->getBoundingRadius()*2.f;
			healthQuad.setSize({((*it)->getHP()/50.f) * healthWidth, 5.f});
			healthQuad.setPosition((*it)->getPosition() + glm::vec2{-healthQuad.getSize().x/2.f, 20.f});
			renderer.queue(healthQuad);

			float radarRadius = std::min(window.getSize().x, window.getSize().y) * 0.25f;
			auto dp = (*it)->getPosition() - sf->getPosition();
			if(glm::length(dp) > radarRadius) {
				auto dir = glm::normalize(dp);
				radarQuad.setRotation(atan2(dir.x, dir.y));
				radarQuad.setPosition(lookTarget + dir * radarRadius);
				radarQuad.setColor({255, 0, 0});
				renderer.queue(radarQuad);
			}
		}
	}

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

		// Find any objects currently being intersected
		auto ship = std::find_if(ships.begin(), ships.end(),
							   [&](Ship* s) { return s != it->owner && glm::length(it->position - s->getPosition()) < s->getBoundingRadius(); });
		if( ship != ships.end() ) {
			it = projectiles.erase(it);
			(*ship)->damage(10.f);
			continue;
		}

		drawProjectile(*it);
		it++;
	}

	renderer.clear(screenTarget);
	renderer.render(screenTarget);

	// Reset the camera for viewport stuff.
	renderer.getViewport().getCamera().setPosition(glm::vec2{window.getSize().x, window.getSize().y}/2.f);

	float healthWidth = window.getSize().x - 40;
	healthQuad.setSize({(sf->getHP()/100.f) * healthWidth, 10.f});
	healthQuad.setPosition({20.f, window.getSize().y-30.f});
	healthQuad.setColor({0, 255, 0});
	renderer.queue(healthQuad);

	fea::TextSurface text;
	text.setPenPosition({20.f, window.getSize().y-35.f});
	text.setPenColor({255, 255, 255});
	text.setPenFont(uifont);
	text.write("The Swordfish");
	renderer.queue(text);

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
	for(auto it = ships.begin();
		it != ships.end();)
	{
		delete *it;
		it = ships.erase(it);
	}
}

void BlastGame::drawBeam(const BackgroundBeam &beam)
{
	quad.setRotation(beam.angle);
	quad.setPosition(beam.offset);
	quad.setSize({2.f, 10000.f});
	quad.setParallax({beam.parallax});
	renderer.queue(quad);
}

void BlastGame::drawProjectile(const ProjectileInfo &pi)
{
	projectile.setPosition(pi.position);
	projectile.setColor(pi.color);
	projectile.setRotation(atan2(pi.velocity.x, pi.velocity.y));
	renderer.queue(projectile);
}
