#include <StdAfx.h>
#include <CryTBRenderer.h>
#include <CryTBBitmap.h>

tb::TBBitmap* CCryTBRenderer::CreateBitmap(int width, int height, tb::uint32* data)
{
	auto pBM = new CCryTBBitmap(width, height);
	pBM->SetData(data);
	return pBM;
}

void CCryTBRenderer::RenderBatch(Batch* batch)
{
	auto pRenderer = gEnv->pRenderer;
	const float fZ = 1.f;

	SVF_P3F_C4B_T2F* verts = new SVF_P3F_C4B_T2F[batch->vertex_count];

	for (int i = 0; i < batch->vertex_count; ++i)
	{
		auto batchVtx = batch->vertex[i];
		verts[i].xyz = Vec3{ batchVtx.x, batchVtx.y, fZ };
		verts[i].st = Vec2{ batchVtx.u, batchVtx.v };
		verts[i].color.dcolor = batchVtx.col;
	}

	auto btmp = static_cast<CCryTBBitmap*>(batch->bitmap);

	pRenderer->Set2DMode(true,
		pRenderer->GetWidth(), pRenderer->GetHeight());

	pRenderer->SetTexture(btmp->GetCryTexture()->GetTextureID());

	pRenderer->SetColorOp(eCO_MODULATE, eCO_MODULATE, DEF_TEXARG0, DEF_TEXARG0);
	pRenderer->SetState(GS_NODEPTHTEST);

	pRenderer->DrawDynVB(verts, nullptr,
		batch->vertex_count, 0,
		prtTriangleList);

	pRenderer->Set2DMode(false, 0, 0);

	SAFE_DELETE_ARRAY(verts);
}

void CCryTBRenderer::SetClipRect(const tb::TBRect& rect)
{
	assert(gEnv);
	assert(gEnv->pRenderer);

	gEnv->pRenderer->SetScissor(rect.x, rect.y,
		rect.w, rect.h);
}