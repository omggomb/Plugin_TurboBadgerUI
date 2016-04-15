/* TurboBadgerUI_Plugin - for licensing and copyright see license.txt */

#include <StdAfx.h>
#include <CPluginTurboBadgerUI.h>
#include <tb_core.h>
#include "CryTBRenderer.h"
#include <tb_message_window.h>
#include <tb_widgets_reader.h>
#include <tb_language.h>
#include <tb_font_renderer.h>
#include <IHardwareMouse.h>
#include <tb_system.h>

#include "CryTBUIKeyUtils.h"
namespace TurboBadgerUIPlugin
{
	void HandleFreeImgError(FREE_IMAGE_FORMAT format, const char* sMessage)
	{
		if (gPlugin)
		{
			gPlugin->LogError(sMessage);
		}

		else
		{
			CryWarning(EValidatorModule::VALIDATOR_MODULE_GAME,
				EValidatorSeverity::VALIDATOR_ERROR,
				sMessage);
		}
	}

	unsigned DLL_CALLCONV FreeImgIORead(void* buffer, unsigned int size, unsigned int count, fi_handle handle)
	{
		assert(gEnv);
		assert(gEnv->pCryPak);

		return gEnv->pCryPak->FReadRaw(buffer, size, count, (FILE*)handle);
	}

	unsigned DLL_CALLCONV FreeImgIOWrite(void* buffer, unsigned size, unsigned count, fi_handle handle)
	{
		assert(gEnv);
		assert(gEnv->pCryPak);

		return gEnv->pCryPak->FWrite(buffer, size, count, (FILE*)handle);
	}

	int DLL_CALLCONV FreeImgIOSeek(fi_handle handle, long offset, int origin)
	{
		assert(gEnv);
		assert(gEnv->pCryPak);

		return gEnv->pCryPak->FSeek((FILE*)handle, offset, origin);
	}

	long DLL_CALLCONV FreeImgIOTell(fi_handle handle)
	{
		assert(gEnv);
		assert(gEnv->pCryPak);

		return gEnv->pCryPak->FTell((FILE*)handle);
	}

	CPluginTurboBadgerUI* gPlugin = NULL;

	CPluginTurboBadgerUI::CPluginTurboBadgerUI()
	{
		gPlugin = this;
	}

	CPluginTurboBadgerUI::~CPluginTurboBadgerUI()
	{
		Release(true);

		gPlugin = NULL;
	}

	bool CPluginTurboBadgerUI::Release(bool bForce)
	{
		bool bRet = true;
		bool bWasInitialized = m_bIsFullyInitialized; // Will be reset by base class so backup

		if (!m_bCanUnload)
		{
			// Note: Type Unregistration will be automatically done by the Base class (Through RegisterTypes)
			// Should be called while Game is still active otherwise there might be leaks/problems
			bRet = CPluginBase::Release(bForce);

			if (bRet)
			{
				if (bWasInitialized)
				{
					gEnv->pGame->GetIGameFramework()->UnregisterListener(this);
					// TODO: Cleanup stuff that can only be cleaned up if the plugin was initialized
					tb::tb_core_shutdown();
					SAFE_DELETE(_pCryTBRenderer);
				}

				// Cleanup like this always (since the class is static its cleaned up when the dll is unloaded)
				gPluginManager->UnloadPlugin(GetName());

				// Allow Plugin Manager garbage collector to unload this plugin
				AllowDllUnload();
			}
		}

		return bRet;
	};

