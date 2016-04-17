#pragma once
#include <tb_widgets.h>

class RootWidget : public tb::TBWidget
{
public:
	TBOBJECT_SUBCLASS(RootWidget, tb::TBWidget);

	RootWidget();
	~RootWidget();

	const bool Init(const CCryTBUIManager* pManager);

	/// <summary>
	/// Resizes the root widget (e.g. call when resolution is changed)
	/// </summary>
	/// <param name="newWidth">The new width.</param>
	/// <param name="newHeight">The new height.</param>
	void Resize(int newWidth, int newHeight);
private:

	CCryTBUIManager* _pUIManager = nullptr;
};
