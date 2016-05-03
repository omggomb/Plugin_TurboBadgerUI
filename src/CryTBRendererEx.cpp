#include <StdAfx.h>
#include <tb_bitmap_fragment.h>
#include "CryTBBitmap.h"
#include "CryTBRendererEx.h"

tb::TBBitmap * CCryTBRendererEx::CreateBitmap(int width, int height, tb::uint32 * data)
{
	auto pBM = new CCryTBBitmap(width, height);
	pBM->SetData(data);
	return pBM;
}

void CCryTBRendererEx::SetClipRect(const tb::TBRect & rect)
{
	gEnv->pRenderer->SetScissor(rect.x, rect.y,
		rect.w, rect.h);
}

const int CCryTBRendererEx::GetRenderTargetID() const
{
	return _pCERenderTargetTexture ? _pCERenderTargetTexture->GetTextureID() : -1;
}

void CCryTBRendererEx::Render2DUI()
{
	SVF_P3F_C4B_T2F verts[6];

	float fZ = 1.f;

	// TL
	verts[0].xyz = Vec3(0, 0, fZ);
	verts[0].st = Vec2(0.f, 1.f);
	verts[0].color.dcolor = 0xffffffff;

	// BL
	verts[1].xyz = Vec3(0, 256, fZ);
	verts[1].st = Vec2(0.f, 0.f);
	verts[1].color.dcolor = 0xffffffff;

	//TR
	verts[2].xyz = Vec3(256, 0, fZ);
	verts[2].st = Vec2(1.f, 1.f);
	verts[2].color.dcolor = 0xffffffff;

	//_------------------------------------

	// BL
	verts[3].xyz = Vec3(0, 256, fZ);
	verts[3].st = Vec2(0.f, 0.f);
	verts[3].color.dcolor = 0xffffffff;

	// BR
	verts[4].xyz = Vec3(256, 256, fZ);
	verts[4].st = Vec2(1.f, 0.f);
	verts[4].color.dcolor = 0xffffffff;

	// TR
	verts[5].xyz = Vec3(256, 0, fZ);
	verts[5].st = Vec2(1.f, 1.f);
	verts[5].color.dcolor = 0xffffffff;

	uint16 inds[]
	{
		0, 1, 2, 3, 4, 5,
	};

	gEnv->pRenderer->SetTexture(_pCERenderTargetTexture->GetTextureID());

	gEnv->pRenderer->DrawDynVB(verts, inds, 6, 6, prtTriangleList);
}

void CCryTBRendererEx::CreateNewRenderTarget(const int nWidth, const int nHeight)
{
	assert(gEnv);
	assert(gEnv->pRenderer);

	if (_pRTData)
	{
		delete[] _pRTData;
		_pRTData = nullptr;
	}

	_pRTData = new unsigned char[nWidth * nHeight * 4];

	for (int i = 0; i < (nWidth * nHeight * 4); i += 4)
	{
		_pRTData[i + 3] = 0x00;
	}

	int nTextureFlags = FT_NOMIPS | FT_DONT_STREAM | FT_DONT_RELEASE
		| FT_USAGE_RENDERTARGET;

	_pCERenderTargetTexture = gEnv->pRenderer->CreateTexture("TBUIRenderTarget",
		nWidth, nHeight,
		1,
		nullptr,
		eTF_B8G8R8A8,
		nTextureFlags);

	_pCERenderTargetTexture->SetHighQualityFiltering(true);

	// TODO: Remove after debug
	_pDefaultUVsTexture = gEnv->pRenderer->EF_GetTextureByName("engineassets/texturemsg/replaceme.tif");

	if (!_pDefaultUVsTexture)
	{
		_pDefaultUVsTexture = gEnv->pRenderer->EF_LoadTexture("engineassets/texturemsg/replaceme.tif");
	}

	_pDefaultUVsTexture->SetHighQualityFiltering();
}

CCryTBRendererEx::CCryTBRendererEx()
{
}

CCryTBRendererEx::~CCryTBRendererEx()
{
}

void CCryTBRendererEx::BeginPaint(int render_target_w, int render_target_h)
{
	auto pRenderer = gEnv->pRenderer;
	assert(pRenderer);

	if (Vec2(render_target_w, render_target_h) != _oldRenderTargetDim)
	{
		_oldRenderTargetDim = Vec2(render_target_w, render_target_h);
		CreateNewRenderTarget(render_target_w, render_target_h);
	}

	pRenderer->SetCullMode(R_CULL_DISABLE);
	pRenderer->Set2DMode(true,
		pRenderer->GetWidth(), pRenderer->GetHeight());

	pRenderer->SetColorOp(eCO_MODULATE, eCO_MODULATE, DEF_TEXARG0, DEF_TEXARG0);
	pRenderer->SetState(GS_BLSRC_SRCALPHA | GS_BLDST_ONEMINUSSRCALPHA | GS_NODEPTHTEST);
}

