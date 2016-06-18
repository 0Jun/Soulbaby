//
//  Textures.cpp
//  sdev
//
//  Created by Darion McCoy on 1/13/14.
//  Copyright (c) 2014 D man. All rights reserved.
//

#include "Textures.h"
#include "ResourcePath.hpp"


namespace Textures{
    
    float TextureContainer::tick = 0;
    
    Shader::Shader(){
        
    }
    
    WaveShader::WaveShader(){
        
        shader.loadFromFile("/Users/darionmccoy/Dropbox/Game Design Stuff/Projects/Backup/HardSoda/HardSoda/shaders/sine.vert", "/Users/darionmccoy/Dropbox/Game Design Stuff/Projects/Backup/HardSoda/HardSoda/shaders/sine.frag");
        
        
        objectTexture.loadFromFile("/Users/darionmccoy/Dropbox/Game Design Stuff/Projects/Backup/HardSoda/HardSoda/textures/tx_menu_item.png");
        
        objectTexture.setRepeated(true);
        object.setTexture(objectTexture);
        object.setTextureRect(sf::IntRect(0,0,100,100));
        object.setScale(10, 10);
        
        // It is important to set repeated to true to enable scrolling upwards
        distortionMap.setRepeated(true);
        
        // Setting smooth to true lets us use small maps even on larger images
        distortionMap.setSmooth(true);
        
        distortionMap.loadFromFile("/Users/darionmccoy/Dropbox/Game Design Stuff/Projects/Backup/HardSoda/HardSoda/textures/tx_map.png");
        
        renderTexture.create(400, 300);
        
        sprite.setTexture(renderTexture.getTexture());
        sprite.setPosition(100, 150);
        sprite.setPosition(0,0);
    }
    
    RedShader::RedShader()
    {
        
        shader.loadFromFile("/Users/darionmccoy/Dropbox/Game Design Stuff/Projects/Backup/HardSoda/HardSoda/shaders/redoverlay.frag",sf::Shader::Fragment);

        
    }
    
    DamageShader::DamageShader()
    {
        
        shader.loadFromFile("/Users/darionmccoy/Dropbox/Game Design Stuff/Projects/Backup/HardSoda/HardSoda/shaders/damage.frag",sf::Shader::Fragment);
        
        
    }
    
    Shader::~Shader(){
        
        
        
    }
    
    RedShader::~RedShader(){
        
    }
    
    WaveShader::~WaveShader(){
        
    }
    
    DamageShader::~DamageShader(){
        
    }
    
    void Shader::Update(){
        
    }
    
    void WaveShader::Update(){
        
        shader.setParameter("currentTexture", sf::Shader::CurrentTexture);
        shader.setParameter("distortionMapTexture", distortionMap);
        shader.setParameter("time", TextureContainer::tick);
        shader.setParameter("distortionFactor",0);
        shader.setParameter("wave_amplitude",20,20);
        shader.setParameter("wave_phase",TextureContainer::tick);
        shader.setParameter("riseFactor",.0f);
        
    }
    
    void RedShader::Update(){
        
        shader.setParameter("currentTexture", sf::Shader::CurrentTexture);
    }
    
    void DamageShader::Update(){
        
        shader.setParameter("currentTexture", sf::Shader::CurrentTexture);
    }
    
    TextureContainer::TextureContainer(){
        
        
        
        //Uses dirent.h to open up the textures directory, get all texture file names, create a map of file names/textures
        
        DIR *dip;
        dip = opendir("/Users/darionmccoy/Dropbox/Game Design Stuff/Projects/Backup/HardSoda/HardSoda/textures");
        struct dirent *dit;
        
        while((dit = readdir(dip)) != NULL){
            
            std::string name = dit->d_name;
            sf::Texture* newTexture;
            newTexture = new sf::Texture;
            newTexture->setSmooth(false);
            std::size_t found = name.find("tx_");
            
            if(found!=std::string::npos){
                
            
                if(newTexture->loadFromFile("/Users/darionmccoy/Dropbox/Game Design Stuff/Projects/Backup/HardSoda/HardSoda/textures/" + name)){
    
                    if(name.find("battle")) newTexture->setRepeated(true);
                    textureMap.insert(std::pair<std::string,sf::Texture*>(name,newTexture));
                    std::cout << name << " texture loaded ... " << std::endl;
                
                }
            }
    
        
            
        }
        
        closedir(dip);
        
        DIR *dip2;
        dip2 = opendir("/Users/darionmccoy/Dropbox/Game Design Stuff/Projects/Backup/HardSoda/HardSoda/levels");
        struct dirent *dit2;
        
        while((dit2 = readdir(dip2)) != NULL){
            
            std::string name = dit2->d_name;
            sf::Texture* newTexture;
            newTexture = new sf::Texture;
            newTexture->setSmooth(false);
            std::size_t found = name.find("tx_");
            
            if(found!=std::string::npos){
                
                
                if(newTexture->loadFromFile("/Users/darionmccoy/Dropbox/Game Design Stuff/Projects/Backup/HardSoda/HardSoda/levels/" + name)){
                    
                    if(name.find("bg")) newTexture->setRepeated(true);
                    textureMap.insert(std::pair<std::string,sf::Texture*>(name,newTexture));
                    std::cout << name << " texture loaded ... " << std::endl;
                    
                }
            }
            
            
            
        }
        
        closedir(dip2);
        
        GameFont.loadFromFile("/Users/darionmccoy/Dropbox/Game Design Stuff/Projects/Backup/HardSoda/HardSoda/fonts/ProggyTiny.ttf");
        const_cast<sf::Texture&>(GameFont.getTexture(16)).setSmooth(false);
    
    }
    
    
    sf::Texture& TextureContainer::SetTexture(std::string temp_name){
        
        //std::cout << temp_name << std::endl;
        return *textureMap.at(temp_name);
    
    }
    
    sf::Font& TextureContainer::GetFont(){
        
        //std::cout << temp_name << std::endl;
        return GameFont;
        
    }
    
    sf::Texture* TextureContainer::SetTextureForShapes(std::string temp_name){
        
        return textureMap.at(temp_name);
        
    }
    
    void TextureContainer::Update(){
        
        waveShader.Update();
        tick += 0.005;
        
    }
        
    
}