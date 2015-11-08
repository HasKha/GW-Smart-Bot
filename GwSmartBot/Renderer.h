#pragma once

#include <Windows.h>
#include <d3d9.h>

#include "MyD3DVertex.h"

/*
This class is essentially a glorified vertex buffer, containing everything
that is necessary to render the vertex buffer. 

classes implementing this class only need to implement Initialize which 
should contain code that:
- populates the vertex buffer "buffer_"
- sets the primitive type "type_"
- sets the primitive count "count_"

afterward just call Render and the vertex buffer will be rendered
*/

class Renderer {
public:
	Renderer() 
		: buffer_(nullptr), 
		type_(D3DPT_TRIANGLELIST), count_(0) {}

	virtual void Render(IDirect3DDevice9* device) {
		device->SetFVF(D3DFVF_CUSTOMVERTEX);
		device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		device->SetRenderState(D3DRS_LIGHTING, FALSE);

		device->SetStreamSource(0, buffer_, 0, sizeof(Vertex));
		device->DrawPrimitive(type_, 0, count_);
	}

protected:
	IDirect3DVertexBuffer9* buffer_;
	D3DPRIMITIVETYPE type_;
	unsigned long count_;

private:
	virtual void Initialize(IDirect3DDevice9* device) = 0;
};
