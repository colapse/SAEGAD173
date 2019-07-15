#include "pch.h"
#include "LevelEditorView.h"


LevelEditorView::LevelEditorView() : View()
{
}

LevelEditorView::LevelEditorView(std::string pViewName) : View(pViewName) {

}
LevelEditorView::LevelEditorView(std::string pViewName, float width, float height) : View(pViewName, width, height) {

}


LevelEditorView::~LevelEditorView()
{
}
