#include "cinder/gl/gl.h"

using namespace std;
using namespace cinder;

class Vertex
{
	public:
		vec2 pos;
		float rad;
		Color col;
		bool grabbed;

		Vertex(float x, float y, Color col_ = Color(1, 1, 1))
		{
			pos = vec2(x, y);
			rad = 5;
			col = col_;
			grabbed = false;
		}

		Vertex(vec2 pos_, Color col_ = Color(1, 1, 1))
		{
			pos = pos_;
			rad = 5;
			col = col_;
			grabbed = false;
		}

		void show();
		void checkGrab(vec2 mousePos);
		void release();
		void move(vec2 mousePos);
};

void Vertex::show()
{
	gl::color(col);
	gl::drawSolidCircle(pos, rad);
}

void Vertex::checkGrab(vec2 mousePos)
{
	grabbed = ((pos.x - mousePos.x)*(pos.x - mousePos.x) + (pos.y - mousePos.y)*(pos.y - mousePos.y) <= rad*rad);
}

void Vertex::release()
{
	grabbed = false;
}

void Vertex::move(vec2 mousePos)
{
	if (grabbed)
	{
		pos = mousePos;
	}
}
