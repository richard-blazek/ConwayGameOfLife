#include "objsdl/objsdl.h"
#include "mylibraries/dynarr.h"

using namespace std;

#include "include/field.h"

inline bool SameField(SDL::Point a, SDL::Point b, uint16 size)
{
	return a/size==b/size;
}


int main()
{
	constexpr uint16 size=15;
	using Field=TField<84, 42, size>;
	Field field;
	SDL::Init i;
	SDL::Window screen("Bakterie", SDL::Rect(SDL::Window::UndefinedPos, Field::width*size, Field::height*size));
	SDL::Renderer rend(screen);
	bool run=false, end=false;
	Field::Rule survive={2,3}, born={3};
	while(!SDL::events::Quit()&&!end)
	{
		while(!SDL::events::Quit()&&!run&&!end)
		{
			rend.Repaint(SDL::Color(0,0,0));
			field.DrawOn(rend);
			rend.Show();
			for(auto action:SDL::events::Handler())
			{
				if(action.Type()==SDL::events::Type::MouseButtonDown)
				{
					field.Negate(action.MouseButton().Position/size);
				}
				if(action.Type()==SDL::events::Type::Keydown&&action.Keyboard().Key==SDL::Keycode::Return)
				{
					run=true;
				}
				if(action.Type()==SDL::events::Type::Quit)
				{
                    end=true;
				}
			}
		}
		while(!SDL::events::Quit()&&run&&!end)
		{
			rend.Repaint(SDL::Color(0,0,0));
			field.DrawOn(rend);
			rend.Show();
			field=field.NewGeneration({2,3}, {3});
			SDL::Wait(500);
			for(auto action:SDL::events::Handler())
			{
				if(action.Type()==SDL::events::Type::Keydown&&action.Keyboard().Key==SDL::Keycode::Space)
				{
					run=false;
				}
				if(action.Type()==SDL::events::Type::Quit)
				{
                    end=true;
				}
			}
		}
	}
	return 0;
}