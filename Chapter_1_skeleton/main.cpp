#include "MegaTinyEngine.h"
#include <iostream>
#include <memory>

class MyGame : public MegaTinyEngine::IGame
{
  public:
    int x = 100, y = 100;
    bool walkLeft = false, walkRight = false, walkUp = false, walkDown = false;

    void gameInitialize(MegaTinyEngine::Core &core) override
    {
        std::cout << "Game initialized" << std::endl;
    }

    void gameUpdate(MegaTinyEngine::Core &core, float deltaTimeInSeconds) override
    {
        if (walkLeft)
            x -= 1;
        if (walkRight)
            x += 1;
        if (walkUp)
            y -= 1;
        if (walkDown)
            y += 1;
    }

    void gameDraw(MegaTinyEngine::Core &core) override
    {
        core.clearScreen({.r = 50, .g = 117, .b = 168, .a = 0});
        core.drawRect(x, y, 10, 10, {.r = 209, .g = 165, .b = 33, .a = 0});
    }

    void gameHandleInput(MegaTinyEngine::Key key, bool pressed) override
    {
        switch (key)
        {
        case MegaTinyEngine::Key::Left:
            walkLeft = pressed;
            break;
        case MegaTinyEngine::Key::Right:
            walkRight = pressed;
            break;
        case MegaTinyEngine::Key::Up:
            walkUp = pressed;
            break;
        case MegaTinyEngine::Key::Down:
            walkDown = pressed;
            break;
        default:
            break;
        }
    }

    void gameDestroy() override
    {
        std::cout << "Game destroyed" << std::endl;
    }
};

int main()
{
    auto core = MegaTinyEngine::Core::create(320, 200, 3, false, "Mega Tiny Game v1.0");
    MyGame game;

    if (core == nullptr)
    {
        std::cerr << "Could not initialize core and create window.";
    }
    else
    {
        core->runGame(game);
    }

    return 0;
}
