#include <stdint.h>
#include <iostream>
#include <unordered_map>
#include <string>
#include <SDL2/SDL.h>
#include <bitset>

struct Display
{
    private:
        const int win_h = 32;           // amount of rows in a column
        const int win_w = 64;           // amount of columns in a row
        const int aspect_ratio = 20;    // aspect ration to apply to every thing

        const char* title = "CHIP-8";           // title of the screen (create after more dinamic title ex: "CHIP-8 - (name game)")
        const unsigned window_flags = (0x0);    // non flags to set to window object'
        const unsigned renderer_flags = (0x0);  // non flags to set to renderer object

        SDL_Window* window = nullptr;       // main window object
        SDL_Renderer* renderer = nullptr;   // main render of object

        const int pixel_w = 1;  // pixel length as width
        const int pixel_h = 1;  // pixel size as heigth
    
    public:

        bool* collision = nullptr;      // check if occuried any collision
        uint8_t FrameBuffer[64*32/8];   // pixels informations (just read from original framebuffer in memory)

        ~Display();
        
        void Init();                // Function to init all main display
        void Print();               // Main function to print pixels in screen
        void Clear(bool ClearFB);   // Clean in frambuffer and in screen
};

struct Sounder
{
    private:
        SDL_AudioSpec specs; // Audio specs information

        const int frenquency = 60;                  // Const for sound frequency
        const unsigned format = AUDIO_U8;           // Const for audio format to storage in memory (singed 0->16 bits)
        const unsigned channels = SDL_GL_STEREO;    // Const for channel (stereo | both sides)
        const int samples = 1;                      // Const for samples (1 byte buffer)

    public:
        Sounder(); // Constructor

        void Play(bool KeepOn); // Play the audio
        void Stop();            // Stop audio
};
class Keyboard
{
    private:
        bool* ON_OFFptr = nullptr;                  // On or off pointer to track if necessary turn off chip8
        bool* PAUSEptr = nullptr;                   // Pause pointer to track if necessary pause program execution
        bool* WAITKEY = nullptr;                    // Pauser pointer to wait for a key to be press
        std::unordered_map<unsigned, uint8_t> keys; // List of all keys in keyboard
    
    public:
        Keyboard(bool* vON_OFFptr, bool* vPAUSEptr, bool *vWAITKEYptr); // Constructor asking for respective pointer
        void HandleEvent();                                             // Check Events in program
        bool CheckPressedKey(uint8_t key);                              // Check if a specifc key was pressed
};