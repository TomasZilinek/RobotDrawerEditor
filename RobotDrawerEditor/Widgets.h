#pragma once
#include <iostream>
#include <bitset>
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include "FunctionHolder.h"
#include "StructureClasses.h"
#include <vector>


#ifndef WIDGETS_H_
#define WIDGETS_H_

using namespace std;

const uint16_t resolution[2] = { sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height };


					//////////////////////
					// FrameState Class //
					//////////////////////


class FrameState {
public:
	FrameState();

	enum object { fillColor, textColor, outlineColor, outlineThickness, image };
	enum mode { hover, click, focus };

	bool hasActive(object what, mode type);
	void setActive(object what, mode type);
	void setInactive(object what, mode type);
	void setAll(bool activity);

	void printInfo();
private:
	static const int BIT_SIZE = 5;
	bitset<BIT_SIZE> bitsetArray[3];
};


					   /////////////////
					   // Frame Class //
					   /////////////////


class Frame : virtual public sf::RectangleShape {
public:
	Frame();
	Frame(sf::Vector2f position);
	~Frame();

	void getFocusedFrames(vector<Frame*>& cont);

	virtual void getFocus() { isFocused = true; }
	virtual void loseFocus() { isFocused = false; }

	virtual void mouseMovement(sf::Event::MouseMoveEvent mouseMoveEvent);
	virtual void mouseButtonChange(sf::Event::EventType type, sf::Event::MouseButtonEvent mouseButtonEvent);
	virtual void keyboardButtonChange(sf::Event::EventType type, sf::Event::KeyEvent keyboardEvent);
	virtual void textEntered(sf::Event::TextEvent character);

	void forceMouseLeft();
	
	sf::Vector2f getRelativePosition() { return relativePosition; };
	sf::Vector2f getChildRealPosition(Frame* childFrame);
	sf::Vector2f getChildRelativePosition(Frame* childFrame);
	const sf::Vector2f& getPosition() const { return RectangleShape::getPosition(); }

	sf::Color getFillColorNormal() { return fillColorNormal; }
	sf::Color getFillColorHover() { return fillColorHover; }
	sf::Color getOutlineColorNormal() { return outlineColorNormal; }
	sf::Color getOutlineColorHover() { return outlineColorHover; }
	float getOutlineThicknessNormal() { return outlineThicknessNormal; }
	float getOtlineThicknessHover() { return outlineThicknessHover; }

	virtual void setRelativePosition(sf::Vector2f newRelativePosition);
	virtual void setRelativePosition(float xPos, float yPos);
	virtual void setPosition(sf::Vector2f newPosition);
	virtual void setPosition(float xPos, float yPos);
	void horizontalCenter();

	virtual void setSize(sf::Vector2f size);
	void setWidth(float width) { setSize(sf::Vector2f(width, getSize().y)); }
	void setHeight(float height) { setSize(sf::Vector2f(getSize().x, height)); }

	virtual void setActivity(bool newActivityBool);
	bool isActive() { return activeBool; }
	void setParentFrame(Frame* newParentFrame);
	virtual void draw(sf::RenderWindow& window);
	virtual void update();
	
	virtual void setHorizontalPadding(float newPadding);  // overwrites size of the frame
	virtual void setVerticalPadding(float newPadding);  // overwrites size of the frame

	void setFillColor(sf::Color newFillColor);
	void setFillColorOnHover(sf::Color newFillColorOnHover);

	void setOutlineThickness(float thickness);
	void setOutlineThicknessOnHover(float thickness);

	void setOutlineColor(sf::Color color);
	void setOutlineColorHover(sf::Color color);

	bool hasMouseOver() { return mouseOver; }

	virtual void addFrame(Frame* newFrame);
	virtual void defaultVisualSetup();
	virtual bool mouseOverAnyOfItems() { return false; }
	virtual bool mouseOverAnyOfItemsRec();
protected:
	bool isFocused;
	FrameState frameState;
	sf::Vector2f realPosition;
	sf::Vector2f relativePosition;
	
	bool realPositionCalculated, relativePositionCalculated;
	void justSetSize(sf::Vector2f size) { sf::RectangleShape::setSize(size); }

	Frame* parentFrame = nullptr;
	vector<Frame*> frames;

	bool horizontallyCentered;  // if it should be horizontally centered (mode not a state)
	bool parentStructureComplete();
	bool positionsCalculated() { return relativePositionCalculated && realPositionCalculated; }

