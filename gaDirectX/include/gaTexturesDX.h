#pragma once

#include <d3d11.h>
#include <gaTextures.h>

namespace gaEngineSDK {
	class TexturesDX final : public Textures
	{
	  public:
	  	/***********************************************************************/
	  	/**
	  	* Constructor and destructor.
	  	*/
	  	/***********************************************************************/
			TexturesDX() = default;

			~TexturesDX() = default;

	  protected:
			/**
			* @brief DX member to save the texture.
			*/
			ID3D11Texture2D* m_pTexture;

			/**
			* @brief DX member to save the resource view shader.
			*/
			ID3D11ShaderResourceView* m_pShaderResourceView;
			
			/**
			* @brief DX member to save the depth stencil view.
			*/
			ID3D11DepthStencilView* m_pDepthStencilView;
			
			/**
			* @brief DX member to save the render target view.
			*/
			ID3D11RenderTargetView* m_pRenderTargetView;
			
			/**
			* @brief DX member to save the depth stencil state.
			*/
			ID3D11DepthStencilState* m_pDepthStencilState;

			friend class GraphicsApiDX;
	};
}