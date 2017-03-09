/*
 This software is provided 'as-is', without any express or implied
 warranty.  In no event will the authors be held liable for any damages
 arising from the use of this software.
 
 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:
 
 1. The origin of this software must not be misrepresented; you must not
 claim that you wrote the original software. If you use this software
 in a product, an acknowledgment in the product documentation would be
 appreciated but is not required.
 2. Altered source versions must be plainly marked as such, and must not be
 misrepresented as being the original software.
 3. This notice may not be removed or altered from any source distribution.
 */


#include <SDL2/SDL.h>
#include <stdbool.h>

unsigned int *raster = NULL;
int screen_width = 256*2;
int screen_height = 144*2;
int width = 144*4;
int height = 256*4;
int s_width = 256;
int s_height = 144;
bool quit = false;

static void setup_data(void);
static void handle_key_down(SDL_Keysym* keysym);
static void check_sdl_events(SDL_Event event);
static void make_current_context(void);

static void setup_data(void) {
    raster = (unsigned int *) malloc((s_width*s_height) * sizeof(unsigned int));
    for(int r = 0; r < s_width*s_height; r++) {
        raster[r] = 0;
    }
}

static void handle_key_down(SDL_Keysym* keysym) {
    switch( keysym->sym ) {
        case SDLK_ESCAPE:
            quit = true;
            break;
        default:
            break;
    }
}

static void check_sdl_events(SDL_Event event) {
    while (SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
                handle_key_down(&event.key.keysym);
                break;
        }
    }
}

static SDL_Window *window = NULL;
static SDL_Texture *texture = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_GLContext context;
static SDL_Event event;

int main(int argc, char **argv) {
    
    // init SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("ERROR SDL_Init");
        return -1;
    }
    
    // create a window
    window = SDL_CreateWindow("software raster SDL2",           // window title
                            SDL_WINDOWPOS_CENTERED,     // x position, centered
                            SDL_WINDOWPOS_CENTERED,     // y position, centered
                            screen_width,               // width, in pixels
                            screen_height,              // height, in pixels
                            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN          // flags
                            );
    
    
    if (window != NULL) {
        context = SDL_GL_CreateContext(window);
        if(context == NULL) {
            printf("\nFailed to create context: %s\n", SDL_GetError());
        }
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
        if (renderer != NULL) {
            texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, s_width, s_height);
            SDL_GL_SetSwapInterval(1);
            setup_data();
            
            if (texture != NULL) {
                while (!quit) {
                    check_sdl_events(event);
                    for (int r_x = 0; r_x < s_width; r_x++) {
                        for (int r_y = 0; r_y < s_height; r_y++) {
                            raster[r_x+r_y*s_width] = rand()*0.000001;
                        }
                    }
                    SDL_UpdateTexture(texture, NULL, raster, s_width * sizeof (Uint32));
                    SDL_RenderClear(renderer);
                    SDL_RenderCopy(renderer, texture, NULL, NULL);
                    SDL_RenderPresent(renderer);
                    SDL_Delay(16);
                }
            }
        }
        // clean up
        free(raster);
        SDL_DestroyTexture(texture);
        SDL_GL_DeleteContext(context);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
    return 0;
}
