#include <StdAfx.h>
#include <tb_system.h>
#include <IHardwareMouse.h>

#include "CryTBUIInputListener.h"
#include "CryTBUIManager.h"
#include "CryTBUIKeyUtils.h"
#include "RootWidget.h"

namespace TurboBadgerUIPlugin
{
	CCryTBUIInputListener::CCryTBUIInputListener()
	{
	}
	CCryTBUIInputListener::~CCryTBUIInputListener()
	{
	}
	bool CCryTBUIInputListener::OnInputEvent(const SInputEvent & event)
	{
		bool bRet = false;
		bool bConsole = gEnv->pConsole->IsOpened();
		if (_bIsActive && !bConsole)
		{
			switch (event.deviceType)
			{
			case eIDT_Mouse:
				bRet = HandleMouseEvents(event);
				break;
			case eIDT_Keyboard:
				bRet = HandleKeyboardEvents(event);
				break;
			default:
				break;
			}
		}
		return _bReceiveExclusive;
	}

	bool CCryTBUIInputListener::OnInputEventUI(const SUnicodeEvent & event)
	{
		bool bRet = false;
		bool bConsole = gEnv->pConsole->IsOpened();

		if (_bIsActive && !bConsole)
		{
			bRet = HandleUnicodeEvents(event);
		}
		return _bReceiveExclusive;
	}

	bool CCryTBUIInputListener::Init(CCryTBUIManager * pTBUIManager)
	{
		assert(pTBUIManager);
		assert(gEnv);
		assert(gEnv->pInput);

		_pTBUIManager = pTBUIManager;

		_rootWidget = _pTBUIManager->GetRootWidget();

		gEnv->pInput->AddEventListener(this);

		return true;
	}

	void CCryTBUIInputListener::Shutdown()
	{
		assert(gEnv);
		assert(gEnv->pInput);

		gEnv->pInput->RemoveEventListener(this);
	}

	void CCryTBUIInputListener::SetReceiveExclusiveInput(const bool bExclusive)
	{
		_bReceiveExclusive = bExclusive;
	}

	void CCryTBUIInputListener::SetActive(const bool bIsActive)
	{
		_bIsActive = bIsActive;
	}

	bool CCryTBUIInputListener::HandleKeyboardEvents(const SInputEvent & event)
	{
		// All regular unicode text input is handeled seperately, so only take care of the the remaining buttons

		if (event.state == eIS_Pressed)// Only handle presses, since we can only invoke keys as press release combination
		{
			tb::MODIFIER_KEYS modifiers = CCryTBUIKeyUtils::CryModiToTBModi((EModifierMask)gEnv->pInput->GetModifiers());
			tb::SPECIAL_KEY specialKeys = CCryTBUIKeyUtils::CryKeyToTBSpecial(event.keyId);

			_rootWidget->InvokeKey(0, specialKeys, modifiers, true);
		}
		// FIXME: Make this dependant on whether the UI should actually receive exclusively
		return false;
	}

	bool CCryTBUIInputListener::HandleMouseEvents(const SInputEvent & event)
	{
		float x = 0;
		float y = 0;
		gEnv->pHardwareMouse->GetHardwareMouseClientPosition(&x, &y);

		tb::MODIFIER_KEYS modifiers = CCryTBUIKeyUtils::CryModiToTBModi((EModifierMask)event.modifiers);

		switch (event.keyId)
		{
		case (eKI_Mouse1) :
		{
			if (event.state == eIS_Pressed)
			{
				_rootWidget->InvokePointerDown(x, y, 1, modifiers, false);
			}
			else if (event.state == eIS_Released)
			{
				_rootWidget->InvokePointerUp(x, y, modifiers, false);
			}
		} break;
		case(eKI_Mouse2) :
		case(eKI_Mouse3) :
		{
			// TODO: Handle middle and right mouse button (Need to use TBEvents for that
		}break;
		case(eKI_MouseWheelUp) :
		case(eKI_MouseWheelDown) :
		{
			if (event.state == eIS_Pressed) // Only handle once
			{
				int delta = (int)event.value / tb::TBSystem::GetPixelsPerLine() * _pTBUIManager->GetMWheelInvertFactor();
				_rootWidget->InvokeWheel(x, y, 0, delta, modifiers);
			}
		}break;
		case (eKI_MouseX) :
		case (eKI_MouseY) :
		{
			_rootWidget->InvokePointerMove(x, y, modifiers, false);
		}break;
		default:
			break;
		}

		// FIXME: Make this dependant on whether the UI should actually receive exclusively
		return false;
	}

	bool CCryTBUIInputListener::HandleUnicodeEvents(const SUnicodeEvent & event)
	{
		// MODIFIERS

		tb::MODIFIER_KEYS modifierKeys = CCryTBUIKeyUtils::CryModiToTBModi((EModifierMask)gEnv->pInput->GetModifiers());

		// SPECIAL KEYS
		tb::SPECIAL_KEY specialKey = CCryTBUIKeyUtils::CryUniToTBSpecial(event.inputChar);

		// FIXME: Make this dependant on whether the UI should actually receive exclusively
		_rootWidget->InvokeKey(event.inputChar, specialKey, modifierKeys, true);
		return false;
	}
}