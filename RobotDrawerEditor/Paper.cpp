#include "Paper.h"


Paper::Paper() {

}

void Paper::draw(sf::RenderWindow& window) {

}

string Paper::getDescription() {
	if (width == 74 && height == 105)
		return "A7";
	else if (width == 105 && height == 148)
		return "A6";
	else if (width == 148 && height == 210)
		return "A5";
	else if (width == 210 && height == 297)
		return "A4";
	else if (width == 297 && height == 420)
		return "A3";
	else if (width == 420 && height == 594)
		return "A3";
	else
		return "unknown";
}
