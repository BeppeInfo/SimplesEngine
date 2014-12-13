#include "include/SoundManager.h"

SoundManager::SoundManager()
{
    //ctor
    Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 4096);
}

SoundManager::~SoundManager()
{
    //dtor
    musicCache.clear();
    soundCache.clear();
}

void SoundManager::PlayMusic( string musicName )
{
    if( !musicName.empty() )
    {
        if( musicCache.find(musicName) == musicCache.end() )
            musicCache[musicName] = Mix_LoadMUS( musicName.c_str() );

        if( musicName != currentSong )
        {
            currentSong = musicName;
            Mix_PlayMusic( musicCache[musicName], -1 );
        }
    }
    else
        Mix_HaltMusic();
}

void SoundManager::PlaySound( string soundName, int times )
{
    if( musicCache.find(soundName) == musicCache.end() )
        soundCache[soundName] = Mix_LoadWAV( soundName.c_str() );

        Mix_PlayChannel( -1, soundCache[soundName], times );
}
