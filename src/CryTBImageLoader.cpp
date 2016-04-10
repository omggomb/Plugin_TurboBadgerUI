#include <StdAfx.h>
#include "CryTBImageLoader.h"
#include <FreeImage.h>
#include <CPluginTurboBadgerUI.h>

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
    if ( _pBitmap )
    {
        FreeImage_Unload( _pBitmap );
    }
}

int CryTBImageLoader::Width()
{
    int width = 0;

    if ( _pBitmap )
    {
        width = FreeImage_GetWidth( _pBitmap );
    }

    return width;
}

int CryTBImageLoader::Height()
{
    int height = 0;

    if ( _pBitmap )
    {
        height = FreeImage_GetHeight( _pBitmap );
    }

    return height;
}

tb::uint32* CryTBImageLoader::Data()
{
    return reinterpret_cast<tb::uint32*>( FreeImage_GetBits( _pBitmap ) );
}

bool CryTBImageLoader::Open( string sFilename )
{
    assert( gEnv );
    assert( gEnv->pRenderer );

    if ( sFilename.find( "button" ) != string::npos )
    {
        assert( false );
    }

    bool bLoadingSuccess = false;
    ICryPak* pCryPak = gEnv->pCryPak;

    if ( pCryPak->IsFileExist( sFilename ) )
    {
        //FILE* pHandle = pCryPak->FOpen( sFilename, "r" );

        if ( true )
        {
            /*_pBitmap = FreeImage_LoadFromHandle( FIF_PNG,
                                                 &TurboBadgerUIPlugin::gPlugin->GetFreeImgIO(),
                                                 ( fi_handle )pHandle );*/

            _pBitmap = FreeImage_Load( FIF_PNG, sFilename );

            if ( _pBitmap )
            {
                // Make sure we don't have to deal with palettes
                if ( FreeImage_GetBPP( _pBitmap ) != 32 )
                {
                    auto oldImage = _pBitmap;
                    _pBitmap = FreeImage_ConvertTo32Bits( _pBitmap );
                    FreeImage_Unload( oldImage );
                }

                FreeImage_FlipVertical( _pBitmap );

                _sFilename = sFilename;
                bLoadingSuccess = true;
            }

            //pCryPak->FClose( pHandle );
        }
    }

    return bLoadingSuccess;
}