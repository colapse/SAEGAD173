#include "pch.h"
#include "TextInput.h"

#ifndef cctype
#include <cctype>
#endif // !cctype


TextInput::TextInput()
{
	minTextMargin = 3;
	hasFocus = false;
	isMouseOver = false;

	_rectComp = new sf::RectangleShape(sf::Vector2f(100,10));
	_textComp = new sf::Text();

	SetBorder(2,sf::Color::Color(99,99,99,255));
	SetBackground(sf::Color::White);
}


TextInput::~TextInput()
{
	if (_textComp != nullptr)
		delete _textComp;
	if (_rectComp != nullptr)
		delete _rectComp;
}

#pragma region Event Functions

void TextInput::_OnFocus() {
	if (hasFocus)
		return;
	hasFocus = true;

	// Loop through btnDownFuncs list
	for (std::function<void()> func : onFocusFuncs) {
		func();
	}
}

void TextInput::_OnFocusExit() {
	if (!hasFocus)
		return;

	hasFocus = false;

	// Loop through btnDownFuncs list
	for (std::function<void()> func : onFocusExitFuncs) {
		func();
	}
}

void TextInput::_OnMouseEnter() {
	if (isMouseOver)
		return;
	isMouseOver = true;

	// Loop through btnDownFuncs list
	for (std::function<void()> func : mouseEnterFuncs) {
		func();
	}
}

void TextInput::_OnMouseExit() {
	if (!isMouseOver)
		return;
	isMouseOver = false;

	// Loop through btnDownFuncs list
	for (std::function<void()> func : mouseExitFuncs) {
		func();
	}
}

void TextInput::_OnTextInput(char c) {
	if (!hasFocus)
		return;

	std::string text = _textComp->getString();
	if (c == '\b' && text.size() > 0)
		SetText(text.erase(text.size()-1,1));
	else if(c != '\b' && ((allowOnlyNumerical && std::isdigit(c)) || (allowOnlyAlphaNumerical && std::isalnum(c)) || (!allowOnlyAlphaNumerical && !allowOnlyNumerical)) && (maxLength < 0 || text.size() < maxLength))
		SetText(text + c);

	// Loop through btnDownFuncs list
	for (std::function<void()> func : textInputFuncs) {
		func();
	}
}

void TextInput::AddOnFocusFunc(std::function<void()> func) {
	onFocusFuncs.push_back(func);
}

void TextInput::AddOnFocusExitFunc(std::function<void()> func) {
	onFocusExitFuncs.push_back(func);
}

void TextInput::AddOnMouseEnterFunc(std::function<void()> func) {
	mouseEnterFuncs.push_back(func);
}

void TextInput::AddOnMouseExitFunc(std::function<void()> func) {
	mouseExitFuncs.push_back(func);
}

void TextInput::AddOnTextInputFunc(std::function<void()> func) {
	textInputFuncs.push_back(func);
}

#pragma endregion


/** SetAutoCharacterSize: Finds the perfect character size for the text component to fit within the textfields bounds */
void TextInput::SetAutoCharacterSize() {
	// Exit if either the _shape or _txtText component is null. (Avoid access violations)
	if (_rectComp == nullptr || _textComp == nullptr)
		return;

	// Calculate the horizontal ratio between the buttons & texts(+ minium margin) width. (Basically checking if the text is overflowing the button horizontally or if its too small.)
	float horRatio = _rectComp->getGlobalBounds().width / (_textComp->getGlobalBounds().width + minTextMargin * 2);
	// Calculate a new character size depending on the horizontal ratio
	int newSize = _textComp->getCharacterSize() * horRatio;

	// Calculate the vertical ratio between the buttons & texts(+ minium margin) height. (Basically checking if the text is overflowing the button vertically or if its too small.)
	float vertRatio = _rectComp->getGlobalBounds().height / (2*newSize + minTextMargin * 2);
	// Check if the text is overflowing vertically with the new character size
	if (vertRatio < 1) {
		// Modify the new character size to fit in vertically
		newSize *= vertRatio;
	}

	// Set the new character size
	_textComp->setCharacterSize(newSize);
}

/** SetTextPosition: Centers the text within the Input field. */
void TextInput::SetTextPosition() {
	if (_rectComp != nullptr || _textComp != nullptr) { // Avoid access violations
		// Calculate & set the center position depending on the texts & buttons boundaries
		//_textComp->setPosition((_rectComp->getGlobalBounds().left + _rectComp->getGlobalBounds().width / 2 - (_textComp->getGlobalBounds().width / 2)), (_rectComp->getGlobalBounds().top + _rectComp->getGlobalBounds().height / 2 - (_textComp->getGlobalBounds().height / 2)));
		_textComp->setPosition(_rectComp->getGlobalBounds().left + minTextMargin, (_rectComp->getGlobalBounds().top + _rectComp->getGlobalBounds().height / 2 - (_textComp->getGlobalBounds().height / 2)));
	}
}

std::string TextInput::GetText() {
	return _textComp->getString();
}
sf::RectangleShape * TextInput::GetRectComponent() {
	return _rectComp;
}
sf::Text * TextInput::GetTextComponent() {
	return _textComp;
}
sf::FloatRect TextInput::GetBoundaries() {
	return _rectComp->getGlobalBounds();
}

void TextInput::SetText(std::string text) {
	_textComp->setString(text);
	SetAutoCharacterSize();
	SetTextPosition();
}
void TextInput::SetPosition(float x, float y) {
	_rectComp->setPosition(x, y);
	SetTextPosition();
}
void TextInput::SetPosition(sf::Vector2f pos) {
	_rectComp->setPosition(pos);
	SetTextPosition();
}
void TextInput::SetSize(float width, float height) {
	_rectComp->setSize(sf::Vector2f(width, height));
	SetAutoCharacterSize();
	SetTextPosition();
}
void TextInput::SetBackground(sf::Color color) {
	_rectComp->setFillColor(color);
}
void TextInput::SetBorder(float thickness, sf::Color color) {
	_rectComp->setOutlineColor(color);
	_rectComp->setOutlineThickness(thickness);
}

void TextInput::SetFont(sf::Font pFont) {
	// Exit if the text isn't set (Avoid access violations)
	if (_textComp == nullptr)
		return;

	font = sf::Font::Font(pFont); // Store the font in the class instance
	_textComp->setFont(this->font); // Assign the new font to the text component
	SetAutoCharacterSize(); // Automatically resize the character size to fit within the input fields boundaries with the new font
	SetTextPosition(); // Reset the position of the text component to the center of the input fields boundaries
}

void TextInput::SetTextColor(sf::Color color) {
	_textComp->setFillColor(color);
}

bool TextInput::HasFocus() {
	return hasFocus;
}
