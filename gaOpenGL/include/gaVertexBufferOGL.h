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
			uint32 m_vertexBufferObject;

			/**
			* @brief Member for size.
			*/
			uint32 m_vertexBufferSize;

			friend class GraphicsApiOGL;
	};
}