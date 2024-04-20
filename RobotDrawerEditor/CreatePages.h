#pragma once

#include "Page.h"
#include "StructureClasses.h"
#include "Program.h"


#ifndef CREATE_PAGES_H_
#define CREATE_PAGES_H_


class AdvancedPage : public Page {
public:
	AdvancedPage(ResourceManager* resourceMan, TimeManager* timeMan, Program* pr, string name);
protected:
	ResourceManager* resourceManager;
	TimeManager* timeManager;
	Program* program;
};


class MenuPage : public AdvancedPage {
public:
	MenuPage(ResourceManager* resourceMan, TimeManager* timeMan, Program* pr);
};


class EditorPage : public AdvancedPage {
public:
	EditorPage(ResourceManager* resourceMan, TimeManager* timeMan, Program* pr);
private:
	void dropdownMenuVisualSetup(DropdownMenu* dropdownMenu, sf::Font& font, const int FONT_SIZE);
	void dropdownItemVisualSetup(TextButton* item, sf::Font& font, const int FONT_SIZE);
};

#endif
