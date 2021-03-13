#pragma once

#include <gaShaders.h>

namespace gaEngineSDK {
	class ShadersOGL final : public Shaders
	{
	  public:
			/***********************************************************************/
			/**
			* Constructor and destructor.
			*/
			/***********************************************************************/
			ShadersOGL() = default;

			virtual ~ShadersOGL() = default;

	  protected:
			/**
			* @brief Member to link the program.
			*/
			uint32_t m_rendererID;
			
			/**
			* @brief Member to save position information
			*/
			unsigned int m_vertexShader;
			
			/**
			* @brief Member to save position information.
			*/
			unsigned int m_fragmentShader;

			friend class GraphicsApiOGL;
	};
}