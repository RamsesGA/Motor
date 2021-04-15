#pragma once

#include <gaInputLayout.h>

namespace gaEngineSDK {
	class InputLayoutOGL final : public InputLayout
	{
	  public:
			/***********************************************************************/
			/**
			* Constructor and destructor.
			*/
			/***********************************************************************/
			InputLayoutOGL() = default;

			virtual ~InputLayoutOGL() = default;

	  protected:
			/**
			* @brief In OGL it is known as vertex array object.
			*/
			uint32 m_inputLayout;

			friend class GraphicsApiOGL;
	};
}