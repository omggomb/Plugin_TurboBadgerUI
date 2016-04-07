#include <StdAfx.h>
#include "CryTBImageLoader.h"

namespace tb
{
    /** Static method used to create an image loader. The system must implement this
    function and create an implementation of the TBImageLoader interface. */
    TBImageLoader* TBImageLoader::CreateFromFile( const char* filename )
    {
        auto pImg = new CryTBImageLoader();
        pImg->Open( filename );
        return pImg;
    }
}

CryTBImageLoader::CryTBImageLoader()
{
}

CryTBImageLoader::~CryTBImageLoader()
{
    assert( gEnv );
    assert( gEnv->pRenderer );

    if ( _pTexture )
    {
        gEnv->pRenderer->RemoveTexture( _pTexture->GetTextureID() );
    }
}

int CryTBImageLoader::Width()
{
    int width = 0;

    if ( _pTexture )
    {
        width = _pTexture->GetWidth();
    }

    return width;
}

int CryTBImageLoader::Height()
{
    int height = 0;

    if ( _pTexture )
    {
        height = _pTexture->GetHeight();
    }

    return height;
}

tb::uint32* CryTBImageLoader::Data()
{
    tb::uint32* pData = nullptr;

    if ( _pTexture )
    {
        // FIXME: This might not work, but the function is named
        // GetData32 so that has to mean something
        pData = reinterpret_cast<tb::uint32*>( _pTexture->GetData32() );
    }

    return pData;
}

bool CryTBImageLoader::Open( string sFilename )
{
    assert( gEnv );
    assert( gEnv->pRenderer );

    _pTexture = gEnv->pRenderer->EF_LoadTexture( sFilename );

    return _pTexture ? true : false;
}