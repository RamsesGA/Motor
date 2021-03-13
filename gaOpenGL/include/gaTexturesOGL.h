#pragma once

#include <gaTextures.h>

namespace gaEngineSDK {
	class TexturesOGL final : public Textures
	{
	  public:
	  	/***********************************************************************/
	  	/**
	  	* Constructor and destructor.
	  	*/
	  	/***********************************************************************/
			TexturesOGL() = default;

			virtual ~TexturesOGL() = default;

	  protected:
			/**
			* @brief Member to save position information.
			*/
			unsigned int m_texture;
			
			/**
			* @brief Member to save position information.
			*/
			unsigned char m_data;
			
			/**
			* @brief It can be the render target view of DX.
			*/
			unsigned int m_framebuffer;
			
			/**
			* @brief It can be the depth Stencil.
			*/
			unsigned int m_renderBufferObject;

			friend class GraphicsApiOGL;
	};
}