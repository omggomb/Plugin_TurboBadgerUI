#include <StdAfx.h>
#include <tb_widgets_reader.h>
#include <tb_language.h>
#include <tb_font_renderer.h>

#include <IHardwareMouse.h>

#include "CPluginTurboBadgerUI.h"
#include "CryTBUIManager.h"
#include "CryTBUIActionListener.h"

void register_tbbf_font_renderer();

namespace TurboBadgerUIPlugin
{
	const string SDefaultSpecs::UIRoot = "./Assets/UI/";
	const string SDefaultSpecs::sLanguage = UIRoot + "resources/language/lng_en.tb.txt";
	const string SDefaultSpecs::sFont = UIRoot + "resources/default_font/segoe_white_with_shadow.tb.txt";
	const string SDefaultSpecs::sFontName = "Segoe";
	const string SDefaultSpecs::sSkin = UIRoot + "resources/default_skin/skin.tb.txt";
	const int SDefaultSpecs::nDefaultFontSize = 14;

	CCryTBUIManager::CCryTBUIManager() :
		_rootWidget(new RootWidget())
	{
	}

	CCryTBUIManager::~CCryTBUIManager()
	{
		SAFE_DELETE(_rootWidget);
	}

	void CCryTBUIManager::OnPostUpdate(float fDeltaTime)
	{
		if (_bIsActive)
		{
			// Update UI
			tb::TBMessageHandler::ProcessMessages();
			_rootWidget->InvokeProcessStates();
			_rootWidget->InvokeProcess();

			// Render UI
			_tbUIRenderer.BeginPaint(_rootWidget->GetRect().w,
				_rootWidget->GetRect().h);

			_rootWidget->InvokePaint(tb::TBWidget::PaintProps());

			_tbUIRenderer.EndPaint();
		}
	}

