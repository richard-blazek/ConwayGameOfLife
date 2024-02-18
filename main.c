#include <SDL2/SDL.h>
#include <stdbool.h>

#define UNIT 20
#define WIDTH 84
#define HEIGHT 42

bool survive(int neighbors)
{
	return neighbors == 2 || neighbors == 3;
}

bool revive(int neighbors)
{
	return neighbors == 3;
}

typedef struct playground
{
	bool data[HEIGHT][WIDTH];
} playground;

bool cell_lives(playground *grid, int x, int y)
{
	bool up = y > 0;
	bool left = x > 0;
	bool down = y < HEIGHT - 1;
	bool right = x < WIDTH - 1;

	int neighbors = 0;
	neighbors += up && left && grid->data[y - 1][x - 1];
	neighbors += up && right && grid->data[y - 1][x + 1];
	neighbors += down && right && grid->data[y + 1][x + 1];
	neighbors += down && left && grid->data[y + 1][x - 1];

	neighbors += up && grid->data[y - 1][x];
	neighbors += down && grid->data[y + 1][x];
	neighbors += left && grid->data[y][x - 1];
	neighbors += right && grid->data[y][x + 1];

	return grid->data[y][x] ? survive(neighbors) : revive(neighbors);
}

playground new_generation(playground *grid)
{
	playground new;
	for (int y = 0; y < HEIGHT; ++y)
	{
		for (int x = 0; x < WIDTH; ++x)
		{
			new.data[y][x] = cell_lives(grid, x, y);
		}
	}
	return new;
}

void draw_playground(SDL_Renderer *rend, playground *grid)
{
	SDL_SetRenderDrawColor(rend, 0, 255, 0, 255);
	for (int y = 0; y < HEIGHT; ++y)
	{
		for (int x = 0; x < WIDTH; ++x)
		{
			if (grid->data[y][x])
			{
				SDL_Rect rect = {x * UNIT, y * UNIT, UNIT, UNIT};
				SDL_RenderFillRect(rend, &rect);
			}
		}
	}
	SDL_SetRenderDrawColor(rend, 128, 128, 128, 255);
	for (int i = 1; i <= HEIGHT; ++i)
	{
		SDL_RenderDrawLine(rend, 0, i * UNIT, WIDTH * UNIT, i * UNIT);
	}
	for (int i = 1; i <= WIDTH; ++i)
	{
		SDL_RenderDrawLine(rend, i * UNIT, 0, i * UNIT, HEIGHT * UNIT);
	}
}

int main()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	playground grid = {{{false}}};
	SDL_Window *screen = SDL_CreateWindow("Bakterie", 20, 20, WIDTH * UNIT, HEIGHT * UNIT, 0);
	SDL_Renderer *rend = SDL_CreateRenderer(screen, -1, 0);
	bool run = false, end = false;
	SDL_Event event;
	while (!end)
	{
		while (!run && !end)
		{
			SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
			SDL_RenderClear(rend);
			draw_playground(rend, &grid);
			SDL_RenderPresent(rend);
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_MOUSEBUTTONDOWN)
				{
					grid.data[event.button.y / UNIT][event.button.x / UNIT] = !grid.data[event.button.y / UNIT][event.button.x / UNIT];
				}
				if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN)
				{
					run = true;
				}
				if (event.type == SDL_QUIT)
				{
					end = true;
				}
			}
			SDL_Delay(20);
		}
		while (run && !end)
		{
			SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
			SDL_RenderClear(rend);
			draw_playground(rend, &grid);
			SDL_RenderPresent(rend);
			grid = new_generation(&grid);
			SDL_Delay(500);
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
				{
					run = false;
				}
				if (event.type == SDL_QUIT)
				{
					end = true;
				}
			}
		}
	}

	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(screen);
	SDL_Quit();
	return 0;
}