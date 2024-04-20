#include "Widgets.h"
#include <cassert>
#include <string>


							/////////////////
							// Frame Class //
							/////////////////


FrameState::FrameState() {
	for (bitset<BIT_SIZE> bset : bitsetArray)
		bset.reset();
}

bool FrameState::hasActive(object what, mode type) {
	return bitsetArray[type][what];
}

void FrameState::setActive(object what, mode type) {
	bitsetArray[type][what] = true;
}

void FrameState::setInactive(object what, mode type) {
	bitsetArray[type][what] = false;
}

void FrameState::setAll(bool activity) {
	if (!activity)
		for (bitset<BIT_SIZE> bset : bitsetArray)
			bset.set();
	else
		for (bitset<BIT_SIZE> bset : bitsetArray)
			bset.reset();
}

void FrameState::printInfo() {
	cout << "                   _________________________" << endl;
	cout << "                   | Hover | Click | Focus |" << endl;

	string names[] = { "fillColor", "textColor", "outlineColor", "outlineThickness", "image" };

	for (int i = 0; i < BIT_SIZE; i++) {
		cout << names[i];

		for (int a = 0; a < names[3].length() - names[i].length(); a++)
			cout << " ";

		cout << "   |   ";
	
		for (bitset<BIT_SIZE> bset : bitsetArray)
			cout << bset[i] << "   |   ";

		cout << endl;
	}

	cout << "                   -------------------------" << endl;
}


							/////////////////
							// Frame Class //
							/////////////////


Frame::Frame() {
	sizeIsSet = false;
	//setHorizontalPadding(DEFAULT_HORIZONTAL_PADDING);
	//setVerticalPadding(DEFAULT_VERTICAL_PADDING);
	sizeIsSet = false;

	onMouseBackNormal();
}

Frame::Frame(sf::Vector2f relativePosition) : Frame() {
	setRelativePosition(relativePosition);
}

void Frame::getFocusedFrames(vector<Frame*>& cont) {
	if (activeBool) {
		for (Frame* childFrame : frames) {
			if (childFrame->isFocused)
				cont.push_back(childFrame);

			childFrame->getFocusedFrames(cont);
		}
	}
}

void Frame::setParentFrame(Frame* newParentFrame) {
	parentFrame = newParentFrame;
	
	if (parentStructureComplete())
		updatePositions();

	if (horizontallyCentered)
		horizontalCenter();
}

void Frame::horizontalCenter() {
	if (parentStructureComplete()) {
		sf::Vector2f position = getPosition();

		setPosition(parentFrame->getPosition().x + (parentFrame->getGlobalBounds().width - getGlobalBounds().width) / 2,
					position.y);
	}

	horizontallyCentered = true;  // setPosition rewrites horizontallyCentered
}

void Frame::setSize(sf::Vector2f size) {
	sizeIsSet = true;
	justSetSize(size);
}

void Frame::setRelativePosition(sf::Vector2f newRelativePosition) {
	justSetRelativePosition(newRelativePosition);
	relativePositionCalculated = true;
	updatePositions();
}

void Frame::setRelativePosition(float xPos, float yPos) {
	setRelativePosition(sf::Vector2f(xPos, yPos));
}

void Frame::setPosition(sf::Vector2f newPosition) {
	justSetRealPosition(newPosition);
	realPositionCalculated = true;
	
	updatePositions();

	horizontallyCentered = false;
}

void Frame::setPosition(float xPos, float yPos) {
	setPosition(sf::Vector2f(xPos, yPos));
}

void Frame::updatePositions() {
	if (!isPage) {
		bool realPositionCalculatedBefore = realPositionCalculated;

		if (!relativePositionCalculated)
			calculateRelativePosition();
		
		if (!realPositionCalculated)
			calculateRealPosition();

		if (!realPositionCalculatedBefore && realPositionCalculated) {
			bool previousHorizontallyCentered = horizontallyCentered;
			setPosition(realPosition);
			horizontallyCentered = previousHorizontallyCentered;
		}
	}

	if (parentStructureComplete())
		for (Frame* frame : frames)
			frame->updatePositions();
}

void Frame::calculateRealPosition() {
	if (parentStructureComplete()) {
		justSetRealPosition(parentFrame->getChildRealPosition(this));
		realPositionCalculated = true;
	}
}

void Frame::calculateRelativePosition() {
	if (parentStructureComplete()) {
		justSetRelativePosition(parentFrame->getChildRelativePosition(this));
		relativePositionCalculated = true;
	}
}

void Frame::justSetRealPosition(sf::Vector2f newPosition) {
	sf::RectangleShape::setPosition(newPosition);
	realPosition = newPosition;
}

void Frame::justSetRelativePosition(sf::Vector2f newRelativePosition) {
	relativePosition = newRelativePosition;
}

sf::Vector2f Frame::getChildRealPosition(Frame* childFrame) {
	if (childFrame->realPositionCalculated)
		return childFrame->getPosition();

	sf::Vector2f childFrameRelativePosition = childFrame->getRelativePosition();

	sf::FloatRect globalBounds = getGlobalBounds();
	sf::Vector2f size = getSize();

	sf::Vector2f childFrameRealPosition = sf::Vector2f(childFrameRelativePosition.x * getSize().x,
													   childFrameRelativePosition.y * getSize().y);

	if (!isPage && parentStructureComplete()) {
		if (realPositionCalculated)
			childFrameRealPosition += realPosition;
		else {
			childFrameRealPosition += parentFrame->getChildRealPosition(this);
		}
	}

	return childFrameRealPosition;
}

sf::Vector2f Frame::getChildRelativePosition(Frame* childFrame) {
	if (childFrame->relativePositionCalculated)
		return childFrame->getRelativePosition();

	sf::Vector2f childRealPosition = childFrame->getPosition();
	sf::Vector2f myRealPosition = getPosition();
	sf::Vector2f mySize = getSize();

	return sf::Vector2f((childRealPosition.x - myRealPosition.x) / mySize.x,
						(childRealPosition.y - myRealPosition.y) / mySize.y);
}

void Frame::mouseMovement(sf::Event::MouseMoveEvent mouseMoveEvent) {
	if (activeBool) {
		sf::FloatRect globalBounds = getGlobalBounds();

		bool prevMouseOver = mouseOver;
		mouseOver = getGlobalBounds().contains(sf::Vector2f(mouseMoveEvent.x, mouseMoveEvent.y));

		if (mouseOver) {
			if (!prevMouseOver)
				onMouseOver();
		}
		else
			if (prevMouseOver)
				onMouseBackNormal();
	}
	
	for (Frame* frame : frames)
		frame->mouseMovement(mouseMoveEvent);
}

void Frame::mouseButtonChange(sf::Event::EventType type, sf::Event::MouseButtonEvent mouseButtonEvent) {
	if (mouseButtonEvent.button == sf::Mouse::Button::Left)
		if (type == sf::Event::MouseButtonReleased || type == sf::Event::MouseButtonPressed)
			for (Frame* frame : frames)
				frame->mouseButtonChange(type, mouseButtonEvent);
}

