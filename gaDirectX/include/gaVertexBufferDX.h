#pragma once

#include <gaVertexBuffer.h>

namespace gaEngineSDK {
	class VertexBufferDX final : public VertexBuffer
	{
	  public:
			/***********************************************************************/
			/**
			* Constructor and destructor.
			*/
			/***********************************************************************/
			VertexBufferDX() = default;

			virtual ~VertexBufferDX() = default;

	  protected:
			
			/**
			* @brief DX member to save the buffer.
			*/
			ID3D11Buffer* m_pVertexBuffer;

			friend class GraphicsApiDX;
	};
}