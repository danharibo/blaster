add_executable(blast
	source/main.cpp
	source/blastgame.cpp
	source/blastshaders.cpp
	source/feapolygon.cpp
	source/ship.cpp
	source/swordfish.cpp
	source/badguy.cpp
	source/explosion.cpp
)
include_directories(
	#/usr/local/include/cegui-0/
)
target_link_libraries( blast SDL2-2.0 GLEW GL featherkit-ui featherkit-sdl2 featherkit-rendering featherkit-structure )

