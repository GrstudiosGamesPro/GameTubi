#include "UIBuilder.h"


UIBuilder* UIBuilder::m_engine = nullptr;

UIBuilder::UIBuilder() {

}

void UIBuilder::create()
{
	if (UIBuilder::m_engine) throw std::exception("UIBuilder already created.");
	UIBuilder::m_engine = new UIBuilder();
	std::cout << "UIBuilder Created" << endl;
}


UIBuilder* UIBuilder::GetInstance()
{
	return m_engine;
}

void UIBuilder::release()
{
	if (!UIBuilder::m_engine) return;
	delete UIBuilder::m_engine;
}

void UIBuilder::Update() {

}

UIBuilder::~UIBuilder()
{
	UIBuilder::m_engine = nullptr;
}