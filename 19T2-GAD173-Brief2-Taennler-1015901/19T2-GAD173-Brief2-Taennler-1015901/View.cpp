#include "pch.h"
#include "View.h"


View::View()
{
	
}


View::View(std::string pViewName, float width, float height) : viewName(pViewName) {
	_viewRect = new sf::RectangleShape(sf::Vector2f(width, height));
}

View::View(std::string pViewName) : viewName(pViewName) {
	_viewRect = new sf::RectangleShape();
}

View::~View()
{
	if (_viewButtons.size() > 0)
	{
		for (Button * b : _viewButtons) {
			delete b;
		}
	}
	if (_viewTextInputs.size() > 0)
	{
		for (TextInput * ti : _viewTextInputs) {
			delete ti;
		}
	}
	if (_viewShapes.size() > 0)
	{
		for (sf::Shape * s : _viewShapes) {
			delete s;
		}
	}
	if (_viewTexts.size() > 0)
	{
		for (sf::Text * t : _viewTexts) {
			delete t;
		}
	}

	if (_viewRect != nullptr)
		delete _viewRect;
}

void View::SetViewName(std::string pViewName) {
	viewName = pViewName;
}

void View::SetViewSize(float width, float height) {
	
}

void View::AddButton(Button * pButton) {
	_viewButtons.push_back(pButton);
}

void View::AddTextInput(TextInput * pTextInput) {
	_viewTextInputs.push_back(pTextInput);
}

void View::AddShape(sf::Shape * pShape) {
	_viewShapes.push_back(pShape);
}

void View::AddText(sf::Text * pText) {
	_viewTexts.push_back(pText);
}

std::vector<Button*> View::GetButtons() {
	return _viewButtons;
}

std::vector<TextInput*> View::GetTextInputs() {
	return _viewTextInputs;
}

std::vector<sf::Shape*> View::GetShapes() {
	return _viewShapes;
}

std::vector<sf::Text*> View::GetTexts() {
	return _viewTexts;
}

std::string View::GetViewName() {
	return viewName;
}

sf::RectangleShape * View::GetViewRect() {
	return _viewRect;
}
