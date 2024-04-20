#pragma once
#include "SFML\Graphics.hpp"
#include "SFML\Network.hpp"
#include "SFML\Audio.hpp"
#include "Page.h"
#include "StructureClasses.h"
#include <vector>

#ifndef PROGRAM_H_
#define PROGRAM_H_


class Program {
public:
	Program();
	~Program();
	
	Program(const Program&) : Program() {}
	/*
	Program& operator=(const Program&) = default;

	Program(Program&&) = default;
	Program& operator=(Program&&) = default;
	*/

	void end();
	void start();

	void newFile();
	void loadFile();

	ResourceManager resourceManager;
	TimeManager timeManager;
private:
	vector<Page*> pages;
	Page* activePage;
	Page* getPage(string pageName);
	void addPage(Page* newPage);
	void createPages();
	void changePage(string pageName);
	void changePage(Page* page);
	bool pageExists(string pageName);
	bool pageExists(Page* page);
	
	void processEvents();
	void render();
	void loadResources();
	
	bool running;
	
	//vector <sf::Thread*> threadsToDelete;
	sf::RenderWindow window;
};

#endif