void CCryTBRendererEx::EndPaint()
{
	auto pRenderer = gEnv->pRenderer;
	assert(pRenderer);

	pRenderer->Set2DMode(false, 0, 0);
}

void CCryTBRendererEx::Translate(int dx, int dy)
{
	_vTranslation = Vec2(dx, dy);
}

void CCryTBRendererEx::SetOpacity(float opacity)
{
	_fOpacity = opacity;
}

float CCryTBRendererEx::GetOpacity()
{
	return _fOpacity;
}

tb::TBRect CCryTBRendererEx::SetClipRect(const tb::TBRect & rect, bool add_to_current)
{
	tb::TBRect old_clip_rect = _clipRect;
	_clipRect = rect;
	_clipRect.x += _vTranslation.x;
	_clipRect.y += _vTranslation.y;

	if (add_to_current)
	{
		_clipRect = _clipRect.Clip(old_clip_rect);
	}

	SetClipRect(_clipRect);

	old_clip_rect.x -= _vTranslation.x;
	old_clip_rect.y -= _vTranslation.y;
	return old_clip_rect;
}

tb::TBRect CCryTBRendererEx::GetClipRect()
{
	tb::TBRect curr_clip_rect = _clipRect;
	curr_clip_rect.x -= _vTranslation.x;
	curr_clip_rect.y -= _vTranslation.y;
	return curr_clip_rect;
}

void CCryTBRendererEx::DrawBitmap(const tb::TBRect & dst_rect, const tb::TBRect & src_rect, tb::TBBitmapFragment * bitmap_fragment)
{
	dst_rect.Offset(_vTranslation.x, _vTranslation.y);
	DrawBitmap(dst_rect, src_rect, bitmap_fragment->GetBitmap());
}

void CCryTBRendererEx::DrawBitmap(const tb::TBRect & dst_rect, const tb::TBRect & src_rect, tb::TBBitmap * bitmap)
{
	dst_rect.Offset(_vTranslation.x, _vTranslation.y);
	auto cryTBBitmap = static_cast<CCryTBBitmap*>(bitmap);

	RenderTextureInternal(dst_rect, src_rect, tb::TBColor(255, 255, 255), cryTBBitmap->GetCryTexture());
}

void CCryTBRendererEx::DrawBitmapColored(const tb::TBRect & dst_rect, const tb::TBRect & src_rect, const tb::TBColor & color, tb::TBBitmapFragment * bitmap_fragment)
{
	dst_rect.Offset(_vTranslation.x, _vTranslation.y);
	DrawBitmapColored(dst_rect, src_rect, color, bitmap_fragment->GetBitmap());
}

void CCryTBRendererEx::DrawBitmapColored(const tb::TBRect & dst_rect, const tb::TBRect & src_rect, const tb::TBColor & color, tb::TBBitmap * bitmap)
{
	dst_rect.Offset(_vTranslation.x, _vTranslation.y);
	auto cryTBBitmap = static_cast<CCryTBBitmap*>(bitmap);

	RenderTextureInternal(dst_rect, src_rect, color, cryTBBitmap->GetCryTexture());
}

void CCryTBRendererEx::DrawBitmapTile(const tb::TBRect & dst_rect, tb::TBBitmap * bitmap)
{
	dst_rect.Offset(_vTranslation.x, _vTranslation.y);
	int timesX = dst_rect.w / bitmap->Width();
	int timesY = dst_rect.h / bitmap->Height();

	tb::TBRect srcRect(0, 0, bitmap->Width(), bitmap->Height());

	tb::TBRect newDst(dst_rect.x, dst_rect.y, bitmap->Width(), bitmap->Height());

	for (int i = 0; i < timesY; i++)
	{
		int oldNewDstX = newDst.x;
		for (int j = 0; j < timesX; ++j)
		{
			newDst.x += bitmap->Width();
			DrawBitmap(newDst, srcRect, bitmap);
		}
		newDst.x = oldNewDstX;
		newDst.y += bitmap->Height();
	}
}

