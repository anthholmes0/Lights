
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
		//bool checkBoundary(vector<vec2> bounding_box);

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
			//if (checkBoundary(bd.bounding_box())) // don't check those boundaries whose bounding boxes lie outside the quandrant the beam lies in
			//{		//TROUBLESHOOTING THIS
				float len_to_intersection = ((source_vec.x - bd.v1->pos.x)*(bd.v1->pos.y - bd.v2->pos.y) - (source_vec.y - bd.v1->pos.y)*(bd.v1->pos.x - bd.v2->pos.x))
						  	/ (sin(angle)*(bd.v1->pos.x - bd.v2->pos.x) - cos(angle)*(bd.v1->pos.y - bd.v2->pos.y));

				float per_along_boundary = (cos(angle)*(source_vec.y - bd.v1->pos.y) - sin(angle)*(source_vec.x - bd.v1->pos.x))
			   	                 	/ (sin(angle)*(bd.v1->pos.x - bd.v2->pos.x) - cos(angle)*(bd.v1->pos.y - bd.v2->pos.y));

				if (0 <= per_along_boundary && per_along_boundary <= 1 && len_to_intersection > 0 && len_to_intersection < min_len)
				{
					min_len = len_to_intersection;
					incident_boundary = &bd;
				}
			//}
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

/*
bool Beam::checkBoundary(vector<vec2> bounding_box) // is this function itself optimized? -> answer: no
{
	bool result = false;

	for (vec2 vertex : bounding_box)
	{
		vec2 source_to_vertex = vertex - source_vec;

		if ((vertex.x - source_vec.x)*cos(angle) >= 0 && (vertex.y - source_vec.y)*sin(angle) >= 0)
		{
			result = true;
		}
	}

	return result;
}
*/
