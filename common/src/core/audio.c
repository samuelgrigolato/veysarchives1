#include "core/audio.h"
#include "core/log.h"


typedef struct PlayingSoundNode {
  Uint32 entryId;
  Uint8 *data;
  Uint32 pos;
  Uint32 length;
  float volume; // controls fading, starts with 0 goes up to 1
  SDL_bool repeat;
  SDL_bool stopping;
  struct PlayingSoundNode *next;
} PlayingSoundNode;


typedef struct LoadedSoundNode {
  Aud_SoundID id;
  Uint8 *data;
  Uint32 length;
  struct LoadedSoundNode *next;
} LoadedSoundNode;


Aud_EntryID nextEntryId = 0;
PlayingSoundNode *playingSounds = NULL;
LoadedSoundNode *loadedSounds = NULL;
SDL_AudioDeviceID device;
void onAudioDeviceCallback(void *userData, Uint8* stream, int len);


LoadedSoundNode* findLoadedSound(Aud_SoundID soundId) {
  LoadedSoundNode *current = loadedSounds;
  while (current != NULL) {
    if (current->id == soundId) {
      return current;
    }
    current = current->next;
  }
  return NULL;
}


Aud_EntryID playSound(Aud_SoundID soundId, SDL_bool repeat, SDL_bool fade) {
  LoadedSoundNode *sound = findLoadedSound(soundId);
  if (sound == NULL) {
    logError("Audio: tried to play an unloaded sound. soundId=%u", soundId);
  }

  PlayingSoundNode *newNode = SDL_malloc(sizeof(PlayingSoundNode));
  newNode->entryId = nextEntryId++;
  newNode->data = SDL_malloc(sound->length);
  SDL_memcpy(newNode->data, sound->data, sound->length);
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


PlayingSoundNode* findPlayingSound(Aud_EntryID entryId) {
  PlayingSoundNode *playingSound = playingSounds;
  while (playingSound != NULL) {
    if (playingSound->entryId == entryId) {
      return playingSound;
    }
    playingSound = playingSound->next;
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
  LoadedSoundNode *loadedSound = SDL_malloc(sizeof(LoadedSoundNode));
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
  LoadedSoundNode **linkedListNode = &loadedSounds;
  while (*linkedListNode != NULL) {
    LoadedSoundNode *loadedSound = *linkedListNode;

    if (loadedSound->id == soundId) {
      SDL_FreeWAV(loadedSound->data);

      *linkedListNode = loadedSound->next; // this is a linked list delete operation

      SDL_free(loadedSound);
      logInfo("Audio: unloaded soundId=%u", soundId);
      return;
    }

    linkedListNode = &(*linkedListNode)->next; // this is a linked list next operation
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
  PlayingSoundNode *playingSound = findPlayingSound(entryId);
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

  PlayingSoundNode **linkedListNode = &playingSounds;
  while (*linkedListNode != NULL) {
    PlayingSoundNode *playingSound = *linkedListNode; // this is a linked list get value operation

    int remaining = playingSound->length - playingSound->pos;
    int bytesPlayed = SDL_min(len, remaining);
    int wordsPlayed = bytesPlayed / 2;
    Sint16 *streamWords = (Sint16*)stream;
    int temp; // auxiliary for __builtin_add_overflow
    for (int i = 0; i < wordsPlayed; i++) {
      Sint16 *word = (Sint16*)(playingSound->data + (playingSound->pos + i * 2));
      Sint16 wordWithVolume = *word * playingSound->volume;
      if (__builtin_add_overflow(streamWords[i], wordWithVolume, &temp)) {
        streamWords[i] = SDL_MAX_SINT16;
      } else {
        streamWords[i] += wordWithVolume;
      }
    }
    if (playingSound->stopping) {
      if (playingSound->volume > 0) {
        playingSound->volume -= 0.2;
      }
    } else {
      if (playingSound->volume < 1) {
        playingSound->volume += 0.05;
      }
    }
    if (((remaining == bytesPlayed && !playingSound->repeat)) || playingSound->volume <= 0) {

      SDL_free(playingSound->data);

      // useful for debugging
      // logInfo("Audio: finished playing a sound");

      *linkedListNode = (*linkedListNode)->next; // this is a linked list delete operation

    } else {

      playingSound->pos += bytesPlayed;
      if (playingSound->pos >= playingSound->length) {
        playingSound->pos = 0;
      }

      // useful for debugging
      // logInfo("Audio: played %d bytes, %d remaining", bytesPlayed, remaining);

      linkedListNode = &(*linkedListNode)->next; // this is a linked list next operation

    }
  }
}
