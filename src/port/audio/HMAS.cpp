#include "HMAS.h"

#define MINIAUDIO_IMPLEMENTATION
#include "audio/miniaudio.h"
#include <spdlog/spdlog.h>
#include "port/Engine.h"
#include "sounds.h"

HMAS::HMAS() {
    ma_result result;
    ma_engine_config engine = ma_engine_config_init();

    engine.channels   = 2;
    engine.noDevice   = MA_TRUE;
    engine.sampleRate = GameEngine_GetSampleRate();

    result = ma_engine_init(&engine, &gAudioEngine);
    if (result != MA_SUCCESS) {
        SPDLOG_ERROR("Failed to initialize audio engine: {}", ma_result_description(result));
        return;
    }
}

void HMAS::RegisterSound(HMAS_AudioId id, const std::string& filePath, HMAS_Info info) {
    if (gRegistry.find(id) != gRegistry.end()) {
        SPDLOG_WARN("Sound with ID {} already registered", static_cast<int>(id));
        return;
    }

    ma_result result = ma_sound_init_from_file(&gAudioEngine, filePath.c_str(), 0, NULL, NULL, &gRegistry[id].sound);
    if (result != MA_SUCCESS) {
        SPDLOG_ERROR("Failed to load sound from file {}: {}", filePath, ma_result_description(result));
        return;
    }

    if(info.loop.start != -1 && info.loop.end != -1) {
        ma_data_source* source = ma_sound_get_data_source(&gRegistry[id].sound);
        ma_data_source_set_loop_point_in_pcm_frames(source, info.loop.start, info.loop.end);
    }

    gRegistry[id].info = info;
    SPDLOG_INFO("Sound with ID {} registered from file {}", static_cast<int>(id), filePath);
}

void HMAS::RegisterSound(HMAS_AudioId id, uint8_t* data, uint32_t size, HMAS_Info info) {
    if (gRegistry.find(id) != gRegistry.end()) {
        SPDLOG_WARN("Sound with ID {} already registered", static_cast<int>(id));
        return;
    }

    ma_decoder_config config = ma_decoder_config_init(ma_format_f32, ma_engine_get_channels(&gAudioEngine), ma_engine_get_sample_rate(&gAudioEngine));
    ma_result result = ma_decoder_init_memory(data, size, &config, &gRegistry[id].decoder);
    if (result != MA_SUCCESS) {
        SPDLOG_ERROR("Failed to initialize decoder from memory: {}", ma_result_description(result));
        return;
    }

    if(info.loop.start != -1 && info.loop.end != -1) {
        ma_data_source_set_loop_point_in_pcm_frames(&gRegistry[id].decoder, info.loop.start, info.loop.end);
    }

    result = ma_sound_init_from_data_source(&gAudioEngine, &gRegistry[id].decoder, 0, NULL, &gRegistry[id].sound);

    if (result != MA_SUCCESS) {
        SPDLOG_ERROR("Failed to load sound from memory: {}", ma_result_description(result));
        return;
    }

    gRegistry[id].info = info;

    SPDLOG_INFO("Sound with ID {} registered from memory buffer", static_cast<int>(id));
}

void HMAS::Play(HMAS_ChannelId channelId, HMAS_AudioId id, bool loop) {
    if(channelId == HMAS_ChannelId::HMAS_MUSIC){
        this->Stop(channelId);
    }

    auto& sample = gRegistry[id];

    float pitch = 1.0f;
    float volume = channelId == HMAS_ChannelId::HMAS_MUSIC ? 0.9f : 1.0f;
    ma_sound_set_pitch(&sample.sound, pitch);
    ma_sound_set_volume(&sample.sound, volume);
    ma_sound_set_looping(&sample.sound, loop);
    ma_sound_seek_to_pcm_frame(&sample.sound, 0);
    ma_result result = ma_sound_start(&sample.sound);
    if (result != MA_SUCCESS) {
        SPDLOG_ERROR("Failed to start sound: {}", ma_result_description(result));
        return;
    }

    auto channel = &this->gChannelSound[channelId];
    channel->sound = &sample.sound;
    channel->cursor = 0;
    channel->pitch = pitch;
    channel->volume = volume;
}

void HMAS::Stop(HMAS_ChannelId channelId) {
    auto channel = &this->gChannelSound[channelId];

    if (channel->sound == nullptr) {
        return;
    }

    ma_sound_stop(channel->sound);
    channel->sound = nullptr;
    channel->cursor = 0;
    channel->pitch = 1.0f;
    channel->volume = 1.0f;
}

bool HMAS::IsPlaying(HMAS_ChannelId channelId) {
    auto channel = &this->gChannelSound[channelId];

    if (channel->sound == nullptr) {
        return false;
    }

    return ma_sound_is_playing(channel->sound);
}

void HMAS::SetPitch(HMAS_ChannelId channelId, float pitch) {
    auto channel = &this->gChannelSound[channelId];

    if (channel->sound == nullptr) {
        return;
    }

    ma_sound_set_pitch(channel->sound, pitch);
    channel->pitch = pitch;
}

void HMAS::SetVolume(HMAS_ChannelId channelId, float volume) {
    auto channel = &this->gChannelSound[channelId];

    if (channel->sound == nullptr) {
        return;
    }

    ma_sound_set_volume(channel->sound, volume);
    channel->volume = volume;
}

