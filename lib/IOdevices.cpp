#include "IOdevices.hpp"


Display::Display()
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
        return;

    window = SDL_CreateWindow(title, 0, 0, win_w, win_h, window_flags);
    if (window == NULL)
        return;

    renderer = SDL_CreateRenderer(window, 0, renderer_flags);
    if (renderer == NULL)
        return;
}

void Display::Print()
{

}

void Display::Clear()
{

}

Sounder::Sounder()
{

}

void Sounder::Play()
{

}
void Sounder::Stop()
{

}

IODevices::IODevices()
{
    
}

void IODevices::Initialize()
{
    std::cout << "Intinializing display controller" << "\n";
    disp = new Display();

    std::cout << "Intinializing sound controller" << "\n";
    sound = new Sounder();
}