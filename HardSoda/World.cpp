//
//  World.cpp
//  HardSoda
//
//  Created by Darion McCoy on 6/18/15.
//  Copyright (c) 2015 Darion. All rights reserved.
//

#include "World.h"
#include "Container.h"


class Container;

sf::Clock World::clock2;

Fade::Fade(std::string mapName,bool dir ) : Transition(mapName)
{

    // OUT closes level
    // IN opens level
    
    direction = dir;
    overlay.setPosition(0,0);
    overlay.setSize(sf::Vector2f(500,500));
    overlay.setFillColor(sf::Color(0,0,0,transparency));
    transparency = (direction == IN) ? 255 : 0;
}

void Fade::Update(){
    
    overlay.setPosition(World::GetInstance()->viewPos.x -240,0);
 
    
    if(direction == IN){
        
        transparency-= 5;
        
        if (transparency <= 0) isDone = true;
        
    }
    
    else{
        
        if(Entity::Player::dead) transparency += 1;
        else transparency+= 5;
        
            if(transparency >= 255){
                
                World::GetInstance()->LoadScene(destination);
                overlay.setPosition(World::GetInstance()->Screen.getCenter().x -240,0);
                World::GetInstance()->viewPos.x = 240;
                World::GetInstance()->Screen.setCenter(World::GetInstance()->viewPos.x,160);
                isDone = true;
                
            }
        
    }
    
    overlay.setFillColor(sf::Color(0,0,0,transparency));
    World::GetInstance()->windowTexture.draw(overlay);
    
}

void World::UpdateTransition(){
    
    WorldScene.transition->Update();
    
    if(WorldScene.transition->isDone) WorldScene.transition.reset();
    

}

World::World(){
    
    
}

void World::Setup(sf::Clock &clock, sf::RenderWindow &window){
    
    GlobalMembers.maxVolume = std::shared_ptr<int> (new int(80));
    GlobalMembers.health = std::shared_ptr<int> (new int(30));
    GlobalMembers.textSpeed = std::shared_ptr<int> (new int(5));
    Screen.setSize(WINDOW_X/3, WINDOW_Y/3);
    windowTexture.create(WINDOW_X,WINDOW_Y);
    clockWorld = &clock;
    windowWorld = &window;
    CurrentScene = std::shared_ptr<SceneScript>(new SceneScript);
    WorldScene.transition.reset();
    
    CreateCharacterScripts();
    GlobalMembers.levelsCompleted.fill(0);
    LoadSceneBank();
    LoadScene("map2_1");
    
}

void World::ReadyScene(std::string mapName){
    
    if(!WorldScene.transition){
    
   
            WorldScene.transition = std::unique_ptr<Transition> (new Fade(mapName,OUT));
    
        
    }
    
}

void World::LoadScene(std::string sceneName){
    
        clockWorld->restart();
        ResetScene();
        SceneScript currentScript = SceneScriptBank.at(sceneName);
        *CurrentScene = currentScript;
        std::cout << "Loading " << CurrentScene->name << std::endl;
        WorldScene.levelContainer->CreateScene();
        WorldScene.isLoaded = true;
        std::cout << "Done Loading" << CurrentScene->name << std::endl;
        if(sceneName != "battle") WorldScene.transition = std::unique_ptr<Transition> (new Fade("",IN));
}

void World::LoadSceneBank(){
    
    SceneScript Menu("menu",MENU);
    SceneScript Map1("map1");
    SceneScript Map2("map2_1");
    SceneScript Map3("map2_2");
    SceneScript Map4("map2_3");
    SceneScript Battle("battle",ENCOUNTER);
    SceneScriptBank.insert(std::pair<std::string,SceneScript>("menu",Menu));
    SceneScriptBank.insert(std::pair<std::string,SceneScript>("map1",Map1));
    SceneScriptBank.insert(std::pair<std::string,SceneScript>("map2_1",Map2));
    SceneScriptBank.insert(std::pair<std::string,SceneScript>("map2_2",Map3));
    SceneScriptBank.insert(std::pair<std::string,SceneScript>("map2_3",Map4));
    SceneScriptBank.insert(std::pair<std::string,SceneScript>("battle",Battle));
    
}

