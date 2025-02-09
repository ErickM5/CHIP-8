#include <SDL2/SDL.h>
#include <stdint.h>
#include <iostream>

#pragma comment(lib, "SDL2/SDL.h")

struct Display
{
    private:
        const int win_h = 32; // amount of rows in a column
        const int win_w = 64; // amount of columns in a row
        const int aspect_ratio = 20; // aspect ration to apply to every thing

        const char* title = "CHIP-8"; // title of the screen (create after more dinamic title ex: "CHIP-8 - (name game)")
        const unsigned window_flags = (0x0); // non flags to set to window object'
        const unsigned renderer_flags = (0x0); // non flags to set to renderer object

        SDL_Window* window = nullptr; // main window object
        SDL_Renderer* renderer = nullptr; // main render of object

    public:
        uint8_t FrameBuffer[32][64]; // pixels informations (just read from original framebuffer in memory)

        Display(); // ~constructor

        void Print(); // Main function to print pixels in screen
        void Clear(); // Clean in frambuffer and in screen

};

struct Sounder
{
    private:
        SDL_AudioSpec specs;

        const uint8_t frenquency = 60;
        const uint8_t format = AUDIO_S8;
        const uint8_t channels = SDL_GL_STEREO;
        const uint8_t samples = 512;

    public:
        Sounder();

        void Play();
        void Stop();
};

class Keyboard
{
    private:
        SDL_KeyboardEvent event;
        bool Keys[16];
    
    public:
        void HandleEvent();
};

class IODevices
{
    private:
        Display* disp;
        Sounder* sound;
        Keyboard* keyb;

    public:
        IODevices();

        void Initialize();
        void StartAll();
};