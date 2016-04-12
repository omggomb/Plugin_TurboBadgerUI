/* TurboBadgerUI_Plugin - for licensing and copyright see license.txt */

#pragma once

#include <IPluginBase.h>
//#include <Game.h>

#include <IPluginManager.h>
#include <CPluginBase.hpp>

#include <IPluginTurboBadgerUI.h>

#define PLUGIN_NAME "TurboBadgerUI"
#define PLUGIN_CONSOLE_PREFIX "[" PLUGIN_NAME " " PLUGIN_TEXT "] " //!< Prefix for Logentries by this plugin

#include <tb_window.h>

#include <FreeImage.h>

class CCryTBRenderer;

namespace TurboBadgerUIPlugin
{
	/**
	* @brief Provides information and manages the resources of this plugin.
	*/
	class CPluginTurboBadgerUI :
		public PluginManager::CPluginBase,
		public IPluginTurboBadgerUI,
		public IGameFrameworkListener,
		public IInputEventListener
	{
	public:
		CPluginTurboBadgerUI();
		~CPluginTurboBadgerUI();

		// IPluginBase
		bool Release(bool bForce = false);

		int GetInitializationMode() const
		{
			return int(PluginManager::IM_Default);
		}

		bool Init(SSystemGlobalEnvironment& env, SSystemInitParams& startupParams, IPluginBase* pPluginManager, const char* sPluginDirectory);

		bool RegisterTypes(int nFactoryType, bool bUnregister);

		const char* GetVersion() const
		{
			return "1.0.0.0";
		}

		const char* GetName() const
		{
			return PLUGIN_NAME;
		}

		const char* GetCategory() const
		{
			return "Visual";
		}

		const char* ListAuthors() const
		{
			return "Martin 'omggomb' Voehringer <omggomb at gmx dot de>";
		}

		const char* ListCVars() const;

		const char* GetStatus() const;

		const char* GetCurrentConcreteInterfaceVersion() const
		{
			return "1.0";
		}

		void* GetConcreteInterface(const char* sInterfaceVersion)
		{
			return static_cast <IPluginTurboBadgerUI*>(this);
		}

		// IPluginTurboBadgerUI
		IPluginBase* GetBase()
		{
			return static_cast<IPluginBase*>(this);
		}

		// IGameFrameworkListener
		void OnPostUpdate(float fDeltaTime) override;
		void OnSaveGame(ISaveGame* pSaveGame) override {}
		void OnLoadGame(ILoadGame* pLoadGame) override {}
		void OnLevelEnd(const char* nextLevel) override {}
		void OnActionEvent(const SActionEvent& event) override {}
		// ~IGameFrameworkListener

		// IInputEvenListener
		bool OnInputEvent(const SInputEvent& event) override;
		bool OnInputEventUI(const SUnicodeEvent& event) override;

		// ~IInputEventListener

		// TODO: Add your concrete interface implementation
		FreeImageIO& GetFreeImgIO()
		{
			return _freeImgIO;
		}
	private:
		void SetupFreeImageIO();

		bool HandleMouseEvents(const SInputEvent& event);
		bool HandleKeyboardEvents(const SInputEvent& event);

		CCryTBRenderer* _pCryTBRenderer = nullptr;
		tb::TBWidget _rootWidget;
		FreeImageIO _freeImgIO;
	};

	extern CPluginTurboBadgerUI* gPlugin;
}

/**
* @brief This function is required to use the Autoregister Flownode without modification.
* Include the file "CPluginTurboBadgerUI.h" in front of flownode.
*/
/*inline void GameWarning( const char* sFormat, ... ) PRINTF_PARAMS( 1, 2 );
inline void GameWarning( const char* sFormat, ... )
{
	va_list ArgList;
	va_start( ArgList, sFormat );
	TurboBadgerUIPlugin::gPlugin->LogV( ILog::eWarningAlways, sFormat, ArgList );
	va_end( ArgList );
};*/
