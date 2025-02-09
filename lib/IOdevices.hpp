#include <stdint.h>
#include <iostream>
#include <unordered_map>
#include <string>

#include <SDL2/SDL.h>

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

        ~Display();
        
        void Init(); // Function to init all main display
        void Print(); // Main function to print pixels in screen
        void Clear(); // Clean in frambuffer and in screen

};

struct Sounder
{
    private:
        SDL_AudioSpec specs;

        const int frenquency = 60;
        const unsigned format = AUDIO_S8;
        const unsigned channels = SDL_GL_STEREO;
        const int samples = 512;

    public:
        Sounder();
        ~Sounder();

        void Play();
        void Stop();
};

class Keyboard
{
    private:
        bool* ON_OFFptr = nullptr;
        bool* PAUSEptr = nullptr;

        std::unordered_map<unsigned, uint8_t> keys;
    
    public:
        Keyboard(bool* vON_OFFptr, bool* vPAUSEptr);
        ~Keyboard();
        void HandleEvent();
};

class IODevices
{
    private:
        Display* disp;
        Sounder* sound;
        Keyboard* keyb;

    public:
        ~IODevices();
        void Initialize(bool* vON_OFFptr, bool* vPAUSEptr);
        void StartAll(bool Print, bool Sound);
};