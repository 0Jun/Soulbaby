#include <SFML/Graphics.hpp>
#include <dirent.h>
#include <iostream>
#include <array>
#include <vector>
#include <random>

#define WINDOW_X 480*3
#define WINDOW_Y 270*3

#include "Container.h"
#include "Audio.h"

// Here is a small helper for you ! Have a look.
#include "ResourcePath.hpp"

World* World::Worldptr = nullptr;

int main()
{
    
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(WINDOW_X,WINDOW_Y), "SFML window", sf::Style::Titlebar);
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);
    window.setKeyRepeatEnabled(true);
        
    sf::Clock clock;
    float timestamp = 0;

    World::GetInstance()->Setup(clock,window);

    // Start the game loop
    while (window.isOpen())
    {
        
        timestamp += clock.getElapsedTime().asMilliseconds();
        clock.restart();
        
        // Process events
        sf::Event event;
        
        while (window.pollEvent(event))
        {
            
            // Close window : exit
            if (event.type == sf::Event::Closed)
            {
                
                window.close();
                
            }
            
            // Espace pressed : exit
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
                
                window.close();
                
            }
            
        }
        
        window.clear(sf::Color::Color(0,0,0));
        World::GetInstance()->windowTexture.clear();
        World::GetInstance()->Run(event,timestamp,clock);
        //window.draw(World::GetInstance()->windowTexture);
        window.setView(*World::GetInstance()->GetView());
        World::GetInstance()->windowTexture.display();
        World::GetInstance()->windowTextureSprite.setTexture(World::GetInstance()->windowTexture.getTexture());
        if(World::GetInstance()->WorldScene.playerPtr->dead) window.draw(World::GetInstance()->windowTextureSprite,&World::GetInstance()->WorldScene.textureContainer.redShader.shader);
        else window.draw(World::GetInstance()->windowTextureSprite);
        
        window.display();
        timestamp = clock.restart().asMilliseconds();
        
    }
    
    return EXIT_SUCCESS;
}
