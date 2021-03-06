//
//  World.h
//  HardSoda
//
//  Created by Darion McCoy on 6/18/15.
//  Copyright (c) 2015 Darion. All rights reserved.
//

#ifndef __HardSoda__World__
#define __HardSoda__World__

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <math.h>
#include <functional>
#include <memory>
#include <map>
#include <sstream>

#define IN 0
#define OUT 1
#define DELAY 0
#define NODELAY 1

#ifndef WINDOW_X
#define WINDOW_X 480*3
#endif

#ifndef WINDOW_Y
#define WINDOW_Y 270*3
#endif

class Audio;
class Container;

namespace Textures{
    
    class TextureContainer;
}

namespace Entity{
    
    class Object;
    class Player;
    class Hud;
}

namespace Level{
    
    class LevelContainer;
    
}

#include "Audio.h"
#include "Textures.h"
#include "LevelContainer.h"
#include "Container.h"

#define MENU 0
#define MAP 1
#define ENCOUNTER 2

struct Attributes{
    
    int playerMaxHP = 30;
    int playerMaxMP = 55;
    std::shared_ptr<int> maxVolume;
    std::shared_ptr<int> health;
    std::shared_ptr<int> textSpeed;
    std::array<int,7> levelsCompleted;
    int currentLevel = 0;
    
};

struct Transition{
    
    Transition(std::string mapName = "")
    {
        if(mapName != "") destination = mapName;
    }
    std::string destination = "";
    virtual void Update(){
        
    }
    
    bool isDone = false;
    
};

struct Fade : public Transition{
    
    Fade(std::string mapName = "", bool dir = IN);
    sf::RectangleShape overlay;
    float transparency = 0;
    void Update();
    bool direction = OUT;
    std::unique_ptr<Entity::Player> playerTemp;
    
    
};


struct Scene{
    
    Scene();
    ~Scene();
    Audio audioContainer;
    Textures::TextureContainer textureContainer;
    std::unique_ptr<Container> objectContainer;
    std::unique_ptr<Level::LevelContainer> levelContainer;
    std::unique_ptr<Transition> transition;
    Entity::Player* playerPtr;
    Entity::Hud* hudPtr;
    bool isLoaded = false;

};

struct SceneScript{
    
    SceneScript(std::string tempname = "map1", int tempmapType = 1, float tempduration = 0);
    std::string name;
    int mapType;
    float duration = 0;
};

struct TimeObject{
    
    TimeObject(){
        
    }
    
    ~TimeObject(){
        
    }
    
    float timer = 0;
    float maxTime = 0;
    bool done = false;
    bool isActive = true;
    int type = DELAY;
    
};

int RoundUp(int numToRound, int multiple = 16);
int RoundDown(int numToRound, int multiple = 100);
void RotateVector(sf::Vector2f& coords,int degree = 45);
bool IsOverlapping(int minA, int maxA, int minB, int maxB);
float GetAngle(sf::Vector2f a, sf::Vector2f b);
int RandomNumber(int max, int min = 0);

float Sine();

class World{
    
    public:
    
    friend class Transition;
    
    World();
    static World *Worldptr;
    void Setup(sf::Clock& clock,sf::RenderWindow& window);
    void Run(sf::Event& event, float timestamp, sf::Clock& clock);
    void ScreenShake(int strength);
    void UpdateCamera();
    void ReadyScene(std::string mapName);
    void UpdateTransition();
    void UpdateTime(float timestamp);
    void RemoveTimeObjects();
    void ResetScene();
    void LoadSceneBank();
    void CreateCharacterScripts();
    static World *GetInstance(){
        
        if(!Worldptr) Worldptr = new World;
        return Worldptr;
        
    }
    
    SceneScript GetScene(std::string sceneName);
    std::map<std::string,SceneScript> SceneScriptBank;
    std::shared_ptr<SceneScript> CurrentScene;
    
    template<class T, typename I = int, typename O = int>
    bool Timer(T& obj, I time = 60, O type = DELAY)
    {
        std::stringstream ss;
        ss << std::hex << &obj;
        const std::string s = ss.str();
        
        bool timesUp = false;
        bool thistime = false;
        
        //std::cout << "Timer called" << std::endl;
        
        if((TimeArray.find(s) != TimeArray.end())){
            
            timeFindit it = TimeArray.equal_range(s);
            
            for(timeIt iter = it.first; iter != it.second; iter++){
                
                //std::cout << "Timer(" << iter->second.timer << "/" << iter->second.maxTime << ")" << std::endl;
                
                if(iter->second.maxTime == time){
                    
                    if(iter->second.timer >= time){
                        
                        //std::cout << "Timer(" << iter->second.timer << "/" << iter->second.maxTime << ") is done" << std::endl;
                        if(iter->second.type == DELAY){
                            
                            timesUp = true;
                            iter->second.done = true;
                                
                        }
                        
                        if(iter->second.type == NODELAY){
                            
                            iter->second.done = true;
                            
                        }
                        
                    }
                    
                    thistime = true;
                    
                }
                
                iter->second.isActive = true;
                
                
            }
            
            
        }
        
        
        if(TimeArray.find(s) == TimeArray.end() || thistime == false ){
            
            //std::cout << "Added Timer(" << time << ")" << std::endl;
            
            TimeObject temp;
            temp.maxTime = time;
            temp.type = type;
            
            if(type == NODELAY){
                
                timesUp = true;
                //temp.done = true;
                
            }
            
            TimeArray.insert ( std::pair<std::string,TimeObject>(s,temp) );
            
            
        }
        
        
        
        return timesUp;
    }
    
    Scene WorldScene;
    void LoadScene(std::string sceneName = "map1");
    
    bool created = false;
    
    //Camera  & members
    sf::View* GetView();
    sf::FloatRect screenPos;
    sf::View Screen;
    sf::Vector2f viewPos;
    float camOrient= 0;
    float vel = 0;
    bool isShaking = false;
    int quakeStrength = 20;
    
    Attributes GlobalMembers;
    std::map<std::string,std::string> CharacterScripts;
    
    sf::Event *eventWorld;
    sf::RenderWindow *windowWorld;
    sf::RenderTexture windowTexture;
    sf::Sprite windowTextureSprite;
    sf::Clock *clockWorld;
    static sf::Clock clock2;
    
    std::unique_ptr<Transition> TransitionOverlay;
    
    
    std::multimap<std::string,TimeObject> TimeArray;
    typedef std::multimap<std::string,TimeObject>::iterator timeIt;
    
    typedef std::pair <std::multimap<std::string,TimeObject>::iterator, std::multimap<std::string,TimeObject>::iterator> timeFindit;
    
};

extern World gameWorld;


#endif /* defined(__HardSoda__World__) */