#pragma once
#ifndef SFML/Graphics.hpp
#include <SFML/Graphics.hpp>
#endif // !SFML/Graphics.hpp
#ifndef Button
#include "Button.h"
#endif // !Button
#ifndef TextInput
#include "TextInput.h"
#endif // !TextInput
#ifndef string
#include <string>
#endif // !string

class View
{
	sf::RectangleShape * _viewRect;

	std::vector<Button*> _viewButtons;
	std::vector<TextInput*> _viewTextInputs;
	std::vector<sf::Shape*> _viewShapes;
	std::vector<sf::Text*> _viewTexts;

	std::string viewName;
public:
	View();
	View(std::string pViewName);
	View(std::string pViewName, float width, float height);
	~View();

	void SetViewName(std::string viewName);
	void SetViewSize(float width, float height);

	void AddButton(Button * button);
	void AddTextInput(TextInput * textInput);
	void AddShape(sf::Shape * textInput);
	void AddText(sf::Text * textInput);

	std::vector<Button*> GetButtons();
	std::vector<TextInput*> GetTextInputs();
	std::vector<sf::Shape*> GetShapes();
	std::vector<sf::Text*> GetTexts();
	std::string GetViewName();
	sf::RectangleShape * GetViewRect();
};

