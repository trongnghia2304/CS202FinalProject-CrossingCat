#pragma once
#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <thread>
#include "MainMenuState.h"


class Game
{
private:
	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	sf::Event ev;
	std::vector<sf::VideoMode> videoModes;
	sf::ContextSettings windowSettings;
	std::map<std::string, int> supportedKeys;
	bool fullscreen;

	sf::Clock dtClock;
	float dt;

	std::vector<State*> states;


	void initVariables();
	void initWindow();
	void initState();

	void initKeys()
	{
		this->supportedKeys.emplace("Escape", sf::Keyboard::Key::Escape);
		this->supportedKeys.emplace("A", sf::Keyboard::Key::A);
		this->supportedKeys.emplace("W", sf::Keyboard::Key::W);
		this->supportedKeys.emplace("D", sf::Keyboard::Key::D);
		this->supportedKeys.emplace("S", sf::Keyboard::Key::S);
	}

	void initTexture() {
        TextureHolder::instance().load(Texture::ID::LeftVehicle,"./resources/images/greenCar.png");
        TextureHolder::instance().add(Texture::ID::RightVehicle, {Texture::ID::LeftVehicle, true, false});
        TextureHolder::instance().load(Texture::ID::LeftAnimal,"./resources/images/greenCar.png");
        TextureHolder::instance().add(Texture::ID::RightAnimal, {Texture::ID::LeftAnimal, true, false});
        TextureHolder::instance().load(Texture::ID::Road,"./resources/images/VehicleRoad.png", {32, 32}, 9);
        TextureHolder::instance().add(Texture::ID::TopRoadLane, {Texture::ID::Road, 1, false, false});
        TextureHolder::instance().add(Texture::ID::MidRoadLane, {Texture::ID::Road, 4, false, false});
        TextureHolder::instance().add(Texture::ID::BotRoadLane, {Texture::ID::Road, 7, false, false});
        TextureHolder::instance().add(Texture::ID::MonoRoadLane, {Texture::ID::Road, 8, false, false});
		TextureHolder::instance().load(Texture::ID::PlayerIdleUp, "resources/spritesheet/main/idleUp.png", { 128, 128 }, 6);
        TextureHolder::instance().load(Texture::ID::PlayerIdleDown, "resources/spritesheet/main/idleDown.png", { 128, 128 }, 6);
        TextureHolder::instance().load(Texture::ID::PlayerIdleLeft, "resources/spritesheet/main/idleLeft.png", { 128, 128 }, 6);
        TextureHolder::instance().load(Texture::ID::PlayerIdleRight, "resources/spritesheet/main/idleRight.png", { 128, 128 }, 6);
		TextureHolder::instance().load(Texture::ID::PlayerJumpLeft, "resources/spritesheet/main/goLeft.png", { 128, 128 }, 6);
        TextureHolder::instance().load(Texture::ID::PlayerJumpRight, "resources/spritesheet/main/goRight.png", { 128, 128 }, 6);
		TextureHolder::instance().load(Texture::ID::PlayerJumpUp, "resources/spritesheet/main/goUp.png", { 128, 128 }, 6);
		TextureHolder::instance().load(Texture::ID::PlayerJumpDown, "resources/spritesheet/main/goDown.png", { 128, 128 }, 4);
		TextureHolder::instance().load(Texture::ID::Null, "resources/spritesheet/NULL.png", { 32, 32 }, 1);
	};

	void updateEvents();
	void updateDt();


public:
	Game();
	virtual ~Game();

	bool running() const;

	void endApplication();

	void updateSFMLEvents();
	void update();

	void render();
	void run();

	void closeWindow();
	void keyReleasedProc();
	void mouseProc();
};
