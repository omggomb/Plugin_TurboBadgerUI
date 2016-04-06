#pragma once

#include <StdAfx.h>
#include <tb_system.h>

class CCryTBFile : public tb::TBFile
{
    public:
        CCryTBFile( string sFilePath );
        ~CCryTBFile();

        long Size() override;

        size_t Read( void* buf, size_t elemSize, size_t count ) override;

    private:
};