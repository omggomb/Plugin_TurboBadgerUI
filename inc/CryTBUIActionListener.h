#pragma once
#include <CryTBUIManager.h>
#include <IActionMapManager.h>

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
};