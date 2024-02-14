#include <SDL.h>
#include <vector>
#include <algorithm>

constexpr int unit = 20;
constexpr int width = 84, height = 42;


class Field
{
public:
	using Rule = std::vector<int>;

private:
	bool table[height][width] = {{false}};

public:
	constexpr Field() = default;
	void Negate(int x, int y)
	{
		table[y][x] = !table[y][x];
	}
	bool Survive(int x, int y, const Rule &survive, const Rule &born) const
	{
		bool up = y > 0;
		bool left = x > 0;
		bool down = y < height - 1;
		bool right = x < width - 1;
		int neighbors = 0;

		neighbors += int(up && left && table[y - 1][x - 1]);
		neighbors += int(up && right && table[y - 1][x + 1]);
		neighbors += int(down && right && table[y + 1][x + 1]);
		neighbors += int(down && left && table[y + 1][x - 1]);

		neighbors += int(up && table[y - 1][x]);
		neighbors += int(down && table[y + 1][x]);
		neighbors += int(left && table[y][x - 1]);
		neighbors += int(right && table[y][x + 1]);

		const auto &rule = table[y][x] ? survive : born;
		return std::find(rule.cbegin(), rule.cend(), neighbors) != rule.cend();
	}
	Field NewGeneration(const Rule &survive, const Rule &born) const
	{
		Field result;
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				result.table[y][x] = Survive(x, y, survive, born);
			}
		}
		return result;
	}
	void DrawOn(SDL_Renderer *rend) const
	{
		SDL_SetRenderDrawColor(rend, 0, 255, 0, 255);
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				if (table[y][x])
				{
					SDL_Rect rect{x * unit, y * unit, unit, unit};
					SDL_RenderFillRect(rend, &rect);
				}
			}
		}
		SDL_SetRenderDrawColor(rend, 128, 128, 128, 255);
		for (int i = 1; i <= height; ++i)
		{
			SDL_RenderDrawLine(rend, 0, i * unit, width * unit, i * unit);
		}
		for (int i = 1; i <= width; ++i)
		{
			SDL_RenderDrawLine(rend, i * unit, 0, i * unit, height * unit);
		}
	}
};


int main()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	Field field;
	SDL_Window *screen = SDL_CreateWindow("Bakterie", 20, 20, width * unit, height * unit, 0);
	SDL_Renderer *rend = SDL_CreateRenderer(screen, -1, 0);
	bool run = false, end = false;
	Field::Rule survive = {2, 3}, born = {3};
	SDL_Event event;
	while (!end)
	{
		while (!run && !end)
		{
			SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
			SDL_RenderClear(rend);
			field.DrawOn(rend);
			SDL_RenderPresent(rend);
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_MOUSEBUTTONDOWN)
				{
					field.Negate(event.button.x / unit, event.button.y / unit);
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
			field.DrawOn(rend);
			SDL_RenderPresent(rend);
			field = field.NewGeneration({2, 3}, {3});
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