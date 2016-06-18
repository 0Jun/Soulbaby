//
//  Audio.cpp
//  HardSoda
//
//  Created by Darion McCoy on 4/11/15.
//  Copyright (c) 2015 Darion. All rights reserved.
//

#include "Audio.h"

Audio::Audio(){
    
    //Uses dirent.h to open up the sfx folder directory, get all the sound file names, and create a map to add sound buffers and retrive with file name/key
    
    
    DIR *dip;
    dip = opendir("/Users/darionmccoy/Dropbox/Game Design Stuff/Projects/Backup/HardSoda/HardSoda/sfx");
    struct dirent *dit;
    
    while((dit = readdir(dip)) != NULL){
        
        std::string name = dit->d_name;
        sf::SoundBuffer buffer;
        std::size_t found = name.find("sfx_");
        AudioItem item;
        
        if(found!=std::string::npos){
        
            if(item.buffer.loadFromFile("/Users/darionmccoy/Dropbox/Game Design Stuff/Projects/Backup/HardSoda/HardSoda/sfx/" + name)){

                AudioContainer.insert(std::pair<std::string,AudioItem>(name,item));
                std::cout << name << " SFX loaded ... " << std::endl;
                
            }
        
        }

    }
    
    closedir(dip);
    
}

void Audio::PlaySFX(std::string soundName){
    
    audioIter iter = AudioContainer.find(soundName + ".wav");
    iter->second.sample.setBuffer(iter->second.buffer);
    iter->second.sample.stop();
    iter->second.sample.setVolume(20);
    iter->second.sample.play();

}

void Audio::PlayMusic(std::string musicName){
    
    music.stop();
    music.openFromFile("/Users/darionmccoy/Dropbox/Game Design Stuff/Projects/Backup/HardSoda/HardSoda/music/" + musicName + ".ogg");
    music.setLoop(true);
    music.play();
    music.setVolume(50);
    
}

Audio::~Audio(){
    
    for(audioIter it = AudioContainer.begin(); it != AudioContainer.end(); it++){
        
        it->second.sample.resetBuffer();
        
    }
    
    
    AudioContainer.clear();
    music.stop();
    
    
}


