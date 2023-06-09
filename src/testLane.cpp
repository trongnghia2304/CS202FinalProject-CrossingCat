// #include "Enums.h"
// #include "RoadLane.h"
// #include "RoadLanes.h"
// #include "Random.h"
// #include "SceneNode.h"
// #include "TextureHolder.h"
// #include <SFML/Graphics.hpp>
// #include <functional>
// #include <memory>
// #include <random>
//
// int main() {
//    TextureHolder::instance().load(Texture::ID::LeftVehicle,"../resources/images/greenCar.png");
//    TextureHolder::instance().add(Texture::ID::RightVehicle, {Texture::ID::LeftVehicle, true, false});
//    TextureHolder::instance().load(Texture::ID::LeftAnimal,"../resources/images/greenCar.png");
//    TextureHolder::instance().add(Texture::ID::RightAnimal, {Texture::ID::LeftAnimal, true, false});
//    TextureHolder::instance().load(Texture::ID::Road,"../resources/images/VehicleRoad.png", {32, 32}, 9, 3);
//    TextureHolder::instance().add(Texture::ID::TopRoadLane, {Texture::ID::Road, 1, false, false});
//    TextureHolder::instance().add(Texture::ID::MidRoadLane, {Texture::ID::Road, 4, false, false});
//    TextureHolder::instance().add(Texture::ID::BotRoadLane, {Texture::ID::Road, 7, false, false});
//    TextureHolder::instance().add(Texture::ID::MonoRoadLane, {Texture::ID::Road, 8, false, false});
//
// //    std::function<Random<std::normal_distribution<double>>(double)>
// //            randomFunction = [](double i) {
// //        return Random(std::normal_distribution<double>(3.0, 1.0));
// //    };
//    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
//
//    SceneNode root;
//    auto laneController = std::make_unique<RoadLanes>(
//            RoadLane::Type::Vehicle, 3, 300,
//            Random(std::normal_distribution<double>(100, 10.0)),
//            [](double i) {
//                return Random(std::normal_distribution<double>(3.0, 1.0));
//            });
//    root.attachChild(std::move(laneController));
//    auto laneController2 = std::make_unique<RoadLanes>(
//            RoadLane::Type::Vehicle, 3, 100,
//            Random(std::normal_distribution<double>(100, 10.0)),
//            [](double i) {
//                return Random(std::normal_distribution<double>(3.0, 1.0));
//            });
//    root.attachChild(std::move(laneController2));
//
//    sf::Clock clock;
//    while (window.isOpen()) {
//        sf::Event event{};
//        while (window.pollEvent(event)) {
//            if (event.type == sf::Event::Closed)
//                window.close();
//        }
//        sf::Time dt = clock.restart();
//
//        window.clear(sf::Color::White);
//        root.update(dt);
//        window.draw(root);
//        window.display();
//    }
//
//    return 0;
// }
