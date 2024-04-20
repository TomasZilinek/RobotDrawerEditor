#pragma once

#include "Geometry.h"
#include <vector>


#ifndef BEZIERCURVE_H_
#define BEZIERCURVE_H_

using namespace std;



							/////////////////////////////
							// SimpleBezierCurve Class //
							/////////////////////////////


class SimpleBezierCurve {
public:
	SimpleBezierCurve();

	//void draw(sf::RenderWindow& window);
private:
	Point2D initialPoint;
	Point2D controlPoint1;
	Point2D controlPoint2;
	Point2D endPoint;
};


							////////////////////////////////
							// CompositeBezierCurve Class //
							////////////////////////////////


class CompositeBezierCurve {
public:
	CompositeBezierCurve();
	CompositeBezierCurve(SimpleBezierCurve* firstCurve);

	void addCurve(SimpleBezierCurve* newCurve, bool endpointIsSharp);

	//void draw(sf::RenderWindow& window);
private:
	vector<SimpleBezierCurve*> curves;
	vector<bool> endPointsSharpness;
};

#endif