void CCryTBRendererEx::DrawRect(const tb::TBRect & dst_rect, const tb::TBColor & color)
{
	dst_rect.Offset(_vTranslation.x, _vTranslation.y);
	if (!dst_rect.IsEmpty())
	{
		// Top
		DrawRectFill(tb::TBRect(dst_rect.x, dst_rect.y, dst_rect.w, 1), color);
		// Bottom
		DrawRectFill(tb::TBRect(dst_rect.x, dst_rect.y + dst_rect.h - 1, dst_rect.w, 1), color);
		// Left
		DrawRectFill(tb::TBRect(dst_rect.x, dst_rect.y + 1, 1, dst_rect.h - 2), color);
		// Right
		DrawRectFill(tb::TBRect(dst_rect.x + dst_rect.w - 1, dst_rect.y + 1, 1, dst_rect.h - 2), color);
	}
}

void CCryTBRendererEx::DrawRectFill(const tb::TBRect & dst_rect, const tb::TBColor & color)
{
	dst_rect.Offset(_vTranslation.x, _vTranslation.y);
	auto whiteID = gEnv->pRenderer->GetWhiteTextureId();
	auto pWhiteTexture = gEnv->pRenderer->EF_GetTextureByID(whiteID);

	tb::TBRect srcRect = tb::TBRect(0, 0, pWhiteTexture->GetWidth(), pWhiteTexture->GetHeight());

	RenderTextureInternal(dst_rect, srcRect, color, pWhiteTexture);
}

void CCryTBRendererEx::FlushBitmapFragment(tb::TBBitmapFragment * bitmap_fragment)
{
	// No batching atm, so no implementation needed
}

void CCryTBRendererEx::RenderTextureInternal(const tb::TBRect & dst_rect, const tb::TBRect & src_rect, const tb::TBColor & color, const ITexture* pSrcTexture)
{
	tb::TBRect dstRect = dst_rect;
	tb::TBRect srcRect = src_rect;

	/*dstRect.y = _pCERenderTargetTexture->GetHeight() - dstRect.y;

	srcRect.y = pSrcTexture->GetHeight() - srcRect.y;*/

	float bitmapWidth = pSrcTexture->GetWidth();
	float bitmapHeight = pSrcTexture->GetHeight();

	float u0 = srcRect.x / bitmapWidth;
	float v0 = srcRect.y / bitmapHeight;
	float u1 = (srcRect.x + srcRect.w) / bitmapWidth;
	float v1 = (srcRect.y + srcRect.h) / bitmapHeight;

	float fTargetWidth = _pCERenderTargetTexture->GetWidth();
	float fTargetHeight = _pCERenderTargetTexture->GetHeight();

	float fX = dstRect.x ? (float)srcRect.x / dstRect.x : 0;
	float fY = dstRect.y ? (float)srcRect.y / dstRect.y : 0;
	float fWidth = dstRect.w ? (float)srcRect.w / dstRect.w : 0;
	float fHeight = dstRect.h ? (float)srcRect.h / dstRect.h : 0;

	fX = dstRect.x / fTargetWidth;
	fY = dstRect.y / fTargetHeight;
	fY = 1.f - fY;
	fY -= dstRect.h / fTargetHeight;
	fWidth = (dstRect.x + dstRect.w) / fTargetWidth;
	fHeight = (dstRect.y + dstRect.h) / fTargetHeight;
	const Vec4 colorFloat = ConvertTBcolorToFloatVec4(color);

	// DEBUG
	/*u0 = v0 = 0.f;
	u1 = v1 = 1.f;

	fX = fY = 0.0f;
	fWidth = fHeight = 1.0f;*/
	// ~DEBUG

	//fY = 1.f - fY;
	/*v0 = 1.f - v0;
	v1 = 1.f - v1;*/

	/*v0 += (srcRect.h / bitmapHeight);
	v1 -= (srcRect.h / bitmapHeight);*/

	/*float fTemp = v0;
	v0 = v1;
	v1 = fTemp;*/

	gEnv->pRenderer->PushUITexture(pSrcTexture->GetTextureID(),
		_pCERenderTargetTexture->GetTextureID(),
		fX, fY, fWidth, fHeight,
		v0, v0, u1, v1,
		colorFloat.x, colorFloat.y, colorFloat.z, colorFloat.w * _fOpacity);
}

const Vec4 CCryTBRendererEx::ConvertTBcolorToFloatVec4(const tb::TBColor color) const
{
	float r = color.r ? 255 / color.r : 0.f;
	float g = color.g ? 255 / color.g : 0.f;
	float b = color.b ? 255 / color.b : 0.f;
	float a = color.a ? 255 / color.a : 0.f;

	return Vec4(r, g, b, a);
}