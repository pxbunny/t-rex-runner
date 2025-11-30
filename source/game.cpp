#include "game.hpp"

Game::Game(Config& config) : config(config)
{
    sf::VideoMode mode = {
        config.WindowWidth,
        config.WindowHeight,
        config.BitsPerPixel
    };

    auto style = sf::Style::Titlebar | sf::Style::Close;
    window.create(mode, config.WindowTitle, style);
    window.setFramerateLimit(config.FramerateLimit);
}

LoadingFilesResult Game::loadFiles(Config& config)
{
    if (!spriteManager.loadTextureFromFile())
        return { "Failed to load texture!" };

    if (!loadFontFromFile(config.FontPath))
        return { "Failed to load font!" };

    return {};
}

void Game::configureUI()
{
    gui.configure(config);
    hud.configure(config);
}

void Game::mainloop()
{
    while (window.isOpen())
    {
        handleEvents();

        control();
        update();
        draw();
    }
}

void Game::handleEvents()
{
    auto state = gameState.getState();

    for (auto event = sf::Event(); window.pollEvent(event);)
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
            return;
        }

        if (event.type == sf::Event::KeyPressed &&
            event.key.code == sf::Keyboard::Space &&
            !isReloadBlocked)
        {
            switch (state)
            {
            case GameState::State::Start:
                gameState.setState(GameState::State::Running);
                break;
            case GameState::State::Dead:
                gameState.setState(GameState::State::Start);
                break;
            default:
                break;
            }
        }
    }

    if (state == GameState::State::Running)
    {
        isReloadBlocked = true;
        return;
    }

    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        isReloadBlocked = false;
    }
}

void Game::control()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        dinosaur.jump();
    }

    // TODO: crouching
}

void Game::update()
{
    gameState.update();
    dinosaur.update(gameState);
    obstacleManager.updateObstacles(gameState);
    hud.update(gameState);

    if (obstacleManager.isColliding(dinosaur.getBoundingBox()))
        gameState.setState(GameState::State::Dead);
}

void Game::draw()
{
    window.clear(sf::Color(186, 167, 153));

    obstacleManager.drawObstacles(window);
    window.draw(dinosaur);
    window.draw(hud);
    window.draw(gui);

    window.display();
}

bool Game::loadFontFromFile(std::string path)
{
    auto result = font.loadFromFile(path);
    hud.setFont(font);
    gui.setFont(font);
    return result;
}
