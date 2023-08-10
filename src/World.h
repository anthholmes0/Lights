#include "cinder/gl/gl.h"
#include "LightSources.h"
#include "Boundary.h"

using namespace std;
using namespace cinder;
using namespace cinder::app;

class World // wrapper class for the 'physics' [kinda not in use atm but still part of the stack lol]
{
	public:
		vector<Boundary> boundaries;
		vector<LightSource> lights;

		void show();

		void addLight(LightSource l);
		void addBoundary(float x1, float y1, float x2, float y2, int absorption);
};

void World::show()
{
	gl::clear(Color(0, 0, 0));

	for (Boundary bd : boundaries)
	{
		bd.show();
	}

	for (LightSource l : lights)
	{
		l.show();
	}
}

void World::addLight(LightSource l)
{
	lights.push_back(l);
}

void World::addBoundary(float x1, float y1, float x2, float y2, int absorption = 100)
{
	boundaries.push_back(Boundary(x1, y1, x2, y2, absorption));
}