void World::ResetScene(){
    
    WorldScene.isLoaded = false;
    WorldScene.objectContainer->ObjectContainer.clear();
    WorldScene.levelContainer->levelArray.clear();
    WorldScene.levelContainer->LvlContainer.clear();
    
}

void World::Run(sf::Event& event, float timestamp, sf::Clock& clock){
    
    UpdateTime(timestamp);
    eventWorld = &event;
    
    if(Timer(*this,100.0f)){
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7)){
            
            ReadyScene("menu");
            
        }
        
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::T)){
            
            ReadyScene("battle");
            
        }
        
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num8)){
            
            ReadyScene("map1");
            
        }
        
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5)){
            
            ReadyScene("map2_1");
            
        }
        
    }

    if(WorldScene.isLoaded){
        
        WorldScene.objectContainer->AddObjects();
        WorldScene.objectContainer->RemoveObjects();
        WorldScene.objectContainer->UpdateObjects();
        
        
            WorldScene.levelContainer->Update();
            WorldScene.textureContainer.Update();
    
        
        WorldScene.objectContainer->CheckCollisions();
        WorldScene.levelContainer->CheckCollisions();
         
        UpdateCamera();
         
        WorldScene.levelContainer->DrawLevel();
        WorldScene.objectContainer->DrawObjects(*windowWorld,*this);
        //WorldScene.levelContainer->DrawBGTop();
    }
    
    if(WorldScene.transition) UpdateTransition();
     
     RemoveTimeObjects();
    
}

void World::UpdateTime(float timestamp){
    
    if(TimeArray.size() > 0){
        
        for(timeIt it = TimeArray.begin(); it != TimeArray.end(); ++it){
            
            if(it->second.isActive == false) it->second.done = true;
            else {it->second.timer += timestamp; it->second.isActive = false;}
        
            
        }
        
    }
}

void World::RemoveTimeObjects(){
    
    if(TimeArray.size() > 0){
        
        for(timeIt it = TimeArray.begin(); it != TimeArray.end();){
            
            if(it->second.done == true){
                
                TimeArray.erase(it++);
                
            }
            
            else{
                
                it++;
            }
            
        }

    }
    
}

void World::ScreenShake(int strength){
    
    quakeStrength = strength;
    isShaking = true;
 
}

void World::UpdateCamera(){
    
    
    if(CurrentScene->mapType == MAP || ENCOUNTER){
        
        //center the camera on player
        
        camOrient = 0;
        vel = 20;
        
        
        float playerPosX = (Entity::Player::dead == false) ? WorldScene.playerPtr->objectShadow.getPosition().x + camOrient : Screen.getCenter().x;
        float playerPosY = (Entity::Player::dead == false) ? WorldScene.playerPtr->objectShadow.getPosition().y + camOrient : Screen.getCenter().y;
        
        //Update the camera position
        
        
        viewPos.x += (playerPosX - Screen.getCenter().x) / vel;
        viewPos.y += (playerPosY - Screen.getCenter().y) / vel;
        Screen.setCenter(viewPos.x, viewPos.y);
        
        sf::Vector2f view = Screen.getCenter();
        sf::Vector2f size = Screen.getSize();
        
        
        if(CurrentScene->mapType == ENCOUNTER){
            
            // If camera is at the end of the level on the left, stop the camera
        
            if(view.x < size.x/2){
                
                viewPos.x = size.x/2;
                Screen.setCenter(size.x/2, viewPos.y);
                camOrient = 0;
                vel = 0;
                
            }
            
            // If right
            
            
            else if(view.x > WorldScene.levelContainer->lvlSize.x-size.x/2){
                
                viewPos.x = WorldScene.levelContainer->lvlSize.x-size.x/2;
                Screen.setCenter(WorldScene.levelContainer->lvlSize.x-size.x/2, viewPos.y);
                camOrient = 0;
                vel = 0;
                
            }
            
            
            if(view.y > WorldScene.levelContainer->lvlSize.y-size.y/2){
                
                viewPos.y = WorldScene.levelContainer->lvlSize.y-size.y/2;
                Screen.setCenter(viewPos.x, WorldScene.levelContainer->lvlSize.y-size.y/2);
                camOrient = 0;
                vel = 0;
                
            }
            
            else if(view.y < size.y/2){
                
                viewPos.y =size.y/2;
                Screen.setCenter(viewPos.x, size.y/2);
                camOrient = 0;
                vel = 0;
                
            }
            
        }
        
        //Pass viewport data to level
        
        WorldScene.levelContainer->view = view;
        
        // If called to shake the camera ...
        
        if(isShaking){
            
        //Lower music volume on screenshake

            
        //if ScreenShake() has been called, shake the screen until back to original position
            
            int x = (rand() % quakeStrength) + viewPos.x;
            int y = rand() % quakeStrength + viewPos.y;
            
            Screen.setCenter(x,y);
            quakeStrength--;
            
            if(quakeStrength <= 0){
                
                isShaking = false;
                
            }
            
        }
        
    }
    
    if(CurrentScene->mapType == MENU){
        
        viewPos.x = 240;
        Screen.setCenter(240,135);
        camOrient = 0;
        vel = 0;
        
        
    }
    

    
}

