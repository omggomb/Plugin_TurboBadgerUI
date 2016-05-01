#pragma once

#include <tb_renderer.h>

class CCryTBRendererEx : public tb::TBRenderer
{
public:
	CCryTBRendererEx();
	~CCryTBRendererEx();

#pragma region tb::TBRenderer
	//------------------------------
	// tb::TBRenderer
	//------------------------------
	void BeginPaint(int render_target_w, int render_target_h) override;
	void EndPaint() override;

	/** Translate all drawing with the given offset */
	void Translate(int dx, int dy) override;

	/** Set the current opacity that should apply to all drawing (0.f-1.f). */
	void SetOpacity(float opacity) override;
	float GetOpacity() override;

	/** Set a clip rect to the renderer. add_to_current should be true when
	pushing a new cliprect that should clip inside the last clip rect,
	and false when restoring.
	It will return the clip rect that was in use before this call. */
	tb::TBRect SetClipRect(const tb::TBRect &rect, bool add_to_current) override;

	/** Get the current clip rect. Note: This may be different from the rect
	sent to SetClipRect, due to intersecting with the previous cliprect! */
	tb::TBRect GetClipRect() override;

	/** Draw the src_rect part of the fragment stretched to dst_rect.
	dst_rect or src_rect can have negative width and height to achieve horizontal and vertical flip. */
	void DrawBitmap(const tb::TBRect &dst_rect, const tb::TBRect &src_rect, tb::TBBitmapFragment *bitmap_fragment) override;

	/** Draw the src_rect part of the bitmap stretched to dst_rect.
	dst_rect or src_rect can have negative width and height to achieve horizontal and vertical flip. */
	void DrawBitmap(const tb::TBRect &dst_rect, const tb::TBRect &src_rect, tb::TBBitmap *bitmap) override;

	/** Draw the src_rect part of the fragment stretched to dst_rect.
	The bitmap will be used as a mask for the color.
	dst_rect or src_rect can have negative width and height to achieve horizontal and vertical flip. */
	void DrawBitmapColored(const tb::TBRect &dst_rect, const tb::TBRect &src_rect, const tb::TBColor &color, tb::TBBitmapFragment *bitmap_fragment) override;

	/** Draw the src_rect part of the bitmap stretched to dst_rect.
	The bitmap will be used as a mask for the color.
	dst_rect or src_rect can have negative width and height to achieve horizontal and vertical flip. */
	void DrawBitmapColored(const tb::TBRect &dst_rect, const tb::TBRect &src_rect, const tb::TBColor &color, tb::TBBitmap *bitmap) override;

	/** Draw the bitmap tiled into dst_rect. */
	void DrawBitmapTile(const tb::TBRect &dst_rect, tb::TBBitmap *bitmap) override;

	/** Draw a 1px thick rectangle outline. */
	void DrawRect(const tb::TBRect &dst_rect, const tb::TBColor &color) override;

	/** Draw a filled rectangle. */
	void DrawRectFill(const tb::TBRect &dst_rect, const tb::TBColor &color) override;

	/** Make sure the given bitmap fragment is flushed from any batching, because it may
	be changed or deleted after this call. */
	void FlushBitmapFragment(tb::TBBitmapFragment *bitmap_fragment) override;

	/** Create a new TBBitmap from the given data (in BGRA32 format).
	Width and height must be a power of two.
	Return nullptr if fail. */
	tb::TBBitmap *CreateBitmap(int width, int height, uint32 *data) override;
	//------------------------------
	// ~tb::TBRenderer
	//------------------------------
#pragma endregion

	void SetClipRect(const tb::TBRect &rect);
	const int GetRenderTargetID() const;
	void Render2DUI();

private:
	void CreateNewRenderTarget(const int nWidth, const int nHeight);
	void RenderTextureInternal(const tb::TBRect & dst_rect, const tb::TBRect & src_rect, const tb::TBColor & color, const ITexture * pSrcTexture);
	const Vec4 ConvertTBcolorToFloatVec4(const tb::TBColor color) const;

private:
	float _fOpacity = 1.f;
	Vec2 _vTranslation = Vec2(0, 0);
	Vec2 _oldRenderTargetDim = Vec2(-1, -1);
	tb::TBRect _clipRect = tb::TBRect(0, 0, 1, 1);
	ITexture* _pCERenderTargetTexture = nullptr;
	unsigned char* _pRTData = nullptr;

	ITexture* _pDefaultUVsTexture = nullptr;
};