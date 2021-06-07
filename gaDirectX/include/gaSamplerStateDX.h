#pragma once

#include <d3d11.h>
#include <gaSamplerState.h>

namespace gaEngineSDK {
	class SamplerStateDX final : public SamplerState
	{
	  public:
			/***********************************************************************/
			/**
			* Constructor and destructor.
			*/
			/***********************************************************************/
			SamplerStateDX() = default;

			~SamplerStateDX() { SAFE_RELEASE(m_pSamplerState); };

		protected:
		  /**
		  * @brief DX member to save the sampler state.
		  */
		  ID3D11SamplerState* m_pSamplerState = nullptr;

			friend class GraphicsApiDX;
	};
}