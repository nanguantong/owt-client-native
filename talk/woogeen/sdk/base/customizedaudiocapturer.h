/*
 * Intel License
 */

#ifndef WOOGEEN_BASE_CUSTOMIZEDAUDIOCAPTURER_H_
#define WOOGEEN_BASE_CUSTOMIZEDAUDIOCAPTURER_H_

#include <memory>
#include "webrtc/base/scoped_ptr.h"
#include "webrtc/modules/audio_device/audio_device_generic.h"
#include "webrtc/system_wrappers/interface/aligned_malloc.h"
#include "webrtc/system_wrappers/interface/thread_wrapper.h"
#include "webrtc/system_wrappers/interface/clock.h"
#include "webrtc/system_wrappers/interface/critical_section_wrapper.h"
#include "talk/woogeen/sdk/include/cpp/woogeen/base/framegeneratorinterface.h"

namespace woogeen {
namespace base {

using namespace webrtc;

// This is a customized audio device which retrieves audio from a
// AudioFrameGenerator implementation as its microphone.
// CustomizedAudioCapturer is not able to output audio.
class CustomizedAudioCapturer : public AudioDeviceGeneric {
 public:
  // Constructs a customized audio device with |frame_generator|. It will read
  // audio from |frame_generator|.
  CustomizedAudioCapturer(
      std::unique_ptr<AudioFrameGeneratorInterface> frame_generator);
  virtual ~CustomizedAudioCapturer();

  // Retrieve the currently utilized audio layer
  int32_t ActiveAudioLayer(
      AudioDeviceModule::AudioLayer& audioLayer) const override;

  // Main initializaton and termination
  int32_t Init() override;
  int32_t Terminate() override;
  bool Initialized() const override;

  // Device enumeration
  int16_t PlayoutDevices() override;
  int16_t RecordingDevices() override;
  int32_t PlayoutDeviceName(uint16_t index,
                            char name[kAdmMaxDeviceNameSize],
                            char guid[kAdmMaxGuidSize]) override;
  int32_t RecordingDeviceName(uint16_t index,
                              char name[kAdmMaxDeviceNameSize],
                              char guid[kAdmMaxGuidSize]) override;

  // Device selection
  int32_t SetPlayoutDevice(uint16_t index) override;
  int32_t SetPlayoutDevice(
      AudioDeviceModule::WindowsDeviceType device) override;
  int32_t SetRecordingDevice(uint16_t index) override;
  int32_t SetRecordingDevice(
      AudioDeviceModule::WindowsDeviceType device) override;

  // Audio transport initialization
  int32_t PlayoutIsAvailable(bool& available) override;
  int32_t InitPlayout() override;
  bool PlayoutIsInitialized() const override;
  int32_t RecordingIsAvailable(bool& available) override;
  int32_t InitRecording() override;
  bool RecordingIsInitialized() const override;

  // Audio transport control
  int32_t StartPlayout() override;
  int32_t StopPlayout() override;
  bool Playing() const override;
  int32_t StartRecording() override;
  int32_t StopRecording() override;
  bool Recording() const override;

  // Microphone Automatic Gain Control (AGC)
  int32_t SetAGC(bool enable) override;
  bool AGC() const override;

  // Volume control based on the Windows Wave API (Windows only)
  int32_t SetWaveOutVolume(uint16_t volumeLeft, uint16_t volumeRight) override;
  int32_t WaveOutVolume(uint16_t& volumeLeft,
                        uint16_t& volumeRight) const override;

  // Audio mixer initialization
  int32_t InitSpeaker() override;
  bool SpeakerIsInitialized() const override;
  int32_t InitMicrophone() override;
  bool MicrophoneIsInitialized() const override;

  // Speaker volume controls
  int32_t SpeakerVolumeIsAvailable(bool& available) override;
  int32_t SetSpeakerVolume(uint32_t volume) override;
  int32_t SpeakerVolume(uint32_t& volume) const override;
  int32_t MaxSpeakerVolume(uint32_t& maxVolume) const override;
  int32_t MinSpeakerVolume(uint32_t& minVolume) const override;
  int32_t SpeakerVolumeStepSize(uint16_t& stepSize) const override;

  // Microphone volume controls
  int32_t MicrophoneVolumeIsAvailable(bool& available) override;
  int32_t SetMicrophoneVolume(uint32_t volume) override;
  int32_t MicrophoneVolume(uint32_t& volume) const override;
  int32_t MaxMicrophoneVolume(uint32_t& maxVolume) const override;
  int32_t MinMicrophoneVolume(uint32_t& minVolume) const override;
  int32_t MicrophoneVolumeStepSize(uint16_t& stepSize) const override;

  // Speaker mute control
  int32_t SpeakerMuteIsAvailable(bool& available) override;
  int32_t SetSpeakerMute(bool enable) override;
  int32_t SpeakerMute(bool& enabled) const override;

  // Microphone mute control
  int32_t MicrophoneMuteIsAvailable(bool& available) override;
  int32_t SetMicrophoneMute(bool enable) override;
  int32_t MicrophoneMute(bool& enabled) const override;

  // Microphone boost control
  int32_t MicrophoneBoostIsAvailable(bool& available) override;
  int32_t SetMicrophoneBoost(bool enable) override;
  int32_t MicrophoneBoost(bool& enabled) const override;

  // Stereo support
  int32_t StereoPlayoutIsAvailable(bool& available) override;
  int32_t SetStereoPlayout(bool enable) override;
  int32_t StereoPlayout(bool& enabled) const override;
  int32_t StereoRecordingIsAvailable(bool& available) override;
  int32_t SetStereoRecording(bool enable) override;
  int32_t StereoRecording(bool& enabled) const override;

  // Delay information and control
  int32_t SetPlayoutBuffer(const AudioDeviceModule::BufferType type,
                           uint16_t sizeMS) override;
  int32_t PlayoutBuffer(AudioDeviceModule::BufferType& type,
                        uint16_t& sizeMS) const override;
  int32_t PlayoutDelay(uint16_t& delayMS) const override;
  int32_t RecordingDelay(uint16_t& delayMS) const override;

  // CPU load
  int32_t CPULoad(uint16_t& load) const override;

  bool PlayoutWarning() const override;
  bool PlayoutError() const override;
  bool RecordingWarning() const override;
  bool RecordingError() const override;
  void ClearPlayoutWarning() override;
  void ClearPlayoutError() override;
  void ClearRecordingWarning() override;
  void ClearRecordingError() override;

  void AttachAudioBuffer(AudioDeviceBuffer* audioBuffer) override;

  void VolumeOverloud(int16_t level) override;

 private:
  static bool RecThreadFunc(void*);
  static bool PlayThreadFunc(void*);
  bool RecThreadProcess();
  bool PlayThreadProcess();

  std::unique_ptr<AudioFrameGeneratorInterface> frame_generator_;

  AudioDeviceBuffer* audio_buffer_;
  std::unique_ptr<uint8_t[], webrtc::AlignedFreeDeleter>
      recording_buffer_;  // Pointer to a useable memory for audio frames.
  CriticalSectionWrapper& crit_sect_;

  size_t recording_frames_in_10ms_;
  int recording_sample_rate_;
  int recording_channel_number_;
  int recording_buffer_size_;

  rtc::scoped_ptr<ThreadWrapper> thread_rec_;

  bool recording_;
  uint64_t last_call_record_millis_;

  Clock* clock_;
};
}
}

#endif  // WOOGEEN_BASE_CUSTOMIZEDAUDIOCAPTURER_H_
