#include <StdAfx.h>
#include <CryTBBitmap.h>

uint CCryTBBitmap::s_numBitmapsCreated = 0;

CCryTBBitmap::CCryTBBitmap( int width, int height )
{
    _width = max( 0, width );
    _height = max( 0, height );

    ++s_numBitmapsCreated;
}

CCryTBBitmap::~CCryTBBitmap()
{
    if ( _pTexture )
    {
        gEnv->pRenderer->RemoveTexture( _pTexture->GetTextureID() );
    }
}

int CCryTBBitmap::Width()
{
    return _width;
}

int CCryTBBitmap::Height()
{
    return _height;
}

void CCryTBBitmap::SetData( tb::uint32* data )
{
    assert( gEnv );
    assert( gEnv->pRenderer );

    auto pRend = gEnv->pRenderer;

    if ( !_pTexture )
    {
        string sTextureName = "TBTexture";
        sTextureName += s_numBitmapsCreated;

        _pTexture = pRend->CreateTexture( sTextureName,
                                          _width, _height,
                                          _numMips,
                                          reinterpret_cast<unsigned char*>( data ),
                                          _cryTexFormat,
                                          _cryFlags );
    }

    else
    {
        pRend->UpdateTextureInVideoMemory( _pTexture->GetTextureID(),
                                           reinterpret_cast<unsigned char*>( data ),
                                           0, 0,
                                           _width, _height,
                                           _cryTexFormat );
    }
}

const ITexture* CCryTBBitmap::GetCryTexture() const
{
    return _pTexture;
}