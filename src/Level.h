#pragma once
#include <SFML/System/Vector2.hpp>
#include <assert.h>
#include <random>
#include "SceneBuilder.h"
#include "SceneNode.h"
#include "Random.h"

const float BASE_MIN_SPAWN_RATE = 0.5f;
const float BASE_MAX_SPAWN_RATE = 1.5f;
const float BASE_SPAWN_RATE_DECREASE = 0.01;

const float BASE_MIN_SPEED = 70.f;
const float BASE_MAX_SPEED = 100.f;
const float BASE_SPEED_INCREASE = 20.f;
const int   MAX_LANE_COUNT = 8;
class Level : public SceneNode {
    Random<std::discrete_distribution<unsigned>> random;

    struct DifficultyMetrics {
        int level;
        float minSpeed;
        float maxSpeed;
        float minSpawnRate;
        float maxSpawnRate;
        vector<float> laneSpawnProb;
    public:
        DifficultyMetrics();
        DifficultyMetrics(int level);
        void increaseLevel();
        int getLevel() const { return level; }
        float getMinSpeed() const { return minSpeed; }
        float getMaxSpeed() const { return maxSpeed; }
        float getMinSpawnRate() const { return minSpawnRate; }
        float getMaxSpawnRate() const { return maxSpawnRate; }
    };

    SceneBuilder sceneBuilder;

public:
    Level(int level, sf::Vector2f sceneSize);
};
