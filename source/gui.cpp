#include "gui.hpp"

void GameOverScreen::configure(Config& config)
{
    text.setString("GAME OVER");
    text.setFillColor(sf::Color::Black);
    
    auto bounds = text.getLocalBounds();
    text.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    text.setPosition(config.WindowWidth / 2, config.WindowHeight / 2);
}

void GameOverScreen::setFont(sf::Font &font)
{
    text.setFont(font);
}

void GameOverScreen::draw(sf::RenderTarget& target, sf::RenderStates ) const
{
    target.draw(text);
}

void GUI::configure(Config& config)
{
    gameOverScreen.configure(config);
}

void GUI::setFont(sf::Font &font)
{
    gameOverScreen.setFont(font);
}

void GUI::draw(sf::RenderTarget& target, sf::RenderStates) const
{
    switch (gameState.getState())
    {
    case GameState::State::Dead:
        target.draw(gameOverScreen);
        break;
    default:
        break;
    }
}