void Frame::keyboardButtonChange(sf::Event::EventType type, sf::Event::KeyEvent keyboardEvent) {
	if (activeBool)
		for (Frame* frame : frames)
			frame->keyboardButtonChange(type, keyboardEvent);
}

void Frame::textEntered(sf::Event::TextEvent character) {
	if (activeBool)
		for (Frame* frame : frames)
			frame->textEntered(character);
}

void Frame::forceMouseLeft() {
	mouseOver = false;
	onMouseBackNormal();

	for (Frame* frame : frames)
		frame->forceMouseLeft();
}

void Frame::onMouseOver() {
	if (frameState.hasActive(FrameState::object::fillColor, FrameState::mode::hover))
		sf::RectangleShape::setFillColor(fillColorHover);

	if (frameState.hasActive(FrameState::object::outlineThickness, FrameState::mode::hover))
		sf::RectangleShape::setOutlineThickness(outlineThicknessHover);
}

void Frame::onMouseBackNormal() {
	sf::RectangleShape::setFillColor(fillColorNormal);
	sf::RectangleShape::setOutlineColor(outlineColorNormal);
	sf::RectangleShape::setOutlineThickness(outlineThicknessNormal);
}

void Frame::setActivity(bool newActivityBool) {
	activeBool = newActivityBool;

	for (Frame* frame : frames)
		frame->setActivity(newActivityBool);
}

void Frame::draw(sf::RenderWindow& window) {
	if (activeBool) {
		window.draw(*this);

		for (Frame* frame : frames)
			frame->draw(window);
	}
}

void Frame::setFillColor(sf::Color newFillColor) {
	if (!mouseOver || mouseOver && !frameState.hasActive(FrameState::object::fillColor, FrameState::mode::hover))
		RectangleShape::setFillColor(newFillColor);

	fillColorNormal = newFillColor;
}

void Frame::setFillColorOnHover(sf::Color newFillColorOnHover) {
	if (mouseOver || !mouseOver && frameState.hasActive(FrameState::object::fillColor, FrameState::mode::hover))
		RectangleShape::setFillColor(newFillColorOnHover);

	fillColorHover = newFillColorOnHover;
	frameState.setActive(FrameState::object::fillColor, FrameState::mode::hover);
}

void Frame::setOutlineThickness(float thickness) {
	if (!mouseOver || mouseOver && !frameState.hasActive(FrameState::object::outlineThickness, FrameState::mode::hover))
		RectangleShape::setOutlineThickness(thickness);

	outlineThicknessNormal = thickness;
}

void Frame::setOutlineThicknessOnHover(float thickness) {
	if (mouseOver || !mouseOver && frameState.hasActive(FrameState::object::outlineThickness, FrameState::mode::hover))
		RectangleShape::setOutlineThickness(thickness);

	outlineThicknessHover = thickness;
	frameState.setActive(FrameState::object::outlineThickness, FrameState::mode::hover);
}

void Frame::setOutlineColor(sf::Color color) {
	if (!mouseOver || mouseOver && !frameState.hasActive(FrameState::object::outlineColor, FrameState::mode::hover))
		RectangleShape::setOutlineColor(color);
	
	outlineColorNormal = color;
}

void Frame::setOutlineColorHover(sf::Color color) {
	if (mouseOver || !mouseOver && frameState.hasActive(FrameState::object::outlineColor, FrameState::mode::hover))
		RectangleShape::setOutlineColor(color);

	outlineColorHover = color;
	frameState.setActive(FrameState::object::outlineColor, FrameState::mode::hover);
}

void Frame::setHorizontalPadding(float newPadding) {
	assert(newPadding >= 0);
	horizontalPadding = newPadding;

	setWidth(2 * newPadding);
	sizeIsSet = false;
}

void Frame::setVerticalPadding(float newPadding) {
	assert(newPadding >= 0);
	verticalPadding = newPadding;

	setHeight(2 * newPadding);
	sizeIsSet = false;
}

void Frame::addFrame(Frame* newFrame) {
	if (newFrame != nullptr) {
		newFrame->setParentFrame(this);
		frames.push_back(newFrame);
	}
}

void Frame::defaultVisualSetup() {
	setFillColor(sf::Color::White);
	setOutlineColor(sf::Color(70, 70, 70));
	setOutlineThickness(1);

	setFillColorOnHover(sf::Color(210, 210, 210));
	setOutlineColorHover(sf::Color::Black);
	setOutlineThicknessOnHover(2);
}

bool Frame::mouseOverAnyOfItemsRec() {
	for (Frame* childFrame : frames)
		if (childFrame->hasMouseOver())
			return true;

	for (Frame* childFrame : frames)
		if (childFrame->mouseOverAnyOfItemsRec())
			return true;

	return false;
}

bool Frame::parentStructureComplete() {
	if (isPage)
		return true;
	
	if (parentFrame == nullptr)
		return false;

	return parentFrame->parentStructureComplete();
}

void Frame::update() {
	sf::RectangleShape::update();

	if (!isPage && parentStructureComplete()) {
		updatePositions();
	}
}

Frame::~Frame() {
	for (Frame* frame : frames)
		delete frame;
}


							/////////////////////
							// TextLabel Class //
							/////////////////////


TextLabel::TextLabel() {
	//defaultVisualSetup();
	textAlignment = left;
	onMouseBackNormal();
}

TextLabel::TextLabel(sf::String text) : TextLabel() {
	setString(text);
}

void TextLabel::onMouseOver() {
	Frame::onMouseOver();

	//cout << "TextLabel::onMouseOver" << endl;

	if (frameState.hasActive(FrameState::object::textColor, FrameState::mode::hover))
		sf::Text::setFillColor(textColorHover);
}

void TextLabel::onMouseBackNormal() {
	Frame::onMouseBackNormal();
	sf::Text::setFillColor(textColorNormal);
}

void TextLabel::setCharacterSize(unsigned int size) {
	sf::Text::setCharacterSize(size);
	update();
}

void TextLabel::setString(sf::String newText) {
	sf::Text::setString(newText);
	update();
}

void TextLabel::setTextColor(sf::Color color) {
	if (!mouseOver)
		sf::Text::setFillColor(color);

	textColorNormal = color;
}

void TextLabel::setTextColorOnHover(sf::Color color) {
	if (mouseOver)
		sf::Text::setFillColor(color);

	textColorHover = color;
	frameState.setActive(FrameState::object::textColor, FrameState::mode::hover);
}

void TextLabel::setRelativePosition(sf::Vector2f newRelativePostion) {
	Frame::setRelativePosition(newRelativePostion);
	update();
}

void TextLabel::setRelativePosition(float xRelativePosition, float yRelativePosition) {
	setRelativePosition(sf::Vector2f(xRelativePosition, yRelativePosition));
}

void TextLabel::setPosition(sf::Vector2f newPosition) {
	Frame::setPosition(newPosition);
	update();
}

