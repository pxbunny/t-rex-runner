#pragma once

#include <SFML/Graphics.hpp>
#include <deque>
#include <memory>

#include "sprites.hpp"
#include "state.hpp"

class Obstacle : public sf::Drawable
{
public:
    Obstacle(SpriteManager& spriteManager) : spriteManager(spriteManager) {}

    virtual void update(GameState&);
    sf::Vector2f getPosition() const { return position; }

protected:
    virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

    sf::Vector2f position = { 600.f, 180.f };
    SpriteType currentSprite;

    SpriteManager& spriteManager;
};

class ObstacleSmallCactus : public Obstacle
{
public:
    ObstacleSmallCactus(SpriteManager&);
};

class ObstacleLargeCactus : public Obstacle
{
public:
    ObstacleLargeCactus(SpriteManager&);
};

class ObstacleBird : public Obstacle
{
public:
    ObstacleBird(SpriteManager&);

    void update(GameState&) override;
};

class ObstacleManager
{
public:
    ObstacleManager(SpriteManager&);
    ~ObstacleManager();

    void generateRandomObstacle();
    void popObstacle();
    void removeAllObstacles();

    void updateObstacles(GameState&);
    void drawObstacles(sf::RenderTarget&);

    bool isColliding(sf::FloatRect boundingBox);

private:
    int timeElapsedSinceLastObstacleMs = 0;

    sf::Vector2f smallCactusSize;
    sf::Vector2f largeCactusSize;
    sf::Vector2f birdSize;

    SpriteManager& spriteManager;

    std::deque<std::unique_ptr<Obstacle>> obstacles;
};
