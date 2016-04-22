#pragma once

#include <renderers\tb_renderer_batcher.h>

class CCryTBRendererEx : public tb::TBRendererBatcher
{
public:
	CCryTBRendererEx();
	~CCryTBRendererEx();

	virtual TBBitmap *CreateBitmap(int width, int height, uint32 *data) = 0;
	virtual void RenderBatch(Batch *batch) = 0;
	virtual void SetClipRect(const TBRect &rect) = 0;
};