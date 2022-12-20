#include "main.hpp"

int main()
{
      srand(time(NULL));
      sf::RenderWindow window(sf::VideoMode(1000, 800), "HELP");
      window.setFramerateLimit(60);

      engine::Vector2 screenSize = {1000, 800};
      engine::Vector2 pos = {screenSize.x/2, screenSize.y/2};
      sf::RectangleShape player;
      player.setSize(sf::Vector2f(25, 25));
      player.setOutlineColor(engine::RedGradient.borderC);
      player.setFillColor(engine::RedGradient.fillC);
      player.setOutlineThickness(5);
      player.setPosition(pos.x, pos.y);

      while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                  if (event.type == sf::Event::Closed)
                        window.close();
            }
            
            player.move((rand() % 11)-5, (rand() % 11)-5);

            window.clear(sf::Color(80,80,80,255));
            window.draw(player);
            window.display();
      }

      return 0;
}