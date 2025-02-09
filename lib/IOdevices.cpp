#include "IOdevices.hpp"

Display::~Display()
{
    
}

void Display::Init()
{
    std::cout << "Initializing display device" << "\n";
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
        return;

    window = SDL_CreateWindow(title, 0, 0, win_w * aspect_ratio, win_h * aspect_ratio, window_flags);
    if (window == NULL)
        return;

    renderer = SDL_CreateRenderer(window, 0, renderer_flags);
    if (renderer == NULL)
        return;
    std::cout << "Display initialize successfully" << "\n";
}

void Display::Print(){}
void Display::Clear(){}

Sounder::Sounder(){}
void Sounder::Play(){}
void Sounder::Stop(){}

Keyboard::Keyboard(bool* vON_OFFptr, bool* vPAUSEptr)
{
    ON_OFFptr = vON_OFFptr;
    PAUSEptr = vPAUSEptr;

    keys.insert({SDLK_0, 0x0});
    keys.insert({SDLK_1, 0x1});
    keys.insert({SDLK_2, 0x2});
    keys.insert({SDLK_3, 0x3});
    keys.insert({SDLK_4, 0x4});
    keys.insert({SDLK_5, 0x5});
    keys.insert({SDLK_6, 0x6});
    keys.insert({SDLK_7, 0x7});
    keys.insert({SDLK_8, 0x8});
    keys.insert({SDLK_9, 0x9});
    keys.insert({SDLK_a, 0xA});
    keys.insert({SDLK_b, 0xB});
    keys.insert({SDLK_c, 0xC});
    keys.insert({SDLK_d, 0xD});
    keys.insert({SDLK_e, 0xE});
    keys.insert({SDLK_f, 0xF});
}
void Keyboard::HandleEvent()
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {            
            case SDL_QUIT:
                (bool&)ON_OFFptr = true;
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE: (bool&)ON_OFFptr = true; break;
                    case SDLK_SPACE: (bool&)PAUSEptr = true; break;

                    default:
                        if (event.key.keysym.sym == SDLK_SPACE)
                            std::cout << "GAME PAUSED" << "\n";

                        if (keys.find(event.key.keysym.sym) != keys.end())
                            keys[event.key.keysym.sym] = true;

                    break;
                }
            default:
                break;
        }
    }
}

void IODevices::Initialize(bool* vON_OFFptr, bool* vPAUSEptr)
{
    std::cout << "Intinializing display controller" << "\n";
    disp = new Display();
    disp->Init();

    std::cout << "Intinializing sound controller" << "\n";
    sound = new Sounder();

    std::cout << "Intinializing sound controller" << "\n";
    keyb = new Keyboard(vON_OFFptr, vPAUSEptr);
}
void IODevices::StartAll(bool Print, bool Sound)
{
    keyb->HandleEvent();

    if (Print)
        disp->Print();

    // if (Sound)
    //     sound->Play();
}