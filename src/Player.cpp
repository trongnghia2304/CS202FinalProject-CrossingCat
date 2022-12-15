#include "Player.h"
#include "Consts.h"
#include "Entity.h"
#include "Enums.h"
//#include "PlayerEffect.h"
#include "PlayerState.h"
#include "SceneNode.h"
#include "World.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <cmath>
#include <iostream>
#include <memory>

void Player::updateCurrent(sf::Time dt) {
    if (isInvincible) {
        invincibleTime += dt;
        if (invincibleTime > invincibleDuration) {
            invincibleTime = sf::Time::Zero;
            isInvincible   = false;
        }
    }

    if (onSizeBoost) {
        localBounds = sizeBoostBounds;
        sizeBoostTime += dt;
        if (sizeBoostTime > sizeBoostDuration) {
            sizeBoostTime = sf::Time::Zero;
            onSizeBoost   = false;
            localBounds   = defaultBounds;
        }
    }

    if (onSpeedBoost) {
        speedBoostTime += dt;
        if (speedBoostTime > speedBoostDuration) {
            speedBoostTime = sf::Time::Zero;
            onSpeedBoost   = false;
        }
    }
    state->update(dt);
    health -= healthReductionRate * dt.asSeconds();
    if (health <= 0 && state->getStateID() != PlayerState::StateID::Dying) {
        health = 0;
        setState(new DyingState(this));
    }
    Entity::updateCurrent(dt);
}

Player::Player()
    : jumpTexture(Texture::ID::PlayerJumpUp),
      idleTexture(Texture::ID::PlayerIdleUp), ripTexture(Texture::RIP),
      state(new IdleState(this)) {
    setState(new IdleState(this));
    animation = AnimationMachine(idleTexture, sf::seconds(5), true);
}

Player::Player(sf::Vector2f position, sf::Vector2f size)
    : Entity(Texture::ID::PlayerIdleUp, position, size, DEF_PLAYER_VELOCITY),
      jumpTexture(Texture::ID::PlayerJumpUp),
      idleTexture(Texture::ID::PlayerIdleUp),
      ripTexture(Texture::ID::RIP), state(new IdleState(this)) {
    setState(new IdleState(this));
    setVelocity({0, 0});
    localBounds = sf::FloatRect(20, 20, GRID_SIZE.x - 40, GRID_SIZE.y - 40);
}

void Player::onKeyPressed(sf::Event::KeyEvent event) {
    if (state->getStateID() == PlayerState::Jumping) {
        return;
    }
    auto newPos     = getPosition();
    auto currentPos = getPosition();
    if (state->getStateID() == PlayerState::Idle) {
        switch (event.code) {
        case sf::Keyboard::W:
        case sf::Keyboard::Up:
            newPos      = {currentPos.x, currentPos.y - GRID_SIZE.y};
            jumpTexture = Texture::ID::PlayerJumpUp;
            idleTexture = Texture::ID::PlayerIdleUp;
            break;
        case sf::Keyboard::S:
        case sf::Keyboard::Down:
            newPos      = {currentPos.x, currentPos.y + GRID_SIZE.y};
            jumpTexture = Texture::ID::PlayerJumpDown;
            idleTexture = Texture::ID::PlayerIdleDown;
            break;
        case sf::Keyboard::A:
        case sf::Keyboard::Left:
            newPos      = {currentPos.x - GRID_SIZE.x, currentPos.y};
            jumpTexture = Texture::ID::PlayerJumpLeft;
            idleTexture = Texture::ID::PlayerIdleLeft;
            break;
        case sf::Keyboard::D:
        case sf::Keyboard::Right:
            newPos      = {currentPos.x + GRID_SIZE.x, currentPos.y};
            jumpTexture = Texture::ID::PlayerJumpRight;
            idleTexture = Texture::ID::PlayerIdleRight;
            break;
        default:
            break;
        }
        if (getPosition() != newPos) {
            newPos.x = newPos.x + getLocalBounds().width / 2;
            newPos.y = newPos.y + getLocalBounds().height / 2;
            newPos   = getNearestGridPosition(newPos);
            setState(new JumpingState(this, newPos));
        }
    }
}

