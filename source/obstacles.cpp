#include "obstacles.hpp"
#include <cstdlib>
#include <ctime>

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
    size = spriteManager.getSize(currentSprite);
}

ObstacleLargeCactus::ObstacleLargeCactus(SpriteManager& spriteManager)
    : Obstacle(spriteManager)
{
    currentSprite = SpriteType::LargeCactus;
    position.y += 2.f; // adding 2 pixels offset to level the cactus with the ground
    size = spriteManager.getSize(currentSprite);
}

ObstacleBird::ObstacleBird(SpriteManager& spriteManager)
    : Obstacle(spriteManager)
{
    currentSprite = SpriteType::BirdAnimation1;
    size = spriteManager.getSize(currentSprite);
}

void ObstacleBird::update(GameState& gameState)
{
    Obstacle::update(gameState);

    currentSprite = gameState.getCurrentFrameNumber() > gameState.getTotalNumberOfFrames() / 2
        ? SpriteType::BirdAnimation1
        : SpriteType::BirdAnimation2;
}

ObstacleManager::ObstacleManager(SpriteManager& spriteManager) : spriteManager(spriteManager)
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
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
    const int spawnIntervalMs = 5000;
    const float despawnXPosition = -200.f;

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

    if (elapsedTime - timeElapsedSinceLastObstacleMs > spawnIntervalMs)
    {
        generateRandomObstacle();
        timeElapsedSinceLastObstacleMs = elapsedTime;
    }

    if (obstacles.empty())
        return;

    for (auto& obstacle : obstacles)
        obstacle->update(gameState);

    while (!obstacles.empty() && obstacles.front()->getPosition().x < despawnXPosition)
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
        if (obstacle->getBoundingBox().intersects(boundingBox))
            return true;
    }

    return false;
}
