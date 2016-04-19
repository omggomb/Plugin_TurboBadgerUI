#pragma once

#include <RootWidget.h>
#include <IGameFramework.h>
#include <IInput.h>
#include <IActionMapManager.h>
#include "CryTBUIInputListener.h"
#include "CryTBUIActionListener.h"
#include "CryTBRenderer.h"

/// <summary>
/// Manages the TB UI integration into CE.
/// </summary>
/// <seealso cref="IGameFrameworkListener" />
/// <seealso cref="ISystemEventListener" />
namespace TurboBadgerUIPlugin
{
	struct SDefaultSpecs
	{
		static const string UIRoot;
		static 	const string sLanguage;
		static 	const string sFont;
		static const string sFontName;
		static 	const string sSkin;
		static const int nDefaultFontSize;
	};

	class CCryTBUIManager : public IGameFrameworkListener,
		public ISystemEventListener
	{
	public:
		CCryTBUIManager();
		~CCryTBUIManager();

		// IGameFrameworkListener
		void OnPostUpdate(float fDeltaTime) override;

		/* EMPTY */
		void OnSaveGame(ISaveGame* pSaveGame) override {}
		void OnLoadGame(ILoadGame* pLoadGame) override {}
		void OnLevelEnd(const char* nextLevel) override {}
		void OnActionEvent(const SActionEvent &event) override {}
		void OnPreRender() override {}
		// ~IGameFrameworkListener

		// ISystemEventListener
		void OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam) override;

		/* EMPTY */
		void OnSystemEventAnyThread(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam) override {}
		// ~ISystemEventListener

		// CCryTBUIManager
		const bool Init();
		void Reset();
		void Shutdown();
		void AddWidgetToRoot(tb::TBWidget* widget);
		void RemoveWidget(const int index);
		void RemoveWidget(const tb::TBID widgetID);
		void LoadWidgetFile(const string sFilepath, const tb::TBID idOfChild);
		const int GetNumImmediateChildren() const;
		tb::TBWidget* GetImmediateChild(const int index) const;
		tb::TBWidget* GetImmediateChild(const tb::TBID childID) const;
		const bool HasImmediateChild(const tb::TBID searchID) const;
		void SetReceiveExclusiveInput(const bool exclusive);
		void SetUIHidden(const bool hide);
		void SetActive(const bool bIsActive);
		const bool IsActive() const { return _bIsActive; }

		RootWidget* GetRootWidget() const;
		const float GetMWheelInvertFactor() const { return 1.f; } // TODO: Move to CVAR
		// ~CCryTBUIManager

	private:
		const string c_sGameFrameworkListenerName = "CryTBUIManager";
		RootWidget* _rootWidget = nullptr;
		std::vector<tb::TBWidget*> _immediateChildrenList;
		bool _bIsActive = true;

		CCryTBUIInputListener _tbUIInputListener;
		CCryTBUIActionListener _tbUIActionListener;
		CCryTBRenderer _tbUIRenderer;

	private:

		void RemoveWidgetInternal(std::vector<tb::TBWidget*>::iterator iterWhere, bool bRemoveFromVector = true);
	};
}