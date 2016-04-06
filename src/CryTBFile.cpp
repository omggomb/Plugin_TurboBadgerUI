#include <StdAfx.h>
#include "CryTBFile.h"

namespace tb
{
    TBFile* TBFile::Open( const char* filename, TBFileMode mode )
    {
        switch ( mode )
        {
            case tb::TBFile::MODE_READ:
                return new CCryTBFile( filename );
                break;

            default:
                break;
        }
    }
}

CCryTBFile::CCryTBFile( string sFilePath )
{
}

CCryTBFile::~CCryTBFile()
{
}

long CCryTBFile::Size()
{
    return 0;
}

size_t CCryTBFile::Read( void* buf, size_t elemSize, size_t count )
{
    return size_t();
}