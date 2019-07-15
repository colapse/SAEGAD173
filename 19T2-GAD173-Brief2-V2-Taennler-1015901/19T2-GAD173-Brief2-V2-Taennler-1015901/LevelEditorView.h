#pragma once
#include "View.h"
class LevelEditorView :
	public View
{
private:

public:
	LevelEditorView();
	LevelEditorView(std::string pViewName);
	LevelEditorView(std::string pViewName, float width, float height);
	~LevelEditorView();


};

