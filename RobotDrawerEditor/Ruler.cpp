#include "Ruler.h"


Ruler::Ruler() {
	setOrientation("horizontal");
}

void Ruler::setSize(sf::Vector2f size) {
	Frame::setSize(size);

	if (!renderTexture.create(size.x, size.y)) {
		cout << "Measure::setSize : could not create RenderTexture" << endl;
		return;
	}

	sprite.setTexture(renderTexture.getTexture());
	sprite.setTextureRect(sf::IntRect(0, 0, (int)size.x, (int)size.y));
	renderTexture.clear(fillColorNormal);
	redraw();
}

void Ruler::draw(sf::RenderWindow& window) {
	Frame::draw(window);
	redraw();
	window.draw(sprite);
}

void Ruler::update() {
	redraw();
}

void Ruler::redraw() {
	// calculating one part length size
	int numberOfParts = (orientation == "horizontal" ? getSize().x : getSize().y) / minimalRealLengthDiff + 1;

	float scopeWidth = rightBoundary - leftBoundary;
	
	float onePartScopeLengthRaw = scopeWidth / numberOfParts;
	float onePartScopeLengthReal = 0;
	
	for (int i = 0; i < LENGTHS_SIZE; i++) {
		if (lengths[i] > onePartScopeLengthRaw) {
			onePartScopeLengthReal = i == 0 ? 1 : lengths[i - 1];
			break;
		}
	}

	// finding the starting point coordinate - spc
	int spc = (leftBoundary / onePartScopeLengthReal) * (onePartScopeLengthReal + 1);

	int drawingPointScope = spc;
	int drawingPointReal = orientation == "horizontal" ? realPosition.x + horizontalPadding : realPosition.y + verticalPadding;
	int endPointReal = orientation == "horizontal" ? realPosition.x + getSize().x - horizontalPadding
												   : realPosition.y + getSize().y - verticalPadding;

	while (drawingPointReal <= endPointReal) {
		//CONTINUE
	}
}

void Ruler::setOrientation(string orientation) {
	orientation = "horizontal";
}

void Ruler::setScope(float beginning, float end) {
	leftBoundary = beginning;
	rightBoundary = end;


}
