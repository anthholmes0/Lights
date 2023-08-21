#pragma once
#include "cinder/gl/gl.h"
#include "Vertex.h"

using namespace std;
using namespace cinder;
using namespace cinder::app;

class Boundary
{
	public:
		//vec2 p1, p2;
		Vertex* v1;
		Vertex* v2;
		float angle;
		int absorption;
		bool grabbed;

		Boundary(float x1, float y1, float x2, float y2, int absorption_)
		{
			//p1 = vec2(x1, y1);
			//p2 = vec2(x2, y2);
			//angle = atan((y2-y1)/(x2-x1));

			v1 = new Vertex(x1, y1);
			v2 = new Vertex(x2, y2);

			angle = atan((y2 - y1)/(x2 - x1));
			absorption = absorption_;
			grabbed = false;
		}

		Boundary(vec2 p1_, vec2 p2_, int absorption_)
		{
			//p1 = p1_;
			//p2 = p2_;

			v1 = new Vertex(p1_);
			v2 = new Vertex(p2_);

			angle = atan((p2_.y - p1_.y)/(p2_.x - p1_.x));
			absorption = absorption_;
			grabbed = false;
		}

		void show();
		void checkGrab(vec2 mousePos);
		void move(vec2 mousePos);
		void release();
		//void set_p1_test(vec2 p1_);
		//vector<vec2> bounding_box();
};

void Boundary::show()
{
	gl::color(Color(1, 1, 1));
	gl::lineWidth(2);

	//gl::drawSolidCircle(p1, 3); //start point
	v1->show();
	gl::drawLine(v1->pos, v2->pos);	    //line
	//gl::drawSolidCircle(p2, 3); //end point
	v2->show();
}

void Boundary::checkGrab(vec2 mousePos)
{
	v1->checkGrab(mousePos);
	v2->checkGrab(mousePos);
	grabbed = (v1->grabbed || v2->grabbed);
}

void Boundary::move(vec2 mousePos)
{
	v1->move(mousePos);
	v2->move(mousePos);

	angle = atan((v2->pos.y - v1->pos.y) / (v2->pos.x - v1->pos.x));
}

void Boundary::release()
{
	grabbed = false;
	v1->release();
	v2->release();
}

/*

void Boundary::set_p1_test(vec2 p1_)
{
	p1 = p1_;
	angle = atan((p2.y - p1.y)/(p2.x - p1.x));
}

//i think this is going to be (or already is) unused
vector<vec2> Boundary::bounding_box() //used for beam intersection optimization
{
	vector<vec2> bb;

	bb.push_back(p1);
	bb.push_back(p2);
	bb.push_back(vec2(p1.x, p2.y));
	bb.push_back(vec2(p2.x, p1.y));

	return bb;
}

*/
