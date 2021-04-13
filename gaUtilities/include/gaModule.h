#pragma once

#include "gaPrerequisitesUtilities.h"

namespace gaEngineSDK {

  template<class T>
  class Module
  {
    public:

      static T&
      instance() {
        if (!(isStartedUp())) {
        }
    
        if (isDestroyed()) {
        }

        return *_instance();
      }
    
      static T*
      instancePtr() {
        if (!(isStartedUp())) {
        }
    
        if (isDestroyed()) {
        }

        return _instance();
      }
    
      template<class... Args>
      static void
      startUp(Args&& ...args) {
        if (isStartedUp()) {
        }
    
        _instance() = new T(std::forward<Args>(args)...);
        isStartedUp() = true;
    
        static_cast<Module*>(_instance())->onStartUp();
      }
    
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
    
      static bool
      isStarted() {
        return isStartedUp() && !(isDestroyed());
      }
    
    protected:
      Module() = default;
    
      virtual
      ~Module() = default;
    
      Module(Module&&) = delete;
    
      Module(const Module&) = delete;
    
      Module&
      operator=(Module&&) = delete;
    
      Module&
      operator=(const Module&) = delete;

      static T*&
      _instance() {
        static T* inst = nullptr;
        return inst;
      }
    
      static bool&
      isStartedUp() {
        static bool inst = false;
        return inst;
      }

      virtual void
      onStartUp() {}
    
      virtual void
      onShutDown() {}
    
      static bool&
      isDestroyed() {
        static bool inst = false;
        return inst;
      }
  };
}