void Player::onCollision(SceneNode *other) {
    if (!other)
        return;

    auto getDirection = [](sf::Vector2f v) {
        if (v.y == 0) {
            v.y = 0;
        } else {
            v.y = v.y / std::abs(v.y);
        }
        if (v.x == 0) {
            v.x = 0;
        } else {
            v.x = v.x / std::abs(v.x);
        }
        return v;
    };

    auto normalize = [](sf::Vector2f v) {
        auto length = std::sqrt(v.x * v.x + v.y * v.y);
        if (length != 0) {
            return sf::Vector2f(v.x / length, v.y / length);
        }
        return sf::Vector2f(0, 0);
    };
    if (other->getCategory() == Category::Obstacle) {
        sf::Vector2f direction = -getDirection(getVelocity());
        auto         newPos    = getPosition() + direction * (GRID_SIZE.x / 2);
        setState(new ObstacleCollidingState(this, newPos));
    }

    if (other->getCategory() == Category::Enemy && !isInvincible) {
        auto enemy = dynamic_cast<Entity *>(other);
        if (enemy) {
            sf::Vector2f direction =
                (enemy->getAbsPosition() - getAbsPosition());
            if (abs(direction.x) > abs(direction.y)) {
                direction.y = 0;
            } else {
                direction.x = 0;
            }
            direction           = -getDirection(direction);
            sf::Vector2f newPos = getPosition() + direction * (GRID_SIZE.x / 2);
            setState(new CollidingState(this, newPos));
        }
    }
}

Category::Type Player::getCategory() const { return Category::Player; }

void Player::setState(PlayerState *newState) {
    delete state;
    state = newState;
}

sf::FloatRect Player::getLocalBounds() const { return localBounds; }

sf::Vector2f Player::getNearestGridPosition(sf::Vector2f pos) const {
    auto absPos  = getAbsTransform() * pos;
    auto gridPos = getAbsTransform().getInverse() * absPos;
    gridPos.x    = std::round(gridPos.x / GRID_SIZE.x) * GRID_SIZE.x;
    gridPos.y    = std::round(gridPos.y / GRID_SIZE.y) * GRID_SIZE.y;
    return gridPos;
}

void Player::drawCurrent(sf::RenderTarget &target,
                         sf::RenderStates  states) const {
    auto sprite = animation.toSprite();
    sprite.setPosition(0, 0);
    if (onSizeBoost) {
        sprite.setPosition(10, 10);
    }
    sprite.setScale(GRID_SIZE.x / sprite.getLocalBounds().width,
                    GRID_SIZE.y / sprite.getLocalBounds().height);
    sprite.setOrigin(sprite.getLocalBounds().width / 2,
                     sprite.getLocalBounds().height / 2);
    auto scale = sprite.getScale();
    if (onSizeBoost) {
        sprite.setScale(scale.x * 0.75, scale.y * 0.75);
    }
    sprite.setOrigin(0, 0);
    target.draw(sprite, states);

    drawHealthBar(target, states);
}

void Player::drawHealthBar(sf::RenderTarget &target,
                           sf::RenderStates  states) const {
    sf::RectangleShape healthBar;
    healthBar.setSize(sf::Vector2f(GRID_SIZE.x, 10));
    healthBar.setFillColor(sf::Color::Red);
    healthBar.setPosition(0, -20);
    target.draw(healthBar, states);
    sf::RectangleShape healthBar2;
    healthBar2.setSize(sf::Vector2f((health / 100) * GRID_SIZE.x, 10));
    healthBar2.setFillColor(sf::Color::Green);
    healthBar2.setPosition(0, -20);
    target.draw(healthBar2, states);
}

void Player::takeSmallSizeBoost() { onSizeBoost = true; }
void Player::takeSpeedBoost() { onSpeedBoost = true; }

void Player::takeFood() {
    health += 20;
    if (health > 100) {
        health = 100;
    }
}

void Player::takeDamage(int damage) {
    if (isInvincible) {
        return;
    }
    health -= damage;
    if (health <= 0) {
        health = 0;
        setState(new DeadState(this));
    }
}