sf::View* World::GetView(){
    
    return &Screen;
    
}

void World::CreateCharacterScripts(){
    
    std::string lim0 = "Say you want to have a party. You want it to be the most incredible party on the planet, one that all of your friends will remember forever. You’re putting a lot of money into it, so you can’t afford to fail. It’s gotta be perfect. To pull off an event like this, you’ll need to get a bunch of things: balloons, chairs, pizza. You decide that just holding it at some bar won’t be cool enough, so you’re gonna hold it in France. You hire a bunch of people to help you make this party happen, with the promise that it’s going to be goddamn legendary.";
    std::string mozza1 = "I don't really have much to say, tbh";
    std::string hauzer2 = "Hmmm. Who do I have to fuck for that job?";
    std::string feet3 = "FUCK ...> OFF";

    CharacterScripts.insert(std::pair<std::string,std::string>("LIM0",lim0));
    CharacterScripts.insert(std::pair<std::string,std::string>("MOZZA0",mozza1));
    CharacterScripts.insert(std::pair<std::string,std::string>("HAUZER2",hauzer2));
    CharacterScripts.insert(std::pair<std::string,std::string>("FEET3",feet3));
    
}

////////////// SCENES /////

Scene::Scene(){
    
    levelContainer = std::unique_ptr<Level::LevelContainer>(new Level::LevelContainer);
    objectContainer = std::unique_ptr<Container>(new Container);
    transition = std::unique_ptr<Transition>(new Transition);
    
}

Scene::~Scene(){
    
}

SceneScript::SceneScript(std::string tempname, int tempmapType, float tempduration){
    
    name = tempname;
    mapType = tempmapType;
    duration = tempduration;
    
}

int RoundUp(int numToRound, int multiple)
{
    if (multiple == 0) return numToRound;
    
    int remainder = abs(numToRound) % multiple;
    
    if (remainder == 0) return numToRound;
    
    if (numToRound < 0) return -(abs(numToRound) - remainder);
    
    return numToRound + multiple - remainder;
}

int RoundDown(int numToRound, int multiple)
{
    if (multiple == 0) return numToRound;
    
    int remainder = abs(numToRound) % multiple;
    
    if (remainder == 0) return numToRound;
    
    if (numToRound < 0) return -(abs(numToRound) - remainder);
    
    return (abs(numToRound) - remainder);
}

void RotateVector(sf::Vector2f& coords, int degree){
    
    double angle = degree * M_PI / 180.0;
    double cs = cos(angle);
    double sn = sin(angle);
    
    double newX = coords.x * cs - coords.y * sn;
    double newY = coords.x * sn + coords.y * cs;
    
    coords.x = newX;
    coords.y = newY;

    
}

bool IsOverlapping(int minA, int maxA, int minB, int maxB){
    
    return minB < maxA && minA < maxB;
    
}

float GetAngle(sf::Vector2f a, sf::Vector2f b){
    
    float temp3 = a.x - b.x;
    float temp4 = a.y - b.y;
    return atan2(temp3, temp4) * 180 / M_PI;
    
}

std::string GetAddress(Entity::Object& obj){
    
    std::stringstream ss;
    ss << "your id is " << std::hex << &obj;
    const std::string s = ss.str();
    return s;
    
}

int RandomNumber(int max, int min){
    
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> rand(min,max);
    return rand(mt);
    
}

float Sine(){
    
    return 0.06 * sin((World::clock2.getElapsedTime().asSeconds()) * 0.5 * M_PI) + 1;
    
}

