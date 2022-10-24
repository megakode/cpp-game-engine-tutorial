//
// MegaTinyEngine v0.1
//
// Simple example of a tiny SDL based C++ game engine.
//
// Created by Peter Bone on 22/09/2022.
//

#pragma once
#include <SDL.h>
#include <memory>
#include <string>

namespace MegaTinyEngine
{

class Core;

enum class Key
{
    Left,
    Right,
    Up,
    Down,
    Action1,
    Action2,
    Action3,
    Unknown // For all keys that are not mapped specifically
};

struct Size
{
    int width;
    int height;
};

struct Color
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

/**
 * This is the interface that our tiny games using the engine must implement.
 */
class IGame
{

  public:
    /**
     * Called once when the game is initialized.
     * Use this to load resources, allocate memory, and set up starting conditions.
     */
    virtual void gameInitialize(Core &core) = 0;

    /**
     * Called once per frame.
     * Use this to update the state of all things in the game: Move player, check if anythings collides, change sprite
     * frames, etc.
     * @param deltaTimeInSeconds The amount of seconds elapsed since last frame. Typically a few milliseconds, e.g.
     * 0.001
     */
    virtual void gameUpdate(Core &core, float deltaTimeInSeconds) = 0;

    /**
     * Called once per frame.
     * Use this to draw everything on screen.
     * @param renderer The SDL renderer used to draw stuff. Pass this to methods like SDL_RenderDrawLine,
     * SDL_RenderDrawRect, etc.
     */
    virtual void gameDraw(Core &core) = 0;

    /**
     * Called every time we receive an event from SDL. It can be mouse move, a button press or window related events.
     * Notice that you can easily receive quite a lot of events in between each game draw/update.
     * If i.e. the mouse or game controller is moved quite of lot of events are generated.
     * So don't move the player each time a button press is received. Instead, set a flag of some sort, and do the
     * actual movement in your gameUpdate method. example
     * @param event
     */
    virtual void gameHandleInput(Key key, bool pressed) = 0;

    /**
     * Called once when the game is to be destroyed.
     * Deallocate and clean up stuff here.
     */
    virtual void gameDestroy() = 0;
};

/**
 * This is the main engine class, that wraps all the SDL window creation, drawing, event handling, and running of the
 * actual game.
 */
class Core
{

  public:
    ~Core();

    // Delete copy constructor and copy assignment
    Core(const Core &) = delete;
    Core &operator=(const Core &) = delete;

    /**
     * Initialize the engine and create a window.
     * @param pixelWidth The logical number of pixels wide
     * @param pixelHeight The logical number of pixels high
     * @param scaling The logical number of pixels can be scaled up to cover a larger physical area of the screen, i.e.
     * x2 or x3 to get a retro pixel look, that still takes up a larger area of the screen.
     * @param resizable Whether the window should be resizable.
     * @param title The window title showed in the title bar.
     * @return Whether everything went ok, and the window was actually created. If this value is false, you should not
     * proceed with calling runGame, but exit the program instead.
     */
    static std::unique_ptr<Core> create(int pixelWidth, int pixelHeight, int scaling, bool resizable,
                                        const std::string &title);

    /*
     * Start running a game. This function is blocking, and will not return until the window is closed.
     * This will call the following game functions:
     *
     * gameInitialize()
     * while(!quit){
     *   gameUpdate()
     *   gameDraw()
     *   gameHandleInput()
     * }
     * gameDestroy()
     *
     */
    int runGame(IGame &game);

    void drawRect(int x, int y, int w, int h, Color color);

    void clearScreen(const Color &color);

    Size getWindowSize();

  protected:
    Core() = default;

  private:
    SDL_Renderer *m_renderer;
    SDL_Window *m_window;
    Key handleKeyMapping(SDL_Keycode keyCode);
};

} // namespace MegaTinyEngine