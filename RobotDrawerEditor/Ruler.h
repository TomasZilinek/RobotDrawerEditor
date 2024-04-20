#pragma once
#include "Widgets.h"

#ifndef RULER_H_
#define RULER_H_


static const int LENGTHS_SIZE = 12;
static const int lengths[LENGTHS_SIZE] = { 1, 2, 4, 5, 10, 25, 50, 100, 250, 500, 1000, 2500};


class Ruler : public Frame {
public:
	Ruler();

	virtual void setSize(sf::Vector2f size);
	virtual void draw(sf::RenderWindow& window);

	void setOrientation(string orientation);
	void setScope(float beginning, float end);
private:
	const int minimalRealLengthDiff = resolution[0] / 22;
	float leftBoundary, rightBoundary;
	string orientation;  // orientation is from {"horizontal", "vertical"}

	sf::RenderTexture renderTexture;
	sf::Sprite sprite;
	virtual void update();
	void redraw();
};

#endif
