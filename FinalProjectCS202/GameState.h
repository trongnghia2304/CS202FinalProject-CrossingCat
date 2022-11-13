#pragma once
#include "State.h"
#include "Player.h"

class GameState : public State
{
private:
	sf::Texture backgroundTexture;
	sf::RectangleShape background;

	sf::Font font;

	int curLv, rows, columns, mines, currentFlags;
	float height, width;
	bool lastPlay;

	Player player;

	//std::string getMode();

public:

	GameState(sf::RenderWindow* window, std::map<std::string, int>* supportedKey, std::vector<State*>* states) : State(window, supportedKeys,states)  {
		this->supportedKeys = supportedKey;
		this->initKeyBinds();
	};
	virtual ~GameState() {

	};
	//void adjustCells(const int totalX, const int totalY);
	//Vector2u setCenter(const int totalX, const int totalY);

	//bool checkLastPlay();

	void updateEvents() {

	};
	void updateInput(const float& dt) {
		this->player.move(dt, 0, 0);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_LEFT"))))
			this->player.move(dt, -1.f, 0.f);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_UP"))))
			this->player.move(dt, 0.f, -1.f);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_RIGHT"))))
			this->player.move(dt, 1.f, 0.f);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_DOWN"))))
			this->player.move(dt, 0.f, 1.f);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CLOSE"))))
			this->endState();
	};
	void update(const float& dt) {
		updateInput(dt);
		this->player.update(dt);
	};
	void render(sf::RenderTarget* target = nullptr) {
		if (!target) {
			target = this->window;
		}
		this->player.render(target);
	};
};