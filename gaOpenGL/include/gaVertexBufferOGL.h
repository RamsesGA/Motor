#pragma once

#include <gaVertexBuffer.h>

namespace gaEngineSDK {
	class VertexBufferOGL final : public VertexBuffer
	{
	  public:
			/***********************************************************************/
			/**
			* Constructor and destructor.
			*/
			/***********************************************************************/
			VertexBufferOGL() = default;

			virtual ~VertexBufferOGL() = default;

	  protected:
			
			/**
			* @brief Member to save position information.
			*/
			unsigned int m_vertexBufferObject;

			/**
			* @brief Member for size.
			*/
			unsigned int m_vertexBufferSize;

			friend class GraphicsApiOGL;
	};
}