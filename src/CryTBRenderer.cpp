#include <StdAfx.h>
#include <CryTBRenderer.h>
#include <CryTBBitmap.h>

tb::TBBitmap* CCryTBRenderer::CreateBitmap( int width, int height, tb::uint32* data )
{
    auto pBM = new CCryTBBitmap( width, height );
    pBM->SetData( data );
    return pBM;
}

void CCryTBRenderer::RenderBatch( Batch* batch )
{
    auto pRenderer = gEnv->pRenderer;
    const float fZ = 1.f;

    SVF_P3F_C4B_T2F* verts = new SVF_P3F_C4B_T2F[batch->vertex_count];

    for ( int i = 0; i < batch->vertex_count; ++i )
    {
        auto batchVtx = batch->vertex[i];
        verts[i].xyz = Vec3 { batchVtx.x, batchVtx.y, fZ };
        verts[i].st = Vec2 { batchVtx.u, batchVtx.v };
        verts[i].color.dcolor = batchVtx.col;
    }

    auto btmp = static_cast<CCryTBBitmap*>( batch->bitmap );

    pRenderer->SetCullMode( R_CULL_DISABLE );
    pRenderer->Set2DMode( true,
                          pRenderer->GetWidth(), pRenderer->GetHeight() );

    pRenderer->SetColorOp( eCO_MODULATE, eCO_MODULATE, DEF_TEXARG0, DEF_TEXARG0 );
    pRenderer->SetState( GS_BLSRC_SRCALPHA | GS_BLDST_ONEMINUSSRCALPHA | GS_NODEPTHTEST );

    if ( btmp )
    {
        pRenderer->SetTexture( btmp->GetCryTexture()->GetTextureID() );
    }

    uint16* pInd = new uint16[batch->vertex_count];

    for ( int i = 0; i < batch->vertex_count; ++i )
    {
        pInd[i] = i;
    }

    pRenderer->DrawDynVB( verts, pInd,
                          batch->vertex_count, batch->vertex_count,
                          prtTriangleList );

    pRenderer->Set2DMode( false, 0, 0 );

    SAFE_DELETE_ARRAY( pInd );
    SAFE_DELETE_ARRAY( verts );
}

void CCryTBRenderer::SetClipRect( const tb::TBRect& rect )
{
    assert( gEnv );
    assert( gEnv->pRenderer );

    gEnv->pRenderer->SetScissor( rect.x, rect.y,
                                 rect.w, rect.h );
}