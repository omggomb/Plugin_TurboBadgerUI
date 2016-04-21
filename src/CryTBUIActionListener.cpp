#include <StdAfx.h>
#include "CPluginTurboBadgerUI.h"
#include "CryTBUIActionListener.h"
#include "CryTBUIManager.h"

namespace TurboBadgerUIPlugin
{
	void CCryTBUIActionListener::OnAction(const ActionId & action, int activationMode, float value)
	{
		bool bConsole = gEnv->pConsole->IsOpened();

		if (_bIsActive && !bConsole)
		{
			// Handle actions
		}
	}

	bool CCryTBUIActionListener::Init(CCryTBUIManager * pTBUIManager)
	{
		assert(gEnv);
		assert(gEnv->pGame);
		assert(gEnv->pGame->GetIGameFramework());
		assert(gEnv->pGame->GetIGameFramework()->GetIActionMapManager());

		if (!gEnv->pGame->GetIGameFramework()->GetIActionMapManager()->AddExtraActionListener(this))
		{
			gPlugin->LogWarning("Failed to register action listener");
			return false;
		}

		SetReceiveExclusiveInput(false);

		return true;
	}

	void CCryTBUIActionListener::Shutdown()
	{
		assert(gEnv);
		assert(gEnv->pGame);
		assert(gEnv->pGame->GetIGameFramework());
		assert(gEnv->pGame->GetIGameFramework()->GetIActionMapManager());

		if (!gEnv->pGame->GetIGameFramework()->GetIActionMapManager()->RemoveExtraActionListener(this))
		{
			gPlugin->LogWarning("Failed to unregister action listener");
		}
	}

	void CCryTBUIActionListener::SetReceiveExclusiveInput(const bool bExclusive)
	{
		gEnv->pGame->GetIGameFramework()->GetIActionMapManager()->EnableFilter(c_sUIExclusiveFilterName, bExclusive);
	}

	void CCryTBUIActionListener::SetActive(const bool bIsActive)
	{
		_bIsActive = bIsActive;
	}
}