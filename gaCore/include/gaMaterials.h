#pragma once

#include "gaPrerequisitesCore.h"
#include "gaTextures.h"

namespace gaEngineSDK {
  class GA_CORE_EXPORT Materials : public Resource
  {
   public:
    /*************************************************************************/
    /*
    * Constructor and destructor.
    */
    /*************************************************************************/
    Materials() = default;

    ~Materials() = default;

    void
    setTexture(String typeTexture, String file, String textureName);

    void
    getNameFile(const String& file);

    /*************************************************************************/
    /*
    * Inheritance.
    */
    /*************************************************************************/

    void
    loadFromFile(const String& file) override;
    
    void
    loadFromFile(const String& file, TYPE_TEXTURES::E typeTexture) override;

    /*************************************************************************/
    /*
    * Members.
    */
    /*************************************************************************/

    /*
    * @brief .
    */
    String m_matName;

    /*
    * @brief .
    */
    String m_typeTexture;

    /*
    * @brief .
    */
    Vector<Textures*> m_vTextures;

    /*
    * @brief .
    */
    Textures* m_pTexture;
  };
}