	bool mouseOver;
	bool clicked;
	bool isPage;

	bool activeBool;
	

	float outlineThicknessNormal, outlineThicknessHover;

	sf::Color fillColorNormal, fillColorHover, outlineColorNormal, outlineColorHover;

	int horizontalPadding, verticalPadding;
	virtual void updatePositions();
	void calculateRealPosition();
	void calculateRelativePosition();
	virtual void onMouseOver();
	virtual void onMouseBackNormal();
	void justSetRealPosition(sf::Vector2f newPosition);
	void justSetRelativePosition(sf::Vector2f newRelativePosition);

	bool sizeIsSet;
	const float DEFAULT_VERTICAL_PADDING = (float)(resolution[0] * 0.02);
	const float DEFAULT_HORIZONTAL_PADDING = (float)(resolution[0] * 0.0005);
};


						/////////////////////
						// TextLabel Class //
						/////////////////////


class TextLabel : virtual public Frame, public sf::Text {
public:
	using Frame::getGlobalBounds;
	using Frame::getPosition;

	TextLabel();
	TextLabel(sf::String text);
	void setCharacterSize(unsigned int size);
	void setString(sf::String newText);
	void setFont(sf::Font& newFont);
	virtual void setSize(sf::Vector2f newSize);

	void setTextColor(sf::Color color);
	void setTextColorOnHover(sf::Color color);

	sf::Color getTextColor() { return textColorNormal; }

	void setFillColor(sf::Color color) { Frame::setFillColor(color); }
	
	void setRelativePosition(sf::Vector2f newRelativePostion);
	void setRelativePosition(float xRelativePosition, float yRelativePosition);
	virtual void setPosition(sf::Vector2f newPosition);

	void setTextAlignment(string alignment="center");  // center, left

	virtual void setHorizontalPadding(float newPadding);
	virtual void setVerticalPadding(float newPadding);
	virtual void draw(sf::RenderWindow& window);

	virtual void update();

	sf::Vector2f getSize();
	sf::FloatRect getGlobalBounds() { return Frame::getGlobalBounds(); }
	sf::Vector2f getRealTextPosition();

	// ambiguous functions:
	void setOutlineThickness(float thickness) { Frame::setOutlineThickness(thickness); }
	void setOutlineColor(sf::Color newColor) { Frame::setOutlineColor(newColor); }

	virtual void defaultVisualSetup();
protected:
	enum alignments { left, center };
	alignments textAlignment;
	virtual void updatePositions();
	void updateText();
	void setTextPosition(sf::Vector2f newPosition);
	virtual void onMouseOver();
	virtual void onMouseBackNormal();
	sf::Color textColorNormal, textColorHover;
};


						//////////////////////
						// ImageLabel Class //
						//////////////////////


class ImageLabel : virtual public Frame {
public:
	ImageLabel();
	void mouseMovement(sf::Event::MouseMoveEvent mouseMoveEvent);

	void setTexture(sf::Texture& newTexture);
	void setTextureOnHover(sf::Texture& newHoverTexture);

	virtual void setResolution(sf::Vector2f newResolution);
	virtual void setResolution(float x, float y);
	virtual void setHorizontalPadding(float newPadding);
	virtual void setVerticalPadding(float newPadding);
	virtual void setPosition(sf::Vector2f newPosition);

	virtual void setRelativePosition(sf::Vector2f newRelativePosition);

	virtual void draw(sf::RenderWindow& window);
	virtual void defaultVisualSetup();
protected:
	void updateImage();
	void updatePositions();

	virtual void onMouseOver();
	virtual void onMouseBackNormal();

	void justSetRealPosition(sf::Vector2f newPosition);
	void justSetRelativePosition(sf::Vector2f newRelativePosition);

	sf::Sprite spriteNormal;
	sf::Sprite spriteHover;
	sf::Sprite* activeSprite;
	vector<sf::Sprite*> spritesToUpdate;

	sf::Vector2f myResolution;
	sf::Vector2f getTextureSize();
};


						//////////////////
						// Button Class //
						//////////////////


class Button : virtual public Frame {
public:
	Button() {}
	Button(sf::SoundBuffer& hoverSound, sf::SoundBuffer& clickSound);
	~Button();

	template<typename F> void setFunction(F function);
	template<typename F, typename A> void setFunction(F function, A argument);
	template<typename C> void setFunction(void(C::*function)(), C* object);
	template<typename C, typename A> void setFunction(void(C::*function)(A), C* object, A argument);

