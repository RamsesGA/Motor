#pragma once

#include <gaConstantBuffer.h>

namespace gaEngineSDK {
	class ConstantBufferOGL final : public ConstantBuffer
	{
	  public:
			/***********************************************************************/
	    /**
	    * Constructor and destructor.
	    */
	    /***********************************************************************/
			ConstantBufferOGL() = default;

			virtual ~ConstantBufferOGL() = default;

	  protected:
			/**
			* @brief Member to store the index.
			*/
			unsigned int m_uniformBufferObject;
			/**
			* @brief Member to save the buffer size.
			*/
			unsigned int m_bufferSize;

			friend class GraphicsApiOGL;
	};
}