#include <StdAfx.h>
#include "CryTBImageLoader.h"

namespace tb
{
    /** Static method used to create an image loader. The system must implement this
    function and create an implementation of the TBImageLoader interface. */
    TBImageLoader* TBImageLoader::CreateFromFile( const char* filename )
    {
        return nullptr;
    }
}

CryTBImageLoader::CryTBImageLoader()
{
}

CryTBImageLoader::~CryTBImageLoader()
{
}

int CryTBImageLoader::Width()
{
    return 0;
}

int CryTBImageLoader::Height()
{
    return 0;
}

tb::uint32* CryTBImageLoader::Data()
{
    return nullptr;
}