#include "Geometry.h"


Point2D::Point2D() {

}

void Point2D::move(int x_diff, int y_diff) {
	x += x_diff;
	y += y_diff;
}

void Point2D::setPosition(int new_x, int new_y) {
	x = new_x;
	y = new_y;
}
