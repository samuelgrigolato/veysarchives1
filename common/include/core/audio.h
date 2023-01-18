#ifndef CORE_AUDIO_h
#define CORE_AUDIO_h

#include "platform.h"
#include <SDL.h>

typedef Uint8 Aud_SoundID;
typedef Uint32 Aud_EntryID;

void Aud_Init();

Aud_SoundID Aud_Load(char *filePath);

void Aud_Unload(Aud_SoundID soundId);

Aud_EntryID Aud_FadeInAndRepeat(Aud_SoundID soundId);

void Aud_FadeOutAndStop(Aud_EntryID entryId);

void Aud_PlayOnce(Aud_SoundID soundId);

#endif
