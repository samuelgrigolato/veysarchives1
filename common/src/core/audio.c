#include "core/audio.h"
#include "core/log.h"


struct PlayingSoundNode {
  Uint32 entryId;
  Uint8 *data;
  Uint32 pos;
  Uint32 length;
  float volume; // controls fading, starts with 0 goes up to 1
  SDL_bool repeat;
  SDL_bool stopping;
  struct PlayingSoundNode *next;
};


struct LoadedSoundNode {
  Aud_SoundID id;
  Uint8 *data;
  Uint32 length;
  struct LoadedSoundNode *next;
};


Aud_EntryID nextEntryId = 0;
struct PlayingSoundNode *playingSounds = NULL;
struct LoadedSoundNode *loadedSounds = NULL;
SDL_AudioDeviceID device;
void onAudioDeviceCallback(void *userData, Uint8* stream, int len);


struct LoadedSoundNode* findLoadedSound(Aud_SoundID soundId) {
  struct LoadedSoundNode *current = loadedSounds;
  while (current != NULL) {
    if (current->id == soundId) {
      return current;
    }
    current = current->next;
  }
  return NULL;
}


Aud_EntryID playSound(Aud_SoundID soundId, SDL_bool repeat, SDL_bool fade) {
  struct LoadedSoundNode *sound = findLoadedSound(soundId);
  if (sound == NULL) {
    logError("Audio: tried to play an unloaded sound. soundId=%u", soundId);
  }
  Uint32 soundLengthInBytes = sizeof(Sint16) * sound->length;

  struct PlayingSoundNode *newNode = malloc(sizeof(struct PlayingSoundNode));
  newNode->entryId = nextEntryId++;
  newNode->data = malloc(soundLengthInBytes);
  SDL_memcpy(newNode->data, sound->data, soundLengthInBytes);
  newNode->length = sound->length;
  newNode->pos = 0;
  newNode->repeat = repeat;
  newNode->volume = fade ? 0 : 1;
  newNode->stopping = SDL_FALSE;

  SDL_LockAudioDevice(device);
  newNode->next = playingSounds;
  playingSounds = newNode;
  SDL_UnlockAudioDevice(device);

  return newNode->entryId;
}


struct PlayingSoundNode* findPlayingSound(Aud_EntryID entryId) {
  struct PlayingSoundNode *current = playingSounds;
  while (current != NULL) {
    if (current->entryId == entryId) {
      return current;
    }
    current = current->next;
  }
  return NULL;
}


void Aud_Init() {
  SDL_AudioSpec desired;
  desired.channels = 2;
  desired.format = AUDIO_S16;
  desired.freq = 44100;
  desired.samples = 4410; // 0.1 seconds
  desired.callback = onAudioDeviceCallback;
  device = SDL_OpenAudioDevice(NULL, 0, &desired, NULL, 0);
  if (device == 0) {
    logError("Audio: failed to open audio device: %s", SDL_GetError());
    exit(1);
  }
  SDL_PauseAudioDevice(device, 0);
}


Aud_SoundID findAvailableSoundId() {
  Aud_SoundID result = 1;
  while (findLoadedSound(result) != NULL) {
    if (result == 255) {
      logError("Audio: no more room for sound ids =(");
      exit(1);
    }
    result++;
  }
  return result;
}


Aud_SoundID Aud_Load(char *filePath) {
  struct LoadedSoundNode *loadedSound = malloc(sizeof(struct LoadedSoundNode));
  loadedSound->id = findAvailableSoundId();

  logInfo("Audio: loading %s as %u", filePath, loadedSound->id);

  char fullFilePath[300];
  sprintf(fullFilePath, "%s%s%s", ASSETS_BASE_DIR, "assets/", filePath);

  SDL_AudioSpec spec;
  if (SDL_LoadWAV(fullFilePath, &spec, &loadedSound->data, &loadedSound->length) == NULL) {
    logError("Audio: failed to load WAV: %s", SDL_GetError());
    exit(1);
  }

  if (spec.channels != 2) {
    logError("Audio: loaded channels != 2: %u", spec.channels);
    exit(1);
  }

  if (spec.format != AUDIO_S16) {
    logError("Audio: loaded format != AUDIO_S16: %u", spec.format);
    exit(1);
  }

  if (spec.freq != 44100) {
    logError("Audio: loaded freq != 44100: %u", spec.freq);
    exit(1);
  }

  loadedSound->next = loadedSounds;
  loadedSounds = loadedSound;

  return loadedSound->id;
}


