#include <StdAfx.h>

#include "CryTBUIManager.h"
#include "RootWidget.h"

namespace TurboBadgerUIPlugin
{
	RootWidget::RootWidget()
	{
	}

	RootWidget::~RootWidget()
	{
	}

	const bool RootWidget::Init(CCryTBUIManager * pManager)
	{
		assert(pManager);

		SetID(tb::TBID("RootWidget"));
		_pUIManager = pManager;

		return true;
	}

	void RootWidget::Resize(int newWidth, int newHeight)
	{
		tb::TBRect rect
		{
			0, 0,
			newWidth, newHeight
		};

		SetRect(rect);
	}
	void RootWidget::OnChildAdded(tb::TBWidget * child)
	{
		tb::TBWidget::OnChildAdded(child);
		++_nChildCount;
	}

	void RootWidget::OnChildRemove(tb::TBWidget * child)
	{
		tb::TBWidget::OnChildRemove(child);
		--_nChildCount;

		if (_nChildCount <= 0)
		{
			_pUIManager->SetModalMode(false);
		}
	}
}