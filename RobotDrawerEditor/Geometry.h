#pragma once

#ifndef GEOMETRY_H_
#define GEOMETRY_H_

/*
 * all coordinates and distances are in millimeters
 */

class Point2D {
public:
	Point2D();

	void move(int x_diff, int y_diff);
	void setPosition(int new_x, int new_y);

	//void draw(sf::RenderWindow& window);
protected:
	int x;
	int y;
};

#endif