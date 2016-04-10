#include <StdAfx.h>
#include <tb_system.h>
#include <CryWindows.h>
#include <CPluginTurboBadgerUI.h>

void TBDebugOut( const char* message )
{
    if ( TurboBadgerUIPlugin::gPlugin )
    {
        TurboBadgerUIPlugin::gPlugin->LogWarning( message );
    }

    else
    {
        CryLogAlways( message );
    }
}

namespace tb
{
    double TBSystem::GetTimeMS()
    {
        assert( gEnv );
        assert( gEnv->pSystem );

        return gEnv->pSystem->GetITimer()->GetAsyncCurTime();
    }

    void TBSystem::RescheduleTimer( double fire_time )
    {
        // Not implemented since we use an update loop
    }

    int TBSystem::GetLongClickDelayMS()
    {
        // TODO: Move to CVar
        return 500;
    }

    int TBSystem::GetPanThreshold()
    {
        // TODO: Move to CVar
        return 5 * GetDPI() / 96;
    }

    int TBSystem::GetPixelsPerLine()
    {
        // TODO: Move to CVar
        return 40 * GetDPI() / 96;
    }

    int TBSystem::GetDPI()
    {
        assert( gEnv );
        assert( gEnv->pRenderer );

        // FIXME: Not crossplatform :/
        auto hdc = GetDC( static_cast<HWND>( gEnv->pRenderer->GetCurrentContextHWND() ) );
        int dpi = GetDeviceCaps( hdc, LOGPIXELSX );
        ReleaseDC( nullptr, hdc );
        return dpi;
    }
}