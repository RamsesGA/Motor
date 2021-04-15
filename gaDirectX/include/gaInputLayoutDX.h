#pragma once

#include <d3d11.h>
#include <gaInputLayout.h>

namespace gaEngineSDK {
	class InputLayoutDX final : public InputLayout
	{
	  public:
			/***********************************************************************/
			/**
			* Constructor and destructor.
			*/
			/***********************************************************************/
			InputLayoutDX() = default;

			~InputLayoutDX() = default;

	  protected:
			/**
			* @brief DX member to save byte off set.
			*/
			uint32* m_inputLayoutByteLength;

			/**
			* @brief DX member to save the vertex layout.
			*/
			ID3D11InputLayout* m_pVertexLayout;

			friend class GraphicsApiDX;
	};
}