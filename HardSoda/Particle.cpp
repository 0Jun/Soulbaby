//
//  Particle.cpp
//  sdev
//
//  Created by Darion McCoy on 1/10/14.
//  Copyright (c) 2014 D man. All rights reserved.
//

#include "Particle.h"
#include "ResourcePath.hpp"
#include <math.h>


namespace Entity
{
    
    //////////////// GUI & RELATED ////////////////////////////////////////////////////////////////////////////////////
    bool Player::dead = false;
    int MenuItem::count = 0;
    sf::Font MenuItem::menuFnt;
    sf::Font HitNum::hitFnt;
    int Object::center = 0;
    int Object::bottomCenter = 1;
    int Object::topLeft = 2;
    
    int Textbox::lineLength = 60;
    int Textbox::maxLines = 3;
    int Textbox::scriptLength = 0;
    int Textbox::progressSpeed = 0;
    std::string Textbox::characters[11] = {"LIM","MOZZA","HAUZER","FEET","NOOK"};
    
    GUI::GUI() : Object(){
        
        type = "GUI";
        
    }
    
    void GUI::Update(){
        
    }
    
    GUI::~GUI(){
        
    }
    
    MenuItem::MenuItem(std::string lableName, char typeName) : GUI(){
        
        if(menuFnt.getInfo().family == ""){
        
        menuFnt.loadFromFile("/Users/darionmccoy/Dropbox/Game Design Stuff/Projects/Backup/HardSoda/HardSoda/fonts/apfont.ttf");
         const_cast<sf::Texture&>(menuFnt.getTexture(16)).setSmooth(false);
    
        }
        
        menuLable.setFont(menuFnt);
        menuLable.setString(lableName);
        menuLable.setPosition(70,135+(count * 15));
        menuLable.setCharacterSize(16);
        menuLable.setColor(sf::Color::White);
        
        type = typeName;
        
        if(type == SWITCH){
            
            objectSprite.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_menu_bar.png"));
            objectSprite.setPosition(menuLable.getPosition().x + (35),menuLable.getPosition().y +7);
            objectSprite.setTextureRect(sf::IntRect(0,0,1,10));
            
            if(lableName == "Volume"){
                
                linkedAttribute = std::weak_ptr<int>(World::GetInstance()->GlobalMembers.maxVolume);
                objectSprite.setTextureRect(sf::IntRect(0,0,*World::GetInstance()->GlobalMembers.maxVolume,10));
            }
            
            else if(lableName == "Health"){
                
                linkedAttribute = std::weak_ptr<int>(World::GetInstance()->GlobalMembers.health);
                objectSprite.setTextureRect(sf::IntRect(0,0,*World::GetInstance()->GlobalMembers.health,10));
                
            }
        
        }
        
        count++;
        
    }
    
    
    
    void MenuItem::Update(){
        
        
        
    };
    
    void MenuItem::ToggleSelection(){
        
        if(menuLable.getColor() == sf::Color::White){
            
            menuLable.setColor(sf::Color::Green);
            menuLable.move(2,0);
            
        }
        
        else{
            
            menuLable.setColor(sf::Color::White);
            menuLable.move(-2,0);
            
        }
    }
    
    void MenuItem::Draw(sf::RenderTarget& window){
        
        World::GetInstance()->windowTexture.draw(objectSprite);
        World::GetInstance()->windowTexture.draw(menuLable);
        
    }
    
    MenuItemHolder::MenuItemHolder() : GUI(){
        
        MenuItem::count = 0;
        
    }
    
    void MenuItemHolder::Update(){
    }
    
    void MenuItemHolder::Draw(sf::RenderTarget &window){
        
    }
    
    MenuItemHolder::~MenuItemHolder(){
    }
    
    
    Menu::Menu() : GUI(){
        
        MenuItemHolder menuHolder;
        MenuItem item1 ("Start",START);
        MenuItem item2("Options",DIVE);
        MenuItem item3("Quit",EXIT);
    
        menuHolder.menuList.push_back(item1);
        menuHolder.menuList.push_back(item2);
        menuHolder.menuList.push_back(item3);
        menuHolder.menuList.at(0).ToggleSelection();
        menuContainer.push_back(menuHolder);
        
    }
    
    void Menu::Update(){
        
        //curent menu is always beginning of vector and what should be drawn
        
        int currentScreen = menuContainer.size()-1;
        
        if(World::GetInstance()->Timer(*this,30.0f)){
        
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
                
                World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_jump2");
                if(menuContainer.at(currentScreen).currentPos != 0){
                    
                    menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos).ToggleSelection();
                    --menuContainer.at(currentScreen).currentPos;
                    menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos).ToggleSelection();
                    
                }
                
