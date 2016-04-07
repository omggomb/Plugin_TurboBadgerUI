#include <StdAfx.h>
#include "CryTBFile.h"

namespace tb
{
    TBFile* TBFile::Open( const char* filename, TBFileMode mode )
    {
        auto pFile = new CCryTBFile();
        pFile->Open( filename, mode );
        return pFile;
    }
}

CCryTBFile::CCryTBFile()
{
}

CCryTBFile::~CCryTBFile()
{
    assert( gEnv );
    assert( gEnv->pCryPak );

    if ( _pFileHandle )
    {
        gEnv->pCryPak->FClose( _pFileHandle );
    }
}

bool CCryTBFile::Open( string sFilename, tb::TBFile::TBFileMode mode )
{
    assert( gEnv );
    assert( gEnv->pCryPak );

    auto pCryPak = gEnv->pCryPak;

    if ( !pCryPak->IsFileExist( sFilename ) )
    {
        CryWarning( EValidatorModule::VALIDATOR_MODULE_GAME,
                    EValidatorSeverity::VALIDATOR_WARNING,
                    "Tried opening file: %s, which does not exist", sFilename );
        return false;
    }

    string sOpenMode;

    switch ( mode )
    {
        case tb::TBFile::MODE_READ:
            sOpenMode = "r";
            break;

        default:
            break;
    }

    sOpenMode += "b";

    _pFileHandle = pCryPak->FOpen( sFilename, sOpenMode );

    if ( _pFileHandle )
    {
        _size = pCryPak->FGetSize( _pFileHandle );
    }

    return _pFileHandle ? true : false; // readability
}

long CCryTBFile::Size()
{
    return _size;
}

size_t CCryTBFile::Read( void* buf, size_t elemSize, size_t count )
{
    assert( gEnv );
    assert( gEnv->pCryPak );

    size_t read = 0;

    if ( _pFileHandle )
    {
        gEnv->pCryPak->FReadRaw( buf, count, elemSize, _pFileHandle );
    }

    else
    {
        CryWarning( EValidatorModule::VALIDATOR_MODULE_GAME,
                    EValidatorSeverity::VALIDATOR_WARNING,
                    "Tried reading file: %s, which is not opened", _sFilename );
    }

    return read;
}