void TextLabel::setTextAlignment(string alignment) {
	if (alignment == "center")
		textAlignment = center;
	else if (alignment == "left")
		textAlignment = left;

	updateText();
}

void TextLabel::setHorizontalPadding(float newPadding) {
	sizeIsSet = false;
	Frame::setHorizontalPadding(newPadding);
	update();
}

void TextLabel::setVerticalPadding(float newPadding) {
	sizeIsSet = false;
	Frame::setVerticalPadding(newPadding);
	update();
}

void TextLabel::updatePositions() {
	Frame::updatePositions();
	updateText();
}

sf::Vector2f TextLabel::getRealTextPosition() {
	sf::FloatRect textGlobalBounds = sf::Text::getGlobalBounds();
	sf::FloatRect textLocalBounds = sf::Text::getLocalBounds();

	return sf::Vector2f(textGlobalBounds.left - textLocalBounds.left, textGlobalBounds.top - textLocalBounds.top);
}

void TextLabel::setTextPosition(sf::Vector2f newPosition) {
	sf::Text::setPosition(newPosition);

	sf::FloatRect textGlobalBounds = sf::Text::getGlobalBounds();

	sf::Text::setPosition(newPosition + newPosition - sf::Vector2f(textGlobalBounds.left, textGlobalBounds.top));
}

void TextLabel::defaultVisualSetup() {
	Frame::defaultVisualSetup();

	setTextColor(sf::Color::Black);
	setTextColorOnHover(sf::Color(50, 50, 50));
}

void TextLabel::updateText() {
	sf::FloatRect textGlobalBounds = sf::Text::getGlobalBounds();
	
	if (!sizeIsSet)
		Frame::justSetSize(sf::Vector2f(textGlobalBounds.width + 2 * horizontalPadding,
										textGlobalBounds.height + 2 * verticalPadding));
	else
		Frame::justSetSize(getSize());

	float textNewXpos;
	float textNewYpos;
	
	if (textAlignment == center) {
		textNewXpos = (realPosition.x + (getSize().x - textGlobalBounds.width) / 2);
		textNewYpos = (realPosition.y + (getSize().y - textGlobalBounds.height) / 2);
	}
	else if (textAlignment == left) {
		textNewXpos = realPosition.x + horizontalPadding;
		textNewYpos = realPosition.y + verticalPadding;
	}

	setTextPosition(sf::Vector2f(textNewXpos, textNewYpos));
}

void TextLabel::setFont(sf::Font& newFont) {
	sf::Text::setFont(newFont);
	update();
}

void TextLabel::setSize(sf::Vector2f newSize) {
	Frame::setSize(newSize);
	updateText();
	sizeIsSet = true;
}

void TextLabel::draw(sf::RenderWindow& window) {
	if (activeBool) {
		Frame::draw(window);
		window.draw(*(sf::Text*)this);
	}
}

sf::Vector2f TextLabel::getSize() {
	return Frame::getSize();
}

void TextLabel::update() {
	Frame::update();
	updateText();
}


						//////////////////////
						// ImageLabel Class //
						//////////////////////

ImageLabel::ImageLabel() {
	activeSprite = &spriteNormal;

	spritesToUpdate.push_back(&spriteNormal);
	spritesToUpdate.push_back(&spriteHover);

	parentFrame = nullptr;
	setFillColor(sf::Color::Transparent);
}

void ImageLabel::mouseMovement(sf::Event::MouseMoveEvent mouseMoveEvent) {
	Frame::mouseMovement(mouseMoveEvent);
}

void ImageLabel::setTexture(sf::Texture& newTexture) {
	if (!mouseOver)
		activeSprite = &spriteNormal;

	spriteNormal.setTexture(newTexture);
}

void ImageLabel::setTextureOnHover(sf::Texture& newHoverTexture) {
	if (mouseOver)
		activeSprite = &spriteHover;

	spriteHover.setTexture(newHoverTexture);
	frameState.setActive(FrameState::object::image, FrameState::mode::hover);
}

void ImageLabel::setResolution(sf::Vector2f newResolution) {
	setResolution(newResolution.x, newResolution.y);
}

void ImageLabel::setResolution(float x, float y) {
	myResolution = sf::Vector2f(x, y);

	if (activeSprite->getTexture() != nullptr) {
		sf::Vector2f textureSize = getTextureSize();
		float xScaleFactor = x / textureSize.x;
		float yScaleFactor = y / textureSize.y;

		for (sf::Sprite* sprite : spritesToUpdate)
			sprite->setScale(xScaleFactor, yScaleFactor);

		setSize(myResolution);
	}
}

void ImageLabel::draw(sf::RenderWindow& window) {
	Frame::draw(window);
	window.draw(*activeSprite);
	
	for (Frame* frame : frames)
		frame->draw(window);
}

void ImageLabel::setHorizontalPadding(float newPadding) {
	Frame::setHorizontalPadding(newPadding);

	sf::FloatRect spriteRect = activeSprite->getGlobalBounds();
	setSize(sf::Vector2f(spriteRect.width + 2 * horizontalPadding,
						 spriteRect.height + 2 * verticalPadding));

	spriteNormal.setPosition(Frame::getPosition() + sf::Vector2f(horizontalPadding, verticalPadding));
	spriteHover.setPosition(Frame::getPosition() + sf::Vector2f(horizontalPadding, verticalPadding));
}

void ImageLabel::setVerticalPadding(float newPadding) {
	Frame::setVerticalPadding(newPadding);

	sf::FloatRect spriteRect = activeSprite->getGlobalBounds();
	setSize(sf::Vector2f(spriteRect.width + 2 * horizontalPadding,
						spriteRect.height + 2 * verticalPadding));

	spriteNormal.setPosition(Frame::getPosition() + sf::Vector2f(horizontalPadding, verticalPadding));
	spriteHover.setPosition(Frame::getPosition() + sf::Vector2f(horizontalPadding, verticalPadding));
}

void ImageLabel::setPosition(sf::Vector2f newPosition) {
	Frame::setPosition(newPosition);

	//cout << "ImageLabel::setPosition to [" << newPosition.x << ", " << newPosition.y << "]" << endl;

	for (sf::Sprite* sprite : spritesToUpdate)
		sprite->setPosition(newPosition);
}

void ImageLabel::setRelativePosition(sf::Vector2f newRelativePosition) {
	relativePositionCalculated = true;
	Frame::setRelativePosition(newRelativePosition);
	
	//cout << "ImageLabel::setRelativePosition to [" << newRelativePosition.x << ", " << newRelativePosition.y << "]"
	//	 << "RealPC = " << realPositionCalculated << ", RelPC = " << relativePositionCalculated << endl;
}

void ImageLabel::justSetRealPosition(sf::Vector2f newPosition) {
	Frame::justSetRealPosition(newPosition);
}

void ImageLabel::justSetRelativePosition(sf::Vector2f newRelativePosition) {
	Frame::justSetRelativePosition(newRelativePosition);
}

sf::Vector2f ImageLabel::getTextureSize() {
	if (activeSprite->getTexture() != nullptr)
		return sf::Vector2f(activeSprite->getTexture()->getSize());
	else
		throw std::runtime_error("Has no texture");
}

