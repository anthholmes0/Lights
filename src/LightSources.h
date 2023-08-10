#pragma once

#include "cmath"
#include "cinder/gl/gl.h"

#include "Beam.h"
#include "Boundary.h"

using namespace std;
using namespace cinder;
using namespace cinder::app;

class LightSource
{
	public:
		vector<Beam> beams;

		void show();

		void move(); // will work on down the line
		bool moved = true;
};

void LightSource::show()
{
	for (Beam bm : beams)
	{
		bm.show();
	}
}

void LightSource::move()
{
	moved = false;
}


class Laser : public LightSource
{
	public:
		Laser(vec2 p1, float angle, vector<Boundary> boundaries, int intensity = 100, Color color = Color(1, 0, 0))
		{
			beams.push_back(Beam(p1, angle, boundaries, intensity, color));
		}
};

class Lamp : public LightSource
{
	public:
		Lamp(vec2 p1, int numRays, vector<Boundary> boundaries, int intensity = 100, Color color = Color(0.992, 0.992, 0.588))
		{
			for (int i = 0; i < numRays; i++)
			{
				beams.push_back(Beam(p1, 2*M_PI*i/float(numRays), boundaries, intensity, color));
			}
		}
};

class Flashlight : public LightSource
{
        public:
                Flashlight(vec2 p1, float angle, float angularWidth, vector<Boundary> boundaries, int intensity = 100, Color color = Color(0.992, 0.992, 0.588))
                {
                        for (int i = 0; i <= 10; i++)
                        {
                                beams.push_back(Beam(p1, angle - angularWidth/2. + float(i)*angularWidth/10., boundaries, intensity, color));
                        }
                }
};

class Sheetlight : public LightSource
{
	public:
		Sheetlight(vec2 p1, vec2 p2, vector<Boundary> boundaries, int intensity = 100, Color color = Color(1, 1, 1))
		{
			float len = sqrt((p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y));
			float ang = atan((p2.y-p1.y)/(p2.x-p1.x)) + M_PI/2; //might need some corrections

			for (int l = 0; l <= len; l += 5)
			{
				float percent = float(l)/len;
				beams.push_back(Beam(vec2(p1.x + (p2.x-p1.x)*percent, p1.y + (p2.y-p1.y)*percent), ang, boundaries, intensity, color));
			}
		}
};

