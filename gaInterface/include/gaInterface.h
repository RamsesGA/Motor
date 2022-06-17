#pragma once

#include <gaBaseInterface.h>
#include <gaSceneGraph.h>

#include "gaPrerequisitesInterface.h"

namespace gaEngineSDK {
  using std::to_string;
  using std::cout;

  class Interface final : public BaseInterface
  {
   public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/

    Interface() = default;

    ~Interface() = default;

    /*************************************************************************/
    /**
    * Inheritance.
    */
    /*************************************************************************/

    /*
    * @brief .
    */
    void
    init(uint32 width, uint32 height, void* hwndHandle) override;

    /*
    * @brief .
    */
    void
    update(const float& deltaTime) override;

    /*
    * @brief .
    */
    void
    render() override;

    /*************************************************************************/
    /**
    * Methods.
    */
    /*************************************************************************/

    /*
    * @brief .
    */
    void
    textCentered(String text);

    /*
    * @brief .
    */
    void
    textColoredCentered(String text, Vector3 rgb);

    /*
    * @brief .
    */
    void
    materialsInterface();

    /*
    * @brief .
    */
    void
    transformInterface();

    /*************************************************************************/
    /**
    * Inputs for ImGui.
    */
    /*************************************************************************/

    void
    isMouseTouchingImGui();

    void
    mouseButtonDown(uint32 button) override;

    void
    mouseButtonReleased(uint32 button) override;

    void
    textEnter(wchar_t unicode) override;

    void
    onKeyPressed(bool alt, bool ctrl, bool shift, bool sys, int32 code) override;

    void
    onKeyReleased(bool alt, bool ctrl, bool shift, bool sys, int32 code) override;

    /*************************************************************************/
    /**
    * Methods for ImGui.
    */
    /*************************************************************************/

    void
    imguiDocking();

    void
    imguiOptionsBar();

    void
    imguiScenegraph();

    void
    imguiShowTreeNodes(String childName, WeakSPtr<SceneNode> childNode);

    void
    imguiModelsInfo();

    void
    imguiCurrentFPS();

    void
    omniverseWindow();

    void
    descompressImages() override;

    /*************************************************************************/
    /**
    * Methods for models.
    */
    /*************************************************************************/

    /*
    * @brief .
    */
    void
    createNodePod();
    
    /*
    * @brief .
    */
    void
    createNodeVela();
    
    /*
    * @brief .
    */
    void
    createNode2B();
    
    /*
    * @brief .
    */
    void
    createNodeKnucles();
    
    /*
    * @brief .
    */
    void
    createNodeGrimoires();
    
    /*
    * @brief .
    */
    void
    createNodeRamlethalSwords();
    
    /*
    * @brief .
    */
    void
    createNodeStranger();
    
    /*
    * @brief .
    */
    void
    createNodePlane();

    /*************************************************************************/
    /**
    * Members.
    */
    /*************************************************************************/

    bool m_showOmniImGui = false;

    /*
    * @brief .
    */
    float m_size;

    /*
    * @brief .
    */
    uint32 m_width = 0;

    /*
    * @brief .
    */
    uint32 m_height = 0;

    /*
    * @brief .
    */
    String m_text;

    /*
    * @brief .
    */
    String m_omniFolderPath;

    /*
    * @brief .
    */
    String m_omniStageName;
  };

  /***************************************************************************/
  /**
  * Export.
  */
  /***************************************************************************/

  extern "C" GA_INTERFACE_EXPORT BaseInterface *
  createNewInterface() {
    Interface* pInterface = new Interface();
    return pInterface;
  }
}