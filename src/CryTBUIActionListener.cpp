#include <StdAfx.h>
#include "CryTBUIActionListener.h"
#include "CryTBUIManager.h"

namespace TurboBadgerUIPlugin
{
	void CCryTBUIActionListener::OnAction(const ActionId & action, int activationMode, float value)
	{
	}

	bool CCryTBUIActionListener::Init(CCryTBUIManager * pTBUIManager)
	{
		return true;
	}

	void CCryTBUIActionListener::Shutdown()
	{
	}

	void CCryTBUIActionListener::SetReceiveExclusiveInput(const bool bExclusive)
	{
	}

	void CCryTBUIActionListener::SetActive(const bool bIsActive)
	{
	}
}