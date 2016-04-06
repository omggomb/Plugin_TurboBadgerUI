#include <StdAfx.h>
#include <tb_system.h>

void TBDebugOut( const char* message )
{
}

namespace tb
{
    double TBSystem::GetTimeMS()
    {
        return 0;
    }

    void TBSystem::RescheduleTimer( double fire_time )
    {
    }

    int TBSystem::GetLongClickDelayMS()
    {
        return 0;
    }

    int TBSystem::GetPanThreshold()
    {
        return 0;
    }

    int TBSystem::GetPixelsPerLine()
    {
        return 0;
    }

    int TBSystem::GetDPI()
    {
        return 0;
    }
}