#pragma once

#include "Widgets.h"
#include "BezierCurve.h"

#ifndef PAPER_H_
#define PAPER_H_


class Paper : public Frame {
public:
	Paper();

	void draw(sf::RenderWindow& window);
	string getDescription();
protected:
	int width, height;  // in millimeters
	vector<SimpleBezierCurve*> curves;
};

#endif
