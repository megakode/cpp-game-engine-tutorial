//
// MegaTinyEngine v0.1
//
// Simple example of a tiny SDL based C++ game engine.
//
// Created by Peter Bone on 22/09/2022.
//

#include <cassert>
#include <iostream>
#include <map>

#include "MegaTinyEngine.h"
#include "SDL_image.h"

namespace MegaTinyEngine
{

std::unique_ptr<Core> Core::create(int pixelWidth, int pixelHeight, int scaling, bool resizable, const std::string &title)
{
    std::unique_ptr<Core> core = std::unique_ptr<Core>(new Core());
    // Init SDL

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    // Create Window

    Uint32 flags = SDL_WINDOW_SHOWN;

    if (resizable)
    {
        flags |= SDL_WINDOW_RESIZABLE;
    }

    core->m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, pixelWidth * scaling, pixelHeight * scaling, flags);
    if (core->m_window == nullptr)
    {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return nullptr;
    }

    // Init SDL_Image library, so we can load images

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        std::cerr << "Could not init PNG image library";
        return nullptr;
    }

    // Create renderer

    core->m_renderer = SDL_CreateRenderer(core->m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_RenderSetLogicalSize(core->m_renderer, pixelWidth, pixelHeight);
    SDL_RenderSetIntegerScale(core->m_renderer, SDL_TRUE);

    if (core->m_renderer == nullptr)
    {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    return core;
}

Core::~Core()
{
    std::cout << "Core destroyed" << std::endl;

    if (m_renderer != nullptr)
    {
        SDL_DestroyRenderer(m_renderer);
    }
    if (m_window != nullptr)
    {
        SDL_DestroyWindow(m_window);
    }

    SDL_Quit();
}

int Core::runGame(IGame &game)
{
    assert(m_renderer); // Please run init() first

    bool quit = false;
    static Uint32 lastTime, currentTime;
    SDL_Event e;

    game.gameInitialize(*this);

    lastTime = currentTime = SDL_GetTicks();

    while (!quit)
    {

        // Let the game do additional drawing
        game.gameDraw(*this);

        // Update the screen
        SDL_RenderPresent(m_renderer);

        currentTime = SDL_GetTicks();

        float frameTime = (float)(currentTime - lastTime) / 1000;

        // Update Game

        game.gameUpdate(*this, frameTime);

        lastTime = currentTime;

        // Get all the SDL events that have occurred since last we checked (windows, keyboard, mouse, etc.)

        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
                if (e.key.repeat == 0)
                    game.gameHandleInput(handleKeyMapping(e.key.keysym.sym), true);
                break;
            case SDL_KEYUP:
                if (e.key.repeat == 0)
                    game.gameHandleInput(handleKeyMapping(e.key.keysym.sym), false);
                break;
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
            case SDL_MOUSEMOTION:
                break;
            }
        }
    }

    game.gameDestroy();

    return 0;
}

Key Core::handleKeyMapping(SDL_Keycode keyCode)
{
    // Create a static mapping here to begin with.
    // Could easily be extended to be run-time configurable.

    static std::map<SDL_Keycode, Key> keyMapping = {
        {SDLK_LEFT, Key::Left},
        {SDLK_RIGHT, Key::Right},
        {SDLK_UP, Key::Up},
        {SDLK_DOWN, Key::Down},
        {SDLK_z, Key::Action1},
        {SDLK_x, Key::Action2},

    };

    // Try to find a key mapping from SDL to our own 'Key' type.
    // If there is no mapping, return Key::Unknown

    auto it = keyMapping.find(keyCode);
    if (it != keyMapping.end())
        return it->second;
    else
        return Key::Unknown;
}

Size Core::getWindowSize()
{
    if (m_renderer != nullptr)
    {
        SDL_Rect size;
        SDL_RenderGetLogicalSize(m_renderer, &size.w, &size.h);
        return {.width = size.w, .height = size.h};
    }
    else
    {
        return {.width = 0, .height = 0};
    }
}

void Core::clearScreen(const Color &color)
{
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(m_renderer);
}

void Core::drawRect(int x, int y, int w, int h, Color color)
{
    SDL_Rect rect = {.x = x, .y = y, .w = w, .h = h};
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(m_renderer, &rect);
}
} // namespace MegaTinyEngine