void ImageLabel::updatePositions() {
	Frame::updatePositions();

	if (realPositionCalculated)
		updateImage();
}

void ImageLabel::onMouseOver() {
	if (frameState.hasActive(FrameState::object::image, FrameState::mode::hover))
		activeSprite = &spriteHover;
}

void ImageLabel::onMouseBackNormal() {
	activeSprite = &spriteNormal;
}

void ImageLabel::defaultVisualSetup() {
	Frame::defaultVisualSetup();

	setResolution(DEFAULT_HORIZONTAL_PADDING * 2, DEFAULT_VERTICAL_PADDING * 2);
}

void ImageLabel::updateImage() {
	for (sf::Sprite* sprite : spritesToUpdate)
		sprite->setPosition(realPosition);

	setResolution(myResolution);
}



						////////////////
						//Button Class//
						////////////////



Button::Button(sf::SoundBuffer& hoverSound, sf::SoundBuffer& clickSound) {
	sounds[0].setBuffer(hoverSound);
	sounds[1].setBuffer(clickSound);

	//defaultVisualSetup();
}

void Button::mouseButtonChange(sf::Event::EventType eventType, sf::Event::MouseButtonEvent mouseButtonEvent) {
	if (activeBool) {
		Frame::mouseButtonChange(eventType, mouseButtonEvent);

		if (eventType == sf::Event::EventType::MouseButtonPressed)
			onButtonPress();
		else if (eventType == sf::Event::EventType::MouseButtonReleased) {
			onButtonRelease();
			clicked = false;
		}
	}
}

void Button::setFillColorOnButtonPress(sf::Color color) {
	if (clicked)
		sf::RectangleShape::setFillColor(color);

	fillColorClick = color;
	frameState.setActive(FrameState::object::fillColor, FrameState::mode::click);
}

void Button::setOutlineColorOnButtonPress(sf::Color color) {
	if (clicked)
		sf::RectangleShape::setFillColor(color);

	outlineColorClick = color;
	frameState.setActive(FrameState::object::outlineColor, FrameState::mode::click);
}

void Button::setOutlineThicknessOnButtonPress(float thickness) {
	if (clicked)
		sf::RectangleShape::setOutlineThickness(thickness);

	outlineThicknessClick = thickness;
	frameState.setActive(FrameState::object::outlineThickness, FrameState::mode::click);
}

void Button::onButtonPress() {
	if (activeBool) {
		if (mouseOver) {
			clicked = true;

			if (frameState.hasActive(FrameState::object::fillColor, FrameState::mode::click))
				sf::RectangleShape::setFillColor(fillColorClick);

			if (frameState.hasActive(FrameState::object::outlineColor, FrameState::mode::click))
				sf::RectangleShape::setOutlineColor(outlineColorClick);

			if (frameState.hasActive(FrameState::object::outlineThickness, FrameState::mode::click))
				sf::RectangleShape::setOutlineThickness(outlineThicknessClick);
		}
	}
}

void Button::onButtonRelease() {
	if (activeBool) {
		if (mouseOver && clicked) {
			if (frameState.hasActive(FrameState::object::fillColor, FrameState::mode::hover))
				sf::RectangleShape::setFillColor(fillColorHover);

			if (frameState.hasActive(FrameState::object::outlineColor, FrameState::mode::hover))
				sf::RectangleShape::setOutlineColor(outlineColorHover);

			if (frameState.hasActive(FrameState::object::outlineThickness, FrameState::mode::hover))
				sf::RectangleShape::setOutlineThickness(outlineThicknessHover);
		}
		else {
			sf::RectangleShape::setFillColor(fillColorNormal);
			sf::RectangleShape::setOutlineColor(outlineColorNormal);
			sf::RectangleShape::setOutlineThickness(outlineThicknessNormal);
		}

		if (hasFunctionBool && mouseOver && clicked)
			executeFunction();
	}
}

void Button::onMouseOver() {
	if (activeBool) {
		if (clicked) {
			if (frameState.hasActive(FrameState::object::fillColor, FrameState::mode::click))
				sf::RectangleShape::setFillColor(fillColorClick);

			if (frameState.hasActive(FrameState::object::outlineColor, FrameState::mode::click))
				sf::RectangleShape::setOutlineColor(outlineColorClick);

			if (frameState.hasActive(FrameState::object::outlineThickness, FrameState::mode::click))
				sf::RectangleShape::setOutlineThickness(outlineThicknessClick);
		}
		else
			Frame::onMouseOver();
	}
}

void Button::onMouseBackNormal() {
	if (activeBool) {
		if (clicked) {
			if (frameState.hasActive(FrameState::object::fillColor, FrameState::mode::click))
				sf::RectangleShape::setFillColor(fillColorClick);

			if (frameState.hasActive(FrameState::object::outlineColor, FrameState::mode::click))
				sf::RectangleShape::setOutlineColor(outlineColorClick);

			if (frameState.hasActive(FrameState::object::outlineThickness, FrameState::mode::click))
				sf::RectangleShape::setOutlineThickness(outlineThicknessClick);
		}
		else
			Frame::onMouseBackNormal();
	}
}

void Button::executeFunction() {
	functionHolder->run();
}

void Button::removeFunction() {
	if (hasFunctionBool)
		delete functionHolder;

	hasFunctionBool = false;
	functionHolder = nullptr;
}

void Button::defaultVisualSetup() {
	Frame::defaultVisualSetup();

	setFillColorOnButtonPress(sf::Color::White);
	setOutlineColorOnButtonPress(sf::Color::Black);
	setOutlineThicknessOnButtonPress(1);
}

Button::~Button() {
	if (hasFunctionBool)
		delete functionHolder;
}


						////////////////////
						//TextButton Class//
						////////////////////


TextButton::TextButton(sf::String text) {
	TextLabel::setString(text);

	//defaultVisualSetup();
	onMouseBackNormal();
}

void TextButton::setTextColorOnButtonPress(sf::Color color) {
	if (clicked)
		sf::Text::setFillColor(color);

	textColorClick = color;
	frameState.setActive(FrameState::object::textColor, FrameState::mode::click);
}

void TextButton::onButtonRelease() {
	Button::onButtonRelease();

	if (mouseOver) {
		if (frameState.hasActive(FrameState::object::textColor, FrameState::mode::hover))
			sf::Text::setFillColor(textColorHover);
	}
	else
		sf::Text::setFillColor(textColorNormal);
}

void TextButton::onMouseOver() {
	Button::onMouseOver();

	if (clicked) {
		if (frameState.hasActive(FrameState::object::textColor, FrameState::mode::click))
			sf::Text::setFillColor(textColorClick);
	}
	else
		if (frameState.hasActive(FrameState::object::textColor, FrameState::mode::hover))
			sf::Text::setFillColor(textColorHover);
}

