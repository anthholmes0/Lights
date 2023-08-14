#include "cinder/gl/gl.h"

#include "Boundary.h"

using namespace std;
using namespace cinder;
using namespace cinder::app;

class Beam
{
	public:
		// physical variables
		vec2 source_vec, incidence_vec;
		float angle, len;
		Color color;

		// interacted boundaries' pointers
		Boundary* source_boundary;
		Boundary* incident_boundary;

		// reflection variables
		int intensity;
		Beam* reflection;

		Beam(vec2 source_vec_, float angle_, vector<Boundary>& boundaries, int intensity_, Color color_, Boundary* source_boundary_ = nullptr)
		{
			source_vec = source_vec_;
			angle = angle_;
			color = color_;

			intensity = intensity_;
			source_boundary = source_boundary_;

			//calling calcIncidence() in the constructor enables recursive reflections since a Beam can be constructed in calcIncidence()
			calcIncidence(boundaries);
		}

		void show();
		void calcIncidence(vector<Boundary>& boundaries);

	protected:
		float MIN_LEN = sqrt(getWindowWidth()*getWindowWidth() + getWindowHeight()*getWindowHeight());
};

void Beam::show()
{
	gl::color(color);
	gl::lineWidth(2);

	gl::drawLine(source_vec, incidence_vec);

	if (reflection != NULL)
	{
		reflection->show();
	}
}

void Beam::calcIncidence(vector<Boundary>& boundaries)
{
	// reset incident boundary variables
	float min_len = MIN_LEN;
	incident_boundary = nullptr;

	// go through all the boundaries and find the one that is (a) intersected by the beam and is (b) closest
	for (Boundary& bd : boundaries)
	{
		if (&bd != source_boundary) // don't reflect of the source boundary
		{
			//could implement optimization here to check only those boundaries whose bounding box overlaps with the beams bounding box

			float len_to_intersection = ((source_vec.x - bd.p1.x)*(bd.p1.y - bd.p2.y) - (source_vec.y - bd.p1.y)*(bd.p1.x - bd.p2.x))
						  / (sin(angle)*(bd.p1.x - bd.p2.x) - cos(angle)*(bd.p1.y - bd.p2.y));

			float per_along_boundary = (cos(angle)*(source_vec.y - bd.p1.y) - sin(angle)*(source_vec.x - bd.p1.x))
			   	                 / (sin(angle)*(bd.p1.x - bd.p2.x) - cos(angle)*(bd.p1.y - bd.p2.y));

			if (0 <= per_along_boundary && per_along_boundary <= 1 && len_to_intersection > 0 && len_to_intersection < min_len)
			{
				min_len = len_to_intersection;
				incident_boundary = &bd;
			}
		}
	}

	// update the incidence_vec and length
	len = min_len;
	incidence_vec = source_vec + len*vec2(cos(angle), sin(angle));

	// construct the reflection beam (if applicable)
	if (incident_boundary != NULL && intensity - incident_boundary->absorption > 0)
	{
		float angle_of_reflection = 2*incident_boundary->angle - angle;
		float intensity_of_reflection = intensity - incident_boundary->absorption;

		reflection = new Beam(incidence_vec, angle_of_reflection, boundaries, intensity_of_reflection, color, incident_boundary);
	}
	else
	{
		reflection = nullptr;
	}
}
