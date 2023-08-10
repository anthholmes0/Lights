#pragma once
#include "cinder/gl/gl.h"

using namespace std;
using namespace cinder;
using namespace cinder::app;

class Boundary
{
	public:
		vec2 p1, p2;
		float angle;
		int absorption;

		Boundary(float x1, float y1, float x2, float y2, int absorption_)
		{
			p1 = vec2(x1, y1);
			p2 = vec2(x2, y2);
			angle = atan((y2-y1)/(x2-x1)); //almost certainly will need quadrant correction
			absorption = absorption_;
		}

		Boundary(vec2 p1_, vec2 p2_, int absorption_)
		{
			p1 = p1_;
			p2 = p2_;
			angle = atan((p2.y-p1.y)/(p2.x-p1.x)); //also will likely run into issue when x2 = x1
			absorption = absorption_;
		}

		void show();
};

void Boundary::show()
{
	gl::color(Color(1, 1, 1));
	gl::lineWidth(2);

	gl::drawSolidCircle(p1, 3); //start point
	gl::drawLine(p1, p2);	    //line
	gl::drawSolidCircle(p2, 3); //end point
}
