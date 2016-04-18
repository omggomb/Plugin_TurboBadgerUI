#include <StdAfx.h>
#include "CryTBUIInputListener.h"

bool CCryTBUIInputListener::OnInputEvent(const SInputEvent & event)
{
	return false;
}

bool CCryTBUIInputListener::OnInputEventUI(const SUnicodeEvent & event)
{
	return false;
}

bool CCryTBUIInputListener::Init(CCryTBUIManager * pTBUIManager)
{
	assert(pTBUIManager);
	assert(gEnv);
	assert(gEnv->pInput);

	_pTBUIManager = pTBUIManager;

	_rootWidget = _pTBUIManager->GetRootWidget();

	// TODO: Register UI listener
}

void CCryTBUIInputListener::Shutdown()
{
}

void CCryTBUIInputListener::SetReceiveExclusiveInput(const bool bExclusive)
{
}

void CCryTBUIInputListener::SetActive(const bool bIsActive)
{
}

bool CCryTBUIInputListener::HandleKeyboardEvents(const SInputEvent & event)
{
	return false;
}

bool CCryTBUIInputListener::HandleMouseEvents(const SInputEvent & event)
{
	return false;
}

bool CCryTBUIInputListener::HandleUnicodeEvents(const SUnicodeEvent & event)
{
	return false;
}