#include <StdAfx.h>
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
}