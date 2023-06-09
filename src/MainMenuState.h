#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include "SettingState.h"
#include "GameState.h"
#include "MenuAnimation.h"

class MainMenuState : public State {
private:
	sf::RectangleShape background;
	sf::Texture backgroundTexture;
	sf::Font font;
	MenuAnimation Meow;
	int curOpt = 1;
public:
	bool isWordMode = true;
	MainMenuState(sf::RenderWindow* window, std::vector<State*>* states);

	MainMenuState(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::vector<State*>* states)
		: MainMenuState(window, states) {
		this->supportedKeys = supportedKeys;
		
		Meow.SetGui(this->gui);
	};

	void initBackground() {

	};

	void initButtons() {
		this->gui->get<tgui::Button>("btnPlay")->onClick([&]() {
			this->states->push_back(new GameState(this->window, this->supportedKeys, this->states));
			});
		this->gui->get<tgui::Button>("btnLoad")->onClick([&]() {
			cout << "load button pressed\n";
			});
		this->gui->get<tgui::Button>("btnSetting")->onClick([&, this]() {
			cout << "setting button pressed\n";
			this->states->push_back(new SettingState(this->window, this->states));
			});

		this->gui->get<tgui::Button>("btnPlay")->onMouseLeave([&, this]() {
			zoomSmall("btnPlay");
			});
		this->gui->get<tgui::Button>("btnLoad")->onMouseLeave([&, this]() {
			zoomSmall("btnLoad");
			});
		this->gui->get<tgui::Button>("btnSetting")->onMouseLeave([&, this]() {
			zoomSmall("btnSetting");
			});

		this->gui->get<tgui::Button>("btnPlay")->onMouseEnter([&, this]() {
			zoomBig("btnPlay");
			});
		this->gui->get<tgui::Button>("btnLoad")->onMouseEnter([&, this]() {
			zoomBig("btnLoad");
			});
		this->gui->get<tgui::Button>("btnSetting")->onMouseEnter([&, this]() {
			zoomBig("btnSetting");
			});
	};

	void updateInput(const float& dt) override {

	};
	void zoomBig(string Button) {
		this->gui->get<tgui::Button>(Button)->setScale({ 1.1f, 1.1f }, { 0.5f,0.5f });
	}
	void zoomSmall(string Button) {
		this->gui->get<tgui::Button>(Button)->setScale({ 1.0f / 1.1f, 1.0f / 1.1f }, { 0.5f,0.5f });
	}
	void updateEvents() override {
		this->gui->handleEvent(this->ev);
		switch (this->ev.type)
		{
		case sf::Event::Closed:
			this->endState();
			break;
		case sf::Event::TextEntered:

			//update
			break;
		default:
			break;
		}
	};

	void update(const float& dt) override {
		Meow.update(dt); 
	};

	void render(sf::RenderTarget* target = nullptr) override;
};