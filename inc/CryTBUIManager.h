#pragma once

#include <RootWidget.h>
#include <IGameFramework.h>
#include <IInput.h>
#include <IActionMapManager.h>
#include "CryTBUIInputListener.h"
#include "CryTBUIActionListener.h"
#include "CryTBRenderer.h"
#include "CryTBRendererEx.h"

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
		friend CCryTBUIActionListener;
		friend CCryTBUIInputListener;
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
		/// <summary>
		/// Initializes this instance.
		/// </summary>
		/// <returns>true on succes</returns>
		const bool Init();
		/// <summary>
		/// Resets this instance.
		/// </summary>
		void Reset();
		/// <summary>
		/// Shuts down this instance and the TB framework
		/// </summary>
		void Shutdown();
		/// <summary>
		/// Adds the widget to the root widget.
		/// </summary>
		/// <param name="widget">The widget to be added</param>
		void AddWidgetToRoot(tb::TBWidget* widget);
		/// <summary>
		/// Removes the widget with the specified index, ignores invalid indices
		/// </summary>
		/// <param name="index">The index of the widget to be removed.</param>
		/// <param name="bDoDelete">Also delete the removed widget?</param>
		/// <returns>The removed widget if bDoDelete is false, else nullptr</returns>
		tb::TBWidget* RemoveWidget(const int index, const bool bDoDelete = true) const;
		/// <summary>
		/// Removes the widget with the specified id, ignores invalid ids
		/// </summary>
		/// <param name="index">The id of the widget to be removed.</param>
		/// <param name="bDoDelete">Also delete the removed widget?</param>
		/// <returns>The removed widget if bDoDelete is false, else nullptr</returns>
		tb::TBWidget* RemoveWidget(const tb::TBID widgetID, const bool bDoDelete = true) const;
		/// <summary>
		/// Loads a widget file using tb::WidgetReader
		/// </summary>
		/// <param name="sFilepath">The filepath.</param>
		/// <param name="idOfChild">The identifier of child to be used as root.</param>
		void LoadWidgetFile(const string sFilepath, const tb::TBID idOfChild);
		/// <summary>
		/// Gets the number of immediate children. <b>Do not use (legacy compatibility)</b>
		/// </summary>
		/// <returns></returns>
		const int GetNumImmediateChildren() const;

		/// <summary>
		/// Gets the immediate child from the specified index
		/// </summary>
		/// <param name="index">The index of the child</param>
		/// <returns>The child for valid indices else nullptr</returns>
		tb::TBWidget* GetImmediateChild(const int index) const;
		/// <summary>
		/// Gets the immediate child from the specified id
		/// </summary>
		/// <param name="index">The id of the child</param>
		/// <returns>The child for valid ids else nullptr</returns>
		tb::TBWidget* GetImmediateChild(const tb::TBID childID) const;
		/// <summary>
		/// Determines whether an immediate child with the specified id exists
		/// </summary>
		/// <param name="searchID">The search identifier.</param>
		/// <returns>true if such a child exists else false</returns>
		const bool HasImmediateChild(const tb::TBID searchID) const;
		/// <summary>
		/// Make the UI the exclusive eventhandler for inputs (raw input and actions)
		/// </summary>
		/// <param name="exclusive">True for exclusive, false to process and forward to other systems</param>
		void SetReceiveExclusiveInput(const bool exclusive);
		/// <summary>
		/// Sets the UI hidden.
		/// </summary>
		/// <param name="hide">True to hide, false to unhide</param>
		void SetUIHidden(const bool hide);
		/// <summary>
		/// Activates or deactivates the complete UI system
		/// </summary>
		/// <param name="bIsActive">true to activate, false to deactivate</param>
		void SetActive(const bool bIsActive);
		/// <summary>
		/// Determines whether this instance is active.
		/// </summary>
		/// <returns></returns>
		const bool IsActive() const { return _bIsActive; }
		/// <summary>
		/// Same as SetReceiveExclusiveInput, but also shows the mouse cursor
		/// </summary>
		/// <param name="bModal">true to enable, false to disable (hide cursor)</param>
		void SetModalMode(const bool bModal);
		/// <summary>
		/// Gets the mouse wheel invert factor
		/// </summary>
		/// <returns></returns>
		const float GetMWheelInvertFactor() const { return 1.f; } // TODO: Move to CVAR

	protected:
		/// <summary>
		/// Gets the root widget.
		/// </summary>
		/// <returns></returns>
		RootWidget* GetRootWidget() const;
		// ~CCryTBUIManager

	private:
		const string c_sGameFrameworkListenerName = "CryTBUIManager";
		RootWidget* _rootWidget = nullptr;
		bool _bIsActive = true;
		bool _bShowMouseCursor = false;

		CCryTBUIInputListener _tbUIInputListener;
		CCryTBUIActionListener _tbUIActionListener;
		CCryTBRenderer _tbUIRenderer;
		CCryTBRendererEx _tbUIRendererEx;

	private:
		void SetShowMouseCursor(const bool bShow);
	};
}