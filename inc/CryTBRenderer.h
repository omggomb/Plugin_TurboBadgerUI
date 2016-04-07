#pragma once

#include <renderers\tb_renderer_batcher.h>

class CCryTBRenderer : public tb::TBRendererBatcher
{
    public:
        // TBRenderBatcher
        tb::TBBitmap* CreateBitmap( int width, int height, tb::uint32* data ) override;
        void RenderBatch( Batch* batch ) override;
        void SetClipRect( const tb::TBRect& rect ) override;
        // ~TBRenderBatcher
};