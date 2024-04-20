#include "Page.h"
#include <iostream>

using namespace std;


Page::Page(string name) {
	hasBackgroundImage = false;
	isPage = true;
	pageName = name;

	Frame::setPosition(sf::Vector2f(0.0f, 0.0f));
	
	relativePosition = sf::Vector2f(0.0f, 0.0f);
	realPositionCalculated = relativePositionCalculated = true;

	setResolution(sf::Vector2f(resolution[0], resolution[1]));
	setSize(sf::Vector2f(resolution[0], resolution[1]));

	cout << "Page '" + name + "' created, this = " << (int)this << endl;

	setFillColor(sf::Color::Transparent);
}

void Page::mouseMovement(sf::Event::MouseMoveEvent mouseMoveEvent) {
	vector<Frame*> focusedFrames;
	getFocusedFrames(focusedFrames);

	Frame* focusedFrameWithMouseOverIt = nullptr;

	for (Frame* focusedFrame : focusedFrames) {
		if (focusedFrame->hasMouseOver() || focusedFrame->mouseOverAnyOfItemsRec()) {
			focusedFrameWithMouseOverIt = focusedFrame;
			break;
		}
	}

	if (focusedFrames.size() == 0 || focusedFrameWithMouseOverIt == nullptr)
		Frame::mouseMovement(mouseMoveEvent);
	else if (focusedFrameWithMouseOverIt != nullptr) {
		forceMouseLeft();
		focusedFrameWithMouseOverIt->mouseMovement(mouseMoveEvent);
	}
	/*
	if (focusedFrame != nullptr) {
		cout << "mouseMovement: focusedFrame = " << focusedFrame << ", hasMouseOver = "
			<< (focusedFrame != nullptr ? (focusedFrame->hasMouseOver() ? "true" : "false") : "???") << endl;
		cout << "focusedFrame->mouseOver = " << focusedFrame->hasMouseOver() << endl;
		cout << "focusedFrame->mouseOverAnyOfItems() = " << focusedFrame->mouseOverAnyOfItems() << "\n__________" << endl;
	}
	*/
}

void Page::mouseButtonChange(sf::Event::EventType type, sf::Event::MouseButtonEvent mouseButtonEvent) {
	vector<Frame*> focusedFrames;
	getFocusedFrames(focusedFrames);
	cout << "--in Page::mouseButtonChange, beginning, focusedFrames.size() = " << focusedFrames.size() << endl;

	if (focusedFrames.size() == 0)
		Frame::mouseButtonChange(type, mouseButtonEvent);
	else
		for (Frame* focusedFrame : focusedFrames)
			focusedFrame->mouseButtonChange(type, mouseButtonEvent);

	focusedFrames.clear();
	getFocusedFrames(focusedFrames);
	cout << "----in Page::mouseButtonChange, end, focusedFrames.size() = " << focusedFrames.size() << endl;
}

void Page::defaultVisualSetup() {
	setFillColor(sf::Color::White);
}
