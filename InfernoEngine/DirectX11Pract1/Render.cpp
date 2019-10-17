
#include "global.h"


void Render()
{
	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; //red,green,blue,alpha
	g_pImmediateContext->ClearRenderTargetView( g_pRenderTargetView, ClearColor );
	g_pSwapChain->Present( 0, 0 );

}