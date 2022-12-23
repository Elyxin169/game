#include "main.hpp"

int main()
{
      //compatibility checking
      if (!sf::Shader::isAvailable()) {
            std::cout << "how old gpu have you got???\n";
      }
      sf::Font monocraft;
      if (!monocraft.loadFromFile("resources/Monocraft.otf")) {
            std::cout << "couldn't load font, did you try to run the program without access to the resources folder?";
      }
      engine::Vector2 screenSize = { float(sf::VideoMode::getDesktopMode().width), float(sf::VideoMode::getDesktopMode().height) };

      srand(time(NULL));
      sf::RenderWindow window(sf::VideoMode(screenSize.x, screenSize.y), "HELP", sf::Style::Fullscreen);

      window.setFramerateLimit(60);

      // defining
      engine::Vector2 pos = { screenSize.x / 2, screenSize.y / 2 };
      sf::View view(sf::FloatRect(0.f, 0.f, screenSize.x, screenSize.y));

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
      sf::Clock clock;
      float lastTime = 0;
      float fps = 69;
      sf::Text fpsText;
      fpsText.setFont(monocraft);
      fpsText.setCharacterSize(20);
      int state = 0;
      engine::Tile menuBkg = engine::genTile({ (float)(screenSize.x / 1.5), (float)(screenSize.y / 1.5) }, { (float)(screenSize.x / 6), (float)(screenSize.y / 6) }, engine::GrayGradient);
      sf::Text menuName;
      menuName.setFont(monocraft);
      menuName.setCharacterSize(40);
      menuName.setString("Tilegame");
      menuName.setPosition((float)(menuBkg.rectangle.getPosition().x * 1.2), (float)(menuBkg.rectangle.getPosition().y * 1.2));

      engine::Button menuPlay = { {(float)(menuBkg.rectangle.getPosition().x * 1.6),(float)(menuBkg.rectangle.getPosition().y * 2.5)}, \
            {(float)(menuBkg.rectangle.getSize().x * 0.15),(float)(menuBkg.rectangle.getSize().y * 0.15)}, {}, "Play!", monocraft, \
             35, engine::GreenGradient, {35, 25} };
      engine::setButton(&menuPlay);
      engine::Button menuCredits = { {(float)(menuBkg.rectangle.getPosition().x * 2.5),(float)(menuBkg.rectangle.getPosition().y * 2.5)}, \
            {(float)(menuBkg.rectangle.getSize().x * 0.15),(float)(menuBkg.rectangle.getSize().y * 0.15)}, {}, "Credits", monocraft, \
             35, engine::RedGradient, {15, 25} };
      engine::setButton(&menuCredits);
      bool menuShift = false;
      bool downShift = false;

      while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                  if (event.type == sf::Event::Closed)
                        window.close();
            }
            if (state == 0) {
                  window.clear(sf::Color(80, 80, 80, 255));
                  engine::renderTile(&menuBkg, &window);
                  engine::renderTile(&menuPlay, &window);
                  engine::renderTile(&menuCredits, &window);
                  if (!menuShift) window.draw(fpsText);
                  if (engine::isClicked(&menuPlay)) {
                        menuShift = true;
                  }
                  if (menuShift && pos.y < menuBkg.rectangle.getSize().y - 50 && !downShift) {
                        pos.y += 8;
                  } else if (menuShift && pos.y > -1500) {
                        pos.y -= 35;
                        downShift = true;
                  } else if (menuShift && pos.y <= -1500) {
                        state = 1; pos = { screenSize.x/2,screenSize.y/2 };
                  }
                  std::cout << pos.y << '\n';
                  view.setCenter(pos.x, pos.y);
                  window.setView(view);
                  window.draw(menuName);
            }
            if (state == -69) { //credits
                  window.clear(sf::Color(80, 80, 80, 255));
                  engine::renderTile(&menuBkg, &window);
                  engine::renderTile(&menuPlay, &window);
                  engine::renderTile(&menuCredits, &window);
                  if (!menuShift) window.draw(fpsText);
                  if (engine::isClicked(&menuPlay)) {
                        menuShift = true;
                  }
                  if (menuShift && pos.y < menuBkg.rectangle.getSize().y - 50 && !downShift) {
                        pos.y += 8;
                  } else if (menuShift && pos.y > -1500) {
                        pos.y -= 35;
                        downShift = true;
                  } else if (menuShift && pos.y <= -1500) {
                        state = 1; pos = { screenSize.x/2,screenSize.y/2 };
                  }
                  std::cout << pos.y << '\n';
                  view.setCenter(pos.x, pos.y);
                  window.setView(view);
                  window.draw(menuName);
            }
            if (state == 1) {
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
                  //align texts here
                  fpsText.setPosition(0 + pos.x - screenSize.x / 2, 0 + pos.y - screenSize.y / 2);
                  window.setView(view);

                  for (int i = 0; i < 48; ++i) {
                        engine::renderTile(&(tiles[i]), &window);
                  }
                  engine::renderTile(&player, &window);
                  window.draw(fpsText);
            }

            fpsText.setString("FPS: " + std::to_string(fps));

            window.display();

            float currentTime = clock.restart().asSeconds();
            fps = 1.f / currentTime;
            lastTime = currentTime;
            // std::cout << "FPS: ~" << fps << '\n';
      }

      return 0;
}