	bool CPluginTurboBadgerUI::Init(SSystemGlobalEnvironment& env, SSystemInitParams& startupParams, IPluginBase* pPluginManager, const char* sPluginDirectory)
	{
		gPluginManager = (PluginManager::IPluginManager*)pPluginManager->GetConcreteInterface(NULL);
		CPluginBase::Init(env, startupParams, pPluginManager, sPluginDirectory);

		FreeImage_SetOutputMessage(HandleFreeImgError);
		SetupFreeImageIO();

		_pCryTBRenderer = new CCryTBRenderer();
		tb::tb_core_init(_pCryTBRenderer);

		gEnv->pGame->GetIGameFramework()->RegisterListener(this, "TBUI", EFRAMEWORKLISTENERPRIORITY::FRAMEWORKLISTENERPRIORITY_HUD);
		gEnv->pInput->AddEventListener(this);

		int width = 128;
		int height = 128;

		width = gEnv->pRenderer->GetWidth();
		height = gEnv->pRenderer->GetHeight();

		/*width = 128;
		height = 512;*/

		auto blah = tb::TBRect(0, 0, width, height);
		_rootWidget.SetRect(blah);

		bool b = tb::g_tb_lng->Load(".\\bin\\win_x64\\Plugins\\TurboBadgerUI\\resources\\language\\lng_en.tb.txt");
		b = tb::g_tb_skin->Load(".\\bin\\win_x64\\Plugins\\TurboBadgerUI\\resources\\default_skin\\skin.tb.txt", ".\\bin\\win_x64\\Plugins\\TurboBadgerUI\\resources\\skin\\skin.tb.txt");

		void register_tbbf_font_renderer();

		register_tbbf_font_renderer();

		tb::g_font_manager->AddFontInfo(".\\bin\\win_x64\\Plugins\\TurboBadgerUI\\resources\\default_font\\segoe_white_with_shadow.tb.txt", "Segoe");

		tb::TBFontDescription desc;
		desc.SetID(tb::TBID("Segoe"));
		desc.SetSize(tb::g_tb_skin->GetDimensionConverter()->DpToPx(14));
		tb::g_font_manager->SetDefaultFontDescription(desc);

		ShowCursor(true);

		// Create the font now.
		tb::TBFontFace* font = tb::g_font_manager->CreateFontFace(tb::g_font_manager->GetDefaultFontDescription());

		// Render some glyphs in one go now since we know we are going to use them. It would work fine
		// without this since glyphs are rendered when needed, but with some extra updating of the glyph bitmap.
		//if (font)
		//font->RenderGlyphs(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~•·åäöÅÄÖ");

		auto window = new tb::TBWindow();
		b = tb::g_widgets_reader->LoadFile(window, ".\\bin\\win_x64\\Plugins\\TurboBadgerUI\\ui_resources\\test_textwindow.tb.txt");

		_rootWidget.AddChild(window);

		window->ResizeToFitContent();
		window->EnsureFocus();

		window = new tb::TBWindow();
		tb::g_widgets_reader->LoadFile(window, ".\\bin\\win_x64\\Plugins\\TurboBadgerUI\\ui_resources\\test_textwindow.tb.txt");

		_rootWidget.AddChild(window);

		window->ResizeToFitContent();
		window->EnsureFocus();

		return true;
	}

	bool CPluginTurboBadgerUI::RegisterTypes(int nFactoryType, bool bUnregister)
	{
		// Note: Autoregister Flownodes will be automatically registered by the Base class
		bool bRet = CPluginBase::RegisterTypes(nFactoryType, bUnregister);

		using namespace PluginManager;
		eFactoryType enFactoryType = eFactoryType(nFactoryType);

		if (bRet)
		{
			if (gEnv && gEnv->pSystem && !gEnv->pSystem->IsQuitting())
			{
				// UIEvents
				if (gEnv->pConsole && (enFactoryType == FT_All || enFactoryType == FT_UIEvent))
				{
					if (!bUnregister)
					{
						// TODO: Register CVars here if you have some
						// ...
					}

					else
					{
						// TODO: Unregister CVars here if you have some
						// ...
					}
				}

				// CVars
				if (gEnv->pConsole && (enFactoryType == FT_All || enFactoryType == FT_CVar))
				{
					if (!bUnregister)
					{
						// TODO: Register CVars here if you have some
						// ...
					}

					else
					{
						// TODO: Unregister CVars here if you have some
						// ...
					}
				}

				// CVars Commands
				if (gEnv->pConsole && (enFactoryType == FT_All || enFactoryType == FT_CVarCommand))
				{
					if (!bUnregister)
					{
						// TODO: Register CVar Commands here if you have some
						// ...
					}

					else
					{
						// TODO: Unregister CVar Commands here if you have some
						// ...
					}
				}

				// Game Objects
				if (gEnv->pGame && gEnv->pGame->GetIGameFramework() && (enFactoryType == FT_All || enFactoryType == FT_GameObjectExtension))
				{
					if (!bUnregister)
					{
						// TODO: Register Game Object Extensions here if you have some
						// ...
					}
				}
			}
		}

		return bRet;
	}

