#include "cinder/gl/gl.h"

#include "Boundary.h"

using namespace std;
using namespace cinder;
using namespace cinder::app;

class Beam
{
	public:
		vec2 source, incidence;
		float angle, len;

		Color color;
		int intensity;

		Beam* reflection = nullptr;

		Beam(vec2 source_, float angle_, vector<Boundary> boundaries, int intensity_, Color color_)
		{
			source = source_;
			angle = angle_;

			intensity = intensity_;
			color = color_;

			calcIncidence(boundaries);
		}


		void show();
		void calcIncidence(vector<Boundary> boundaries);
};

void Beam::show()
{
	gl::color(color);
	gl::lineWidth(2);

	gl::drawLine(source, incidence);

	if (reflection != NULL)
	{
		reflection->show();
	}
}

void Beam::calcIncidence(vector<Boundary> boundaries)
{
	//reset reflection
	reflection = nullptr;

	//some helpful variables
	float min_len = sqrt(getWindowWidth()*getWindowWidth() + getWindowHeight()*getWindowHeight());
	bool incident_upon_boundary = false;
	Boundary* incident_boundary;

	//go through all the boundaries and find the one that is (a) intersected by the beam and is (b) closest
	for (Boundary& bd : boundaries)
	{
		float len_to_intersection = ((source.x - bd.p1.x)*(bd.p1.y - bd.p2.y) - (source.y - bd.p1.y)*(bd.p1.x - bd.p2.x))
					  / (sin(angle)*(bd.p1.x - bd.p2.x) - cos(angle)*(bd.p1.y - bd.p2.y));

		float per_along_boundary = (cos(angle)*(source.y - bd.p1.y) - sin(angle)*(source.x - bd.p1.x))
			                 / (sin(angle)*(bd.p1.x - bd.p2.x) - cos(angle)*(bd.p1.y - bd.p2.y));

		if (0 <= per_along_boundary && per_along_boundary <= 1 && len_to_intersection > 0 && len_to_intersection < min_len)
		{
			min_len = len_to_intersection;
			incident_upon_boundary = true;
			incident_boundary = &bd;
		}
	}

	//calculate the incident location vector
	len = min_len;
	incidence = source + len*vec2(cos(angle), sin(angle));

	//if the beam is incident upon a boundary, construct a reflection beam and add it to the associated light source's beam [reflect?] vector
	if (incident_upon_boundary && intensity - incident_boundary->absorption > 0)
	{
		float angle_of_reflection = 2*incident_boundary->angle - angle;
		float intensity_of_reflection = intensity - incident_boundary->absorption;

		reflection = new Beam(incidence, angle_of_reflection, boundaries, intensity_of_reflection, color);
	}
}