void TextButton::onMouseBackNormal() {
	Button::onMouseBackNormal();

	if (clicked) {
		if (frameState.hasActive(FrameState::object::textColor, FrameState::mode::click))
			sf::Text::setFillColor(textColorClick);
	}
	else
		sf::Text::setFillColor(textColorNormal);
}


						/////////////////////
						//ImageButton Class//
						/////////////////////


ImageButton::ImageButton() {
	spritesToUpdate.push_back(&spriteClick);
}

void ImageButton::setResolution(sf::Vector2f newResolution) {
	ImageLabel::setResolution(newResolution);
}

void ImageButton::setTextureOnButtonPress(sf::Texture& texture) {
	if (clicked)
		activeSprite = &spriteClick;

	spriteClick.setTexture(texture);
	frameState.setActive(FrameState::object::image, FrameState::mode::click);
}

void ImageButton::onButtonPress() {
	Button::onButtonPress();

	if (mouseOver)
		if (frameState.hasActive(FrameState::object::image, FrameState::mode::click))
			activeSprite = &spriteClick;
}

void ImageButton::onButtonRelease() {
	Button::onButtonRelease();

	if (mouseOver) {
		if (frameState.hasActive(FrameState::object::image, FrameState::mode::hover))
			activeSprite = &spriteHover;
	}
	else
		activeSprite = &spriteNormal;
}

void ImageButton::onMouseOver() {
	Button::onMouseOver();

	if (clicked) {
		if (frameState.hasActive(FrameState::object::image, FrameState::mode::click))
			activeSprite = &spriteClick;
	}
	else
		if (frameState.hasActive(FrameState::object::image, FrameState::mode::hover))
			activeSprite = &spriteHover;
}

void ImageButton::onMouseBackNormal() {
	Button::onMouseBackNormal();

	if (clicked) {
		if (frameState.hasActive(FrameState::object::image, FrameState::mode::click))
			activeSprite = &spriteClick;
	}
	else
		activeSprite = &spriteNormal;
}


						///////////////////
						//TextEntry Class//
						///////////////////


TextEntry::TextEntry(TimeManager* newTimeManager) : TextEntry(newTimeManager, "") {
	
}

TextEntry::TextEntry(TimeManager* newTimeManager, sf::String text) : TextButton(text) {
	timeManager = newTimeManager;
	blinkCursor();
	setString(text);

	cursorWidth = 3;
	cursorRect.setSize(sf::Vector2f(3, cursorRect.getSize().y));
	setTextAlignment("left");

	//defaultVisualSetup();
	onMouseBackNormal();
}

void TextEntry::draw(sf::RenderWindow& window) {
	TextButton::draw(window);

	if (cursorVisible && hasFocus)
		window.draw(cursorRect);
}

void TextEntry::mouseMovement(sf::Event::MouseMoveEvent mouseMoveEvent) {
	Button::mouseMovement(mouseMoveEvent);
}

void TextEntry::keyboardButtonChange(sf::Event::EventType type, sf::Event::KeyEvent keyboardEvent) {
	Frame::keyboardButtonChange(type, keyboardEvent);

	if (type == sf::Event::KeyPressed) {
		if (keyboardEvent.code == sf::Keyboard::Left || keyboardEvent.code == sf::Keyboard::Right)
			moveCursor(keyboardEvent.code == sf::Keyboard::Right);
	}
}

void TextEntry::textEntered(sf::Event::TextEvent character) {
	bool wasBackSpace = false;

	if (hasFocus) {
		int prevCursorTextPos = cursorTextPos;
		int prevLeftIndex = shownTextLeftIndex;
		int prevRightIndex = shownTextRightIndex;

		if (character.unicode == 8) {  // ascii for Backspace
			backSpacePressed();
			wasBackSpace = true;
		}
		else if (character.unicode == 13) {  // ascii for Enter
			enterPressed();
			return;
		}
		else
			insertCharacterAtCursorPosition(character);

		cursorVisible = true;

		// updating shown text
		float textEntryWidth = getSize().x - horizontalPadding * 3;
		float entireTextPixelWidth = getTextWidthFromString(entireText);
		float textToShowPixelWidth;
		int i;
		sf::String textToShow;

		if (entireTextPixelWidth > textEntryWidth) {
			if (wasBackSpace && prevRightIndex == entireText.getSize())
				setTextStringMadeFromRight(entireText.getSize() - 1);
			else if (prevCursorTextPos > prevLeftIndex && prevCursorTextPos <= prevRightIndex)
				setTextStringMadeFromLeft(cursorTextPos, entireText.substring(shownTextLeftIndex, cursorTextPos - shownTextLeftIndex));
			else if (prevCursorTextPos == prevRightIndex + 1)  // cursor was on the most right
				setTextStringMadeFromRight(prevCursorTextPos - (wasBackSpace * 2));
			else
				cout << "case else, previousCursorTextPosition = " << prevCursorTextPos
					 << ", cursorTextPosition = " << cursorTextPos
					 << ", leftIndex = " << shownTextLeftIndex << ", rightIndex = " << shownTextRightIndex << endl;

		}
		else {
			shownTextLeftIndex = 0;
			shownTextRightIndex = entireText.getSize() - 1;

			setStringFromIndexes();
		}
	}

	updateCursor();
}

void TextEntry::backSpacePressed() {
	if (cursorTextPos != 0) {
		sf::String firstPart = entireText.substring(0, cursorTextPos - 1);

		entireText = firstPart + entireText.substring(cursorTextPos, entireText.getSize() - firstPart.getSize() + 1);
		setCursorTextPosition(cursorTextPos - 1);
		shownTextRightIndex--;
	}
}

void TextEntry::enterPressed() {
	functionHolder->run();
}

void TextEntry::insertCharacterAtCursorPosition(sf::Event::TextEvent character) {
	sf::String firstPart = entireText.substring(0, cursorTextPos);
	sf::String secondPart = entireText.substring(cursorTextPos);

	entireText = firstPart + character.unicode + secondPart;
	setCursorTextPosition(cursorTextPos + 1);
}

void TextEntry::moveCursor(bool direction) {
	int prevCursorTextPos = cursorTextPos;

	if (direction) {
		if (cursorTextPos != entireText.getSize())
			setCursorTextPosition(cursorTextPos + 1);
	}
	else
		if (cursorTextPos != 0)
			setCursorTextPosition(cursorTextPos - 1);

	// setting sf::Text's sf::String if cursor moved from view
	if (prevCursorTextPos != cursorTextPos) {
		if (direction) {
			if (prevCursorTextPos - 1 == shownTextRightIndex)
				setTextStringMadeFromRight(shownTextRightIndex + 1);
		}
		else
			if (prevCursorTextPos == shownTextLeftIndex) {
				sf::String textToPass = entireText[shownTextLeftIndex - 1] + sf::Text::getString();
				textToPass = textToPass.substring(0, textToPass.getSize() - 1);

				setTextStringMadeFromLeft(shownTextLeftIndex - 1 + textToPass.getSize(), textToPass);
			}
	}
	cursorVisible = true;
	updateCursor();
}

