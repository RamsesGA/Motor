#pragma once

#include <gaBaseAudio.h>

#include "gaPrerequisitesAudioSYS.h"

namespace gaEngineSDK {
  class AudioSYS final : public BaseAudio
  {
   public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/

    AudioSYS() = default;

    ~AudioSYS() = default;

    /*************************************************************************/
    /**
    * Virtual methods.
    */
    /*************************************************************************/

    /*
    * @brief .
    */
    void
    init(uint32 maxChannels) override;

    /*
    * @brief .
    */
    void
    createAudioSrc(audioSRC* source) override;

    /*
    * @brief .
    */
    void
    destroyAudioSrc(audioSRC* source) override;

    /*
    * @brief .
    */
    void
    playAudio(audioSRC* source) override;

    /*
    * @brief .
    */
    void
    playAudioOneShot(audioSRC* source) override;

    /*
    * @brief .
    */
    void
    playAudioLoop(audioSRC* source) override;

    /*
    * @brief .
    */
    void
    pauseAudio(audioSRC* source) override;

    /*
    * @brief .
    */
    void
    stopAudio(audioSRC* source) override;

    /*
    * @brief .
    */
    void
    updateSrcState(audioSRC* source) override;

    /*************************************************************************/
    /**
    * Sets.
    */
    /*************************************************************************/

    /*
    * @brief .
    */
    void
    setAudioSrc(audioSRC* source, audio* newAudio) override;

    /*
    * @brief .
    */
    void
    setAudioSrc(audioSRC* source, 
                String data,
                int32 numberOfSamplers,
                int32 sampleRate,
                AUDIO_TYPE::E type) override;

    /*
    * @brief .
    */
    void
    setAudioAttributes(audioSRC* source, float volume, bool loop, Vector3 position) override;

    /*
    * @brief .
    */
    void
    setAudioVolume(audioSRC* source, float volume) override;

    /*
    * @brief .
    */
    void
    setAudioLoop(audioSRC * source, bool loop) override;

    /*
    * @brief .
    */
    void
    setAudio3D_Position(audioSRC * source, Vector3 position) override;

    /*
    * @brief .
    */
    void
    setListenerVolume(float volume) override;

    /*
    * @brief .
    */
    void
    setListener3D_PositionOrientation(Vector3 position, Quaternions rotation) override;
  };

  /***************************************************************************/
  /**
  * Export.
  */
  /***************************************************************************/
  extern "C" GA_AUDIO_EXPORT BaseAudio *
  newAudio() {
    AudioSYS* pAudio = new AudioSYS();
    return pAudio;
  }
}