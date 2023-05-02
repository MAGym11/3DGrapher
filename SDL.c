#include "../../include/SDL2/SDL.h"
#include <stdio.h>
#include "windows.h"
#include <math.h>

#define WIDTH 1280
#define HEIGHT 720

struct point
{
	int x, y, z;
	int px, py;
	double tz;
	int id;
};

struct triangle
{
	struct point point1, point2, point3;
};

struct camera
{
	int x, y, z;
	double xr, yr;
	double sx, cx, sy, cy;
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

void projectPoint(struct point* point, struct camera c)
{
	point->tz = ((point->x - c.x) * c.sy + (point->z - c.z) * c.cy) * c.cx + (-point->y - c.y) * -c.sx;
	if (point->tz == 0) point->tz++;
	point->px = (int)(((point->x - c.x) * c.cy + (point->z - c.z) * -c.sy) * WIDTH/2) / (point->tz) + WIDTH/2;
	point->py = (int)((((point->x - c.x) * c.sy + (point->z - c.z) * c.cy) * c.sx + (-point->y - c.y) * c.cx) * WIDTH/2) / (point->tz) + HEIGHT/2;
}

void clipLine(struct point* point1, struct point point2, struct camera camera)
{
	float s = (float)(point1->tz) / (float)(point1->tz - point2.tz);
	int x = point1->x;
	int y = point1->y;
	int z = point1->z;
	point1->x += s * (point2.x - point1->x);
	point1->y += s * (point2.y - point1->y);
	point1->z += s * (point2.z - point1->z);
	projectPoint(point1, camera);
	point1->x = x;
	point1->y = y;
	point1->z = z;
}

void renderLine(SDL_Renderer* renderer, struct point point1, struct point point2, struct camera camera)
{
	if (point1.tz < 0 && point2.tz < 0) return;
	if (point1.tz < 0)
	{
		clipLine(&point1, point2, camera);
	}
	if (point2.tz < 0)
	{
		clipLine(&point2, point1, camera);
	}
	SDL_RenderDrawLine(renderer, point1.px, point1.py, point2.px, point2.py);
}

void renderTriangle(SDL_Renderer* renderer, struct triangle triangle, struct camera camera)
{
	projectPoint(&(triangle.point1), camera);
	projectPoint(&(triangle.point2), camera);
	projectPoint(&(triangle.point3), camera);
	renderLine(renderer, triangle.point1, triangle.point2, camera);
	renderLine(renderer, triangle.point2, triangle.point3, camera);
	renderLine(renderer, triangle.point3, triangle.point1, camera);
}

void render(SDL_Renderer* renderer, struct triangle triangle, struct camera camera, struct point* axes)
{
    SDL_SetRenderDrawColor(renderer, 0xff, 0, 0xff, 0xff);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0x36, 0x39, 0x3e, 0xff);
    SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 0, 0xff, 0xff, 0xff);
	projectPoint(&(axes[0]), camera);
	projectPoint(&(axes[1]), camera);
	projectPoint(&(axes[2]), camera);
	projectPoint(&(axes[3]), camera);
	projectPoint(&(axes[4]), camera);
	projectPoint(&(axes[5]), camera);
	renderLine(renderer, axes[0], axes[1], camera);
	renderLine(renderer, axes[2], axes[3], camera);
	renderLine(renderer, axes[4], axes[5], camera);

	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
	renderTriangle(renderer, triangle, camera);
    SDL_RenderPresent(renderer);
}

int processEvents(SDL_Window* window, short* keys)
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
						*keys = *keys | 0x8000;
						break;
					case SDLK_s:
						*keys = *keys | 0x4000;
						break;
					case SDLK_a:
						*keys = *keys | 0x2000;
						break;
					case SDLK_d:
						*keys = *keys | 0x1000;
						break;
					case SDLK_SPACE:
						*keys = *keys | 0x0800;
						break;
					case SDLK_LSHIFT:
						*keys = *keys | 0x0400;
						break;
					case SDLK_RSHIFT:
						*keys = *keys | 0x0400;
						break;
					case SDLK_UP:
						*keys = *keys | 0x80;
						break;
					case SDLK_DOWN:
						*keys = *keys | 0x40;
						break;
					case SDLK_LEFT:
						*keys = *keys | 0x20;
						break;
					case SDLK_RIGHT:
						*keys = *keys | 0x10;
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

    window = SDL_CreateWindow("3D Grapher", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    int running = 1;
	
	struct point x  = {0xffff, 0, 1, 0, 0, 0.0, 0};
	struct point nx = {-0xffff, 0, 1, 0, 0, 0.0, 1};
	struct point y  = {0, 0xffff, 1, 0, 0, 0.0, 2};
	struct point ny = {0, -0xffff, 1, 0, 0, 0.0, 3};
	struct point z  = {0, 0, 0xffff, 0, 0, 0.0, 4};
	struct point nz = {0, 0, -0xffff, 0, 0, 0.0, 5};
	struct point axes[6] = {x, nx, y, ny, z, nz};

	struct point point1 = {100, 100, 1000, 0, 0, 0.0, 6};
	struct point point2 = {128, 128, 1000, 0, 0, 0.0, 7};
	struct point point3 = {100, 128, 1000, 0, 0, 0.0, 8};
	struct triangle triangle = {point1, point2, point3};
	short keys = 0;
	struct camera camera = {0, 0, 0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0};

    while (running)
	{
        running = processEvents(window, &keys);
        render(renderer, triangle, camera, axes);
		if (keys & 0x8000)
		{
			camera.z += 2 * camera.cy;
			camera.x += 2 * camera.sy;
		}
		if (keys & 0x4000)
		{
			camera.z -= 2 * camera.cy;
			camera.x -= 2 * camera.sy;
		}
		if (keys & 0x2000)
		{
			camera.x -= 2 * camera.cy;
			camera.z -= -2 * camera.sy;
		}
		if (keys & 0x1000)
		{
			camera.x += 2 * camera.cy;
			camera.z += -2 * camera.sy;
		}
		if (keys & 0x0800) camera.y -= 2;
		if (keys & 0x0400) camera.y += 2;
		if (keys & 0x0080)
		{
			camera.xr += 0.1;
			camera.sx = sin(camera.xr);
			camera.cx = cos(camera.xr);
		}
		if (keys & 0x0040)
		{
			camera.xr -= 0.1;
			camera.sx = sin(camera.xr);
			camera.cx = cos(camera.xr);
		}
		if (keys & 0x0020)
		{
			camera.yr -= 0.1;
			camera.sy = sin(camera.yr);
			camera.cy = cos(camera.yr);
		}
		if (keys & 0x0010)
		{
			camera.yr += 0.1;
			camera.sy = sin(camera.yr);
			camera.cy = cos(camera.yr);
		}
		keys = 0;
        SDL_Delay(16);
    }
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    SDL_Quit();
    
    return 0;
}