	virtual void mouseButtonChange(sf::Event::EventType eventType, sf::Event::MouseButtonEvent mouseButtonEvent);

	void setFillColorOnButtonPress(sf::Color color);
	void setOutlineColorOnButtonPress(sf::Color color);
	void setOutlineThicknessOnButtonPress(float thickness);

	bool hasFunction() { return hasFunctionBool; }
	BaseFunction* getFunction() { return hasFunctionBool ? functionHolder : nullptr; }
	void removeFunction();

	virtual void defaultVisualSetup();
protected:
	virtual void onButtonPress();
	virtual void onButtonRelease();
	virtual void onMouseOver();
	virtual void onMouseBackNormal();

	virtual void executeFunction();

	BaseFunction* functionHolder = nullptr;	
	sf::Color fillColorClick, outlineColorClick;
	float outlineThicknessClick;
	
	bool hasFunctionBool;
	bool clicked;
	sf::Sound sounds[2];
};

template<typename F> void Button::setFunction(F function)
{
	if (hasFunctionBool)
		delete functionHolder;

	hasFunctionBool = true;
	functionHolder = new Functor<F>(function);
}

template<typename F, typename A> void Button::setFunction(F function, A argument)
{
	if (hasFunctionBool)
		delete functionHolder;

	hasFunctionBool = true;
	functionHolder = new FunctionWithArg<F, A>(function);
}

template<typename C> void Button::setFunction(void(C::*function)(), C* object)
{
	if (hasFunctionBool)
		delete functionHolder;

	hasFunctionBool = true;
	functionHolder = new ClassFunction<C>(function, object);
}

template<typename C, typename A> void Button::setFunction(void(C::*function)(A), C* object, A argument)
{
	if (hasFunctionBool)
		delete functionHolder;

	hasFunctionBool = true;
	functionHolder = new ClassFunctionWithArg<C, A>(function, object, argument);
}


						//////////////////////
						// TextButton Class //
						//////////////////////


class TextButton : virtual public Button, virtual public TextLabel {
public:
	using Button::mouseButtonChange;
	using TextLabel::mouseMovement;
	using TextLabel::setPosition;
	using TextLabel::updatePositions;
	using TextLabel::getPosition;
	using TextLabel::draw;
	using Text::getFillColor;
	using Button::onButtonPress;

	TextButton() {}
	TextButton(sf::String text);

	void setTextColorOnButtonPress(sf::Color color);
	virtual void defaultVisualSetup() { Button::defaultVisualSetup(); TextLabel::defaultVisualSetup(); }
protected:
	virtual void onButtonRelease();
	virtual void onMouseOver();
	virtual void onMouseBackNormal();

	sf::Color textColorClick;
};



						///////////////////////
						// ImageButton Class //
						///////////////////////


class ImageButton : virtual public Button, virtual public ImageLabel {
public:
	using ImageLabel::mouseMovement;
	using Button::mouseButtonChange;
	using ImageLabel::setPosition;
	using ImageLabel::updatePositions;
	using ImageLabel::draw;

	ImageButton(); 

	virtual void setResolution(sf::Vector2f newResolution);
	void setTextureOnButtonPress(sf::Texture& texture);

	virtual void defaultVisualSetup() { ImageLabel::defaultVisualSetup(); }
protected:
	virtual void onButtonPress();
	virtual void onButtonRelease();
	virtual void onMouseOver();
	virtual void onMouseBackNormal();

	sf::Sprite spriteClick;
};


								/////////////////////
								// TextEntry Class //
								/////////////////////


class TextEntry : virtual public TextButton {
public:
	TextEntry(TimeManager* newTimeManager);
	TextEntry(TimeManager* newTimeManager, sf::String string);

	virtual void draw(sf::RenderWindow& window);
	virtual void mouseMovement(sf::Event::MouseMoveEvent mouseMoveEvent);

	virtual void keyboardButtonChange(sf::Event::EventType type, sf::Event::KeyEvent keyboardEvent);
	virtual void textEntered(sf::Event::TextEvent character);
	virtual void setCharacterSize(unsigned int size);

	void setCursorColor(sf::Color color) { cursorRect.setFillColor(color); }

	void setFillColorOnFocus(sf::Color color);
	void setOutlineColorOnFocus(sf::Color color);
	void setOutlineThicknessOnFocus(float thickness);

	void setString(sf::String newText);
	void blinkCursor();