                else{
                    
                    menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos).ToggleSelection();
                    menuContainer.at(currentScreen).currentPos = menuContainer.at(currentScreen).menuList.size()-1;
                    menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos).ToggleSelection();
                    
                }
            
            }
            
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
                
                World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_jump2");
                if(menuContainer.at(currentScreen).currentPos != menuContainer.at(currentScreen).menuList.size()-1){
                    
                    menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos).ToggleSelection();
                    ++menuContainer.at(currentScreen).currentPos;
                    menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos).ToggleSelection();
                    
                }
                
                else{
                    
                    menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos).ToggleSelection();
                    menuContainer.at(currentScreen).currentPos = 0;
                    menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos).ToggleSelection();
                }
                
            }
            
            
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && HasSwitch(menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos))){
                
                --*menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos).linkedAttribute.lock();
                // switch visul changes need to go here
                std::cout << *World::GetInstance()->GlobalMembers.maxVolume << std::endl;
                menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos).objectSprite.setTextureRect(sf::IntRect(0,0,*menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos).linkedAttribute.lock(),10));
            
            }
        
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && HasSwitch(menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos))){
                
                ++*menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos).linkedAttribute.lock();
                // switch visul changes need to go here
                std::cout << *World::GetInstance()->GlobalMembers.maxVolume << std::endl;
                menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos).objectSprite.setTextureRect(sf::IntRect(0,0,*menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos).linkedAttribute.lock(),10));
            
            }
        
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) DoMenuAction(menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos));
            
            else if((sf::Keyboard::isKeyPressed(sf::Keyboard::X) && menuContainer.size() != 1)) menuContainer.pop_back();
                
                

        }
    }
    
    void Menu::DoMenuAction(MenuItem& item){
        
        if(item.type == START) World::GetInstance()->ReadyScene("map1");
        
        
        else if(item.type == DIVE){
            
            MenuItemHolder menuHolder;
            MenuItem item1 ("Volume",SWITCH);
            MenuItem item2("Health",SWITCH);
            
            menuHolder.menuList.push_back(item1);
            menuHolder.menuList.push_back(item2);
            menuHolder.menuList.at(0).ToggleSelection();
            menuContainer.push_back(menuHolder);
            
        }
        
        else if(item.type == EXIT) World::GetInstance()->windowWorld->close();
    }
    
    bool Menu::HasSwitch(MenuItem& item){
        
        return item.type == SWITCH;
        
    }
    
    void Menu::Draw(sf::RenderTarget& window){
        
        for (auto i : menuContainer.at(menuContainer.size()-1).menuList) i.Draw(window);
        
    }
    
    Menu::~Menu(){
        
    
    }
    
    MenuItem::~MenuItem(){
    
    }
    
    Textbox::Textbox(int name) : GUI()
    {
        std::cout << "creating texbox" << std::endl;
        
        
        characterName = characters[name];
        
        //std::cout << characterName + std::to_string(World::GetInstance()->GlobalMembers.levelsFinished) << std::endl;
        
        script = World::GetInstance()->CharacterScripts.at(characterName + std::to_string(0));
        
        int lineCount = 0;
        
        //iterate through string to insert breaks
        
        if(script.length() >= lineLength){
        
            for(int i = lineLength; i < script.length(); i+= lineLength){
            
                if(i <= script.length()){
                    
                    //check if the current string position is a letter (this logic has to be flipped for some reason)
                    
                    if(strncmp(&script.at(i)," ",1))
                    {
                        
                        //check previous letters for the first space to break there instead of breaking mid-word
                        
                        for(int n = i; n != 0; n--){
                            
                            if(!strncmp(&script.at(n)," ",1)){
                                
                                script.replace(n,1,"\n");
                                i -= i-n;
                                break;
                                
                            }
                            
                        }
                    
                    }
                    
                    //if not, treat this letter as a space
                    
                    else{
                    
                        script.replace(i,1,"\n");
                        i++;
                                   
                    }
                    
                    lineCount++;
                    
                    //if the max lines for the text box has been reached, insert the trigger to erase the current text to begin progressing the next lines
                    
                    if(lineCount == maxLines){
                        
                        script.replace(i,1,">");
                        lineCount = 0;
                    
                    }
                    
                }
                
                else break;
            }
            
        }
        
        scriptLength = script.length();
        
        backDrop.setFillColor(sf::Color::Black);
        backDrop.setSize(sf::Vector2f(WINDOW_X/3,100));
        
        objectSprite.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_portraits.png"));
        
        portraitSize = sf::Vector2i(60,60);
        
        objectSprite.setTextureRect(sf::IntRect(0,portraitSize.y * name,portraitSize.x,portraitSize.y));
        
        boxText.setFont(World::GetInstance()->WorldScene.textureContainer.GetFont());
        boxText.setCharacterSize(16);
        
        boxName.setFont(World::GetInstance()->WorldScene.textureContainer.GetFont());
        boxName.setString(characterName);
        boxName.setCharacterSize(16);
        
        progressSpeed = *World::GetInstance()->GlobalMembers.textSpeed.get();
        
        
    }
    
    void Textbox::Draw(sf::RenderTarget &window){
        
        World::GetInstance()->windowTexture.draw(backDrop);
        World::GetInstance()->windowTexture.draw(boxText);
        World::GetInstance()->windowTexture.draw(boxName);
        World::GetInstance()->windowTexture.draw(objectSprite);
        
    }
    
    Textbox::~Textbox(){
        
        scriptLength = 0;
    }
    
    void Textbox::Update(){
        
        backDrop.setPosition(World::GetInstance()->viewPos.x-((WINDOW_X/3)/2),World::GetInstance()->viewPos.y + 50);
        objectSprite.setPosition(backDrop.getPosition().x + 10,backDrop.getPosition().y +10);
        boxName.setPosition(objectSprite.getPosition().x + objectSprite.getTextureRect().width + 10,objectSprite.getPosition().y);
        boxText.setPosition(objectSprite.getPosition().x + objectSprite.getTextureRect().width + 10,objectSprite.getPosition().y + 15);
        
        if(newScript.length() != scriptLength && script.length() != 0){
            
            if(World::GetInstance()->Timer(*this,30.0f, NODELAY)){
                
                // if end of text has been reach, stop progress until player presses action
                
                for(int p = 0; p != progressSpeed; p++){
                    
                    //if our progression goes beyond the limit, break and set to limit
                    std::cout << newScript.length() + 1 << " | " <<  script.length() << std::endl;

                    if(script.length() == 0){
                        
                        break;
                        
                    }
                    
                    // if trigger character, stop progress and move to next set of lines when player presses "Z"
                    
                    else if(!strncmp(&script.at(0),">",1)){ 
                        
                        
                        if(World::GetInstance()->Timer(*this,60.0f)){
                        
                            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z)){
                            
                                newScript.erase(0);
                                script.erase(0,1);
                            
                            }
                            
                        }
                        
                    }
                    
                    // if none of the conditions above are not true, proceed
                    
                    else{
                        
                        if(World::GetInstance()->Timer(*this,40.0f, NODELAY)){
                            
                            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z)){
                                
                                unsigned long int lastLinePos = script.find(">");
                                std::cout << "trigger at " << lastLinePos << std::endl;
                                newScript.append(script,0,lastLinePos);
                                script.erase(0,lastLinePos);
                                boxText.setString(newScript);
                                
                            }
                            
                        }
                        
                        else{
                        
                            newScript.append(script,0,1);
                            script.erase(0,1);
                            World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_txt"); 
                            
                        }
                        
                        boxText.setString(newScript);
                                            
                    }
                    
                }
                
                              
            
            } 
            
        }
        
        else{
            
            if(World::GetInstance()->Timer(*this,40.0f)){
                
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) misDestroyed = true;
                
            }
            
        }
        
    }
    
    BattleBackground::BattleBackground(){
                
        objectSprite.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_battle_bg_" + std::to_string(World::GetInstance()->GlobalMembers.currentLevel) + ".png"));
        objectSprite.setTextureRect(sf::IntRect (0,0,2000,2000));
        objectSprite.setOrigin(2000/2,2000/2);
        type = "BG";
        
    }
    
    Door::Door(std::string pathway) : Object() {
        
        levelPath = stoi(pathway.erase(0,pathway.find("_")+1));
        
        objectSprite.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_misc.png"));
        objectSprite.setTextureRect(sf::IntRect(0,42+(32*0),16,32));
        SetHitBox(sf::Vector2f(objectSprite.getTextureRect().width,objectSprite.getTextureRect().height),2);
        Object::type = "Prop";
        std::cout << "Door created" << std::endl;
        
    }

    int Door::getPath(){
        
        return levelPath;
    
    }
    
    void Door::Update(){
        
        if (World::GetInstance()->Timer(*this,20.0f)){ 
            
            objectSprite.setTextureRect(sf::IntRect(16*framePos,42+(32*0),16,32));
            framePos = (framePos > 1) ? 0 : ++framePos;
            
        }
        
    }
    
    void Door::Draw(sf::RenderTarget &window){
        
        World::GetInstance()->windowTexture.draw(objectSprite);
    }
    
    void Door::isCollided(int var){
        
        std::cout << "Calling ReadyScene() ..." << std::endl;
        World::GetInstance()->GlobalMembers.currentLevel = levelPath;
        World::GetInstance()->ReadyScene("battle");
    }
    
    
    ///////////////////////
    
    Hud::Hud() : GUI (){
        
        World::GetInstance()->WorldScene.hudPtr = this;
        hnumFont.loadFromFile("/Users/darionmccoy/Dropbox/Game Design Stuff/Projects/Backup/HardSoda/HardSoda/fonts/pxsmll.ttf");
        hhealthNum.setFont(hnumFont);
        hpowerNum.setFont(hnumFont);
        
        hbar.setTextureRect(sf::IntRect(0,0,62,39));
        hbar.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_menu.png"));
        
        hitem.setTextureRect(sf::IntRect(0,0,16,16));
        hitem.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_menu_item.png"));
        
        hhealth.setTextureRect(sf::IntRect(-20,0,24,4));
        hhealth.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_menu_health_pow.png"));
        
        hpower.setTextureRect(sf::IntRect(-20,4,24,4));
        hpower.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_menu_health_pow.png"));
        
    }

    
    void Hud::Update(){
        
        
        if(hmaxhealth == 0 || hmaxpow == 0){
            
            hmaxhealth = World::GetInstance()->WorldScene.playerPtr->playerHealth;
            hmaxpow = World::GetInstance()->WorldScene.playerPtr->playerPow;
            
        }
        
        if(World::GetInstance()->WorldScene.playerPtr->playerHealth < 0) World::GetInstance()->WorldScene.playerPtr->playerHealth = 0;
        
        
        if(World::GetInstance()->WorldScene.playerPtr->playerPow < 0) World::GetInstance()->WorldScene.playerPtr->playerPow = 0;
        
        hbar.setPosition(World::GetInstance()->viewPos.x - 225,40);
        hitem.setPosition(hbar.getPosition().x + 11,hbar.getPosition().y + 11);
        
        hhealth.setTextureRect(sf::IntRect(-World::GetInstance()->WorldScene.playerPtr->playerHealth,0,World::GetInstance()->WorldScene.playerPtr->playerHealth + 4,4));
        hhealth.setPosition(hbar.getPosition().x + 53,hbar.getPosition().y + 4);
        
        hpower.setTextureRect(sf::IntRect(-World::GetInstance()->WorldScene.playerPtr->playerPow,4,World::GetInstance()->WorldScene.playerPtr->playerPow + 4,4));
        hpower.setPosition(hbar.getPosition().x + 50,hbar.getPosition().y + 10);
        
        std::string temphealth (std::to_string(World::GetInstance()->WorldScene.playerPtr->playerHealth) + "/" + std::to_string(hmaxhealth));
        std::string temppow (std::to_string(World::GetInstance()->WorldScene.playerPtr->playerPow) + "/" + std::to_string(hmaxpow));
        
        hhealthNum.setString(temphealth);
        hpowerNum.setString(temppow);
        hhealthNum.setCharacterSize(16);
        hpowerNum.setCharacterSize(16);
        
        hhealthNum.setPosition(hhealth.getPosition().x + World::GetInstance()->WorldScene.playerPtr->playerHealth + 6,hhealth.getPosition().y - 12);
        hpowerNum.setPosition(hhealth.getPosition().x + World::GetInstance()->WorldScene.playerPtr->playerPow + 3,hpower.getPosition().y - 12);
        
        if(hdamaged){
            
            hhealth.setTextureRect(sf::IntRect(-World::GetInstance()->WorldScene.playerPtr->playerHealth,8,World::GetInstance()->WorldScene.playerPtr->playerHealth + 4,4));
            
            if(World::GetInstance()->Timer(*this,200.0f)) hdamaged = false;
            
        }
        
        //hitem.setTextureRect(sf::IntRect(World::GetInstance()->WorldScene.playerPtr->selectedWeapon * 16,0,16,16));
        
    }
    
    void BattleBackground::Update(){
        
         objectSprite.rotate(0.05);
        
    }
    
    
    void Hud::Draw(sf::RenderTarget& window){
        
        World::GetInstance()->windowTexture.draw(hbar);
        World::GetInstance()->windowTexture.draw(hitem);
        World::GetInstance()->windowTexture.draw(hhealth);
        World::GetInstance()->windowTexture.draw(hpower);
        World::GetInstance()->windowTexture.draw(hhealthNum);
        World::GetInstance()->windowTexture.draw(hpowerNum);
        
    }
    
    void BattleBackground::Draw(sf::RenderTarget& window){
        
        World::GetInstance()->windowTexture.draw(objectSprite,&World::GetInstance()->WorldScene.textureContainer.waveShader.shader);
        
    }
    
    Hud::~Hud(){
        
        
    }
    
    Door::~Door(){
        
    }
    
    BattleBackground::~BattleBackground(){
        
    }

    
    //////////////// PLAYER DEFINITIONS //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
        
    Player::Player(sf::Vector2i pos) : Object ()
    {
        
        std::cout << "Player created" << std::endl;
        objectSprite.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_player.png"));
        objectSprite.setPosition(550,550);
        objectSprite.setTextureRect(sf::IntRect(0, south, 13, 24));
        SetCharacterOrigin();
        spriteHeight = objectSprite.getTextureRect().height;
        spriteWidth = objectSprite.getTextureRect().width;
        playerHealth = World::GetInstance()->GlobalMembers.playerMaxHP;
        playerPow = World::GetInstance()->GlobalMembers.playerMaxMP;
        World::GetInstance()->WorldScene.playerPtr = this;
        SetShadow();
        type = "Player";
        scaleTemp = sf::Vector2f(0.12,0);
        SetHitBox(sf::Vector2f(6,6),1);
        dead = false;
        
    }
    
    Player::~Player(){
        
        posXtemp1 = NULL;
        posYtemp2 = NULL;
        delete posXtemp1;
        delete posYtemp2;
        dead = true;
        
        
    }
    
    void Player::Update()
    {
        
        // Walking
        if(!World::GetInstance()->WorldScene.transition){
            
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) && World::GetInstance()->Timer(*this,60.0f, NODELAY)){
                
                itemQueue textbox;
                textbox.properties["itemType"] = "Textbox";
                textbox.properties["ActorName"] = std::to_string(2);
                World::GetInstance()->WorldScene.objectContainer->Queue.push_back(textbox);
                
            }
        
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) movement = swest;
        
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) movement = nwest;
        
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) movement = neast;
        
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) movement = seast;
        
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) movement = south;
        
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) movement = west;
        
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) movement = north;
        
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) movement = east;

        }
        
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) movement = idle;
        
        
        
        // Animation
        
        if(movement != idle){
            
            vel.y = 1;
            vel.x = 0;
            RotateVector(vel,45*movement);
            
            if(World::GetInstance()->Timer(*this,60.0f, NODELAY)){
                
                frame_pos = (frame_pos >= 5) ? 0 : frame_pos+1;
                if(frame_pos == 1 || frame_pos == 4) World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_step"); 
                objectSprite.setTextureRect(sf::IntRect(frame_pos * spriteWidth, movement * FRAME, spriteWidth, spriteHeight));
            
            }
            
        }
        
        else{
              
              vel.x = 0;
              vel.y = 0;
              objectSprite.setTextureRect(sf::IntRect(0, objectSprite.getTextureRect().top, spriteWidth, spriteHeight));
          
          }
    
        
        vel.x = round(vel.x);
        vel.y = round(vel.y);
        objectSprite.move(vel.x,vel.y);
        UpdateShadow();
        UpdateHitBox();
        *posXtemp1 = objectSprite.getPosition().x;
        *posYtemp2 = objectSprite.getPosition().y;
        
        //std::cout << vel.x << "|" << vel.y << std::endl;
        
        // breathing effect
        
        objectSprite.setScale(Sine(),Sine());
        
    }
    
    BattlePlayer::BattlePlayer(){
        
        hotSpot.x = objectSprite.getPosition().x + 6;
        hotSpot.y = objectSprite.getPosition().y + 12;
        std::cout << objectSprite.getPosition().x << "," << objectSprite.getPosition().y << std::endl;
        
        
    }
    
    void BattlePlayer::Update(){
        
        if(!World::GetInstance()->WorldScene.transition){
            
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) && World::GetInstance()->Timer(*this,60.0f, NODELAY)){
                
                itemQueue textbox;
                textbox.properties["itemType"] = "Textbox";
                textbox.properties["ActorName"] = std::to_string(3);
                World::GetInstance()->WorldScene.objectContainer->Queue.push_back(textbox);
                
            }

        
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) movement = swest;
        
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) movement = nwest;
        
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) movement = neast;
        
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) movement = seast;
        
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) movement = south;
        
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) movement = west;
        
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) movement = north;
        
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) movement = east;
            
        
        }
        
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) movement = idle;
        
        // Animation
        
        if(movement != idle){
            
            vel.y = 2.25;
            vel.x = 0;
            RotateVector(vel,45*movement);
            
            if(World::GetInstance()->Timer(*this,30.0f,NODELAY)){
                
                frame_pos = (frame_pos >= 5) ? 0 : frame_pos+1;
    
                if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Z)){
                    
                    objectSprite.setTextureRect(sf::IntRect(frame_pos * spriteWidth, movement * FRAME, spriteWidth, spriteHeight));
                    fireDir = movement;
                    
                }
                
                else objectSprite.setTextureRect(sf::IntRect(frame_pos * spriteWidth, objectSprite.getTextureRect().top, spriteWidth, spriteHeight));
                
            }
            
            
        }
        
        else{
            
            vel.x = 0;
            vel.y = 0;
            objectSprite.setTextureRect(sf::IntRect(0, objectSprite.getTextureRect().top, spriteWidth, spriteHeight));
            
        }
        
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::X) && World::GetInstance()->Timer(*this,100.0f,NODELAY)){
            
            itemQueue bullet;
            bullet.properties["PosX"] = std::to_string(hotSpot.x);
            bullet.properties["PosY"] = std::to_string(hotSpot.y);
            bullet.properties["itemType"] = "PlayerLaser";
            bullet.properties["Direction"] = std::to_string(fireDir);
            bullet.properties["VelX"] = std::to_string(vel.x);
            bullet.properties["VelX"] = std::to_string(vel.y);
            World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);
            World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_laser9");
            
        }
        
        vel.x = round(vel.x);
        vel.y = round(vel.y);
        objectSprite.move(vel.x,vel.y);
        //std::cout << vel.x << "|" << vel.y << std::endl;
        hotSpot.x = objectSprite.getPosition().x;
        hotSpot.y = objectSprite.getPosition().y - objectSprite.getTextureRect().height/3;
        UpdateShadow();
        *posXtemp1 = objectSprite.getPosition().x;
        *posYtemp2 = objectSprite.getPosition().y;
        
        objectSprite.setScale(Sine(),Sine());
        
    }
    
    BattlePlayer::~BattlePlayer(){
        
        itemQueue particles;
        particles.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
        particles.properties["PosY"] = std::to_string(objectSprite.getPosition().y);
        particles.properties["itemType"] = "DeathPoof";
        
        
        World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
        World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
        World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
        
        particles.properties["itemType"] = "DeathBoom";
        World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
        
        
        World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_playerDeath");
        
        World::GetInstance()->ScreenShake(20);
    }
    
    //////////// END PLAYER DEFINITIONS ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    ////////// MISC
    
    
    // Constructors
    
    int Object::totalObjects = 0;
    int Fixed::spriteRotation = 0;
    int Fixed::assetHeight = 0;
    
    Object::Object()
    {
        
        totalObjects++;
        
    }
    
    Fixed::Fixed() : Particle(){
        
        
        
        
        objectSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_particles.png")));
        
        if(assetHeight == 0){
                
            sf::Texture temp = *objectSprite.getTexture();
            sf::Vector2u tempSize = temp.getSize();
            assetHeight = tempSize.y/4;
            
        }
        
        if(spriteRotation != 2) spriteRotation++;
        else spriteRotation = 0;
        
    }
    
    Spark::Spark() : Fixed(){
        
        int randomSpark = RandomNumber(2);
        if (randomSpark == 0) objectSprite.setTextureRect(sf::IntRect(0, 128, 5, 5));
        else if (randomSpark ==  1) objectSprite.setTextureRect(sf::IntRect(0, 133, 5, 5));
        else if (randomSpark == 2) objectSprite.setTextureRect(sf::IntRect(0, 138, 7, 7));
        
        vel.y = int(RandomNumber(7));
        RotateVector(vel,RandomNumber(180));
        SetEffectOrigin();
        maxFrame = 4;
        animSpeed = 40.0f;
        deacceleration = 0.5;
        
    }
    
    Spark::~Spark(){
        
    }
    
    DeathPoof::DeathPoof() : Fixed(){
        
        objectSprite.setTextureRect(sf::IntRect(0, 106, 17, 17));
        vel.y = 3;
        RotateVector(vel,RandomNumber(180));
        SetEffectOrigin();
        maxFrame = 4;
        animSpeed = 60.0f;
        deacceleration = 0.5;
        
    }
    
    DamageSpark::DamageSpark() : Fixed(){
        
        //int sparkType = std::rand() % 3;
        int x = std::rand() % 4 + -8;
        int y = std::rand() % 4 + -8;
        
        objectSprite.setTextureRect(sf::IntRect(0, spriteRotation*35, 44, 35));
        objectSprite.move(x,y);
        SetEffectOrigin();
        maxFrame = 4;
        animSpeed = 0.5f;

    }
    
    DeathBoom::DeathBoom() : Fixed(){
        
        objectSprite.setTextureRect(sf::IntRect(0, 145, 25, 25));
        SetEffectOrigin();
        maxFrame = 2;
        animSpeed = 0.5f;
        
    }
    
    Hit::Hit(){
        
        objectSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_hit.png")));
        objectSprite.setTextureRect(sf::IntRect(0, 0, 30, 30));
        objectSprite.setOrigin(15,15);
    
    }
    
    
    Projectile::Projectile() : Object ()
    {
        
        objectSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_projectiles.png")));
        type = "Projectile";
        
    }
    
    EnemyProjectile::EnemyProjectile() : Projectile(){
        
        type = "EnemyProjectile";
        
    }
    
    EnemyBlip::EnemyBlip() : EnemyProjectile(){
        
        objectSprite.setTextureRect(sf::IntRect(0, 19,7, 7));
        vel.y = 1;
        SetEffectOrigin();
        SetHitBox(sf::Vector2f(3,3));
        active = true;

        
    }
    
    Particle::Particle() : Object ()
    {
        
        objectSprite.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_particles.png"));
        
        
    }
    
    Passive::Passive() : Particle(){
        
        
        
    }
    
    Prop::Prop() : Object(){
        
        
        
    }
    
    PropFlame::PropFlame() : Prop(){
        
        objectSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_misc.png")));
        objectSprite.setTextureRect(sf::IntRect(0, 5, 11, 30));
        maxFrame = 5;
        animSpd = 100.0f;
        frame = RandomNumber(5);
        
    }
    
    float Object::GetYPosition(){
        
        return objectSprite.getPosition().y;
        
    }
    
    sf::IntRect Object::GetObjectBounds(){
        
        return sf::IntRect(objectHitBox.getPosition().x - objectHitBox.getOrigin().x,objectHitBox.getPosition().y - objectHitBox.getOrigin().y,objectHitBox.getSize().x,objectHitBox.getSize().y);
    }
    
    Dirt::Dirt() : Particle (){
        
        isAnimated = false;
        objectSprite.setTextureRect(sf::IntRect(0, 6, 2, 2));
        gravity = 0.2f;
        
        int q = rand() % 3;
        
        if(q == 0 || q == 1)
        {
            
            objectSprite.setScale(0.5,0.5);
            
        }
        
        
    }
    
    Gib::Gib() : Particle(){
        
        objectSprite.setTexture(gibTexture);
        isAnimated = false;
        gravity = 0.3f;
        
    }
    
    
    Bullet::Bullet() : Projectile ()
    {
        
        objectSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_projectiles.png")));
        objectSprite.setTextureRect(sf::IntRect(0, 0, 25, 4));
        damage = 3;
        
    }
    
    PlayerLaser::PlayerLaser() : Projectile()
    {
        
        objectSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_projectiles.png")));
        objectSprite.setTextureRect(sf::IntRect(0, 0, 10, 10));
        objectSprite.setOrigin(5,5);
        SetEffectOrigin();
        vel.y = 4;
        damage = 7;
        SetHitBox(sf::Vector2f(4,4));
        
    }
    
    Explosion::Explosion() : Object ()
    {
        
        objectSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_polygon.png")));
        objectSprite.setOrigin(40,40);
        objectSprite.setTextureRect(sf::IntRect(0, 0, 80, 80));
        
    }
    
    Cluster::Cluster() : Object ()
    {
        
        
        
    }
    
    
    Laser::Laser() : Projectile ()
    {
        
        objectSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_blip_3.png")));
        objectSprite.setOrigin(3,3);
        objectSprite.setTextureRect(sf::IntRect(0, 0, 6, 6));
        Projectile::damage = 10;
        
    }
    
    HitNum::HitNum() : Object()
    {
        
        if(hitFnt.getInfo().family == ""){
            
            hitFnt.loadFromFile("/Users/darionmccoy/Dropbox/Game Design Stuff/Projects/Backup/HardSoda/HardSoda/fonts/apfont.ttf");
            const_cast<sf::Texture&>(hitFnt.getTexture(16)).setSmooth(false);
            
        }
        
        num.setFont(hitFnt);
        num.setCharacterSize(16);
        num.setColor(sf::Color::White);
        
        numbg.setFillColor(sf::Color::Black);
        velZ = 99;
        
        
        
    }
    
    
    // Update Functions
    
    
    void Object::Update()
    {
        
        
        
    }
    

    void Fixed::Update(){
        
        
        if(top == -1)top = objectSprite.getTextureRect().top;
        
        if(World::GetInstance()->Timer(*this,animSpeed))
        {
            
                objectSprite.setTextureRect(sf::IntRect(0,(frame * assetHeight) + top,objectSprite.getTextureRect().width,objectSprite.getTextureRect().height));
            
            
            if(frame < maxFrame) {
                
                frame++; 
                animSpeed -= spdReduceRate;
                vel.x -= (vel.x * deacceleration);
                vel.y -= (vel.y * deacceleration);
                if(rotation) RotateVector(vel,animSpeed);
            
            }
            
            else misDestroyed = true;
            
        }
        
        objectSprite.move(vel.x,vel.y);

    }
    
    void Hit::Update(){
        
        if(objectSprite.getGlobalBounds().intersects(World::GetInstance()->WorldScene.playerPtr->objectSprite.getGlobalBounds())){
            
        World::GetInstance()->WorldScene.playerPtr->playerHealth -= 15;
        World::GetInstance()->WorldScene.hudPtr->hdamaged = true;
        
        }
        
        sf::Vector2f newOrigin(objectSprite.getPosition().x-15,objectSprite.getPosition().y-15);
        
        World::GetInstance()->WorldScene.levelContainer->CheckCollison(newOrigin,newOrigin, *this);
    
        misDestroyed = true;
        
        
    }
    
    void Particle::Update()
    {
        sf::Vector2f prevPos = objectSprite.getPosition();
        
        if(World::GetInstance()->Timer(*this,5.0f))
        {
            
            if(isAnimated){
                
                if(frame == 3)
                {
                    
                    frame = 0;
                    
                }
                
                objectSprite.setTextureRect(sf::IntRect(frame * objectSprite.getTextureRect().width,
                                                                objectSprite.getTextureRect().top,
                                                                objectSprite.getTextureRect().width,
                                                                objectSprite.getTextureRect().height));
                
                frame++;
                    
                }
            
            vel.y++;
            
        }
            
    
        if (gravity <= 0.1)
        {
            
            misDestroyed = true;
            
        }

        objectSprite.move(vel.x * (0.3f),vel.y * (gravity) * gravity);
        World::GetInstance()->WorldScene.levelContainer->CheckCollison(prevPos, objectSprite.getPosition(), *this);
        
    
    }
       
    void Projectile::Update()
    {
        
        sf::Vector2f prevPos = objectSprite.getPosition();
        
        if(World::GetInstance()->Timer(*this,30.0f))
        {
            
            objectSprite.setTextureRect(sf::IntRect(frame * objectSprite.getTextureRect().width,
                                                            0,
                                                            objectSprite.getTextureRect().width,
                                                            objectSprite.getTextureRect().height));
            frame++;
            
            if(frame == 3)
            {
                
                frame = 0;
                
            }
            
        }
        
       
        if(vel.x < 0)
        {
            
            objectSprite.setTextureRect(sf::IntRect(objectSprite.getTextureRect().width,
                                                    0,
                                                    -objectSprite.getTextureRect().width,
                                                    objectSprite.getTextureRect().height));
            
        }
        
        objectSprite.move(vel.x,0);
        
        
        if(objectSprite.getPosition().x >= World::GetInstance()->WorldScene.levelContainer->lvlSize.x || objectSprite.getPosition().x <= 0)
        {
            
            isCollided = true;
            
        }
        
        else{
            
            World::GetInstance()->WorldScene.levelContainer->CheckCollison(prevPos, objectSprite.getPosition(), *this);
            
        }

        
        
    }
    
    void EnemyBlip::Update(){
        
        if(World::GetInstance()->Timer(*this,10.0f))
        {
            
            
            objectSprite.setTextureRect(sf::IntRect((frame * 7),19,7,7));
            
            frame++;
            
            if(frame >= 2)
            {
                
                frame = 0;
                
            }
            
        }
        
        
        objectSprite.move(vel.x,vel.y);
        
    }
    
    void PlayerLaser::Update(){
        
        
        if(World::GetInstance()->Timer(*this,10.0f))
        {
            
            
            objectSprite.setTextureRect(sf::IntRect((10+(frame) * 5),0,5,19));
            objectSprite.setOrigin(2,0);
            
            frame++;
            
            if(frame >= 2)
            {
                
                frame = 0;
                
            }
            
        }
        
        
        if(World::GetInstance()->Timer(*this,700.0f))
        {
            misDestroyed = true;
            
        }
        

        
        objectSprite.move(vel.x,vel.y);


    }
    
    void PlayerLaser::isCollided(int var){
        
        misDestroyed = true;
        
        itemQueue particles;
        particles.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
        particles.properties["PosY"] = std::to_string(objectSprite.getPosition().y);
        particles.properties["itemType"] = "Spark";
        World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
        World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
        World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
        
    }
    
       
    
    void Explosion::Update()
    {
        
        if(World::GetInstance()->Timer(*this,0.0001f))
        {
            
            objectSprite.setTextureRect(sf::IntRect(anim_frameX * 80,anim_frameY * 80, 80, 80));
            anim_frameY++;
            
            if(anim_frameY >= 3)
            {
                anim_frameX++;
                anim_frameY = 0;
            }
            
            
        }
        
        if(anim_frameX >= 5)
        {
            
            misDestroyed = true;
            
        }
        
        
    }
    
    
    void Cluster::Update()
    {
        
        if(World::GetInstance()->Timer(*this,10.0f) && maxBomb > 0)
        {

            itemQueue bomb;
            bomb.properties["PosX"] = std::to_string(objectSprite.getPosition().x + (rand() % 32 + -16));
            bomb.properties["PosY"] = std::to_string(objectSprite.getPosition().y + (rand() % 32 + -16));
            bomb.properties["itemType"] = "Explosion";
            World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bomb);
            
            maxBomb--;
            
            if(maxBomb <= 0)
            {
                
                misDestroyed = true;
                
            }
            
            
        }
        
        
        
    }
    
    void Prop::Update(){
        
        objectSprite.setTextureRect(sf::IntRect(frame * objectSprite.getTextureRect().width,objectSprite.getTextureRect().top,objectSprite.getTextureRect().width,objectSprite.getTextureRect().height));
        
        if(maxFrame != 0){
        
            if(World::GetInstance()->Timer(*this,animSpd,NODELAY)) frame = (frame == maxFrame) ? 0 : frame+1;
        
        }
    }
    
    void Laser::Update()
    {
        
        //float laserStartpnt = *playerPosX + 20 + (3 * length);
        
        // Since Scaling throws off origin, move laser by including sprite size and multiplying length x size of sprite "(6 * length)"
        
        
        if(World::GetInstance()->Timer(*this,30.f))
        {
            
            if(frame == 3) frame = 0;
           
            objectSprite.setTextureRect(sf::IntRect(frame * 6, 0, 6, 6));
            frame++;
            
        }
        
        if((*playerPosX + 20 + (3 * length)) + (3 * length) <= 399)
        {
            
            length = length * 1.3;
            float laserStartpnt = *playerPosX + 20 + (3 * length);
            objectSprite.setPosition(laserStartpnt,*playerPosY + 14);
            objectSprite.setScale((length),1);
            
        }
        
        
        
        if((*playerPosX + 20 + (3 * length)) + (3 * length) >= 400)
        {
        
            
            while((*playerPosX + 20 + (3 * length)) + (3 * length) > 400)
            {
                
                length = round(length);
                length--;
                
            }
            
            /// Creates explosion at end point
            
            if(height == 10)
            {
                
                World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_big_exp");
                    

                
                for(int i = 0; i < 30; i++)
                {
                    
                    itemQueue particles;
                    particles.properties["PosX"] = std::to_string(*playerPosX + 20 + (3 * length) + (3 * length));
                    particles.properties["PosY"] = std::to_string(objectSprite.getPosition().y);
                    particles.properties["itemType"] = "Spark";
                    World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
                    
                }
                
                itemQueue bomb;
                bomb.properties["PosX"] = std::to_string(*playerPosX + 20 + (3 * length) + (3 * length));
                bomb.properties["PosY"] = std::to_string(objectSprite.getPosition().y);
                bomb.properties["itemType"] = "Cluster";
                World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bomb);
                std::this_thread::sleep_for(std::chrono::milliseconds(80));
                World::GetInstance()->ScreenShake(20);
                
            }
            
            height = height / 1.1;
            objectSprite.setScale((length),height);
            float laserStartpnt = *playerPosX + 20 + (3 * length);
            objectSprite.setPosition(laserStartpnt,*playerPosY + 14);
            
            if(height <= 0.1)
            {
                
                misDestroyed = true;
                
            }
            
        }
        
    }
    
    // Misc functions
    
    int Projectile::GetDamage(){
        
        return damage;
        
    }
    
    void HitNum::Update(){
        
        if(World::GetInstance()->Timer(*this,60)){num.move(0,-1); numbg.setPosition(num.getPosition().x-2,num.getPosition().y+7);}
        
        if(World::GetInstance()->Timer(*this,1000)) misDestroyed = true;
        
    }
    
    // Draw functions
    
    
    void Object::Draw(sf::RenderTarget& window)
    {
        World::GetInstance()->windowTexture.draw(objectSprite);
        //World::GetInstance()->windowTexture.draw(objectHitBox);
    }
    
    void Object::DrawShadow(sf::RenderTarget& window)
    {
        
        World::GetInstance()->windowTexture.draw(objectShadow);
        
    }
    
    void HitNum::Draw(sf::RenderTarget& window)
    {
        
        World::GetInstance()->windowTexture.draw(numbg);
        World::GetInstance()->windowTexture.draw(num);
        
        
    }
    
    // Desctructors
    
    Object::~Object()
    {
        
        
    }
    
    Projectile::~Projectile()
    {
        
        
    }
    
    EnemyProjectile::~EnemyProjectile()
    {
        
    }
    
    EnemyBlip::~EnemyBlip(){
        
    }
    
    
    HitNum::~HitNum()
    {
        
        
        
        
    }
    
    DeathPoof::~DeathPoof(){
        
    }
    
    DeathBoom::~DeathBoom(){
        
    }
    
    Hit::~Hit(){
       
        
    }
    
    Prop::~Prop(){
        
    }
    
    PropFlame::~PropFlame(){
        
    }
    
    Particle::~Particle()
    {
        
        
        
    }
    
    Fixed::~Fixed(){
        
        
    }
    
    DamageSpark::~DamageSpark(){
        
        
    }
    
    Passive::~Passive(){
        
        
        
    }
    
    Dirt::~Dirt(){
        
    }
    
    Gib::~Gib(){
        
    }
    
    Explosion::~Explosion()
    {
        
        
        
    }
    
    Bullet::~Bullet()
    {
        
        for(int i = 0; i < 6; i++)
        {
            
            itemQueue particles;
            particles.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
            particles.properties["PosY"] = std::to_string(objectSprite.getPosition().y);
            particles.properties["itemType"] = "Spark";
            World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
            
        }
        
        itemQueue bomb;
        bomb.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
        bomb.properties["PosY"] = std::to_string(objectSprite.getPosition().y);
        bomb.properties["itemType"] = "Cluster";
        World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bomb);
        
        World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_bullet_exp");

        
        
    }
    
    Cluster::~Cluster()
    {
        
        
        
    }
    
    Laser::~Laser()
    {
        
        playerPosX = NULL;
        playerPosY = NULL;
        delete playerPosX;
        delete playerPosY;
        
    }
    
    PlayerLaser::~PlayerLaser()
    {
        itemQueue particles;
        particles.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
        particles.properties["PosY"] = std::to_string(objectSprite.getPosition().y);
        particles.properties["itemType"] = "DamageSpark";
        World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
        
    }
    
    void isDestroyed(Entity::Enemy& object)
    {
        if(object.health <= 0){
            
            itemQueue bomb;
            bomb.properties["PosX"] = std::to_string(object.objectSprite.getPosition().x + 5);
            bomb.properties["PosY"] = std::to_string(object.objectSprite.getPosition().y + 5);
            bomb.properties["itemType"] = "Explosion";
            World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bomb);
            
            object.misDestroyed = true;
            
        }
        
    }
    
    void isDestroyed(Entity::Bullet& bullet){
        
        if(bullet.isCollided == true){
            
            for(int i = 0; i < 4; i++)
            {
                
                itemQueue particles;
                particles.properties["PosX"] = std::to_string(bullet.objectSprite.getPosition().x);
                particles.properties["PosY"] = std::to_string(bullet.objectSprite.getPosition().y);
                particles.properties["itemType"] = "Spark";
                World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
                
            }
            
            itemQueue bomb;
            bomb.properties["PosX"] = std::to_string(bullet.objectSprite.getPosition().x);
            bomb.properties["PosY"] = std::to_string(bullet.objectSprite.getPosition().y);
            bomb.properties["itemType"] = "Cluster";
            World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bomb);
            World::GetInstance()->ScreenShake(10);
            World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_bullet_exp");
            bullet.misDestroyed = true;
            
        }
        
    }
    
    bool Object::isDestroyed(){
        
        if((objectSprite.getPosition().x < 0 || objectSprite.getPosition().x > World::GetInstance()->WorldScene.levelContainer->lvlSize.x || objectSprite.getPosition().y < 0 || objectSprite.getPosition().y > World::GetInstance()->WorldScene.levelContainer->lvlSize.y) && type != "GUI") misDestroyed = true;
        
        return misDestroyed;
        
    }
    
    /*
     ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––
     Actor definitions – includes enemies
     ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––
     */
    
    
    // Function for collision detection

    
    // Constructors
    
    int Enemy::totalEnemies = 0;
    std::array<std::string,25> LevelManager::enemyList;
    
    LevelManager::LevelManager() : Object(){
        
        objectSprite.setTextureRect(sf::IntRect(0,0,0,0));
    
        bg.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_battle_bg_1.png"));
        bg.setTextureRect(sf::IntRect (0,0,2000,2000));
        bg.setOrigin(2000/2,2000/2);
        
        if(enemyList[0] == ""){
            
            enemyList[0] = "Star";
            enemyList[1] = "Squid";
            enemyList[2] = "Slime";
            enemyList[3] = "Star2";
            enemyList[4] = "Squid2";
            enemyList[5] = "Slime2";
            enemyList[6] = "Star3";
            enemyList[7] = "Squid3";
            enemyList[8] = "Slime3";
            enemyList[9] = "Mozza";
            enemyList[10] = "Mozza";
            enemyList[11] = "Mozza";
            enemyList[12] = "Mozza";
            enemyList[13] = "Mozza";
            enemyList[14] = "Mozza";
            enemyList[15] = "Mozza";
            enemyList[16] = "Mozza";
            enemyList[17] = "Mozza";
            enemyList[18] = "Mozza";
            
            //bosses start at 19
            
            enemyList[19] = "Mozza";
            enemyList[20] = "Hauzer";
            enemyList[21] = "Lim";
            enemyList[22] = "Boss4";
            enemyList[23] = "Boss5";
            enemyList[24] = "Boss6";
            enemyList[25] = "Boss7";
            
        }
        
        type = "BG";

    }
    
    void LevelManager::Update(){
        
        if(maxEnemies > 0){
        
            if(World::GetInstance()->Timer(*this,1250.0f,NODELAY)){
                
                int lvlwidth = fieldSize;
                int lvlheight = fieldSize;;
                
                std::random_device rd;
                std::mt19937 mt(rd());
                std::uniform_int_distribution<int> randomLeftwidth(0,lvlwidth);
                std::uniform_int_distribution<int> randomTopheight(0,lvlheight);
                std::uniform_int_distribution<int> randomEnemy(0,5);
                
                int enemyr = RandomNumber(5);
                
                for(int i = 0; i != 2; i++){
                    
                    
                    Entity::itemQueue enemy;
                    
                    if(enemyr > 2) enemyr = 2;
                    enemy.properties["itemType"] = enemyList[3 * World::GetInstance()->GlobalMembers.currentLevel + enemyr];
                    std::cout << "calling(" << 3 * World::GetInstance()->GlobalMembers.currentLevel + enemyr << "): " << enemyList[3 * World::GetInstance()->GlobalMembers.currentLevel + enemyr] << std::endl;
                    enemy.properties["PosX"] = std::to_string(randomLeftwidth(mt));
                    enemy.properties["PosY"] = std::to_string(randomTopheight(mt));
                    std::cout << "enemys left: " << maxEnemies << std:: endl;
                    World::GetInstance()->WorldScene.objectContainer->Queue.push_back(enemy);
                    maxEnemies--;
                }
                
            }
            
        }
        
        else if( int(bg.getColor().a) != 0 ){
            
            if(World::GetInstance()->Timer(*this,50.0f,NODELAY)){
                
                if(int(bg.getColor().a)-2 < 0) bg.setColor(sf::Color(255,255,255,0));     
                else bg.setColor(sf::Color(255,255,255,int(bg.getColor().a)-2));
                World::GetInstance()->WorldScene.audioContainer.music.setVolume(World::GetInstance()->WorldScene.audioContainer.music.getVolume()-0.5);
                std::cout <<  World::GetInstance()->WorldScene.audioContainer.music.getVolume() << std::endl;   
                
            }
            
        }
        
        else{
            
            misDestroyed = true;
            Entity::itemQueue boss;
            boss.properties["PosX"] = std::to_string(600/2);
            boss.properties["PosY"] = std::to_string(600/2);
            boss.properties["itemType"] = enemyList[19 + World::GetInstance()->GlobalMembers.currentLevel];;
            std::cout << "enemys left: " << maxEnemies << std:: endl;
            World::GetInstance()->WorldScene.objectContainer->Queue.push_back(boss);
            World::GetInstance()->WorldScene.audioContainer.PlayMusic("boss");
                
        }
        
        bg.rotate(0.05);

    }
    
    void LevelManager::Draw(sf::RenderTarget &window){
        
        World::GetInstance()->windowTexture.draw(bg,&World::GetInstance()->WorldScene.textureContainer.waveShader.shader);
    }
    
    LevelManager::~LevelManager(){
        
    }
    
    Actor::Actor(int actor) : character(actor), Object()
    {
        
        objectSprite.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_actors.png"));
        objectSprite.setTextureRect(sf::IntRect (0,actor * 191,16,24));
        name = Textbox::characters[character];
        SetCharacterOrigin();
        SetShadow();
        type = "Actor";
        SetHitBox(sf::Vector2f(40,40),0);
        
    }
    
    void Actor::Update(){
        
        Move();
        UpdateShadow();
        objectSprite.setScale(Sine(),Sine());
    }
    
    void Actor::Move()
    {
        if(PlayerDistance(CLOSE)){
            actorMovement = RoundUp((GetAngle(objectShadow.getPosition(),World::GetInstance()->WorldScene.playerPtr->objectShadow.getPosition())),45);
        objectSprite.setTextureRect(sf::IntRect(framePos * 16, abs(((180 - actorMovement) / 45) * 24),16, 24));
        }
        
        else if(idleBehavior == none) objectSprite.setTextureRect(sf::IntRect(framePos * 16, south,16, 24));
        
        else if(idleBehavior == looking){
            
            if(World::GetInstance()->Timer(*this,5000.0,DELAY)) objectSprite.setTextureRect(sf::IntRect(framePos * 16, RandomNumber(7)*24,16, 24));
        
            }
        
    }
    
    void Actor::isCollided(int var){
        
        if(World::GetInstance()->Timer(*this,0,NODELAY)){
            
            itemQueue textbox;
            textbox.properties["itemType"] = "Textbox";
            textbox.properties["ActorName"] = std::to_string(character);
            World::GetInstance()->WorldScene.objectContainer->Queue.push_back(textbox);
            
        }
            
    }
    
    Enemy::Enemy() : Object()
    {
        enemyID = totalEnemies;
        ++totalEnemies;
        objectSprite.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_enemies.png"));
        type = "Enemy";
        scaleTemp = sf::Vector2f(0.12,0);
        objectSprite.setColor(sf::Color(255,255,255,0));
        hurtPos.x = 0;
        hurtPos.y = 0;
        SetHitBox(sf::Vector2f(16,16),1);
    }
    
    EnemyNode::EnemyNode() : Enemy()
    {
        objectSprite.setTextureRect(sf::IntRect (0,0,16,16));
        SetShadow();
        moveType = NODE;
        type = "EnemyNode";
    }
    
    Slime::Slime() : Enemy()
    {
        objectSprite.setTextureRect(sf::IntRect (0,16,24,21));
        SetCharacterOrigin();
        SetShadow();
        moveType = JUMPER;
        health = 20;
    }
    
    Star::Star() : Enemy()
    {
        objectSprite.setTextureRect(sf::IntRect (0,58,28,26));
        SetCharacterOrigin();
        SetShadow();
        moveType = NORMAL;
        speed = 2;
        health = 10;
        vib = 5;
        
    }
    
    Squid::Squid() : Enemy()
    {
        objectSprite.setTextureRect(sf::IntRect (0,84,33,27));
        SetCharacterOrigin();
        SetShadow();
        moveType = NORMAL;
        speed = 0.5;
        health = 20;
        hasAttack = true;
        
        SetHitBox(sf::Vector2f(20,20),1);
    
    }
    
    Boss::Boss()
    {
        std::cout << "Boss created" << std::endl;
        healthBar.setSize(sf::Vector2f(0,8));
        healthBar.setFillColor(sf::Color::Yellow);
        bossName.setFont(World::GetInstance()->WorldScene.textureContainer.GetFont());
        bossName.setCharacterSize(16);
        
    }
    
    Mozza::Mozza(){
        
        objectSprite.setTextureRect(sf::IntRect (0,111,116,88));
        speed = 0.5;
        maxhealth = 1250;
        health = maxhealth;
        moveType = NORMAL;
        SetCharacterOrigin();
        SetHitBox(sf::Vector2f(50,50),1);
        SetShadow();
        hasAttack = true;
        bossName.setString("Mozza");
        targetPlayer = false;
    }
    
    
    // Update Functions
    
    void Enemy::Move(){
        
        if(moveType == NORMAL){
            
            
            vel.x = 0;
            vel.y = -speed;
            
            //set move to player
            RotateVector(vel,-(GetAngle(objectSprite.getPosition(),World::GetInstance()->WorldScene.playerPtr->objectSprite.getPosition())));
            
            if(!hasAttack)objectSprite.move(vel.x,vel.y + velZ);
            
            else if(!PlayerDistance(MID)) objectSprite.move(vel.x,vel.y + velZ);
            
        }
        
        if(moveType == JUMPER){
            
            vel.x = 0;
            vel.y = -speed;
            RotateVector(vel,-(GetAngle(objectSprite.getPosition(),World::GetInstance()->WorldScene.playerPtr->objectSprite.getPosition())));
            
            
            if(World::GetInstance()->Timer(*this,900,NODELAY)) velZ = -1;
            
            if(velZ != 0) objectSprite.move(vel.x,vel.y + velZ);
            posZ -= velZ;
            
            if(objectSprite.getPosition().y < objectShadow.getPosition().y){
                
                velZ += 0.2;
                
            }
            
            else{
                
                velZ = 0;
                
            }
            
        }
        
        //remove this below to activate angle rotation (this makes the enemy always look to player)
        
        if(targetPlayer) fireDir = GetAngle(objectSprite.getPosition(),World::GetInstance()->WorldScene.playerPtr->objectSprite.getPosition());
        
    }
    
    
    void Enemy::Act(){
        
        
        
        if(active){
            
            if(hurt) objectSprite.move(-hurtPos);
            Move();
            if(hasAttack) Attack();
            UpdateShadow();
            objectSprite.setScale(Sine(),Sine());
            if( moveType != NODE && health <= 0) misDestroyed = true;
            
        }
        
        else{
            
            if(World::GetInstance()->Timer(*this,30.0f,NODELAY)){
                
                if( int(objectSprite.getColor().a) < 255 ) objectSprite.setColor(sf::Color(255,255,255,int(objectSprite.getColor().a)+5));
                else active = true;
                
            }
            
            
        }
        
        if(hurt){
            
            if(World::GetInstance()->Timer(*this,400.0f,NODELAY)){ 
                hurt = false;
                hurtPos.y = 0;
                hurtPos.x = 0;
            }
            
            else{
            hurtPos.x = RandomNumber(2);
            hurtPos.y = RandomNumber(2);
            objectSprite.move(hurtPos);
            }
            
        }

    }
    
    
    void Enemy::Update()
                 
    {
        
        if(!World::GetInstance()->WorldScene.playerPtr->dead)Act();
        
    }
    
    void Enemy::Attack(){
        
        if(PlayerDistance(MID)){
            
            if(World::GetInstance()->Timer(*this,1000)){
                
                itemQueue bullet;
                bullet.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
                bullet.properties["PosY"] = std::to_string(objectSprite.getPosition().y - 6);
                bullet.properties["itemType"] = "EnemyBlip";
                bullet.properties["Direction"] = std::to_string(fireDir);
                World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);
                
                bullet.properties["Direction"] = std::to_string(fireDir -10);
                World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);
                
                bullet.properties["Direction"] = std::to_string(fireDir +10);
                World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);
                    
            }
            
        }
        
    }
    
    void Enemy::Draw(sf::RenderTarget& window){
        
        World::GetInstance()->WorldScene.textureContainer.dmgShader.shader.setParameter("damage",health);
        World::GetInstance()->windowTexture.draw(objectSprite);
    }
    
    void Boss::Update(){
        
        Act();
        healthBar.setPosition(World::GetInstance()->viewPos.x-((WINDOW_X/3)/2),World::GetInstance()->viewPos.y+((WINDOW_Y/3)/2-8));
        bossName.setPosition(healthBar.getPosition().x,healthBar.getPosition().y-22);
        currhealth += -(healthBar.getSize().x - (float(WINDOW_X)/3)/(maxhealth/health)) / 20;
        healthBar.setSize(sf::Vector2f(currhealth,8));
        
    }
    
    void Mozza::Attack(){
        
        if(World::GetInstance()->Timer(*this,100)){
            
            itemQueue bullet;
            bullet.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
            bullet.properties["PosY"] = std::to_string(objectSprite.getPosition().y - 6);
            bullet.properties["itemType"] = "EnemyBlip";
            bullet.properties["Speed"] = std::to_string(3);
            bullet.properties["Direction"] = std::to_string(fireDir);
            World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);
            
            bullet.properties["Direction"] = std::to_string(fireDir+45);
            World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);
            
            bullet.properties["Direction"] = std::to_string(fireDir+90);
            World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);
            
            
            bullet.properties["Direction"] = std::to_string(fireDir+135);
            World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);
            
            
            bullet.properties["Direction"] = std::to_string(fireDir+180);
            World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);
            
            
            bullet.properties["Direction"] = std::to_string(fireDir+225);
            World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);
            
            
            bullet.properties["Direction"] = std::to_string(fireDir+270);
            World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);
            
            
            bullet.properties["Direction"] = std::to_string(fireDir+315);
            World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);
            
            //World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_particle_hit");
            fireDir += 5;
        }

        
    }
    
    void Boss::Draw(sf::RenderTarget& window){
        
        World::GetInstance()->windowTexture.draw(objectSprite);
        World::GetInstance()->windowTexture.draw(objectHitBox);
        World::GetInstance()->windowTexture.draw(healthBar);
        World::GetInstance()->windowTexture.draw(bossName);
        
        
        
    }

    
    // Destructors
    
    Enemy::~Enemy()
    {
        --totalEnemies;
        
        std::cout << "Enemy destroyed" << std::endl;
        
        itemQueue particles;
        particles.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
        particles.properties["PosY"] = std::to_string(objectSprite.getPosition().y);
        particles.properties["itemType"] = "DeathPoof";
        

        World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
        World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
        World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
        
        particles.properties["itemType"] = "DeathBoom";
        World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
        
    }
    
    Boss::~Boss(){
        
    }
    
    Mozza::~Mozza(){
        
    }
    
    EnemyNode::~EnemyNode(){
        
    }
    
    Actor::~Actor(){
        
        
    }
    
    Slime::~Slime(){
        
    }
    
    Star::~Star(){
        
        
    }
    
    Squid::~Squid(){
        
    }
    
    ////// MISC FUNCTIONS
    
    
    void Object::GetZone(int vecPos){
        
        zone.clear();
        
        int zoneSize = 32;
        
        sf::RectangleShape tempshape (sf::Vector2f (zoneSize,zoneSize));
        
        tempshape.setFillColor(sf::Color::Color(255,255,255,30));
        
        sf::IntRect bounds = GetObjectBounds();
        
        int leftTop = (RoundDown(bounds.left,zoneSize) + RoundDown(bounds.top,zoneSize) + (RoundDown(bounds.top,zoneSize)*2))/100 ;
        int rightTop = (RoundDown(bounds.width,zoneSize) + RoundDown(bounds.top,zoneSize) + (RoundDown(bounds.top,zoneSize)*2))/100 ;
        int leftBttm = (RoundDown(bounds.left,zoneSize) + RoundDown(bounds.height,zoneSize) + (RoundDown(bounds.height,zoneSize)*2))/100 ;
        int rightBttm = (RoundDown(bounds.width,zoneSize) + RoundDown(bounds.height,zoneSize) + (RoundDown(bounds.height,zoneSize)*2))/100 ;
        
        //std::cout << leftTop << std::endl;
        
        std::vector<int> vec = {leftTop,rightTop,leftBttm,rightBttm};
        std::sort(vec.begin(),vec.end());
        vec.erase( unique( vec.begin(), vec.end() ), vec.end() );

        for(int i : vec){
            
            World::GetInstance()->WorldScene.objectContainer->zoneMatrix.insert(std::make_pair(i,vecPos));
            zone.append(std::to_string(i) + ",");
            
        }
        
        //std::cout << "zone list: " << zone << std::endl;
               
    }
    
    
    // This function is called to set the origin to the bottom and center of the sprite for accurate draw order
    
    void Object::CheckLvlCollision(){
        
    }
    
    void Player::CheckLvlCollision(){
        
        sf::Vector2f currPos = objectShadow.getPosition();
        sf::Vector2f prevPos = objectShadow.getPosition() - vel;
        prevPos.x = round(prevPos.x);
        prevPos.y = round(prevPos.y);
        
        World::GetInstance()->WorldScene.levelContainer->GetSurroundingTiles(prevPos,currPos,objectShadow.getTextureRect());
        
        int spriteWidth = abs(objectShadow.getTextureRect().width);
        int spriteHeight = objectShadow.getTextureRect().height;
        int tileWidth = 16;
        
        if(World::GetInstance()->WorldScene.levelContainer->collisionArray.size() > 0){
            
            for(Level::collArrayIter it = World::GetInstance()->WorldScene.levelContainer->collisionArray.begin(); it != World::GetInstance()->WorldScene.levelContainer->collisionArray.end(); it++){
                
                if(IsOverlapping(currPos.x - spriteWidth/2, currPos.x + spriteWidth/2, it->x,it->x + tileWidth) && IsOverlapping(currPos.y - spriteHeight/2, currPos.y + spriteHeight/2, it->y,it->y + tileWidth) ){
                    
                    // Check previous interesection of X axis only, if there is no interesct, we know the X axis needs to be cleared by this block on this axis
                    
                    if(!IsOverlapping(prevPos.x - spriteWidth/2,prevPos.x + spriteWidth/2, it->x, it->x + tileWidth)){
                        
                        // If collision on left ...
                        
                        if(prevPos.x - spriteWidth/2 > currPos.x - spriteWidth/2){
                            
                            currPos.x = it->x + tileWidth + spriteWidth/2;
                            
                        }
                        
                        // If collision on right ...
                        
                        else if(prevPos.x - spriteWidth/2 < currPos.x - spriteWidth/2){
                            
                            currPos.x = it->x - spriteWidth/2;
                            
                        }
                        
                        objectSprite.setPosition(currPos.x,currPos.y);
                        objectShadow.setPosition(currPos.x,currPos.y);
                        
                    }
                    
                    // Check previous interesection of Y axis only, if there is no interesct, we know the Y axis needs to be cleared by this block on this axis
                    
                    if(!IsOverlapping(prevPos.y - spriteHeight/2,prevPos.y + spriteHeight/2, it->y, it->y + tileWidth)){
                        
                        // If collision on top ...
                        
                        if(prevPos.y - spriteHeight/2 > currPos.y - spriteHeight/2){
                            
                            currPos.y = it->y + tileWidth + spriteHeight/2;
                            
                        }
                        
                        // If collision on bottom ...
                        
                        else if(prevPos.y - spriteHeight/2 < currPos.y - spriteHeight/2){
                            
                            currPos.y = it->y - spriteHeight/2;
                            
                        }
                        
                        objectSprite.setPosition(currPos.x,currPos.y);
                        objectShadow.setPosition(currPos.x,currPos.y);
                        
                    }
                    
                    frame_pos = 0;
                    vel.x = 0;
                    vel.y = 0;
                    objectSprite.setTextureRect(sf::IntRect(0,objectSprite.getTextureRect().top,objectSprite.getTextureRect().width,objectSprite.getTextureRect().height));
                    
                }
                
                
            }
            
        }
    

    }
    
    void BattlePlayer::CheckLvlCollision(){
        
        
    }
    
    void Object::SetCharacterOrigin(){
        
        objectSprite.setOrigin(objectSprite.getTextureRect().width/2,objectSprite.getTextureRect().height);
        
    }
    
    void Object::SetEffectOrigin(){
        
        objectSprite.setOrigin(objectSprite.getTextureRect().width/2,objectSprite.getTextureRect().height/2);
        
    }
    
    void Object::SetShadow()
    {
        objectShadow.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_misc.png")));
        objectShadow.setTextureRect(sf::IntRect(0, 0, 15, 5));
        objectShadow.setOrigin(7,2);
        objectShadow.setPosition(objectSprite.getPosition());
        
    }
    
    void Object::UpdateShadow(){
        
        objectShadow.setPosition(objectSprite.getPosition().x,objectSprite.getPosition().y + posZ);
    }
    
    void Enemy::isDamaged(int damage){
        
        if(active){
            
            if(moveType == NORMAL) vel.y = 0;
            //objectSprite.setScale(1.2,1.2);
            
            int newDamage = GetRandDmg(damage);
            health -= newDamage;
            hurt = true;
        }
        
    }
    
    void Enemy::isCollided(int var) {
        
        isDamaged(var);
    }
    
    void Object::SetHitBox(sf::Vector2f size, int originPos){
        
        objectHitBox.setSize(size);
        objectHitBox.setFillColor(sf::Color::Yellow);
        
        // originpos specifys the origin of the hitbox to the center(0 = default), the bottom center (1)
        
        if(originPos == center) objectHitBox.setOrigin(size.x/2,size.y/2);
        else if (originPos == bottomCenter) objectHitBox.setOrigin(size.x/2,size.y);
        else if (originPos == topLeft) objectHitBox.setOrigin(0,0);
        
    }
    
    void Object::UpdateHitBox(){
        
        objectHitBox.setPosition(objectSprite.getPosition());
    }
    
    void Object::isCollided(int var){
        
        objectSprite.setColor(sf::Color::Red);
        
    }
    
    int GetRandDmg(int damage){
        
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<int> random(damage-2,damage+1);
        return random(mt);
        
    }
    
    bool enemyPred (const std::unique_ptr<Entity::Object>& a)
    {
        int aType = (a->type == "Enemy") ? 1 : 0;
        return aType;
    }
    
    bool enemyProjectilePred (const std::unique_ptr<Entity::Object>& a)
    {
        int aType = (a->type == "EnemyProjectile") ? 1 : 0;
        return aType;
    }
    
    bool projectilePred (const std::unique_ptr<Entity::Object>& a)
    {
        int aType = (a->type == "Projectile") ? 1 : 0;
        return aType;
    }
    
    bool propPred (const std::unique_ptr<Entity::Object>& a)
    {
        int aType = (a->type == "Prop") || (a->type == "Actor")  ? 1 : 0;
        return aType;
    }
    
    
    bool Object::PlayerDistance(int distance){
        
        
        return sqrt((objectShadow.getPosition().x - World::GetInstance()->WorldScene.playerPtr->objectShadow.getPosition().x)*(objectShadow.getPosition().x - World::GetInstance()->WorldScene.playerPtr->objectShadow.getPosition().x) + (objectShadow.getPosition().y - World::GetInstance()->WorldScene.playerPtr->objectShadow.getPosition().y)*(objectShadow.getPosition().y - World::GetInstance()->WorldScene.playerPtr->objectShadow.getPosition().y)) <= distance ? true : false;
        
        
    }
    
    
    
}


