#include "CreatePages.h"
#include <iostream>
#include <vector>


							////////////////////////
							// AdvancedPage Class //
							////////////////////////


AdvancedPage::AdvancedPage(ResourceManager * resourceMan, TimeManager * timeMan, Program * pr, string name) : Page(name) {
	resourceManager = resourceMan;
	timeManager = timeMan;
	program = pr;
}


							////////////////////
							// MenuPage Class //
							////////////////////


MenuPage::MenuPage(ResourceManager* resourceMan, TimeManager* timeMan, Program* pr)
		: AdvancedPage(resourceMan, timeMan, pr, "menuPage") {

	sf::Texture& background = resourceManager->textureHolder.get(Textures::menuBg);
	setTexture(background);

	sf::Font& arial = resourceManager->fontHolder.get(Fonts::Arial);
	vector<TextButton*> buttons;

	TextButton* newFileButton = new TextButton("New File");
	TextButton* loadFileButton = new TextButton("Load File");
	TextButton* quitButton = new TextButton("Quit");

	buttons.push_back(newFileButton);
	buttons.push_back(loadFileButton);
	buttons.push_back(quitButton);

	const int FONT_SIZE = resolution[1] * 0.04;
	sf::Vector2f buttonPosition = sf::Vector2f();
	int i = 0;

	for (TextButton* textButton : buttons) {
		textButton->setFont(arial);
		textButton->setCharacterSize(FONT_SIZE);

		textButton->setHorizontalPadding(30);
		textButton->setVerticalPadding(20);

		textButton->setFillColor(sf::Color(71, 68, 68, 200));
		textButton->setFillColorOnHover(sf::Color(103, 101, 101, 220));
		textButton->setFillColorOnButtonPress(sf::Color(103, 101, 101, 40));

		textButton->setOutlineColorHover(sf::Color::Black);
		textButton->setOutlineThicknessOnHover(3);

		textButton->setTextColor(sf::Color::White);
		textButton->setPosition(sf::Vector2f(0, resolution[1] / buttons.size() + i * textButton->getSize().y * 1.2));
		textButton->horizontalCenter();

		cout << resolution[1] / buttons.size() + i * (FONT_SIZE * 1.2) << endl;

		addFrame(textButton);

		cout << "position = " << textButton->getPosition().x << ", " << textButton->getPosition().y << endl;
		i++;
	}

	newFileButton->setFunction(&Program::newFile, program);
	loadFileButton->setFunction(&Program::loadFile, program);
	quitButton->setFunction(&Program::end, program);
}


								//////////////////////
								// EditorPage Class //
								//////////////////////


