#ifndef BLASTGAME_HPP
#define BLASTGAME_HPP

#include <featherkit/structure.hpp>

#include <featherkit/userinterface.hpp>

#include <featherkit/ui/sdl2windowbackend.hpp>
#include <featherkit/ui/sdl2inputbackend.hpp>

#include <featherkit/render2d.hpp>

class BlastGame : public fea::Application
{
public:
	BlastGame();
protected:
	void setup(const std::vector<std::string> &args) override;
	void loop() override;
	void destroy() override;
private:

	fea::Window window;
	fea::InputHandler input;

	fea::Renderer2D renderer;

	fea::Shader framebufferShader;
	fea::Shader brightnessShader;
	fea::Quad framebufferQuad;

	fea::Quad quad;
	fea::Texture beam;
	fea::Texture circle;

	void drawBeamY(const float X);
	void drawBeamX(const float Y);
};

#endif // BLASTGAME_HPP
