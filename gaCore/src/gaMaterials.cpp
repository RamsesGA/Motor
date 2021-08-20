#include "gaMaterials.h"
#include "gaGraphicsApi.h"

namespace gaEngineSDK {

  void
  Materials::setTexture(String typeTexture, String file, String textureName) {
    m_typeTexture = typeTexture;
    m_matName = textureName;
    loadFromFile(file);
  }

  void 
  Materials::getNameFile(const String& file) {
    String tempName = "";
    String tempMatName = "";

    uint32 sizeFile = file.size();
    for (uint32 i = 0; i < sizeFile; ++i) {
      tempName += file[i];

      if ('/' == file[i]) {
        tempName.clear();
      }
    }

    m_pTexture->m_textureName = tempName;
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
    m_vTextures[m_vTextures.size() - 1]->m_typeTexture = m_typeTexture;
  }

  void 
  Materials::loadFromFile(const String& file, TYPE_TEXTURES::E typeTexture) {
    auto myGraphicsApi = g_graphicApi().instancePtr();
    m_pTexture = myGraphicsApi->loadTextureFromFile(file);

    switch (typeTexture) {
      case gaEngineSDK::TYPE_TEXTURES::kDiffuse:
        m_pTexture->m_typeTexture = "kDiffuse";
        break;
      case gaEngineSDK::TYPE_TEXTURES::kAlbedo:
        m_pTexture->m_typeTexture = "kAlbedo";
        break;
      case gaEngineSDK::TYPE_TEXTURES::kMetallic:
        m_pTexture->m_typeTexture = "kMetallic";
        break;
      case gaEngineSDK::TYPE_TEXTURES::kNormal:
        m_pTexture->m_typeTexture = "kNormal";
        break;
      case gaEngineSDK::TYPE_TEXTURES::kRoughness:
        m_pTexture->m_typeTexture = "kRoughness";
        break;
      case gaEngineSDK::TYPE_TEXTURES::kSpecular:
        m_pTexture->m_typeTexture = "kSpecular";
        break;
      case gaEngineSDK::TYPE_TEXTURES::kAO:
        m_pTexture->m_typeTexture = "kAO";
        break;
      case gaEngineSDK::TYPE_TEXTURES::kEmissive:
        m_pTexture->m_typeTexture = "kEmissive";
        break;
      default:
        m_pTexture->m_typeTexture = "kNothing";
        break;
    }

    getNameFile(file);
  }
}