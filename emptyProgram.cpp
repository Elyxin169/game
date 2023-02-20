#include "main.hpp"
#define width 1920
#define height 1080
#define fps 60

sf::Event event;

int main()
{
      sf::Font Monocraft;
      if (!Monocraft.loadFromFile("src/Monocraft.otf"))
            std::cout << "Sources folder not found!\n";

      sf::RenderWindow window(sf::VideoMode(width, height), "Window Label", sf::Style::Fullscreen);
      mainloop
      {
            while (eventsLeft)
            {
                  if (event.type == sf::Event::Closed)
                        window.close();
            }
            window.clear(sf::Color(100, 100, 100));
            window.display();
      }

      return 0;
}