EditorPage::EditorPage(ResourceManager* resourceMan, TimeManager* timeMan, Program* pr)
		: AdvancedPage(resourceMan, timeMan, pr, "editorPage") {

	sf::Font& arial = resourceManager->fontHolder.get(Fonts::Arial);
	sf::Texture& cross_close = resourceManager->textureHolder.get(Textures::cross_close);
	const int FONT_SIZE = int(resolution[1] * 0.017);

	// frames
	Frame* horizontalPanel_0 = new Frame();
	DropdownMenuRow* horizontalPanel_1 = new DropdownMenuRow();
	Frame* horizontalPanel_2 = new Frame();
	SelectButtonBar* leftVerticalPanel = new SelectButtonBar();	

	const float hp0Height = resolution[1] * 0.035f;
	const float hp1Height = resolution[1] * 0.04f;
	const float hp2Height = resolution[1] * 0.08f;

	sf::Color horizontalPanel_0_color = sf::Color(60, 60, 60);

	horizontalPanel_0->setPosition(0, 0);
	horizontalPanel_0->setSize(sf::Vector2f(resolution[0], hp0Height));
	horizontalPanel_0->setFillColor(horizontalPanel_0_color);

	horizontalPanel_1->setPosition(0, hp0Height);
	horizontalPanel_1->setSize(sf::Vector2f(resolution[0], hp1Height));
	horizontalPanel_1->setFillColor(sf::Color(240, 240, 240));

	horizontalPanel_2->setPosition(0, hp0Height + hp1Height);
	horizontalPanel_2->setSize(sf::Vector2f(resolution[0], hp2Height));
	horizontalPanel_2->setFillColor(sf::Color(220, 220, 220));

	leftVerticalPanel->setPosition(0, hp0Height + hp1Height + hp2Height);
	leftVerticalPanel->setSize(sf::Vector2f(resolution[0] * 0.03, resolution[1]));
	leftVerticalPanel->setFillColor(sf::Color(250, 250, 250));
	leftVerticalPanel->setOutlineThickness(1);
	leftVerticalPanel->setOutlineColor(sf::Color::Black);

	TextLabel* titleLabel = new TextLabel("Robot Drawer Editor");
	titleLabel->setFont(arial);
	titleLabel->setCharacterSize(FONT_SIZE);
	titleLabel->setFillColor(horizontalPanel_0_color);
	titleLabel->setTextColor(sf::Color(200, 200, 200));
	titleLabel->setRelativePosition(sf::Vector2f(0.01, 0.33));

	horizontalPanel_0->addFrame(titleLabel);

	ImageButton* crossCloseImageButton = new ImageButton();
	const float closeImageLabelSize = hp0Height * 0.95f;

	crossCloseImageButton->setTexture(cross_close);
	crossCloseImageButton->setPosition(sf::Vector2f(resolution[0] - closeImageLabelSize, 0));
	crossCloseImageButton->setResolution(sf::Vector2f(closeImageLabelSize, closeImageLabelSize));
	crossCloseImageButton->setFillColor(sf::Color::Transparent);
	crossCloseImageButton->setFillColorOnHover(sf::Color(150, 150, 150, 100));
	crossCloseImageButton->setFillColorOnButtonPress(sf::Color(42, 111, 221));
	crossCloseImageButton->setFunction(&Program::end, program);

	horizontalPanel_0->addFrame(crossCloseImageButton);

	addFrame(horizontalPanel_0);
	addFrame(horizontalPanel_1);
	addFrame(horizontalPanel_2);
	addFrame(leftVerticalPanel);

	//- horizontalPanel_1
	DropdownMenu* fileDropdown = new DropdownMenu("File");
	DropdownMenu* editDropdown = new DropdownMenu("Edit");
	DropdownMenu* viewDropdown = new DropdownMenu("View");
	DropdownMenu* objectDropdown = new DropdownMenu("Object");
	DropdownMenu* helpDropdown = new DropdownMenu("Help");

	int lastXcoord = 4;
	int maxHeight = 0;

	for (DropdownMenu* dropdownMenu : { fileDropdown , editDropdown, viewDropdown, objectDropdown, helpDropdown }) {
		dropdownMenuVisualSetup(dropdownMenu, arial, FONT_SIZE);
		dropdownMenu->setPosition(sf::Vector2f(horizontalPanel_1->getPosition().x + lastXcoord, hp0Height + 4));

		lastXcoord += dropdownMenu->getSize().x + 2;
		horizontalPanel_1->addItem(dropdownMenu);

		if (dropdownMenu->getSize().y > maxHeight)
			maxHeight = dropdownMenu->getSize().y;
	}

	for (DropdownMenu* dropdownMenu : { fileDropdown , editDropdown, viewDropdown, objectDropdown, helpDropdown })
		dropdownMenu->setHeight(maxHeight);

	//-- FileDropdownMenu
	TextButton* newFileButton = new TextButton("New File");
	TextButton* openButton = new TextButton("Open File");
	TextButton* openRecentButton = new TextButton("Open Recent");
	TextButton* saveButton = new TextButton("Save");
	TextButton* saveAsButton = new TextButton("Save As");
	TextButton* quitButton = new TextButton("Quit");

	quitButton->setFunction(&Program::end, program);
	saveAsButton->setFunction(
		[] {
			cout << "------saveAsButton clicked" << endl;
		}
	);

	for (TextButton* textButton : { newFileButton, openButton, openRecentButton, saveButton, saveAsButton, quitButton }) {
		dropdownItemVisualSetup(textButton, arial, FONT_SIZE);
		fileDropdown->addItem(textButton);
	}

	//-- EditDropdownMenu
	TextButton* undoButton = new TextButton("Undo");
	TextButton* redoButton = new TextButton("Redo");
	TextButton* cutButton = new TextButton("Cut");
	TextButton* pasteButton = new TextButton("Paste");
	TextButton* deleteButton = new TextButton("Delete");
	TextButton* selectAllButton = new TextButton("Select All");

	for (TextButton* textButton : { undoButton, redoButton, cutButton, pasteButton, deleteButton, selectAllButton }) {
		dropdownItemVisualSetup(textButton, arial, FONT_SIZE);
		editDropdown->addItem(textButton);
	}

	// horizontalPanel_2

	// leftVerticalPanel
	sf::Texture& selectTool = resourceManager->textureHolder.get(Textures::selectTool);
	sf::Texture& moveTool = resourceManager->textureHolder.get(Textures::moveTool);
	sf::Texture& editPathsTool = resourceManager->textureHolder.get(Textures::editPathsTool);
	sf::Texture& rectanglesTool = resourceManager->textureHolder.get(Textures::rectanglesTool);
	sf::Texture& ellipsesTool = resourceManager->textureHolder.get(Textures::ellipsesTool);
	sf::Texture& polygonsTool = resourceManager->textureHolder.get(Textures::polygonsTool);
	sf::Texture& spiralsTool = resourceManager->textureHolder.get(Textures::spiralsTool);
	sf::Texture& freehandTool = resourceManager->textureHolder.get(Textures::freehandTool);
	sf::Texture& curvesTool = resourceManager->textureHolder.get(Textures::curvesTool);
	sf::Texture& textTool = resourceManager->textureHolder.get(Textures::textTool);

	ImageButton* selectButton = new ImageButton();
	ImageButton* moveButton = new ImageButton();
	ImageButton* editPathsButton = new ImageButton();
	ImageButton* rectanglesButton = new ImageButton();
	ImageButton* ellipsesButton = new ImageButton();
	ImageButton* polygonsButton = new ImageButton();
	ImageButton* spiralsButton = new ImageButton();
	ImageButton* freehandButton = new ImageButton();
	ImageButton* curvesButton = new ImageButton();
	ImageButton* textButton = new ImageButton();

	selectButton->setTexture(selectTool);
	moveButton->setTexture(moveTool);
	editPathsButton->setTexture(editPathsTool);
	rectanglesButton->setTexture(rectanglesTool);
	ellipsesButton->setTexture(ellipsesTool);
	polygonsButton->setTexture(polygonsTool);
	spiralsButton->setTexture(spiralsTool);
	freehandButton->setTexture(freehandTool);
	curvesButton->setTexture(curvesTool);
	textButton->setTexture(textTool);

	string toolNames[] = { "selectTool", "moveTool", "editPathsTool", "rectanglesTool", "ellipsesTool", "polygonsTool",
						   "spiralsTool", "freehandTool", "curvesTool", "textTool" };

	int buttonYpos = leftVerticalPanel->getPosition().y + leftVerticalPanel->getSize().x * 0.025;
	int i = 0;

	for (ImageButton* button : { selectButton, moveButton, editPathsButton, rectanglesButton, ellipsesButton, polygonsButton,
								 spiralsButton, freehandButton, curvesButton, textButton }) {

		button->setPosition(sf::Vector2f(leftVerticalPanel->getPosition().x + leftVerticalPanel->getSize().x * 0.025,
										 buttonYpos));
		button->setResolution(sf::Vector2f(leftVerticalPanel->getSize().x * 0.95,
										   leftVerticalPanel->getSize().x * 0.95));
		button->setFillColor(sf::Color::Transparent);

		leftVerticalPanel->addItem(button, toolNames[i]);

		buttonYpos += button->getSize().y + 3;
		i++;
	}
}

