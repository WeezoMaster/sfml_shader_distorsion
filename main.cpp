#include <SFML/Graphics.hpp>
#include <iostream>
#include <libgen.h>


int main(int argc, char* argv[])
{
	std::string DATA_PATH = dirname(argv[0]);

    sf::RenderWindow window(sf::VideoMode(1280, 720), "SHADER EX");
    sf::View ViewBG(sf::FloatRect(0, 0, 1280, 720));
    sf::View ViewFinal(sf::FloatRect(0, 0, 1280, 720));

    /**
    **  PLAYER PART
    **/
    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile(DATA_PATH + "/player.png"))
    {
        sf::err() << "Failed to load object texture, exiting..." << std::endl;
        return -1;
    }

    sf::Sprite player;
    player.setTexture(playerTexture);
    player.setScale(2.f, 2.f);

    /**
    **  BACKGROUND PART
    **/
    sf::Texture bgTexture;
    if (!bgTexture.loadFromFile(DATA_PATH + "/bg_texture.png"))
    {
        sf::err() << "Failed to load object texture, exiting..." << std::endl;
        return -1;
    }

    sf::Sprite background;
    background.setTexture(bgTexture);
    background.setScale(2.f, 2.f);


    /**
    **  RT PART
    **/
    sf::Texture distortionMap;

    // It is important to set repeated to true to enable scrolling upwards
    distortionMap.setRepeated(true);

    // Setting smooth to true lets us use small maps even on larger images
    distortionMap.setSmooth(true);

    if (!distortionMap.loadFromFile(DATA_PATH + "/distortion_map.png"))
    {
        sf::err() << "Failed to load distortion map, exiting..." << std::endl;
        return -1;
    }

    sf::RenderTexture renderTexture;
    renderTexture.create(1280, 720);

    sf::Sprite sprite;
    sprite.setTexture(renderTexture.getTexture());

    sf::Shader shader;

    if (!shader.loadFromFile(DATA_PATH+"/shader.fs", sf::Shader::Fragment))
    {
        sf::err() << "Failed to load shader, exiting..." << std::endl;
        return -1;
    }

    float distortionFactor = .05f;
    float riseFactor = .2f;
    float p = 0.f;
    float texSizeY = distortionMap.getSize().y;
    float texSizeX = distortionMap.getSize().x;

    sf::Vector2f lastPos;
    sf::Vector2f deltaPos;
    std::cout << texSizeX << std::endl;


    shader.setUniform("currentTexture", sf::Shader::CurrentTexture);
    shader.setUniform("distortionMapTexture", distortionMap);
    shader.setUniform("distortionFactor", distortionFactor);

    sf::Clock clock;



    while (true)
    {
        sf::Event event;
        if (window.pollEvent(event)) {
            switch (event.type)
            {
                case sf::Event::Closed: return 0;
                default: break;
            }
        }

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
			player.move(0.2, 0.f);

		}
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
			player.move(-0.2, 0.f);
		}
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
			player.move(0.f, 0.2);
		}
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
			player.move(0.f, -0.2);
		}
        p = 0.f;
        //-------- TESTS
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
            p = 0.5f;
		}

        
        deltaPos.y += ViewBG.getCenter().y- lastPos.y;
        deltaPos.x += ViewBG.getCenter().x- lastPos.x;

        lastPos.y = ViewBG.getCenter().y;
        lastPos.x = ViewBG.getCenter().x;
        
        float dy = deltaPos.y * (1.0/720.0);  //AH
        float dx = deltaPos.x * (1.0/1280.0); // AH !!!!!

        std::cout << dy << ";" << dx << std::endl;
        shader.setUniform("yOffset", dy);
        shader.setUniform("xOffset", dx);

        renderTexture.clear(sf::Color(255,0,255));

        renderTexture.setView(ViewBG);
        renderTexture.draw(background);
        renderTexture.draw(player);

        ViewBG.setCenter(player.getPosition());

        renderTexture.display();

        window.clear();
        window.setView(ViewFinal);
        
        window.draw(sprite, &shader);

        window.display();
    }
}