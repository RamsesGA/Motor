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

			~TexturesDX() { onRelease(); };

			/***********************************************************************/
			/**
			* Hereditary methods.
			*/
			/***********************************************************************/

			/*
			* @brief Return the shader resource view.
			*/
			void*
		  getTexture() override {
				return m_pShaderResourceView;
			};

			void
		  onRelease() {
				SAFE_RELEASE(m_pTexture);
				SAFE_RELEASE(m_pShaderResourceView);
				SAFE_RELEASE(m_pDepthStencilView);
				SAFE_RELEASE(m_pRenderTargetView);
				SAFE_RELEASE(m_pDepthStencilState);
			};

	  protected:
			/**
			* @brief DX member to save the texture.
			*/
			ID3D11Texture2D* m_pTexture = nullptr;

			/**
			* @brief DX member to save the resource view shader.
			*/
			ID3D11ShaderResourceView* m_pShaderResourceView = nullptr;
			
			/**
			* @brief DX member to save the depth stencil view.
			*/
			ID3D11DepthStencilView* m_pDepthStencilView = nullptr;
			
			/**
			* @brief DX member to save the render target view.
			*/
			ID3D11RenderTargetView* m_pRenderTargetView = nullptr;
			
			/**
			* @brief DX member to save the depth stencil state.
			*/
			ID3D11DepthStencilState* m_pDepthStencilState = nullptr;

			friend class GraphicsApiDX;
	};
}