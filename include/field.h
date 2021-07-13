#pragma once

template<int w, int h, int sz>
class TField
{
public:
	constexpr static uint16 height=h;
	constexpr static uint16 width=w;
	constexpr static uint16 size=sz;
	using Rule=containers::DynArr<uint8>;
private:
	bool table[height][width]= {{false}};
public:
	constexpr TField()=default;
	void Negate(SDL::Point xy)
	{
		table[xy.y][xy.x]=!table[xy.y][xy.x];
	}
	bool Survive(uint8 x, uint8 y, const Rule& survive, const Rule& born)const
	{
		bool up=y>0;
		bool left=x>0;
		bool down=y<height-1;
		bool right=x<width-1;
		uint8 neighbors=0;

		neighbors+=int(up&&left&&table[y-1][x-1]);
		neighbors+=int(up&&right&&table[y-1][x+1]);
		neighbors+=int(down&&right&&table[y+1][x+1]);
		neighbors+=int(down&&left&&table[y+1][x-1]);

		neighbors+=int(up&&table[y-1][x]);
		neighbors+=int(down&&table[y+1][x]);
		neighbors+=int(left&&table[y][x-1]);
		neighbors+=int(right&&table[y][x+1]);

		return table[y][x]?survive.find(neighbors)!=survive.size():born.find(neighbors)!=born.size();
	}
	TField NewGeneration(const Rule& survive, const Rule& born)const
	{
		TField result;
		for(uint8 y=0; y<height; ++y)
		{
			for(uint8 x=0; x<width; ++x)
			{
				result.table[y][x]=Survive(x,y, survive, born);
			}
		}
		return result;
	}
	void DrawOn(SDL::DrawBase& rend)const
	{
		for(uint16 y=0; y<height; ++y)
		{
			for(uint16 x=0; x<width; ++x)
			{
				if(table[y][x])
				{
					rend.Draw(SDL::Rect(x*size,y*size, size,size), SDL::Color(0,255,0));
				}
			}
		}
		for(uint16 i=1; i<=height; ++i)
		{
			rend.Draw(SDL::Line(SDL::Point(0, i)*size, SDL::Point(width, i)*size), SDL::Color(128,128,128));
		}
		for(uint16 i=1; i<=width; ++i)
		{
			rend.Draw(SDL::Line(SDL::Point(i, 0)*size, SDL::Point(i,height)*size), SDL::Color(128,128,128));
		}
	}
};