void TextEntry::updateCursor() {
	sf::Vector2f textPosition = sf::Text::getPosition();
	float cursorPosX = 3 + textPosition.x + getTextWidthFromString(entireText.substring(shownTextLeftIndex,
																						cursorTextPos - shownTextLeftIndex));

	cursorPosition = sf::Vector2f(cursorPosX, textPosition.y);
	cursorRect.setPosition(cursorPosition);
}

void TextEntry::setCursorTextPosition(int position) {
	cursorTextPos = position;
}

float TextEntry::getTextWidthFromString(sf::String str) {
	sf::String previousShownString = getString();

	sf::Text::setString(str);
	float width = sf::Text::getGlobalBounds().width;
	sf::Text::setString(previousShownString);

	return width;
}

void TextEntry::setCharacterSize(unsigned int size) {
	TextButton::setCharacterSize(size);

	cursorHeight = size * 1.1;
	cursorRect.setSize(sf::Vector2f(cursorWidth, cursorHeight));
}

void TextEntry::setFillColorOnFocus(sf::Color color) {
	if (hasFocus)
		sf::RectangleShape::setFillColor(color);

	fillColorFocus = color;
	frameState.setActive(FrameState::object::fillColor, FrameState::mode::focus);
}

void TextEntry::setOutlineColorOnFocus(sf::Color color) {
	if (hasFocus)
		sf::RectangleShape::setFillColor(color);

	outlineColorFocus = color;
	frameState.setActive(FrameState::object::outlineColor, FrameState::mode::focus);
}

void TextEntry::setOutlineThicknessOnFocus(float thickness) {
	if (hasFocus)
		sf::RectangleShape::setOutlineThickness(thickness);

	outlineThicknessFocus = thickness;
	frameState.setActive(FrameState::object::outlineThickness, FrameState::mode::focus);
}

void TextEntry::setString(sf::String newText) {
	entireText = newText;
	setCursorTextPosition(newText.getSize());
	float textEntryWidth = getSize().x - horizontalPadding * 3;
	
	// updating shown text
	float entireTextPixelWidth = getTextWidthFromString(entireText);

	if (entireTextPixelWidth < textEntryWidth) {
		shownTextLeftIndex = 0;
		shownTextRightIndex = entireText.getSize() - 1;
		sf::Text::setString(entireText);
	}
	else {
		sf::String textToShow = entireText;
		float textToShowPixelWidth = entireTextPixelWidth;
		int i = 0;

		while (textToShowPixelWidth >= textEntryWidth && i < entireText.getSize() - 1) {
			textToShow = textToShow.substring(1);  // until the end of the string
			textToShowPixelWidth = getTextWidthFromString(textToShow);
			i++;
		}

		shownTextLeftIndex = i;
		shownTextRightIndex = entireText.getSize() - 1;
		sf::Text::setString(textToShow);
	}

	updateCursor();
}

void TextEntry::blinkCursor() {
	if (hasFocus) {
		cursorVisible = cursorVisible ? false : true;
		timeManager->setTimeout(&TextEntry::blinkCursor, this, 500);
	}
}

void TextEntry::setTextStringMadeFromLeft(int index, sf::String startingString) {
	float textEntryWidth = getSize().x - horizontalPadding * 3;
	sf::String textToShow = startingString + entireText[index];
	float textToShowPixelWidth = getTextWidthFromString(textToShow);

	if (textToShowPixelWidth > textEntryWidth) {
		cout << "(from left) longer, textToShow = " << textToShow.toAnsiString() << endl;

		while (textToShowPixelWidth > textEntryWidth) {
			textToShow = textToShow.substring(0, textToShow.getSize() - 1);
			textToShowPixelWidth = getTextWidthFromString(textToShow);
		}

		shownTextLeftIndex = index - startingString.getSize();
		shownTextRightIndex = shownTextLeftIndex + textToShow.getSize() - 1;

		cout << "(from left) final textToShow = " << textToShow.toAnsiString() << endl;

		setStringFromIndexes();

		return;
	}
	cout << "(from left) normal, textToShow = " << textToShow.toAnsiString() << endl;

	int i = index + 1;

	while (textToShowPixelWidth < textEntryWidth && i < entireText.getSize()) {
		textToShow += entireText[i];
		textToShowPixelWidth = getTextWidthFromString(textToShow);
		i++;
	}

	cout << "(from left) final textToShow = " << textToShow.toAnsiString() << endl;

	shownTextLeftIndex = index - startingString.getSize();
	shownTextRightIndex = shownTextLeftIndex + textToShow.getSize() - 1;

	setStringFromIndexes();
}

void TextEntry::setTextStringMadeFromRight(int index, sf::String startingString) {
	float textEntryWidth = getSize().x - horizontalPadding * 3;
	sf::String textToShow = entireText[index] + startingString;
	float textToShowPixelWidth = getTextWidthFromString(textToShow);
	int i = index - 1;

	if (textToShowPixelWidth > textEntryWidth) {
		cout << "(from right) larger, textToShow = " << textToShow.toAnsiString() << endl;

		while (textToShowPixelWidth > textEntryWidth) {
			textToShow = textToShow.substring(1);
			textToShowPixelWidth = getTextWidthFromString(textToShow);
		}

		shownTextRightIndex = index;
		shownTextLeftIndex = shownTextRightIndex - textToShow.getSize() + 1;

		cout << "(from right) final textToShow = " << textToShow.toAnsiString() << endl;

		setStringFromIndexes();
		return;
	}
	cout << "(from right) normal, textToShow = " << textToShow.toAnsiString() << endl;

	while (textToShowPixelWidth < textEntryWidth && i > 0) {
		textToShow = entireText[i] + textToShow;
		textToShowPixelWidth = getTextWidthFromString(textToShow);
		i--;
	}

	cout << "(from right) final textToShow = " << textToShow.toAnsiString() << endl;

	shownTextRightIndex = index;
	shownTextLeftIndex = shownTextRightIndex - (index - i - 1);

	setStringFromIndexes();
}

void TextEntry::onButtonPress() {
	if (mouseOver) {
		if (frameState.hasActive(FrameState::object::fillColor, FrameState::mode::click))
			sf::RectangleShape::setFillColor(fillColorClick);  // looking partially focused and hovered

		if (frameState.hasActive(FrameState::object::outlineColor, FrameState::mode::click))
			sf::RectangleShape::setOutlineColor(outlineColorClick);

		if (frameState.hasActive(FrameState::object::outlineThickness, FrameState::mode::click))
			sf::RectangleShape::setOutlineThickness(outlineThicknessClick);
	}
}

void TextEntry::onButtonRelease() {
	bool hadFocusBefore = hasFocus;
	hasFocus = mouseOver && clicked;

	if (hasFocus && !hadFocusBefore) {
		blinkCursor();
		cursorVisible = true;
	}

	if (hasFocus) {
		if (frameState.hasActive(FrameState::object::fillColor, FrameState::mode::hover))
			sf::RectangleShape::setFillColor(fillColorHover);  // looking partially focused and hovered

		if (frameState.hasActive(FrameState::object::outlineColor, FrameState::mode::focus))
			sf::RectangleShape::setOutlineColor(outlineColorFocus);

		if (frameState.hasActive(FrameState::object::outlineThickness, FrameState::mode::focus))
			sf::RectangleShape::setOutlineThickness(outlineThicknessFocus);
	}
	else
		TextButton::onButtonRelease();
}