	const char* CPluginTurboBadgerUI::ListCVars() const
	{
		return "..."; // TODO: Enter CVARs/Commands here if you have some
	}

	const char* CPluginTurboBadgerUI::GetStatus() const
	{
		return "OK";
	}

	void CPluginTurboBadgerUI::OnPostUpdate(float fDeltaTime)
	{
		tb::TBMessageHandler::ProcessMessages();
		_rootWidget.InvokeProcessStates();
		_rootWidget.InvokeProcess();

		_pCryTBRenderer->BeginPaint(gEnv->pRenderer->GetWidth(),
			gEnv->pRenderer->GetHeight());

		_rootWidget.InvokePaint(tb::TBWidget::PaintProps());

		_pCryTBRenderer->EndPaint();
	}

	bool CPluginTurboBadgerUI::OnInputEvent(const SInputEvent& event)
	{
		bool bRet = false;

		if (!gEnv->pConsole->IsOpened()) // Don't handle input if console is open
		{
			switch (event.deviceType)
			{
			case (eIDT_Mouse) :
				bRet = HandleMouseEvents(event);
				break;

			case (eIDT_Keyboard) :
				bRet = HandleKeyboardEvents(event);
				break;

			default:
				break;
			}
		}

		return bRet;
	}

	void CPluginTurboBadgerUI::SetupFreeImageIO()
	{
		_freeImgIO.read_proc = FreeImgIORead;
		_freeImgIO.seek_proc = FreeImgIOSeek;
		_freeImgIO.tell_proc = FreeImgIOTell;
		_freeImgIO.write_proc = FreeImgIOWrite;
	}

	bool CPluginTurboBadgerUI::HandleMouseEvents(const SInputEvent& event)
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
				_rootWidget.InvokePointerDown(x, y, 1, modifiers, false);
			}
			else if (event.state == eIS_Released)
			{
				_rootWidget.InvokePointerUp(x, y, modifiers, false);
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
				int delta = (int)event.value / tb::TBSystem::GetPixelsPerLine() * GetMWheelInvertFactor();
				_rootWidget.InvokeWheel(x, y, 0, delta, modifiers);
			}
		}break;
		case (eKI_MouseX) :
		case (eKI_MouseY) :
		{
			_rootWidget.InvokePointerMove(x, y, modifiers, false);
		}break;
		default:
			break;
		}

		// FIXME: Make this dependant on whether the UI should actually receive exclusively
		return false;
	}

	bool CPluginTurboBadgerUI::OnInputEventUI(const SUnicodeEvent & event)
	{
		// MODIFIERS

		tb::MODIFIER_KEYS modifierKeys = CCryTBUIKeyUtils::CryModiToTBModi((EModifierMask)gEnv->pInput->GetModifiers());

		// SPECIAL KEYS
		tb::SPECIAL_KEY specialKey = CCryTBUIKeyUtils::CryUniToTBSpecial(event.inputChar);

		// FIXME: Make this dependant on whether the UI should actually receive exclusively
		_rootWidget.InvokeKey(event.inputChar, specialKey, modifierKeys, true);
		return false;
	}

	bool CPluginTurboBadgerUI::HandleKeyboardEvents(const SInputEvent& event)
	{
		// All regular unicode text input is handeled seperately, so only take care of the the remaining buttons

		if (event.state == eIS_Pressed)// Only handle presses, since we can only invoke keys as press release combination
		{
			tb::MODIFIER_KEYS modifiers = CCryTBUIKeyUtils::CryModiToTBModi((EModifierMask)gEnv->pInput->GetModifiers());
			tb::SPECIAL_KEY specialKeys = CCryTBUIKeyUtils::CryKeyToTBSpecial(event.keyId);

			_rootWidget.InvokeKey(0, specialKeys, modifiers, true);
		}
		// FIXME: Make this dependant on whether the UI should actually receive exclusively
		return false;
	}

	// TODO: Add your plugin concrete interface implementation
}