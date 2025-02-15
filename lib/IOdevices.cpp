#include "IOdevices.hpp"

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

void Display::Print()
{
    std::cout << "Draw as requested! " << "\n";
    int x, y;
    x = y = 0;
    for (bool p: FrameBuffer)
    {
        std::cout << p;
        // if (p)
        // {
        //     SDL_Rect pixel = {x*aspect_ratio, y*aspect_ratio, pixel_w*aspect_ratio, pixel_h*aspect_ratio};
        //     SDL_RenderDrawRect(renderer, &pixel);
        //     SDL_RenderPresent(renderer);
        // }
        y += y = 7 ? (y * -1) : 1;
        x = y == 0 ? 1 : 0;
    }
}
void Display::Clear(bool ClearFB)
{
    SDL_RenderClear(renderer);
    if (ClearFB)
        for(auto& i : FrameBuffer){i=0x0;}
}
Display::~Display()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
}

Sounder::Sounder(){}
void Sounder::Play(bool KeepOn){}
void Sounder::Stop(){}

Keyboard::Keyboard(bool* vON_OFFptr, bool* vPAUSEptr, bool* vWAITKEYptr)
{
    ON_OFFptr = vON_OFFptr;
    PAUSEptr = vPAUSEptr;
    WAITKEY = vWAITKEYptr;

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
                *ON_OFFptr = false;
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    std::cout << event.key.keysym.sym << "\n";
                    // std::cout << *ON_OFFptr << "\n";
                    case SDLK_ESCAPE: 
                        *ON_OFFptr = false; 
                        std::cout << *ON_OFFptr << "\n";
                        break;
                    case SDLK_SPACE:
                        if (*WAITKEY) 
                        {
                            *WAITKEY=false;
                            break;
                        }
                        *PAUSEptr = *PAUSEptr == false;
                        std::cout << "GAME PAUSED" << "\n";
                        std::cout << *PAUSEptr << "\n";
                        break;

                    default:
                        if (keys.find(event.key.keysym.sym) != keys.end())
                            keys[event.key.keysym.sym] = true; *WAITKEY=true;

                    break;
                }
            default:
                break;
        }
    }
}