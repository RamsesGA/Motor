#pragma once
#include <gaVector2.h>
#include <gaVector3.h>
#include <gaVector4.h>
#include <gaMatrix4x4.h>

#include "gaPrerequisitesCore.h"
#include "gaTextures.h"

using namespace gaEngineSDK;

/***************************************************************************/
/**
* Structs
*/
/***************************************************************************/
namespace ConstantBuffer1 {
	struct E {
		Matrix4x4 mView;
		Matrix4x4 mProjection;
	};
}

namespace ConstantBuffer2 {
	struct E {
		Matrix4x4 mWorld;
		Vector4 vMeshColor;
	};
}

namespace Matrices {
	struct E {
		Matrix4x4 World;
		Matrix4x4 Projection;
		Vector4   Color;
	};
}

namespace ViewCB {
	struct E {
		Matrix4x4 View;
	};
}

namespace Vertex {
	struct E {
		Vector3 position;
		Vector2 texCoords;
		Vector3 normal;
		Vector3 tangent;
		Vector3 bitangent;
	};
}

namespace Texture {
	struct E {
		String type;
		String path;
		Textures* texture;
	};
}