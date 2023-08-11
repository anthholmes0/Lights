#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "cmath"

#include "World.h"

using namespace std;
using namespace cinder;
using namespace cinder::app;

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
	//w1.addBoundary(WIDTH/2, 50, WIDTH/2, HEIGHT-50, 10);
	//w1.addBoundary(WIDTH/2, HEIGHT-50, WIDTH/4, HEIGHT-50, 10);

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
		bd.absorption = 1;
	}

	//lights
	//w1.addLight(Laser(vec2(50, 50), M_PI/4, w1.boundaries));
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
	// DEBUG //
	/*
	for (LightSource& l : w1.lights)
	{
		for (Beam& bm : l.beams)
		{
			cout << "Beam Incidence: " << bm.incident_upon_boundary << endl;

			string title = "Reflection ";
			Beam* b = bm.reflection;
			while (b != NULL)
			{
				cout << title << "Incidence: " << b->incident_upon_boundary << endl;
				b = b->reflection;
				title += "Reflection ";
			}
		}
	}
	*/
}

void BasicApp::mouseUp(MouseEvent event)
{

}

void BasicApp::mouseMove(MouseEvent event)
{
	ivec2 mousePos = event.getPos();

	w1.boundaries.at(0).set_p1_test(mousePos); // move boundary around with mouse

	/*
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

}

CINDER_APP(BasicApp, RendererGl)