void TextEntry::onMouseOver() {
	if (hasFocus) {
		if (frameState.hasActive(FrameState::object::fillColor, FrameState::mode::hover))
			sf::RectangleShape::setFillColor(fillColorHover);  // setting fill color back from partial appearance
	}
	else
		TextButton::onMouseOver();
}

void TextEntry::onMouseBackNormal() {
	if (hasFocus) {
		if (frameState.hasActive(FrameState::object::fillColor, FrameState::mode::focus))
			sf::RectangleShape::setFillColor(fillColorFocus);  // setting fill color back from partial appearance

		if (frameState.hasActive(FrameState::object::outlineColor, FrameState::mode::focus))
			sf::RectangleShape::setOutlineColor(outlineColorFocus);

		if (frameState.hasActive(FrameState::object::outlineThickness, FrameState::mode::focus))
			sf::RectangleShape::setOutlineThickness(outlineThicknessFocus);
	}
	else
		TextButton::onMouseBackNormal();
}

void TextEntry::defaultVisualSetup() {
	TextButton::defaultVisualSetup();

	setOutlineThickness(2);
	setOutlineThicknessOnHover(3);
	setOutlineThicknessOnButtonPress(4);
	setOutlineThicknessOnFocus(5);

	setFillColorOnFocus(sf::Color(230, 230, 230));
	setFillColorOnButtonPress(sf::Color(210, 210, 210));
	setOutlineColorOnFocus(sf::Color::Black);

	setCursorColor(sf::Color::Black);

	update();
}

void TextEntry::update() {
	TextButton::update();

	setString(entireText);
}


								////////////////////////
								// DropdownMenu Class //
								////////////////////////


DropdownMenu::DropdownMenu(string text) {
	setString(text);
}

void DropdownMenu::onButtonPress() {
	TextButton::onButtonPress();

	if (mouseOver) {
		if (!isFocused)
			getFocus();
		else
			loseFocus();

		if (!isOpenBool)
			open();
		else
			close();
	}
	else if (!mouseOverAnyOfItems()) {
		close();
		loseFocus();
	}
}

void DropdownMenu::onButtonRelease() {
	TextButton::onButtonRelease();

	if (!mouseOver) {
		close();
		loseFocus();
	}
}

void DropdownMenu::mouseMovement(sf::Event::MouseMoveEvent mouseMoveEvent) {
	TextButton::mouseMovement(mouseMoveEvent);

	mouseOverAnyOfItemsBool = mouseOverAnyOfItems();
}

bool DropdownMenu::mouseOverAnyOfItems() {
	if (mouseOver)
		return false;

	for (Frame* item : items)
		if (item->hasMouseOver())
			return true;

	for (Frame* item : items)
		if (item->mouseOverAnyOfItems())
			return true;

	return false;
}

void DropdownMenu::setActivity(bool newActivityBool) {
	TextButton::setActivity(newActivityBool);
	
	if (newActivityBool)
		for (Frame* item : items)
			item->setActivity(clicked);
}

void DropdownMenu::setRelativePosition(sf::Vector2f newRelativePosition) {
	TextButton::setRelativePosition(newRelativePosition);
	updateItems();
}

void DropdownMenu::setPosition(sf::Vector2f newPosition) {
	TextButton::setPosition(newPosition);
	updateItems();
}

void DropdownMenu::close() {
	cout << "in close(): isOpenBool = " << isOpenBool << endl;

	if (isOpenBool) {
		for (Frame* item : items) {
			item->forceMouseLeft();
			item->setActivity(false);
			item->loseFocus();
		}

		clicked = false;
		isOpenBool = false;
		mouseOverAnyOfItemsBool = false;
	}
}

void DropdownMenu::open() {
	cout << "in open(): isOpenBool = " << isOpenBool << endl;

	if (!isOpenBool) {
		for (Frame* item : items) {
			item->setActivity(true);
			item->forceMouseLeft();
		}

		isOpenBool = true;
		getFocus();
	}
}

void DropdownMenu::getFocus() {
	Frame::getFocus();
}

void DropdownMenu::loseFocus() {
	Frame::loseFocus();

	for (Frame* item : items)
		item->loseFocus();
}

void DropdownMenu::addItem(Frame* newItem) {
	if (positionsCalculated())
		newItem->setPosition(getPosition().x, getPosition().y + getSize().y + currentItemsHeight);

	//cout << "y size = " << getSize().y << ", positionsCalculated = " << positionsCalculated() << endl;
	//cout << "RealPC = " << realPositionCalculated << ", RelPC = " << relativePositionCalculated << endl;

	items.push_back(newItem);

	if (newItem->getSize().x > largestItemWidth) {
		largestItemWidth = newItem->getSize().x;

		for (Frame* item : items)
			item->setWidth(largestItemWidth);
	}
	else
		newItem->setWidth(largestItemWidth);

	newItem->setActivity(clicked);
	currentItemsHeight += newItem->getSize().y;
	addFrame(newItem);
}

void DropdownMenu::updatePositions() {
	if (parentStructureComplete()) {
		TextButton::updatePositions();
		updateItems();
	}
}

void DropdownMenu::updateItems() {
	float currentHeight = 0;

	for (Frame* frame : items) {
		frame->setPosition(sf::Vector2f(getPosition().x, realPosition.y + getSize().y + currentHeight));
		currentHeight += frame->getSize().y;
	}
}


								///////////////////////////
								// SelectButtonBar Class //
								///////////////////////////


SelectButtonBar::SelectButtonBar() {
	sf::Color fillColor = sf::Color(170, 210, 250);

	setFillColorOfSelectedItem(fillColor);
	setItemsFillColorOnHover(sf::Color(200, 223, 250, 200));
	setItemsFillColorOnButtonPress(fillColor);
	setOutlineColorOfSelectedItem(sf::Color(90, 160, 250));
	setOutlineThicknessOfSelectedItem(2);
}

void SelectButtonBar::addItem(Button* newItem, string description) {
	/*
	BaseFunction* f;

	if (newItem->hasFunction()) {
		f = newItem->getFunction();

		newItem->setFunction(
			[newItem, this, f] {
				this->select(newItem);
				f->run();
			}
		);
	}
	else*/
		newItem->setFunction(&SelectButtonBar::select, this, description);

	items.push_back(pair<Button*, string>(newItem, description));
	newItem->setFillColorOnHover(allitemsFillColorOnHover);
	newItem->setFillColorOnButtonPress(allitemsFIllColorOnButtonPress);
	newItem->setActivity(activeBool);
	addFrame(newItem);

	if (items.size() == 1)
		select(description);
}

