#pragma once

#include <tb_bitmap_fragment.h>

class CryTBImageLoader : public tb::TBImageLoader
{
    public:
        CryTBImageLoader();
        ~CryTBImageLoader();

        /** Return the width of the loaded bitmap. */
        int Width();

        /** Return the height of the loaded bitmap. */
        int Height();

        /** Return the data of the loaded bitmap.
        This data should always be in 32bit RGBA format. */
        tb::uint32* Data();

        /// <summary>
        /// Opens the specified image as texture in CE.
        /// </summary>
        /// <param name="sFilename">The filename.</param>
        /// <returns>true on succes</returns>
        bool Open( string sFilename );

    private:
        /// <summary>
        /// The CE texture used to get the desirec information
        /// </summary>
        ITexture* _pTexture = nullptr;
};