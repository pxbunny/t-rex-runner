#include <string>

#include "hud.hpp"

std::string getFullScoreString(int score, int highestScore)
{
    auto scoreString = std::to_string(score);
    auto highestScoreString = std::to_string(highestScore);

    auto scoreLength = scoreString.length();
    auto highestScoreLength = highestScoreString.length();

    auto scorePadding = std::string(5 - scoreLength, '0');
    auto highestScorePadding = std::string(5 - highestScoreLength, '0');

    return "HI " + highestScorePadding + highestScoreString + " " + scorePadding + scoreString;
}

void ScoreDisplay::configure(Config& config)
{
    text.setFillColor(sf::Color::Black);
    text.setCharacterSize(14);

    text.setString(getFullScoreString(score, highestScore));
    auto bounds = text.getLocalBounds();
    text.setOrigin(bounds.left + bounds.width, bounds.top);

    auto windowWidth = config.WindowWidth;
    auto windowPadding = config.WindowPadding;
    text.setPosition(windowWidth - windowPadding, windowPadding);
}

void ScoreDisplay::update(GameState &gameState)
{
    auto scoreString = getFullScoreString(gameState.getScore(), gameState.getHighestScore());   
    text.setString(scoreString);
    auto bounds = text.getLocalBounds();
    text.setOrigin(bounds.left + bounds.width, bounds.top);
}

void ScoreDisplay::setFont(sf::Font& font)
{
    text.setFont(font);
}

void ScoreDisplay::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(text);
}

void HUD::configure(Config& config)
{
    score.configure(config);
}

void HUD::update(GameState &gameState)
{
    score.update(gameState);
}

void HUD::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(score);
}
