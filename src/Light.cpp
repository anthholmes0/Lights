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
	//w1.addBoundary(WIDTH/2, 50, WIDTH/2, HEIGHT-50, 50);

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
	w1.addLight(Laser(vec2(WIDTH-75, 250), M_PI/3, w1.boundaries));
	//w1.addLight(Lamp(vec2(WIDTH/2+25, 250), 10, w1.boundaries));
}

void BasicApp::update()
{
	//nothing atm
}

void BasicApp::draw()
{
	w1.show();
}

void BasicApp::mouseDown(MouseEvent event)
{

}

void BasicApp::mouseUp(MouseEvent event)
{

}

void BasicApp::mouseMove(MouseEvent event)
{
	ivec2 mousePos = event.getPos();

	for (LightSource& l : w1.lights)
	{
		for (Beam& bm : l.beams)
		{
			bm.source = mousePos;
			bm.calcIncidence(w1.boundaries);
		}
	}
}

void BasicApp::mouseDrag(MouseEvent event)
{

}

CINDER_APP(BasicApp, RendererGl)

