#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "cmath"

#include "World.h"

using namespace std;
using namespace cinder;
using namespace cinder::app;

/*
ISSUE: NOT OPTIMIZED (SLOW AS SHIT)

	PROGRAM FLOW:
		ADD BOUNDARIES
		ADD LIGHTS
			ADD LIGHT SOURCE BEAMS

		LOOP:
		CALC LSB REFLECTION BEAMS
			ITERATIVELY CALC BEAM REFLECTIONS

	OBJECT STRUCTURE:
	WORLD ->
		BOUNDARIES
		LIGHTS ->
			BEAMS -> REFLECTION -> REFLECTION -> ... -> REFLECTION
			 - recalcing reflections from the light-origin beams every loop is horribly slow
			 - many reflections are unchanged loop to loop and don't need to be recalced
			 - each beam (inc reflects) has info on it's 'source' and 'incident' boundaries
			 - this could be used e.g. only recalc downstream reflections if the source/incident boundary of a given beam is moved
			 - this is a promising approach, but flawed, as a boundary could also be moved to a position 'in between' a beam's src/inc boundaries

	IDEA:
	 - only check lights for incidence against boundaries that have been moved
	 - this cuts out a lot of beam/boundary pairs
	 - wouldn't need to delete/reconstruct reflects (check light-origin beams, if intersect with moved boundary, then redo reflects, if not, check reflect, iterate)
*/


class BasicApp : public App
{
	public:
		void setup() override;
		void update() override;
		void draw() override;

		void mouseDown(MouseEvent event) override;
		void mouseUp(MouseEvent event) override;
		void mouseMove(MouseEvent event) override;
		void mouseDrag(MouseEvent event) override;

		World w1 = World();

	protected:
		int WIDTH = getWindowWidth();
		int HEIGHT = getWindowHeight();
};

void BasicApp::setup()
{
	//boundaries
	w1.addBoundary(WIDTH/2, 50, WIDTH/2, HEIGHT-50, 10);
	w1.addBoundary(WIDTH/2, HEIGHT-50, WIDTH/4, HEIGHT-50, 10);

	/*
	w1.addBoundary(50, 50, 100, 50);
	w1.addBoundary(100, 50, 100, HEIGHT-100);
	w1.addBoundary(100, HEIGHT-100, WIDTH/2-25, HEIGHT-100);
	w1.addBoundary(WIDTH/2-25, HEIGHT-100, WIDTH/2-25, 50);
	w1.addBoundary(WIDTH/2-25, 50, WIDTH/2+25, 50);
	w1.addBoundary(WIDTH/2+25, 50, WIDTH/2+25, HEIGHT-100);
	w1.addBoundary(WIDTH/2+25, HEIGHT-100, WIDTH-100, HEIGHT-100);
	w1.addBoundary(WIDTH-100, HEIGHT-100, WIDTH-100, 50);
	w1.addBoundary(WIDTH-100, 50, WIDTH-50, 50);
	w1.addBoundary(WIDTH-50, 50, WIDTH-50, HEIGHT-50);
	w1.addBoundary(WIDTH-50, HEIGHT-50, 50, HEIGHT-50);
	w1.addBoundary(50, HEIGHT-50, 50, 50);
	for (Boundary& bd : w1.boundaries)
	{
		bd.absorption = 10;
	}
	*/

	//lights
	//w1.addLight(Laser(vec2(WIDTH/2, HEIGHT/2), M_PI/4, w1.boundaries));
	w1.addLight(Lamp(vec2(WIDTH/2, HEIGHT/2), 20, w1.boundaries));
}

void BasicApp::update()
{
	for (LightSource& l : w1.lights)
	{
		for (Beam& bm : l.beams)
		{
			bm.calcIncidence(w1.boundaries);
		}
	}
}

void BasicApp::draw()
{
	w1.show();
}

void BasicApp::mouseDown(MouseEvent event)
{
	vec2 mousePos = event.getPos();

	for (Boundary& bd : w1.boundaries)
	{
		bd.checkGrab(mousePos);
	}

	// DEBUG //
	/*
	cout << "Boundaries:" << endl;
	for (Boundary& bd : w1.boundaries)
	{
		cout << "-> " << &bd << endl;
	}

	cout << "Beam Incident Boundaries:" << endl;
	for (LightSource& l : w1.lights)
        {
                for (Beam& bm : l.beams)
                {
                        cout << "-> " << bm.incident_boundary << endl;

                        Beam* b = bm.reflection;
                        while (b != NULL)
                        {
                                cout << "    -> " << b->incident_boundary << endl;
                                b = b->reflection;
                        }
                }
        }

	cout << "Beam Source Boundaries:" << endl;
	for (LightSource& l : w1.lights)
	{
		for (Beam& bm : l.beams)
		{
			cout << "-> " << bm.source_boundary << endl;

			Beam* b = bm.reflection;
			while (b != NULL)
			{
				cout << "    -> " << b->source_boundary << endl;
				b = b->reflection;
			}
		}
	}
	cout << endl;
	*/
}

void BasicApp::mouseUp(MouseEvent event)
{
	for (Boundary& bd : w1.boundaries)
	{
		bd.release();
	}
}

void BasicApp::mouseMove(MouseEvent event)
{
	/*
	vec2 mousePos = event.getPos();

	for (Boundary& bd : w1.boundaries)
	{
		if (bd.grabbed)
		{
			bd.move(mousePos);
		}
	}

	//w1.boundaries.at(0).set_p1_test(mousePos); // move boundary around with mouse

	//w1.lights.at(0).beams.at(0).angle = atan((mousePos.y - HEIGHT/2) / (mousePos.x - WIDTH/2));

	for (LightSource& l : w1.lights)
	{
		for (Beam& bm : l.beams)
		{
			bm.source = mousePos;
			bm.calcIncidence(w1.boundaries);
		}
	}
	*/
}

void BasicApp::mouseDrag(MouseEvent event)
{
        vec2 mousePos = event.getPos();

        for (Boundary& bd : w1.boundaries)
        {
                if (bd.grabbed)
                {
                        bd.move(mousePos);
                }
        }
}

CINDER_APP(BasicApp, RendererGl)