	void CCryTBUIManager::OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam)
	{
		// Not influenced by _bIsActive!

		switch (event)
		{
		case ESYSTEM_EVENT_RESIZE:
			_rootWidget->Resize(wparam, lparam);
			break;
		case ESYSTEM_EVENT_CHANGE_FOCUS:
			SetActive(wparam);
			break;

		default:
			break;
		}
	}

	const bool CCryTBUIManager::Init()
	{
		assert(gEnv);
		assert(gEnv->pSystem);
		assert(gEnv->pGame);
		assert(gEnv->pGame->GetIGameFramework());

		// Register for system events
		if (!gEnv->pSystem->GetISystemEventDispatcher()->RegisterListener(this))
		{
			TurboBadgerUIPlugin::gPlugin->LogError("Failed to register CryTBUIManager for system events");
			return false;
		}

		// Register for game framework evetns
		gEnv->pGame->GetIGameFramework()->RegisterListener(this, c_sGameFrameworkListenerName, FRAMEWORKLISTENERPRIORITY_HUD);

		if (!_rootWidget->Init(this))
		{
			return false;
		}

		// Init input listener
		if (!_tbUIInputListener.Init(this))
		{
			return false;
		}

		// Init action listener
		if (!_tbUIActionListener.Init(this))
		{
			return false;
		}

		// Init TB
		if (!tb::tb_core_init(&_tbUIRenderer))
		{
			return false;
		}

		// New font renderer
		register_tbbf_font_renderer();

		Reset();

		return true;
	}

	void CCryTBUIManager::Reset()
	{
		assert(gPlugin);

		if (!tb::g_tb_lng->Load(SDefaultSpecs::sLanguage))
		{
			gPlugin->LogWarning("Failed to load default language on reset");
		}

		if (!tb::g_tb_skin->Load(SDefaultSpecs::sSkin))
		{
			gPlugin->LogWarning("Failed to load default skin on reset");
		}

		tb::g_font_manager->AddFontInfo(SDefaultSpecs::sFont, SDefaultSpecs::sFontName);

		tb::TBFontDescription desc;
		desc.SetID(tb::TBID(SDefaultSpecs::sFontName));
		desc.SetSize(tb::g_tb_skin->GetDimensionConverter()->DpToPx(SDefaultSpecs::nDefaultFontSize));
		tb::g_font_manager->SetDefaultFontDescription(desc);

		// Required
		tb::g_font_manager->CreateFontFace(tb::g_font_manager->GetDefaultFontDescription());

		_rootWidget->Resize(gEnv->pRenderer->GetWidth(), gEnv->pRenderer->GetHeight());
	}

	void CCryTBUIManager::Shutdown()
	{
		assert(gEnv);
		assert(gEnv->pSystem);
		assert(gEnv->pGame);
		assert(gEnv->pGame->GetIGameFramework());

		_tbUIActionListener.Shutdown();

		_tbUIInputListener.Shutdown();

		gEnv->pGame->GetIGameFramework()->UnregisterListener(this);

		if (!gEnv->pSystem->GetISystemEventDispatcher()->RemoveListener(this))
		{
			TurboBadgerUIPlugin::gPlugin->LogWarning("Failed to unregister CryTBUIManager for system events");
		}

		// Delete all widgets
		for (auto it = _immediateChildrenList.begin(); it != _immediateChildrenList.end(); ++it)
		{
			RemoveWidgetInternal(it, false);
		}

		_immediateChildrenList.clear();

		tb::tb_core_shutdown();
	}

	void CCryTBUIManager::AddWidgetToRoot(tb::TBWidget * widget)
	{
		if (widget)
		{
			if (!HasImmediateChild(widget->GetID()))
			{
				// FIXME: Maybe copy it so the caller can't delete it?
				_immediateChildrenList.push_back(widget);
				_rootWidget->AddChild(widget);
			}
			else
			{
				TurboBadgerUIPlugin::gPlugin->LogWarning("Tried to add a widget to the root widget whose ID is already present.");
			}
		}
		else
		{
			TurboBadgerUIPlugin::gPlugin->LogError("Call to AddWidgetToRoot received nullpointer");
		}
	}

	void CCryTBUIManager::RemoveWidget(const int index)
	{
		if (index < 0 || index >= _immediateChildrenList.size())
		{
			TurboBadgerUIPlugin::gPlugin->LogWarning("Tried to remove immediate child with out of bounds index: %d", index);
		}
		else
		{
			auto widgetToRemoveIter = (_immediateChildrenList.begin() + index);

			RemoveWidgetInternal(widgetToRemoveIter);
		}
	}

	void CCryTBUIManager::RemoveWidget(const tb::TBID widgetID)
	{
		if (HasImmediateChild(widgetID))
		{
			auto widgetToRemoveIter = std::find_if(_immediateChildrenList.begin(),
				_immediateChildrenList.end(),
				[widgetID](tb::TBWidget* widget)
			{
				return widget->GetID() == widgetID;
			});

			RemoveWidgetInternal(widgetToRemoveIter);
		}
		else
		{
			TurboBadgerUIPlugin::gPlugin->LogWarning("Tried to remove non existing immediate child: %d", widgetID);
		}
	}

	void CCryTBUIManager::LoadWidgetFile(const string sFilepath, const tb::TBID idOfChild)
	{
		tb::TBWidget* childWidget = GetImmediateChild(idOfChild);

		if (childWidget)
		{
			tb::g_widgets_reader->LoadFile(childWidget, sFilepath);
		}
		else
		{
			TurboBadgerUIPlugin::gPlugin->LogWarning("Tried to load file %s into immediate child %d, bot widget doesn't exist", sFilepath, idOfChild);
		}
	}

	const int CCryTBUIManager::GetNumImmediateChildren() const
	{
		return _immediateChildrenList.size();
	}

	tb::TBWidget* CCryTBUIManager::GetImmediateChild(const int index) const
	{
		tb::TBWidget* widget = nullptr;

		if (index < 0 || index >= _immediateChildrenList.size())
		{
			TurboBadgerUIPlugin::gPlugin->LogWarning("Tried to get immediate child with out of bounds index: %d", index);
		}
		else
		{
			widget = _immediateChildrenList[index];
		}

		return widget;
	}

	tb::TBWidget* CCryTBUIManager::GetImmediateChild(const tb::TBID childID) const
	{
		auto it = std::find_if(_immediateChildrenList.begin(),
			_immediateChildrenList.end(),
			[childID](tb::TBWidget* widget)
		{
			return widget->GetID() == childID;
		});

		tb::TBWidget* outwidget = nullptr;
		if (it != _immediateChildrenList.end())
		{
			outwidget = *it;
		}

		return outwidget;
	}

	const bool CCryTBUIManager::HasImmediateChild(const tb::TBID searchID) const
	{
		return GetImmediateChild(searchID) == nullptr ? false : true;
	}

	void CCryTBUIManager::SetReceiveExclusiveInput(const bool exclusive)
	{
		_tbUIActionListener.SetReceiveExclusiveInput(exclusive);
		_tbUIInputListener.SetReceiveExclusiveInput(exclusive);
	}

	void CCryTBUIManager::SetUIHidden(const bool hide)
	{
		SetActive(!hide);

		if (hide)
		{
			_rootWidget->SetVisibility(tb::WIDGET_VISIBILITY_INVISIBLE);
		}
		else
		{
			_rootWidget->SetVisibility(tb::WIDGET_VISIBILITY_VISIBLE);
		}
	}

	void CCryTBUIManager::SetActive(const bool bIsActive)
	{
		_tbUIActionListener.SetActive(bIsActive);
		_tbUIInputListener.SetActive(bIsActive);

		_bIsActive = bIsActive;
	}

	void CCryTBUIManager::SetModalMode(const bool bModal)
	{
		//SetReceiveExclusiveInput(true);

		////gEnv->pHardwareMouse->Hide(!bModal);

		//if (bModal)
		//{
		//	gEnv->pHardwareMouse->IncrementCounter();
		//	gEnv->pHardwareMouse->IncrementCounter();
		//	gEnv->pHardwareMouse->IncrementCounter();
		//}
		//else
		//{
		//	gEnv->pHardwareMouse->DecrementCounter();
		//}
	}

	RootWidget * CCryTBUIManager::GetRootWidget() const
	{
		return _rootWidget;
	}

	void CCryTBUIManager::RemoveWidgetInternal(std::vector<tb::TBWidget*>::iterator iterWhere, bool bRemoveFromVector)
	{
		_rootWidget->RemoveChild(*iterWhere);

		if (bRemoveFromVector)
		{
			_immediateChildrenList.erase(iterWhere);
		}

		delete *iterWhere;
	}
}