#include <fmod.hpp>

#include "gaAudioSYS.h"

using std::cout;
using namespace FMOD;

namespace gaEngineSDK {
  System *g_fInitSys;

  /*************************************************************************/
  /**
  * Virtual methods.
  */
  /*************************************************************************/

  void
  AudioSYS::init(uint32 maxChannels) {
    FMOD_RESULT fResult = FMOD::System_Create(&g_fInitSys);
    if (FMOD_OK != fResult) {
      cout << "Error FMOD, System Create (INIT)\n";
    }

    fResult = g_fInitSys->init(maxChannels, FMOD_INIT_NORMAL, nullptr);
    if (FMOD_OK != fResult) {
      cout << "Error FMOD, init (INIT)\n";
    }
  }

  void
  AudioSYS::createAudioSrc(audioSRC* source) {
    source = {};
  }

  void
  AudioSYS::destroyAudioSrc(audioSRC* source) {
  
  }

  void
  AudioSYS::playAudio(audioSRC* source) {
  
  }

  void
  AudioSYS::playAudioOneShot(audioSRC* source) {
  
  }

  void
  AudioSYS::playAudioLoop(audioSRC* source) {
    g_fInitSys->playSound((Sound*)source->sourceHandle,
                          nullptr,
                          false,
                          nullptr);
  }

  void
  AudioSYS::pauseAudio(audioSRC* source) {
  
  }

  void
  AudioSYS::stopAudio(audioSRC* source) {
  
  }

  void
  AudioSYS::updateSrcState(audioSRC* source) {
  
  }

  /*************************************************************************/
  /**
  * Sets.
  */
  /*************************************************************************/

  void
  AudioSYS::setAudioSrc(audioSRC* source, audio* newAudio) {
    setAudioSrc(source,
                newAudio->data,
                newAudio->numberOfSamplers,
                newAudio->sampleRate,
                newAudio->type);
  }

  void
  AudioSYS::setAudioSrc(audioSRC* source,
                        String data,
                        int32 numberOfSamplers,
                        int32 sampleRate,
                        AUDIO_TYPE::E type) {
    FMOD_MODE soundMode = FMOD_OPENMEMORY;

    const int32 bytesPerSample = 2;

    int32 channels = 0;
    int32 format = 0;

    if (type == AUDIO_TYPE::kAudioMono) {
      channels = 1;
      soundMode |= FMOD_2D;
    }
    else {
      channels = 2;
      soundMode |= FMOD_3D;
    }

    g_fInitSys->createSound(data.c_str(),
                            soundMode,
                            nullptr,
                            (Sound**)& source->sourceHandle);

    source->duration = (numberOfSamplers / sampleRate);
  }

  void
  AudioSYS::setAudioAttributes(audioSRC* source, float volume, bool loop, Vector3 position) {
  
  
  }

  void
  AudioSYS::setAudioVolume(audioSRC* source, float volume) {
  
  
  }

  void
  AudioSYS::setAudioLoop(audioSRC* source, bool loop) {
  
  
  }

  void
  AudioSYS::setAudio3D_Position(audioSRC* source, Vector3 position) {
  
  }

  void
  AudioSYS::setListenerVolume(float volume) {
  
  }

  void
  AudioSYS::setListener3D_PositionOrientation(Vector3 position, Quaternions rotation) {
  
  }
}