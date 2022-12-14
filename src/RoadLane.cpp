#include "RoadLane.h"
#include "Animal.h"
#include "Vehicle.h"
#include <iostream>

RoadLane::RoadLane()
        : speedX(0), height(0),
          frequency(), laneTexture(), commuterTexture(), commuterSize(0, 0), direction(Direction::Left) {
    setPosition(0, 0);
}

RoadLane::RoadLane(Texture::ID commuterTexture, Texture::ID laneTexture, float y, float speed,
                   Random<std::normal_distribution<double>> frequency) : laneTexture(laneTexture), frequency(frequency),
                                                                         speedX(-speed), height(DEF_LANE_HEIGHT),
                                                                         commuterTexture(commuterTexture),
                                                                         commuterSize(DEF_COMMUTER_SIZE),
                                                                         direction(Direction::Left) {
    setPosition(0, y);
}

RoadLane::RoadLane(Texture::ID commuterTexture,
                   Texture::ID laneTexture, float y, float laneHeight,
                   float commuterWidth, float commuterHeight, RoadLane::Direction direction,
                   float speed, Random<std::normal_distribution<double>> frequency)
        : laneTexture(laneTexture),
          frequency(frequency), height(laneHeight), commuterTexture(commuterTexture),
          commuterSize(commuterWidth, commuterHeight), direction(direction), speedX(speed) {
    setPosition(0, y);
}

void RoadLane::updateCommuters(sf::Time dt) {
    // TODO: doesnt rly work
    auto isLastCommuterFarEnough = [&]() {
        if (!commuters.empty()) {
            auto startX = getDirection() == Direction::Right ? 0 : WINDOW_VIDEO_MODE.width;
            auto &last = commuters.back();
            return std::abs((float) startX - last->getPosition().x) >= MINIMUM_WIDTH_BETWEEN_VEHICLES;
        } else
            return true;
    };

    timer -= dt;
    if (timer <= sf::Time::Zero && isLastCommuterFarEnough()) {
        // Add a new vehicle
        std::cout << "New commuter" << std::endl;
        auto commuter = newCommuter();
        commuters.push_back(commuter.get());
        attachChild(std::move(commuter));
        timer = sf::seconds(frequency.get<float>());
    }

    // Remove vehicles out of screen
    while (!commuters.empty() && commuters.front()->isOutOfScreen()) {
        std::cout << "Remove commuter" << std::endl;
        detachChild(*commuters.front());
        commuters.pop_front();
    }
}

void RoadLane::updateCurrent(sf::Time dt) {
    updateCommuters(dt);
}

void RoadLane::setLaneTexture(Texture::ID texture) {
    laneTexture = texture;
}

void RoadLane::setCommuterTexture(Texture::ID id) {
    commuterTexture = id;
}

void RoadLane::setCommuterSize(sf::Vector2f size) {
    commuterSize = size;
}

void RoadLane::setDirection(RoadLane::Direction newDirection) {
    direction = newDirection;
}

void RoadLane::setSpeed(float speed) {
    speedX = speed;
}

void RoadLane::setFrequency(const Random<std::normal_distribution<double>> &newFrequency) {
    frequency = newFrequency;
}

void RoadLane::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
    auto sprite = TextureHolder::instance().getSheet(laneTexture).getSprite(0);
    // Set scale to match the height of the road
    auto scaleFactor = height / (float) sprite.getGlobalBounds().height;
    sprite.setScale(sprite.getScale().x * scaleFactor, sprite.getScale().y * scaleFactor);

    for (float i = 0; i < (float) WINDOW_VIDEO_MODE.width; i += sprite.getGlobalBounds().width) {
        sprite.setPosition(i, 0);
        target.draw(sprite, states);
    }
}

void RoadLane::setPosY(float y) {
    setPosition(0, y);
}

void RoadLane::setLaneHeight(float newHeight) {
    height = newHeight;
}

RoadLane::Type RoadLane::getType() const {
    return RoadLane::Type::Unknown;
}

RoadLane::Direction RoadLane::getDirection() const {
    return direction;
}

sf::Vector2f RoadLane::getVelocity() const {
    if (direction == Direction::Right)
        return {speedX, 0};
    else
        return {-speedX, 0};
}

RoadLane::Type VehicleLane::getType() const {
    return RoadLane::Type::Vehicle;
}

void VehicleLane::onLightChanged() {
    stopSpawning = !stopSpawning;
    for (auto &commuter: commuters)
        dynamic_cast<Vehicle *>(commuter)->onLightChanged();
}

std::unique_ptr<Entity> VehicleLane::newCommuter() const {
    auto pos =
            getDirection() == Direction::Right
            ? sf::Vector2f(-commuterSize.x + 1, 0)
            : sf::Vector2f((float) WINDOW_VIDEO_MODE.width - 1, 0);
    auto vehicle = std::make_unique<Vehicle>(commuterTexture, pos, commuterSize, getVelocity());
    vehicle->adjustBounds(0, 0, 0, 40);
    vehicle->adjustSpriteBounds(0, -30);
    return vehicle;
}

void VehicleLane::updateCommuters(sf::Time dt) {
    if (!stopSpawning)
        RoadLane::updateCommuters(dt);
}

RoadLane::Type AnimalLane::getType() const {
    return RoadLane::Type::Animal;
}

std::unique_ptr<Entity> AnimalLane::newCommuter() const {
    auto pos =
            getDirection() == Direction::Right
            ? sf::Vector2f(-commuterSize.x + 1, 0)
            : sf::Vector2f((float) WINDOW_VIDEO_MODE.width - 1, 0);
    return std::make_unique<Animal>(commuterTexture, pos, commuterSize, getVelocity());
}
