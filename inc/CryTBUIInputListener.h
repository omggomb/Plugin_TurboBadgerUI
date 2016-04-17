#pragma once
#include <IInput.h>
#include "CryTBUIManager.h"

class CCryTBUIInputListener : public IInputEventListener
{
public:
	// IInputEventListener
	bool OnInputEvent(const SInputEvent &event) override;
	bool OnInputEventUI(const SUnicodeEvent &event) override;
	// ~IInputEventListener

	// CCryTBUIInputListener
	bool Init(CCryTBUIManager* pTBUIManager);
	void Shutdown();
	void SetReceiveExclusiveInput(const bool bExclusive);
	void SetActive(const bool bIsActive);
	// ~CCryTBUIInputListener

private:
	CCryTBUIManager* _pTBUIManager = nullptr;

private:

	bool HandleKeyboardEvents(const SInputEvent &event);
	bool HandleMouseEvents(const SInputEvent &event);
	bool HandleUnicodeEvents(const SUnicodeEvent& event);
};