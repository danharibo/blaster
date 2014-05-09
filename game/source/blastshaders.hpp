#ifndef BLASTSHADERS_HPP
#define BLASTSHADERS_HPP
#include <string>

namespace Shaders {
	struct Framebuffer {
		static std::string VertexSource;
		static std::string FragmentSource;
	};

	struct BrightnessExtract {
		static std::string FragmentSource;
	};
}

#endif // BLASTSHADERS_HPP