void HMAS::SetPause(HMAS_ChannelId channelId, bool pause) {
    auto channel = &this->gChannelSound[channelId];

    if (channel->sound == nullptr) {
        return;
    }

    if(pause) {
        ma_sound_get_cursor_in_pcm_frames(channel->sound, &channel->cursor);
        ma_sound_stop(channel->sound);
    } else {
        ma_result result = ma_sound_start(channel->sound);
        if (result != MA_SUCCESS) {
            SPDLOG_ERROR("Failed to resume sound: {}", ma_result_description(result));
            return;
        }
        ma_sound_seek_to_pcm_frame(channel->sound, channel->cursor);
    }
}

void HMAS::AddEffect(HMAS_ChannelId channelId, HMAS_EffectType type, HMAS_EffectTransition transition, uint32_t frames, float target) {
    auto& channel = gChannelSound[channelId];
    channel.effects.push_back({type, transition, frames, target});
}

bool HMAS::IsIDRegistered(HMAS_AudioId id) {
    return gRegistry.find(id) != gRegistry.end();
}

void HMAS::ProcessEffects() {
    for (size_t i = 0; i < sizeof(gChannelSound) / sizeof(gChannelSound[0]); i++){
        auto& channel = gChannelSound[i];

        if (channel.sound == nullptr) {
            continue;
        }

        if (channel.effects.empty()) {
            continue;
        }

        auto& effect = channel.effects.front();
        if (effect.numFrames > 0) {
            effect.numFrames--;
            switch (effect.type) {
                case HMAS_EffectType::HMAS_EFFECT_VOLUME: {
                    float volume = effect.transition == HMAS_EffectTransition::HMAS_LINEAR ?
                    Lerp(channel.volume, effect.target, 1.0f / effect.numFrames) : effect.target;
                    this->SetVolume((HMAS_ChannelId) i, std::max(0.0f, volume));
                    break;
                }
                case HMAS_EffectType::HMAS_EFFECT_PITCH: {
                    float pitch = effect.transition == HMAS_EffectTransition::HMAS_LINEAR ?
                    Lerp(channel.pitch, effect.target, 1.0f / effect.numFrames) : effect.target;
                    this->SetPitch((HMAS_ChannelId) i, std::max(0.1f, pitch));
                    break;
                }
                case HMAS_EffectType::HMAS_EFFECT_PAUSE: {
                    this->SetPause((HMAS_ChannelId) i, effect.target < 0.5f);
                    break;
                }
                case HMAS_EffectType::HMAS_EFFECT_STOP: {
                    this->Stop((HMAS_ChannelId) i);
                    break;
                }
                default:
                    SPDLOG_WARN("Unknown effect type: {}", static_cast<int>(effect.type));
                    break;
            }
        } else {
            SPDLOG_DEBUG("Removing effect: Type={}, Frames={}, Target={}", static_cast<int>(effect.type), effect.numFrames, effect.target);
            channel.effects.erase(channel.effects.begin());
        }
    }
}

void HMAS::CreateBuffer(uint8_t *samples, uint32_t bufferSizeInBytes) {
    this->ProcessEffects();
    ma_uint32 bufferSizeInFrames = bufferSizeInBytes / ma_get_bytes_per_frame(ma_format_f32, ma_engine_get_channels(&gAudioEngine));
    ma_engine_read_pcm_frames(&gAudioEngine, samples, bufferSizeInFrames, NULL);
}

HMAS::~HMAS() {
    for (auto& pair : gRegistry) {
        ma_sound_uninit(&pair.second.sound);
    }
    gRegistry.clear();
    ma_engine_uninit(&gAudioEngine);
}

// Expose C API for HMAS

extern "C" void HMAS_Play(HMAS_ChannelId channelId, HMAS_AudioId id, bool loop) {
    GameEngine::Instance->gHMAS->Play(channelId, id, loop);
}

extern "C" void HMAS_Stop(HMAS_ChannelId channelId) {
    GameEngine::Instance->gHMAS->Stop(channelId);
}

extern "C" bool HMAS_IsPlaying(HMAS_ChannelId channelId) {
    return GameEngine::Instance->gHMAS->IsPlaying(channelId);
}

extern "C" void HMAS_SetPitch(HMAS_ChannelId channelId, float pitch) {
    GameEngine::Instance->gHMAS->SetPitch(channelId, pitch);
}

extern "C" void HMAS_SetVolume(HMAS_ChannelId channelId, float volume) {
    GameEngine::Instance->gHMAS->SetVolume(channelId, volume);
}

extern "C" void HMAS_SetPause(HMAS_ChannelId channelId, bool pause) {
    GameEngine::Instance->gHMAS->SetPause(channelId, pause);
}

extern "C" void HMAS_AddEffect(HMAS_ChannelId channelId, HMAS_EffectType type, HMAS_EffectTransition transition, uint32_t frames, float target) {
    GameEngine::Instance->gHMAS->AddEffect(channelId, type, transition, frames, target);
}

extern "C" bool HMAS_IsIDRegistered(HMAS_AudioId id) {
    return GameEngine::Instance->gHMAS->IsIDRegistered(id);
}