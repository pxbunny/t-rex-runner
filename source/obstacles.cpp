#include "obstacles.hpp"

void Obstacle::update(GameState& gameState)
{
    const float obstacleSpeed = 8.f;

    if (gameState.getState() != GameState::State::Running)
        return;

    position.x -= obstacleSpeed;
}

void Obstacle::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    spriteManager.setSprite(currentSprite, position);
    target.draw(spriteManager.getSprite());
}

ObstacleSmallCactus::ObstacleSmallCactus(SpriteManager& spriteManager)
    : Obstacle(spriteManager)
{
    currentSprite = SpriteType::SmallCactus;
}

ObstacleLargeCactus::ObstacleLargeCactus(SpriteManager& spriteManager)
    : Obstacle(spriteManager)
{
    currentSprite = SpriteType::LargeCactus;
    position.y += 2.f; // adding 2 pixels offset to level the cactus with the ground
}

ObstacleBird::ObstacleBird(SpriteManager& spriteManager)
    : Obstacle(spriteManager)
{
    currentSprite = SpriteType::BirdAnimation1;
}

void ObstacleBird::update(GameState& gameState)
{
    Obstacle::update(gameState);

    currentSprite = gameState.getCurrentFrameNumber() > gameState.getTotalNumberOfFrames() / 2
        ? SpriteType::BirdAnimation1
        : SpriteType::BirdAnimation2;
}

ObstacleManager::ObstacleManager(SpriteManager& spriteManager)
    : spriteManager(spriteManager)
{
    smallCactusSize = spriteManager.getSize(SpriteType::SmallCactus);
    largeCactusSize = spriteManager.getSize(SpriteType::LargeCactus);
    birdSize = spriteManager.getSize(SpriteType::BirdAnimation1);
}

ObstacleManager::~ObstacleManager()
{
    removeAllObstacles();
}

void ObstacleManager::generateRandomObstacle()
{
    switch (rand() % 3)
    {
    case 0:
        obstacles.push_back(std::make_unique<ObstacleSmallCactus>(spriteManager));
        break;
    case 1:
        obstacles.push_back(std::make_unique<ObstacleLargeCactus>(spriteManager));
        break;
    case 2:
        obstacles.push_back(std::make_unique<ObstacleBird>(spriteManager));
        break;
    }
}

void ObstacleManager::popObstacle()
{
    obstacles.pop_front();
}

void ObstacleManager::removeAllObstacles()
{
    obstacles.clear();
}

void ObstacleManager::updateObstacles(GameState& gameState)
{
    auto state = gameState.getState();

    if (state == GameState::State::Start)
    {
        removeAllObstacles();
        timeElapsedSinceLastObstacleMs = gameState.getInGameTimeMs();
        return;
    }

    if (state != GameState::State::Running)
        return;

    auto elapsedTime = gameState.getInGameTimeMs();

    if (elapsedTime - timeElapsedSinceLastObstacleMs > 5000)
    {
        generateRandomObstacle();
        timeElapsedSinceLastObstacleMs = elapsedTime;
    }

    if (obstacles.empty())
        return;

    for (auto& obstacle : obstacles)
        obstacle->update(gameState);

    while (!obstacles.empty() && obstacles.front()->getPosition().x < -200.f)
        popObstacle();
}

void ObstacleManager::drawObstacles(sf::RenderTarget& target)
{
    for (auto& obstacle : obstacles)
        target.draw(*obstacle);
}

bool ObstacleManager::isColliding(sf::FloatRect boundingBox)
{
    if (obstacles.empty())
        return false;

    for (const auto& obstacle : obstacles)
    {
        auto position = obstacle->getPosition();
        auto size = sf::Vector2f();

        if (typeid(*obstacle) == typeid(ObstacleBird))
            size = birdSize;
        else if (typeid(*obstacle) == typeid(ObstacleSmallCactus))
            size = smallCactusSize;
        else if (typeid(*obstacle) == typeid(ObstacleLargeCactus))
            size = largeCactusSize;
        else
            continue;

        sf::FloatRect obstacleBoundingBox(position.x, position.y - size.y, size.x, size.y);
        if (obstacleBoundingBox.intersects(boundingBox))
            return true;
    }

    return false;
}
