#pragma once
#include <tb_widgets.h>

namespace TurboBadgerUIPlugin
{
	class CCryTBUIManager;

	class RootWidget : public tb::TBWidget
	{
	public:
		TBOBJECT_SUBCLASS(RootWidget, tb::TBWidget);

		RootWidget();
		~RootWidget();

		const bool Init(CCryTBUIManager* pManager);

		/// <summary>
		/// Resizes the root widget (e.g. call when resolution is changed)
		/// </summary>
		/// <param name="newWidth">The new width.</param>
		/// <param name="newHeight">The new height.</param>
		void Resize(int newWidth, int newHeight);

		void OnChildRemove(tb::TBWidget* child) override;
		void OnChildAdded(tb::TBWidget* child) override;
	private:

		CCryTBUIManager* _pUIManager = nullptr;
		int _nChildCount = 0;
	};
}