#include "../../include/SDL2/SDL.h"
#include <stdio.h>
#include "windows.h"

#define WIDTH 853
#define HEIGHT 480

struct point
{
	int x, y, z;
	int px, py;
};

struct triangle
{
	struct point point1, point2, point3;
};

/*
struct cube
{
	int width;
	int x, y, z;
	struct triangle triangles[12];
};

void setCube(struct cube cube)
{
	cube.triangles[0] = {{cube.x, cube.y, cube.z}, {cube.x + cube.width, cube.y, cube.z}, {cube.x + cube.width, cube.y + cube.width, cube.z}};
	cube.triangles[1] = {{cube.x, cube.y, cube.z}, {cube.x, cube.y + cube.width, cube.z}, {cube.x + cube.width, cube.y + cube.width, cube.z}};
	cube.triangles[2] = {{cube.x, cube.y, cube.z}, {cube.x, cube.y, cube.z + cube.width}, {cube.x + cube.width, cube.y, cube.z + cube.width}};
	cube.triangles[3] = {{cube.x, cube.y, cube.z}, {cube.x + cube.width, cube.y, cube.z}, {cube.x + cube.width, cube.y, cube.z + cube.width}};
	cube.triangles[4] = {{cube.x, cube.y, cube.z}, {cube.x, cube.y, cube.z + cube.width}, {cube.x, cube.y + cube.width, cube.z + cube.width}};
	cube.triangles[5] = {{cube.x, cube.y, cube.z}, {cube.x, cube.y + cube.width, cube.z}, {cube.x, cube.y + cube.width, cube.z + cube.width}};
	cube.triangles[6] = {{cube.x + cube.width, cube.y + cube.width, cube.z + cube.width}, {cube.x, cube.y + cube.width, cube.z + cube.width}, {cube.x, cube.y, cube.z + cube.width}};
	cube.triangles[7] = {{cube.x + cube.width, cube.y + cube.width, cube.z + cube.width}, {cube.x + cube.width, cube.y, cube.z + cube.width}, {cube.x, cube.y, cube.z + cube.width}};
	cube.triangles[8] = {{cube.x + cube.width, cube.y + cube.width, cube.z + cube.width}, {cube.x + cube.width, cube.y, cube.z}, {cube.x, cube.y, cube.z + cube.width}};
}

void renderCube(SDL_Renderer* renderer, struct cube cube)
{
	for (int i = 0; i < 12; i++)
	{
		renderTriangle(renderer, cube.triangles[i]);
	}
}
*/

void renderPoint(struct point* point)
{
	(*point).px = (((*point).x * WIDTH/2) / (*point).z) + WIDTH/2;
	(*point).py = (((*point).y * HEIGHT/2) / (*point).z) + HEIGHT/2;
}

void renderTriangle(SDL_Renderer* renderer, struct triangle triangle)
{
	renderPoint(&(triangle.point1));
	renderPoint(&(triangle.point2));
	renderPoint(&(triangle.point3));
	printf("%d\n", triangle.point1.px);
	SDL_RenderDrawLine(renderer,
		triangle.point1.px,
		triangle.point1.py,
		triangle.point2.px,
		triangle.point2.py);
	SDL_RenderDrawLine(renderer,
		triangle.point2.px,
		triangle.point2.py,
		triangle.point3.px,
		triangle.point3.py);
	SDL_RenderDrawLine(renderer,
		triangle.point3.px,
		triangle.point3.py,
		triangle.point1.px,
		triangle.point1.py);
}

void render(SDL_Renderer* renderer, struct triangle triangle)
{
    SDL_SetRenderDrawColor(renderer, 0xff, 0, 0xff, 0xff);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
	renderTriangle(renderer, triangle);
    SDL_RenderPresent(renderer);
}

int processEvents(SDL_Window* window, char* keys)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
	{
        switch (event.type)
		{
            case SDL_WINDOWEVENT_CLOSE:
            {
                if (window)
				{
                    SDL_DestroyWindow(window);
                    window = NULL;
                }
            }
            break;
            case SDL_KEYDOWN:
            {
                switch (event.key.keysym.sym)
				{
                    case SDLK_ESCAPE:
                        return 0;
                        break;
					case SDLK_w:
						*keys = *keys | 0x80;
						break;
					case SDLK_s:
						*keys = *keys | 0x40;
						break;
                }
            }
            break;
            case SDL_QUIT:
            {
                return 0;
            }
            break;
        }
    }
    return 1;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    SDL_Window *window;
    SDL_Renderer *renderer;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("3D Grapher", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 853, 480, 0);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    int running = 1;

	struct point point1 = {100, 100, 1000, 0, 0};
	struct point point2 = {128, 128, 1000, 0, 0};
	struct point point3 = {100, 128, 1000, 0, 0};
	struct triangle triangle = {point1, point2, point3};
	char keys = 0;
	int camerax = 0;
	int cameray = 0;

    while (running)
	{
        running = processEvents(window, &keys);
        render(renderer, triangle);
		if (keys & 0x80) printf("w");
		keys = 0;
        SDL_Delay(100);
    }
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    SDL_Quit();
    
    return 0;
}
