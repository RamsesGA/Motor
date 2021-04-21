#pragma once

#include <d3d11.h>
#include <gaConstantBuffer.h>

namespace gaEngineSDK {
	class ConstantBufferDX final : public ConstantBuffer
	{
	  public:
			/***********************************************************************/
	    /**
	    * Constructor and destructor.
	    */
	    /***********************************************************************/
			ConstantBufferDX() = default;

			virtual ~ConstantBufferDX() = default;

	  protected:
			/**
			* @brief DX member to save the buffer.
			*/
			ID3D11Buffer* m_pConstantBuffer = nullptr;

			friend class GraphicsApiDX;
	};
}