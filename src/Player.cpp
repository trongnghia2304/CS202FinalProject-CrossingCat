#include "Player.h"
#include "TextureHolder.h"
#include "Consts.h"

#define vUp {0, speed}
#define vDown {0, -speed}
#define vLeft {-speed, 0}
#define vRight {speed, 0}

void Player::drawCurrent(sf::RenderTarget &target, sf::RenderStates state) const {
    // Downcast Vehicle back to sf::Transformable then upcast to sf::Sprite to preserve properties i.e., position, scale, origin, rotation.
    sf::Transformable trans = *this;
    auto sprite = dynamic_cast<sf::Sprite &>(trans);

    sprite.setTexture(TextureHolder::instance().get(spriteStage), true);
    target.draw(sprite, state);
}

void Player::updateCurrent(sf::Time dt) {
    // TODO: time between sprites is currently a constant. does it need to be custom?
    spriteStage.next((int) (dt / TIME_BETWEEN_SPRITE));

    auto pos = getPosition();
    pos.x += getVelocity().x * (float) dt.asSeconds();
    pos.y += getVelocity().y * (float) dt.asSeconds();
    setPosition(pos);
}

Player::Player() : speed(0) {}

Player::Player(float speed, float x, float y, float w, float h) : speed(speed), Entity(Texture::ID::PlayerSprites,
                                                                                       {0, 0}, x, y, w, h) {}

void Player::onKeyPressed(sf::Event::KeyEvent event) {
    switch (event.code) {
        case sf::Keyboard::W:
            getVelocity() += vUp;
            break;
        case sf::Keyboard::S:
            getVelocity() += vDown;
            break;
        case sf::Keyboard::A:
            getVelocity() += vLeft;
            break;
        case sf::Keyboard::D:
            getVelocity() += vRight;
            break;
        default:
            break;
    }
}

void Player::onKeyReleased(sf::Event::KeyEvent event) {
    switch (event.code) {
        case sf::Keyboard::W:
            getVelocity() -= vUp;
            break;
        case sf::Keyboard::S:
            getVelocity() -= vDown;
            break;
        case sf::Keyboard::A:
            getVelocity() -= vLeft;
            break;
        case sf::Keyboard::D:
            getVelocity() -= vRight;
            break;
        default:
            break;
    }
}
