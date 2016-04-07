#pragma once

#include <StdAfx.h>
#include <tb_system.h>

/// <summary>
/// CryPak implementation of the TBFile interface
/// </summary>
/// <seealso cref="tb::TBFile" />
class CCryTBFile : public tb::TBFile
{
    public:
        CCryTBFile();
        ~CCryTBFile();

        /// <summary>
        /// Opens the specified filename.
        /// </summary>
        /// <param name="sFilename">The filename.</param>
        /// <param name="mode">The mode.</param>
        /// <seealso cref="tg::TBFile::TBFileMode/>
        /// <returns>true on success</returns>
        bool Open( string sFilename, tb::TBFile::TBFileMode mode );

        // TBFile
        long Size() override;

        size_t Read( void* buf, size_t elemSize, size_t count ) override;
        // ~TBFile

    private:
        /// <summary>
        /// The path to the opened file, if any
        /// </summary>
        string _sFilename = "";
        /// <summary>
        /// The handle to the openend file (nullptr if none open or failed)
        /// </summary>
        FILE* _pFileHandle = nullptr;
        /// <summary>
        /// The size of the file or 0 if none opened
        /// </summary>
        long _size = 0L;
};