void SelectButtonBar::select(string description) {
	if (selectedButton != nullptr)
		deselectItemVisually(selectedButton);

	if (descriptionExists(description)) {
		selectedDescription = description;
		selectedButton = getItemByDescription(description);
		selectItemVisually(selectedButton);
	}

	cout << "selecting item with description: " << description << endl;
}

void SelectButtonBar::select(Button* button) {
	if (selectedButton != nullptr)
		deselectItemVisually(selectedButton);

	for (pair<Button*, string> p : items) {
		if (p.first == button) {
			selectedButton = button;
			selectedDescription = p.second;
			selectItemVisually(selectedButton);
			return;
		}
	}

	cout << "selecting item with description: " << selectedDescription << endl;
}

vector<string> SelectButtonBar::getDescriptions() {
	vector<string> descriptions;

	std::transform(begin(items), end(items),
		std::back_inserter(descriptions),
		[](auto const& pair) { return pair.second; });

	return descriptions;
}

void SelectButtonBar::setFillColorOfSelectedItem(sf::Color color) {
	selectedItemFillColor = color;
}

void SelectButtonBar::setItemsFillColorOnButtonPress(sf::Color color) {
	allitemsFIllColorOnButtonPress = color;
}

void SelectButtonBar::setItemsFillColorOnHover(sf::Color color) {
	allitemsFillColorOnHover = color;
}

void SelectButtonBar::setOutlineColorOfSelectedItem(sf::Color color) {
	selectedItemOutlineColor = color;
}

void SelectButtonBar::setOutlineThicknessOfSelectedItem(const float thickness) {
	selectedItemOutlineThickness = thickness;
}

bool SelectButtonBar::descriptionExists(string description) {
	return getItemByDescription(description) != nullptr;
}

Button* SelectButtonBar::getItemByDescription(string description) {
	for (pair<Button*, string> p : items)
		if (p.second == description)
			return p.first;

	return nullptr;
}

void SelectButtonBar::deselectItemVisually(Button* item) {
	selectedButton->setFillColor(selectedItemFillColorOriginal);
	selectedButton->setOutlineColor(selectedItemOutlineColorOriginal);
	selectedButton->setOutlineThickness(selectedItemOutlineThicknessOriginal);
}

void SelectButtonBar::selectItemVisually(Button* item) {
	selectedItemFillColorOriginal = item->getFillColorNormal();
	selectedItemOutlineColorOriginal = item->getOutlineColorNormal();
	selectedItemOutlineThicknessOriginal = item->getOutlineThicknessNormal();

	selectedButton->setFillColor(selectedItemFillColor);
	selectedButton->setOutlineColor(selectedItemOutlineColor);
	selectedButton->setOutlineThickness(selectedItemOutlineThickness);
}


							///////////////////////////
							// DropdownMenuRow Class //
							///////////////////////////


void DropdownMenuRow::addItem(DropdownMenu* newItem) {
	if (newItem != nullptr) {
		items.push_back(newItem);
		newItem->setActivity(activeBool);
		addFrame(newItem);
	}
}

void DropdownMenuRow::mouseMovement(sf::Event::MouseMoveEvent mouseMoveEvent) {
	Frame::mouseMovement(mouseMoveEvent);

	if (isFocused) {
		if (openItem != nullptr && !openItem->hasMouseOver() && mouseOverAnyOfItems()) {
			closeOpenedItem();

			for (DropdownMenu* item : items) {
				if (item->hasMouseOver()) {
					item->open();
					item->loseFocus();  // due to repetitive mouse event checking
					openItem = item;
					break;
				}
			}
		}
	}
}

void DropdownMenuRow::mouseButtonChange(sf::Event::EventType type, sf::Event::MouseButtonEvent mouseButtonEvent) {
	if (!activeBool) {
		Frame::mouseButtonChange(type, mouseButtonEvent);
		return;
	}

	cout << "______DropdownMenuRow::mouseButtonChange begin_______" << endl;

	bool mouseReleasedOverButtonInSubitem = false;

	if (mouseButtonEvent.button == sf::Mouse::Left && type == sf::Event::MouseButtonReleased) {
		cout << "released" << endl;
		cout << "before, openItem = " << openItem << endl;

		if (openItem != nullptr) {
			if (openItem->mouseOverAnyOfItems())
				mouseReleasedOverButtonInSubitem = true;
		}

		cout << "after, openItem = " << openItem << endl;
	}

	Frame::mouseButtonChange(type, mouseButtonEvent);
	
	if (mouseReleasedOverButtonInSubitem) {  // needs to be done after Frame::mouseButtonChange
		cout << "mouseReleasedOverButtonInSubitem = " << mouseReleasedOverButtonInSubitem << endl;

		if (openItem != nullptr) {
			loseFocus();
			cout << "additional, openItem = " << openItem << endl;
		}
	}
	
	if (mouseButtonEvent.button == sf::Mouse::Left && type == sf::Event::MouseButtonPressed) {
		cout << "pressed" << endl;
		cout << "before, openItem = " << openItem << endl;

		if (mouseOver) {
			if (openItem != nullptr) {
				if (openItem->hasMouseOver() || !mouseOverAnyOfItems()) {
					vector<Frame*> focusedFrames;
					getFocusedFrames(focusedFrames);
					cout << "before loseFocus: focusedFrames.size() = " << focusedFrames.size() + isFocused << endl;

					DropdownMenu* oi = openItem;
					loseFocus();
					cout << "just lost focus in || clause" << endl;
					cout << "openItem IS open AFTER losing focus = " << oi->isOpen() << endl;

					focusedFrames.clear();
					getFocusedFrames(focusedFrames);
					cout << "after loseFocus: focusedFrames.size() = " << focusedFrames.size() + isFocused << endl;
				}
			}
			else
				if (mouseOverAnyOfItems()) {
					getFocus();

					for (DropdownMenu* item : items)
						if (item->hasMouseOver()) {
							item->open();
							item->loseFocus();  // due to repetitive mouse event checking
							openItem = item;
							break;
						}
				}
		}
		else {
			if (openItem != nullptr && !openItem->mouseOverAnyOfItems()) {
				loseFocus();
				cout << "lost in else" << endl;
			}
		}

		cout << "after, openItem = " << openItem << endl;
	}

	cout << "_________normal end of DropdownMenuRow::mouseButtonChange_________, openItem = " << openItem << endl;
	cout << "         me focusedBool = " << isFocused << endl;
}

void DropdownMenuRow::getFocus() {
	Frame::getFocus();
}

void DropdownMenuRow::loseFocus() {
	Frame::loseFocus();

	cout << "in loseFocus(), beginning, openItem = " << openItem << endl;

	closeOpenedItem();

	cout << "DropdownMenuRow::loseFocus" << endl;
}

bool DropdownMenuRow::mouseOverAnyOfItems() {
	for (DropdownMenu* item : items)
		if (item->hasMouseOver())
			return true;
	
	return false;
}

void DropdownMenuRow::closeOpenedItem() {
	if (openItem != nullptr) {
		openItem->close();
		openItem->loseFocus();
		openItem = nullptr;
	}
}