void EditorPage::dropdownMenuVisualSetup(DropdownMenu * dropdownMenu, sf::Font& font, const int FONT_SIZE) {
	dropdownMenu->setFont(font);
	dropdownMenu->setVerticalPadding(8);
	dropdownMenu->setHorizontalPadding(20);
	dropdownMenu->setCharacterSize(FONT_SIZE);
	dropdownMenu->setFillColor(sf::Color(220, 220, 220));
	dropdownMenu->setFillColorOnHover(sf::Color(210, 210, 210));
	dropdownMenu->setTextColor(sf::Color::Black);
	dropdownMenu->setTextAlignment("center");
}

void EditorPage::dropdownItemVisualSetup(TextButton* item, sf::Font& font, const int FONT_SIZE) {
	item->setFont(font);
	item->setVerticalPadding(8);
	item->setHorizontalPadding(8);
	item->setCharacterSize(FONT_SIZE);
	item->setFillColor(sf::Color(210, 210, 210));
	item->setFillColorOnHover(sf::Color(130, 170, 244));
	item->setFillColorOnButtonPress(sf::Color(180, 200, 230));
	item->setTextColor(sf::Color::Black);

	if (item->getSize().x < resolution[0] * 0.1)
		item->setWidth(resolution[0] * 0.1);
}
