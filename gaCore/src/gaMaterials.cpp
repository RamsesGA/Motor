#include "gaMaterials.h"
#include "gaGraphicsApi.h"

namespace gaEngineSDK {

  void
  Materials::setTexture(String typeTexture, String file, String textureName) {
    m_typeTexture = typeTexture;
    m_matName = textureName;
    loadFromFile(file);
  }

  /***************************************************************************/
  /*
  * Inheritance.
  */
  /***************************************************************************/
  void 
  Materials::loadFromFile(const String& file) {
    auto myGraphicsApi = g_graphicApi().instancePtr();
    m_vTextures.push_back(myGraphicsApi->loadTextureFromFile(file));
    m_vTextures[m_vTextures.size()-1]->m_textureName = m_matName;
  }
}