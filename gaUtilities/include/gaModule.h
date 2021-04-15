#pragma once

#include "gaPrerequisitesUtilities.h"

namespace gaEngineSDK {

  template<class T>
  class Module
  {
    public:

      /*
      * @brief We return a reference to the instance of the initialized module.
      */
      static T&
      instance() {
        if (!(isStartedUp())) {
        }
    
        if (isDestroyed()) {
        }

        return *_instance();
      }
    
      /*
      * @brief We return a pointer to the instance of the initialized module.
      */
      static T*
      instancePtr() {
        if (!(isStartedUp())) {
        }
    
        if (isDestroyed()) {
        }

        return _instance();
      }
    
      /*
      * @brief Initialize the module using specific parameters.
      */
      template<class... Args>
      static void
      startUp(Args&& ...args) {
        if (isStartedUp()) {
        }
    
        _instance() = new T(std::forward<Args>(args)...);
        isStartedUp() = true;
    
        static_cast<Module*>(_instance())->onStartUp();
      }
    
      /*
      * @brief Initializes a specialized type of the module.
      *        The type must derive from the type with which
      *        the module is initialized.
      */
      template<class SubType, class... Args>
      static void
      startUp(Args&& ...args) {
        static_assert(std::is_base_of<T, SubType>::value,
                      "The module does not initialize with the derivation provided. ");
    
        if (isStartedUp()) {
        }
    
        _instance() = new SubType(std::forward<Args>(args)...);
        isStartedUp() = true;
    
        static_cast<Module*>(_instance())->onStartUp();
      }
    
      /*
      * @brief Close this module and release it.
      */
      static void
      shutDown() {
        if (isDestroyed()) {
        }
    
        if (!(isStartedUp())) {
        }
    
        static_cast<Module*>(_instance())->onShutDown();
    
        delete (_instance());
        isDestroyed() = true;
      }
    
      /*
      * @brief Bool that returns whether it was initialized or not.
      */
      static bool
      isStarted() {
        return isStartedUp() && !(isDestroyed());
      }
    
    protected:
      /***********************************************************************/
      /*
      * Constructor and destructor.
      */
      /***********************************************************************/
      Module() = default;
    
      virtual
      ~Module() = default;
    
      Module(Module&&) = delete;
    
      Module(const Module&) = delete;

      /***********************************************************************/
      /*
      * Operator overload.
      */
      /***********************************************************************/
    
      Module&
      operator=(Module&&) = delete;
    
      Module&
      operator=(const Module&) = delete;

      /***********************************************************************/
      /*
      * Methods.
      */
      /***********************************************************************/

      /*
      * @brief Returns a singleton instance of this module.
      */
      static T*&
      _instance() {
        static T* inst = nullptr;
        return inst;
      }
    
      /*
      * @brief Checks has the Module been started up.
      */
      static bool&
      isStartedUp() {
        static bool inst = false;
        return inst;
      }

      /*
      * @brief Override if you want your module 
      *        to be notified once it has been constructed and started.
      */
      virtual void
      onStartUp() {}
    
      /*
      * @brief Override if you want your module 
      *        to be notified just before it is deleted.
      */
      virtual void
      onShutDown() {}
    
      /*
      * @brief Checks has the Module been shut down.
      */
      static bool&
      isDestroyed() {
        static bool inst = false;
        return inst;
      }
  };
}