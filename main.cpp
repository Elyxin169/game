#include "main.hpp"

int main()
{
      srand(time(NULL));
      sf::RenderWindow window(sf::VideoMode(1920, 1080), "HELP", sf::Style::Fullscreen);
      window.setFramerateLimit(60);

      engine::Vector2 screenSize = { 1920, 1080 };
      engine::Vector2 pos = { screenSize.x / 2, screenSize.y / 2 };
      sf::View view(sf::FloatRect(0.f, 0.f, 1920.f, 1080.f));

      engine::Tile player = engine::genTile({ 25, 25 }, { 69, 420 }, engine::GreenGradient); //size, position, gradient
      engine::Tile tiles[48];
      for (int i = -4; i < 44; i++) {
            int q = rand() % 3;
            engine::Gradient temp;
            switch (q) {
            case 0:
            temp = engine::RedGradient;
            break;
            case 1:
            temp = engine::BlueGradient;
            break;
            case 2:
            temp = engine::GreenGradient;
            break;
            default:
            break;
            }
            tiles[i + 4] = engine::genTile({ 38, 38 }, { float(i * 48),5 }, temp);
      }


      while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                  if (event.type == sf::Event::Closed)
                        window.close();
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                  pos.y += 5.5;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
                  pos.y -= 5.5;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                  pos.x += 5.5;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                  pos.x -= 5.5;
            player.rectangle.setPosition(pos.x, pos.y);
            view.setCenter(pos.x, pos.y);
            window.clear(sf::Color(80, 80, 80, 255));
            for (int i = 0; i < 48; ++i) {
                  engine::renderTile(tiles[i], &window);
            }
            engine::renderTile(player, &window);
            window.setView(view);
            window.display();
      }

      return 0;
}