void Aud_Unload(Aud_SoundID soundId) {
  struct LoadedSoundNode **nextCandidate = &loadedSounds;
  while (*nextCandidate != NULL) {
    if ((*nextCandidate)->id == soundId) {
      SDL_FreeWAV((*nextCandidate)->data);
      *nextCandidate = (*nextCandidate)->next; 
      free(*nextCandidate);
      logInfo("Audio: unloaded soundId=%u", soundId);
      return;
    }
    nextCandidate = &(*nextCandidate)->next;
  }
  logError("Audio: trying to unload soundID=%u but couldn't find it!", soundId);
  exit(1);
}


Aud_EntryID Aud_FadeInAndRepeat(Aud_SoundID soundId) {
  Aud_EntryID entryId = playSound(soundId, SDL_TRUE, SDL_TRUE);
  logInfo("Audio: fading soundID=%u in and repeating as entryID=%u", soundId, entryId);
  return entryId;
}


void Aud_FadeOutAndStop(Aud_EntryID entryId) {
  logInfo("Audio: fading entryID=%u out", entryId);
  SDL_LockAudioDevice(device);
  struct PlayingSoundNode *playingSound = findPlayingSound(entryId);
  if (playingSound != NULL) {
    playingSound->stopping = SDL_TRUE;
  }
  SDL_UnlockAudioDevice(device);
}


void Aud_PlayOnce(Aud_SoundID soundId) {
  playSound(soundId, SDL_FALSE, SDL_FALSE);
}


void onAudioDeviceCallback(void *userData, Uint8 *stream, int len) {
  // useful for debugging
  // logInfo("Audio: callback called with len=%d", len);

  for (int i = 0; i < len; i++) {
    stream[i] = 0; // silence
  }

  struct PlayingSoundNode **playingSound = &playingSounds;
  while (*playingSound != NULL) {
    int remaining = (*playingSound)->length - (*playingSound)->pos;
    int bytesPlayed = SDL_min(len, remaining);
    int wordsPlayed = bytesPlayed / 2;
    Sint16 *streamWords = (Sint16*)stream;
    int temp; // auxiliary for __builtin_add_overflow
    for (int i = 0; i < wordsPlayed; i++) {
      Sint16 *word = (Sint16*)((*playingSound)->data + ((*playingSound)->pos + i * 2));
      Sint16 wordWithVolume = *word * (*playingSound)->volume;
      if (__builtin_add_overflow(streamWords[i], wordWithVolume, &temp)) {
        streamWords[i] = SDL_MAX_SINT16;
      } else {
        streamWords[i] += wordWithVolume;
      }
    }
    if ((*playingSound)->stopping) {
      if ((*playingSound)->volume > 0) {
        (*playingSound)->volume -= 0.2;
      }
    } else {
      if ((*playingSound)->volume < 1) {
        (*playingSound)->volume += 0.05;
      }
    }
    if (((remaining == bytesPlayed && !(*playingSound)->repeat)) || (*playingSound)->volume <= 0) {

      free((*playingSound)->data);
      *playingSound = (*playingSound)->next;

      // useful for debugging
      logInfo("Audio: finished playing a sound");

    } else {
      (*playingSound)->pos += bytesPlayed;
      if ((*playingSound)->pos >= (*playingSound)->length) {
        (*playingSound)->pos = 0;
      }

      // useful for debugging
      logInfo("Audio: played %d bytes, %d remaining", bytesPlayed, remaining);

      playingSound = &(*playingSound)->next;
    }
  }
}
