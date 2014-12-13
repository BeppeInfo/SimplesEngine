#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include "include/utils.h"

class SoundManager
{
    public:
        static SoundManager& Activate()
        {
            static SoundManager sdmng;
            return sdmng;
        }

        virtual ~SoundManager();

        void PlayMusic(string);
        void PlaySound(string, int = 0);

    private:
        SoundManager();
        SoundManager(const SoundManager&);                 // Prevent copy-construction
        SoundManager& operator=(const SoundManager&);      // Prevent assignment

        map<string,Mix_Music*> musicCache;
        string currentSong;
        map<string,Mix_Chunk*> soundCache;
};

#endif // SOUNDMANAGER_H
