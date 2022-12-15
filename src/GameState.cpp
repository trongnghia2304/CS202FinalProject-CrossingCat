#include "GameState.h"
#include "Consts.h"
#include "Enums.h"
#include "Level.h"
#include "SceneNode.h"
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <memory>

GameState::GameState(sf::RenderWindow *window,
                     std::map<std::string, int> *supportedKeys,
                     std::vector<State *> *states)
    : State(window, supportedKeys, states) {
    this->gui->loadWidgetsFromFile("resources/Template/GameTemplate.txt");
    this->initKeyBinds();

    auto pPlayer = std::unique_ptr<class Player>(
        new class Player({window->getSize().x / 2 - GRID_SIZE.x,
                          (float)window->getSize().y - GRID_SIZE.y},
                         GRID_SIZE));
    player = pPlayer.get();
    pauseMenu = new PauseMenu(window, states);
    world = new World(sf::Vector2f(window->getSize()));
    world->setDebug(true, true);
    camera = new Camera(*player, *window, *world);
    world->attachChild(std::move(pPlayer));
};

GameState::~GameState() {
    delete pauseMenu;
    delete world;
    delete camera;
};

void GameState::updateEventsPauseMenu() {
    pauseMenu->ev = this->ev;
    pauseMenu->updateEvents();
}

void GameState::updateEvents() {
    this->gui->handleEvent(this->ev);

    switch (this->ev.type) {
    case sf::Event::Closed:
        this->endState();
        break;
    case sf::Event::TextEntered:

        // update
        break;
    default:
        break;
    }

    updateEventsPauseMenu();

    if (pauseMenu->getQuit()) {
        this->endState();
    }
};

void GameState::updateInput(const float &dt) {
    this->pauseMenu->updateInput();

    if (this->ev.type == sf::Event::KeyPressed) {
        this->player->onKeyPressed(this->ev.key);
    }
};

void GameState::update(const float &dt) {
    float transDt = dt;
    if (pauseMenu->isPausing())
        transDt = 0;
    updateInput(transDt);
    world->update(sf::seconds(transDt));
    camera->update(sf::seconds(transDt));
    // this->player.update(dt);
    player->update(sf::Time(sf::seconds(transDt)));

    std::set<SceneNode::Pair> collisionPairs;
    world->checkSceneCollision(*world, collisionPairs);
    for (auto pair : collisionPairs) {
        SceneNode *nodeA;
        SceneNode *nodeB;
        if (pair.first->getCategory() == Category::Player) {
            nodeA = pair.first;
            nodeB = pair.second;
        } else if (pair.second->getCategory() == Category::Player) {
            nodeA = pair.second;
            nodeB = pair.first;
        } else {
            continue;
        }
        switch (nodeB->getCategory()) {
        case Category::Obstacle:
            player->onCollision(nodeB);
            break;
        case Category::Enemy:
            player->takeDamage(40);
            player->onCollision(nodeB);
            break;
        case Category::Reward:
            std::cout << "Reward" << std::endl;
            world->getCurrentLevel()->removeObject(*nodeB);
            break;
        case Category::SmallSizeBoost:
            player->takeSmallSizeBoost();
            world->getCurrentLevel()->removeObject(*nodeB);
            break;
        case Category::SpeedBoost:
            player->takeSpeedBoost();
            world->getCurrentLevel()->removeObject(*nodeB);
            break;
        case Category::Health:
            player->takeFood();
            world->getCurrentLevel()->removeObject(*nodeB);
            break;
        default:
            break;
        }
    }
    if (player->isDead())
        this->endState();
};

void GameState::render(sf::RenderTarget *target) {
    if (!target) {
        target = this->window;
    }
    // this->player.render(target);
    this->gui->draw();
    target->draw(*world);
    target->draw(*player);
    pauseMenu->render(target);
};
