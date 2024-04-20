#include "SFML\Graphics.hpp"
#include "SFML\System\String.hpp"
#include "Program.h"
#include "CreatePages.h"
#include <iostream>
#include <assert.h>
#include <Windows.h>


string ExePath() {
	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	string::size_type pos = string(buffer).find_last_of("\\/");
	return string(buffer).substr(0, pos);
}


Program::Program() : window(sf::VideoMode(resolution[0], resolution[1]), "MyProgram", sf::Style::Fullscreen) {
	window.setVerticalSyncEnabled(true);
	/*
	sf::Thread* thread = new sf::Thread( // start thread to load Pages
		[this]() {
			this->loadResources();
			for (int i = 0; i < PageS_TO_CREATE_NUMBER; i++)
				if(window.isOpen())
					createAndSetupPage(Pages_names_to_load[i]);
		}
	);
	

	thread->launch();
	threadsToDelete.push_back(thread);
	*/

	cout << ExePath() << endl;

	loadResources();
}

void Program::end() {
	activePage->setActivity(false);
	window.close();
}

void Program::start() {
	createPages();

	if (!running) {
		running = true;

		sf::Clock clock;
		sf::Time timeSinceLastUpdate = sf::Time::Zero;
		sf::Time last_lapsed;
		sf::Time TimePerFrame = sf::seconds(0.0333f);

		while (window.isOpen()) {
			processEvents();
			last_lapsed = clock.restart();
			timeSinceLastUpdate += last_lapsed;
			timeManager.setTimeChange(last_lapsed);

			while (timeSinceLastUpdate > TimePerFrame) {
				timeSinceLastUpdate -= TimePerFrame;
				processEvents();
			}

			render();
		}
	}
}

void Program::createPages() {
	Page* menuPage = new MenuPage(&resourceManager, &timeManager, this);
	addPage(menuPage);
	
	Page* editorPage = new EditorPage(&resourceManager, &timeManager, this);
	addPage(editorPage);

	changePage(editorPage);
}

void Program::processEvents() {
	sf::Event event;

	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed)
			window.close();
		if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) {
			if (event.key.code == sf::Keyboard::Escape)
				window.close();
			else
				activePage->keyboardButtonChange(event.type, event.key);
		}
		if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased)
			activePage->mouseButtonChange(event.type, event.mouseButton);
		if (event.type == sf::Event::MouseMoved)
			activePage->mouseMovement(event.mouseMove);
		if (event.type == sf::Event::TextEntered) {
			activePage->textEntered(event.text);
			//cout << event.text.unicode << endl;
		}
	}
}

void Program::render() {
	window.clear(sf::Color::White);

	vector<Frame*> focusedFrames;
	activePage->getFocusedFrames(focusedFrames);
	
	activePage->draw(window);

	if (focusedFrames.size() != 0)
		for (Frame* focusedFrame : focusedFrames)
			focusedFrame->draw(window);

	window.display();
}

void Program::addPage(Page* newPage) {
	assert(newPage != nullptr);
	pages.push_back(newPage);
	//cout << "Page with name: '" << newPage->getName() << "' added" << endl;
}

void Program::changePage(string pageName) {
	if (pageExists(pageName))
	{
		if (activePage != nullptr)
			activePage->setActivity(false);
		activePage = getPage(pageName);
		activePage->setActivity(true);

		cout << "activePage changed to: '" << pageName << "'" << endl;
	}
	else {
		cout << "Program::changePage (name = '" << pageName << "') -> page not found" << endl;
	}
}

void Program::changePage(Page* page) {
	assert(page != nullptr);
	changePage(page->getName());
}

Page* Program::getPage(string pageName) {
	for (Page* page : pages) {
		if (pageName == page->getName())
			return page;
	}

	return nullptr;
}

bool Program::pageExists(string pageName) {
	return getPage(pageName) != nullptr;
}

bool Program::pageExists(Page* page) {
	return pageExists(page->getName());
}

void Program::loadResources() {
	string pathPrefix = "..\\RobotDrawerEditor\\resources";

	resourceManager.fontHolder.load(Fonts::Arial, pathPrefix + "\\Arial.ttf");

	resourceManager.textureHolder.load(Textures::menuBg, pathPrefix + "\\main_menu_backgronud.jpg");
	resourceManager.textureHolder.load(Textures::selectTool, pathPrefix + "\\selectAndTransform.png");
	resourceManager.textureHolder.load(Textures::moveTool, pathPrefix + "\\move.png");
	resourceManager.textureHolder.load(Textures::editPathsTool, pathPrefix + "\\editPathsByNodes.png");
	resourceManager.textureHolder.load(Textures::rectanglesTool, pathPrefix + "\\rectanglesAndSquares.png");
	resourceManager.textureHolder.load(Textures::ellipsesTool, pathPrefix + "\\ellipsesAndCircles.png");
	resourceManager.textureHolder.load(Textures::polygonsTool, pathPrefix + "\\polygonsAndStars.png");
	resourceManager.textureHolder.load(Textures::spiralsTool, pathPrefix + "\\spirals.png");
	resourceManager.textureHolder.load(Textures::freehandTool, pathPrefix + "\\drawFreehand.png");
	resourceManager.textureHolder.load(Textures::curvesTool, pathPrefix + "\\curvesAndStraightLines.png");
	resourceManager.textureHolder.load(Textures::textTool, pathPrefix + "\\text.png");

	resourceManager.textureHolder.load(Textures::cross_close, pathPrefix + "\\cross_close.png");

	cout << "resources loaded" << endl;
}

void Program::newFile() {
	cout << "New File!" << endl;

	changePage("editorPage");
}

void Program::loadFile() {
	cout << "Load File!" << endl;
}

Program::~Program() {
	for (int i = 0; i < pages.size(); i++)
		delete pages[i];
	//for (int i = 0; i < threadsToDelete.size(); i++)
	//	delete threadsToDelete[i];
	pages.clear();
}
