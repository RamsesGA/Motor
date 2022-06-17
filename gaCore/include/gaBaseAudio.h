#pragma once

#include <gaModule.h>
#include <gaVector3.h>
#include <gaQuaternions.h>

#include "gaPrerequisitesCore.h"

namespace gaEngineSDK {
  namespace AUDIO_SRC_STATES {
    enum E {
      kStopped = 0,
      kPlaying,
      kPaused
    };
  }

  namespace AUDIO_TYPE {
    enum E {
      /*kAIFF,
      kASF,
      kASX,
      kDLS,
      kFLAC,
      kFSB,
      kIT,
      kM3U,
      kMIDI,
      kMOD,
      kMP2,
      kMP3,
      kOggVorbis,
      kPLS,
      kS3M,
      kVAG,
      kWAV,
      kWAX,
      kWMA,
      kXM,
      kXMA*/
      kAudioMono,
      kAudioStereo
    };
  }

  struct audioSRC {
    bool loop = false;

    int32 duration = 0;
    
    uint32 dataBufferHandle = 0;

    float volume = 0.0f;

    Vector3 position3D;

    AUDIO_SRC_STATES::E state;

    void* sourceHandle;
  };

  struct audioListener {
    int32 handle = 0;

    float masterVolume = 0.0f;

    Vector3 position;

    Quaternions rotation;
  };

  struct audio {
    String data;
    int32 numberOfSamplers = 0;
    int32 sampleRate = 0;
    AUDIO_TYPE::E type;
  };

  class GA_CORE_EXPORT BaseAudio : public Module<BaseAudio>
  {
   public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/

    BaseAudio() = default;

    ~BaseAudio() = default;

    /*
    * @brief .
    */
    void
    setObject(BaseAudio* api) {
      BaseAudio::_instance() = api;
    }

    /*************************************************************************/
    /**
    * Virtual methods.
    */
    /*************************************************************************/

    /*
    * @brief .
    */
    virtual void
    init(uint32 maxChannels) {
      GA_UNREFERENCED_PARAMETER(maxChannels);
    };

    /*
    * @brief .
    */
    virtual void
    createAudioSrc(audioSRC *source) {
      GA_UNREFERENCED_PARAMETER(source);
    };

    /*
    * @brief .
    */
    virtual void
    destroyAudioSrc(audioSRC* source) {
      GA_UNREFERENCED_PARAMETER(source);
    };

    /*
    * @brief .
    */
    virtual void
    playAudio(audioSRC* source) {
      GA_UNREFERENCED_PARAMETER(source);
    };

    /*
    * @brief .
    */
    virtual void
    playAudioOneShot(audioSRC* source) {
      GA_UNREFERENCED_PARAMETER(source);
    };

    /*
    * @brief .
    */
    virtual void
    playAudioLoop(audioSRC* source) {
      GA_UNREFERENCED_PARAMETER(source);
    };

    /*
    * @brief .
    */
    virtual void
    pauseAudio(audioSRC* source) {
      GA_UNREFERENCED_PARAMETER(source);
    };

    /*
    * @brief .
    */
    virtual void
    stopAudio(audioSRC* source) {
      GA_UNREFERENCED_PARAMETER(source);
    };

    /*
    * @brief .
    */
    virtual void
    updateSrcState(audioSRC* source) {
      GA_UNREFERENCED_PARAMETER(source);
    };

    /*************************************************************************/
    /**
    * Sets.
    */
    /*************************************************************************/

    /*
    * @brief .
    */
    virtual void
    setAudioSrc(audioSRC* source, audio* newAudio) {
      GA_UNREFERENCED_PARAMETER(source);
      GA_UNREFERENCED_PARAMETER(newAudio);
    };

    /*
    * @brief .
    */
    virtual void
    setAudioSrc(audioSRC* source, 
                String data,
                int32 numberOfSamplers,
                int32 sampleRate,
                AUDIO_TYPE::E type) {
      GA_UNREFERENCED_PARAMETER(source);
      GA_UNREFERENCED_PARAMETER(data);
      GA_UNREFERENCED_PARAMETER(numberOfSamplers);
      GA_UNREFERENCED_PARAMETER(sampleRate);
      GA_UNREFERENCED_PARAMETER(type);
    };

    /*
    * @brief .
    */
    virtual void
    setAudioAttributes(audioSRC* source, float volume, bool loop, Vector3 position) {
      GA_UNREFERENCED_PARAMETER(source);
      GA_UNREFERENCED_PARAMETER(volume);
      GA_UNREFERENCED_PARAMETER(loop);
      GA_UNREFERENCED_PARAMETER(position);
    };

    /*
    * @brief .
    */
    virtual void
    setAudioVolume(audioSRC* source, float volume) {
      GA_UNREFERENCED_PARAMETER(source);
      GA_UNREFERENCED_PARAMETER(volume);
    };

    /*
    * @brief .
    */
    virtual void
    setAudioLoop(audioSRC* source, bool loop) {
      GA_UNREFERENCED_PARAMETER(source);
      GA_UNREFERENCED_PARAMETER(loop);
    };

    /*
    * @brief .
    */
    virtual void
    setAudio3D_Position(audioSRC* source, Vector3 position) {
      GA_UNREFERENCED_PARAMETER(source);
      GA_UNREFERENCED_PARAMETER(position);
    };

    /*
    * @brief .
    */
    virtual void
    setListenerVolume(float volume) {
      GA_UNREFERENCED_PARAMETER(volume);
    };

    /*
    * @brief .
    */
    virtual void
    setListener3D_PositionOrientation(Vector3 position, Quaternions rotation) {
      GA_UNREFERENCED_PARAMETER(position);
      GA_UNREFERENCED_PARAMETER(rotation);
    };
  };

  /***************************************************************************/
  /**
  * INSTANCE.
  */
  /***************************************************************************/

  GA_CORE_EXPORT BaseAudio&
  g_baseAudio();
}