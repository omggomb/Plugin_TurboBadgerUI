#include <StdAfx.h>
#include "CryTBBitmap.h"
#include "CryTBRendererEx.h"

tb::TBBitmap * CCryTBRendererEx::CreateBitmap(int width, int height, tb::uint32 * data)
{
	auto pBM = new CCryTBBitmap(width, height);
	pBM->SetData(data);
	return pBM;
}

CCryTBRendererEx::CCryTBRendererEx()
{
}

CCryTBRendererEx::~CCryTBRendererEx()
{
}

void CCryTBRendererEx::BeginPaint(int render_target_w, int render_target_h)
{
}

void CCryTBRendererEx::EndPaint()
{
}

void CCryTBRendererEx::Translate(int dx, int dy)
{
}

void CCryTBRendererEx::SetOpacity(float opacity)
{
}

float CCryTBRendererEx::GetOpacity()
{
	return 0.0f;
}

tb::TBRect CCryTBRendererEx::SetClipRect(const tb::TBRect & rect, bool add_to_current)
{
	return tb::TBRect();
}

tb::TBRect CCryTBRendererEx::GetClipRect()
{
	return tb::TBRect();
}

void CCryTBRendererEx::DrawBitmap(const tb::TBRect & dst_rect, const tb::TBRect & src_rect, tb::TBBitmapFragment * bitmap_fragment)
{
}

void CCryTBRendererEx::DrawBitmap(const tb::TBRect & dst_rect, const tb::TBRect & src_rect, tb::TBBitmap * bitmap)
{
}

void CCryTBRendererEx::DrawBitmapColored(const tb::TBRect & dst_rect, const tb::TBRect & src_rect, const tb::TBColor & color, tb::TBBitmapFragment * bitmap_fragment)
{
}

void CCryTBRendererEx::DrawBitmapColored(const tb::TBRect & dst_rect, const tb::TBRect & src_rect, const tb::TBColor & color, tb::TBBitmap * bitmap)
{
}

void CCryTBRendererEx::DrawBitmapTile(const tb::TBRect & dst_rect, tb::TBBitmap * bitmap)
{
}

void CCryTBRendererEx::DrawRect(const tb::TBRect & dst_rect, const tb::TBColor & color)
{
}

void CCryTBRendererEx::DrawRectFill(const tb::TBRect & dst_rect, const tb::TBColor & color)
{
}

void CCryTBRendererEx::FlushBitmapFragment(tb::TBBitmapFragment * bitmap_fragment)
{
}