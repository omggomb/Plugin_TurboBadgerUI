#pragma once
#include <IActionMapManager.h>

namespace TurboBadgerUIPlugin
{
	class CCryTBUIManager;

	class CCryTBUIActionListener : public IActionListener
	{
	public:
		// IActionListener
		void OnAction(const ActionId &action, int activationMode, float value) override;

		/* EMPTY */
		void AfterAction() override {}
		// ~IActionListener

		// CCryTBUIActionListener
		bool Init(CCryTBUIManager* pTBUIManager);
		void Shutdown();
		void SetReceiveExclusiveInput(const bool bExclusive);
		void SetActive(const bool bIsActive);
		// ~CCryTBUIActionListener

	private:
		CCryTBUIManager* _pTBUIManager = nullptr;
		bool _bIsActive = true;
		const string  c_sUIExclusiveFilterName = "UIExclusive";
	};
}