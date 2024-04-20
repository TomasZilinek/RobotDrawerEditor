#pragma once
#include "SFML\Graphics.hpp"
#include "Widgets.h"
#include <vector>

#ifndef PAGE_H_
#define PAGE_H_


class Page : public ImageLabel {
public:
	Page(string name);

	virtual void mouseMovement(sf::Event::MouseMoveEvent mouseMoveEvent);
	virtual void mouseButtonChange(sf::Event::EventType, sf::Event::MouseButtonEvent mouseButtonEvent);

	string getName() { return pageName; }
protected:
	virtual void defaultVisualSetup();
	string pageName;
	bool hasBackgroundImage;
};

#endif