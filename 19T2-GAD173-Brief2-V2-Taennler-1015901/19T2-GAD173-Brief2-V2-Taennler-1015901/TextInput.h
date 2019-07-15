#pragma once
#ifndef SFML_GRAPHICS_HPP
#include <SFML/Graphics.hpp>
#endif // !SFML_GRAPHICS_HPP
#ifndef functional
#include <functional>
#endif // !functional
class TextInput
{
	sf::RectangleShape * _rectComp;
	sf::Text * _textComp;
	sf::Font font;

	// Private Variables: Input Event Functions. They store functions (Such as lambda functions) which will be executed on specific input events
	std::vector<std::function<void()>> onFocusFuncs;
	std::vector<std::function<void()>> onFocusExitFuncs;
	std::vector<std::function<void()>> mouseEnterFuncs;
	std::vector<std::function<void()>> mouseExitFuncs;
	std::vector<std::function<void()>> textInputFuncs;

	// Private Variables: Settings
	float minTextMargin; // The minimum space between the texts boundaries and the buttons border

	// Private Variables: States
	bool hasFocus;
	bool isMouseOver;

	void SetAutoCharacterSize();
	void SetTextPosition();
public:
	bool allowOnlyNumerical = false;
	bool allowOnlyAlphaNumerical = false;
	int maxLength = -1; // -1 = unlimited

	TextInput();
	~TextInput();

	void _OnFocus();
	void _OnFocusExit();
	void _OnMouseEnter();
	void _OnMouseExit();
	void _OnTextInput(char c);

	void AddOnFocusFunc(std::function<void()> func);
	void AddOnFocusExitFunc(std::function<void()> func);
	void AddOnMouseEnterFunc(std::function<void()> func);
	void AddOnMouseExitFunc(std::function<void()> func);
	void AddOnTextInputFunc(std::function<void()> func);

	std::string GetText();
	sf::RectangleShape * GetRectComponent();
	sf::Text * GetTextComponent();
	sf::FloatRect GetBoundaries();

	void SetText(std::string text);
	void SetPosition(float x, float y);
	void SetPosition(sf::Vector2f pos);
	void SetSize(float width, float height);
	void SetBackground(sf::Color color);
	void SetBorder(float thickness, sf::Color color);
	void SetFont(sf::Font pFont);
	void SetTextColor(sf::Color color);

	bool HasFocus();
};

