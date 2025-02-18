#include "IOdevices.hpp"

Display::Display(uint8_t* CollPtr)
{
    collision = CollPtr;
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

void Display::Print()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    bool pixelOn;

    int x, y = 0;
    for (int i = 0; i < sizeof(FrameBuffer) * 8; i++)
    {
        y = i/32;
        x = i%64;
        pixelOn = (FrameBuffer[i/8]>>(i%8))>0;

        SDL_Rect rect = {x * aspect_ratio, y * aspect_ratio,aspect_ratio,aspect_ratio};
        if (pixelOn) {
            // std::cout << "■";
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }else{
            // std::cout << " ";
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        }
        SDL_RenderFillRect(renderer, &rect);
    }

    SDL_Delay(100);
    SDL_RenderPresent(renderer);
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
bool Keyboard::CheckPressedKey(uint8_t key)
{
    return keys[key];
}