	virtual void defaultVisualSetup();
protected:
	virtual void onButtonPress();
	virtual void onButtonRelease();
	virtual void onMouseOver();
	virtual void onMouseBackNormal();

	virtual void update();
	virtual void executeFunction() {}  // overridden to stop executing function on button release

	void setTextStringMadeFromLeft(int index, sf::String startingString="");  // creates sf::String starting at index to the left and long as many symbols as fit
	void setTextStringMadeFromRight(int index, sf::String startingString="");  // the same as createTextStringFromLeft but from right to left

	void updateCursor();
	void setCursorTextPosition(int position);

	void backSpacePressed();
	void enterPressed();
	void insertCharacterAtCursorPosition(sf::Event::TextEvent character);
	void moveCursor(bool direction);  // false -> left, true -> right
	float getTextWidthFromString(sf::String str);  // returns sf::Text's width if str was set as its string
	void setStringFromIndexes() { sf::Text::setString(entireText.substring(shownTextLeftIndex, shownTextRightIndex - shownTextLeftIndex + 1)); }

	sf::String entireText;
	sf::RectangleShape cursorRect;
	sf::Vector2f cursorPosition;
	sf::Color fillColorFocus, outlineColorFocus;
	float outlineThicknessFocus;

	int shownTextLeftIndex, shownTextRightIndex;

	float cursorHeight, cursorWidth;
	int cursorTextPos = 0;
	bool cursorVisible;
	bool hasFocus;
	TimeManager* timeManager;
};


								////////////////////////
								// DropdownMenu Class //
								////////////////////////


class DropdownMenu : virtual public TextButton {
public:
	DropdownMenu(string text);

	virtual void onButtonPress();
	virtual void onButtonRelease();
	virtual void mouseMovement(sf::Event::MouseMoveEvent mouseMoveEvent);

	virtual void setActivity(bool newActivityBool);

	virtual void setRelativePosition(sf::Vector2f newRelativePosition);
	virtual void setPosition(sf::Vector2f newPosition);

	bool isOpen() { return isOpenBool; }
	void close();
	void open();

	virtual void getFocus();
	virtual void loseFocus();

	void addItem(Frame* newItem);
	virtual bool mouseOverAnyOfItems();
protected:
	bool isOpenBool;
	bool mouseOverAnyOfItemsBool;
	float largestItemWidth, currentItemsHeight;
	virtual void updatePositions();
	void updateItems();
	vector<Frame*> items;
};


									///////////////////////////
									// SelectButtonBar Class //
									///////////////////////////


class SelectButtonBar : public Frame {
public:
	SelectButtonBar();
	void addItem(Button* newItem, string description);
	void select(string description);
	void select(Button* button);

	vector<string> getDescriptions();

	void setFillColorOfSelectedItem(sf::Color color);
	void setItemsFillColorOnButtonPress(sf::Color color);
	void setItemsFillColorOnHover(sf::Color color);
	void setOutlineColorOfSelectedItem(sf::Color color);
	void setOutlineThicknessOfSelectedItem(const float thickness);
protected:
	string selectedDescription;
	Button* selectedButton;
	vector<pair<Button*, string>> items;

	// selecgtedItem<property>Original -> original property of the item before selection
	// selectedItem<property> -> property of any selected item
	sf::Color selectedItemFillColor, selectedItemFillColorOriginal;
	sf::Color allitemsFillColorOnHover;
	sf::Color allitemsFIllColorOnButtonPress;
	sf::Color selectedItemOutlineColor, selectedItemOutlineColorOriginal;
	float selectedItemOutlineThickness, selectedItemOutlineThicknessOriginal;

	bool descriptionExists(string description);
	Button* getItemByDescription(string description);
	void deselectItemVisually(Button* item);
	void selectItemVisually(Button* item);
};


							///////////////////////////
							// DropdownMenuRow Class //
							///////////////////////////


class DropdownMenuRow : public Frame {
public:
	void addItem(DropdownMenu* newItem);

	virtual void getFocus();
	virtual void loseFocus();

	virtual void mouseMovement(sf::Event::MouseMoveEvent mouseMoveEvent);
	virtual void mouseButtonChange(sf::Event::EventType type, sf::Event::MouseButtonEvent mouseButtonEvent);

	virtual bool mouseOverAnyOfItems();
protected:
	void closeOpenedItem();

	vector<DropdownMenu*> items;
	DropdownMenu* openItem;
};


#endif
