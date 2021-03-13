#pragma once

#include <gaIndexBuffer.h>

namespace gaEngineSDK {
	class IndexBufferOGL final : public IndexBuffer
	{
	  public:
			/***********************************************************************/
			/**
			* Constructor and destructor.
			*/
			/***********************************************************************/
			IndexBufferOGL() = default;

			virtual ~IndexBufferOGL() = default;

	  protected:
			/**
			* @brief Member to save the index.
			*/
			unsigned int m_indexBufferObject;
			/**
			* @brief Member to store size.
			*/
			unsigned int m_indexBufferSize;

			friend class GraphicsApiOGL;
	};
}