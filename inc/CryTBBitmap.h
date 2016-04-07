#pragma once

#include <tb_renderer.h>

/// <summary>
/// Represenst a rendered bitmap.
/// Note that updating currently creates a new CE texture each time.
/// </summary>
/// <seealso cref="tb::TBBitmap" />
class CCryTBBitmap : public tb::TBBitmap
{
    public:
        CCryTBBitmap( int width, int height );
        // TBBitmap
        int Width() override;
        int Height() override;

        /** Update the bitmap with the given data (in BGRA32 format).
        Note: Implementations for batched renderers should call TBRenderer::FlushBitmap
        to make sure any active batch is being flushed before the bitmap is changed. */
        void SetData( tb::uint32* data ) override;
        // ~TBBitmap

        /// <summary>
        /// Gets the cry texture.
        /// </summary>
        /// <returns>Pointer to this bitmaps texture or nullptr if none</returns>
        const ITexture* GetCryTexture() const;
    private:
        /// <summary>
        /// Total number of bitmaps created (including updates).
        /// Used for naming.
        /// </summary>
        static uint s_numBitmapsCreated;

        /// <summary>
        /// Number of mips set when creating a new CE texture
        /// </summary>
        const int _numMips = 0;
        /// <summary>
        /// Texture format used for creating a new CE texture
        /// </summary>
        const ETEX_Format _cryTexFormat = eTF_B8G8R8A8;
        /// <summary>
        /// Texture creation flags used for creating a new CE texture
        /// </summary>
        const int _cryFlags = 0;

        int _width = 0;
        int _height = 0;
        ITexture* _pTexture = nullptr;
};
