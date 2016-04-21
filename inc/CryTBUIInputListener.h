#pragma once
#include <IInput.h>

namespace TurboBadgerUIPlugin
{
	class CCryTBUIManager;
	class RootWidget;

	class CCryTBUIInputListener : public IInputEventListener
	{
	public:
		CCryTBUIInputListener();
		~CCryTBUIInputListener();

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
		RootWidget* _rootWidget = nullptr;
		bool _bIsActive = true;
		bool _bReceiveExclusive = false;
	private:

		bool HandleKeyboardEvents(const SInputEvent &event);
		bool HandleMouseEvents(const SInputEvent &event);
		bool HandleUnicodeEvents(const SUnicodeEvent& event);
	};
}