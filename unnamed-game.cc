// #define SFML_STATIC
#include "engine.hh"
#include <chrono>
#define fps 60
#pragma GCC diagnostic ignored "-Wwrite-strings"

enum p1920x1080
{
      width = 1920,
      height = 1080,
      BtnWidth = 183,
      BtnHeight = 85,
      BtnY = 540,
      BtnOffset = int((p1920x1080::width - p1920x1080::width / 1.5) / 2),
      BtnOffsetY = int((p1920x1080::height - p1920x1080::height / 1.5) / 2)
};
enum gameStates
{
      inMenu = 0,
      inGame = 1
};
enum menuStates
{
      settings = 0,
      credits = 1,
      base = 2
};

sf::Event event;

int main()
{
      sf::Font Monocraft;
      if (!Monocraft.loadFromFile("src/Monocraft.otf"))
            std::cout << "Sources folder not found!\n";

      sf::RenderWindow window(sf::VideoMode(p1920x1080::width, p1920x1080::height), "Window Label", sf::Style::Fullscreen);
      window.setFramerateLimit(fps);

      tsf::Tile menuBkg = tsf::genTile(
          vec2(p1920x1080::width / 1.5, p1920x1080::height / 1.5),
          vec2((p1920x1080::width - p1920x1080::width / 1.5) / 2, (p1920x1080::height - p1920x1080::height / 1.5) / 2), // x/1.5/4 = x/6
          tsf::Gray);
      tsf::TextBox menuText = tsf::genTextBox(
          tsf::genTile(
              vec2(p1920x1080::width / 1.5 / 1.5, p1920x1080::height / 1.5 / 3),
              vec2(tsf::center(p1920x1080::width, p1920x1080::width / 1.5 / 1.5), p1920x1080::height / 4),
              tsf::cleanGray),
          "tile idk",
          Monocraft,
          69,
          centerAlign);
      tsf::Button playBtn = tsf::genButton(
          tsf::genTile(
              vec2(p1920x1080::BtnWidth, p1920x1080::BtnHeight),
              vec2(p1920x1080::BtnWidth + BtnOffset, p1920x1080::BtnY),
              tsf::Red),
          "Play",
          Monocraft,
          28,
          centerAlign);
      tsf::Button settingsBtn = tsf::genButton(
          tsf::genTile(
              vec2(p1920x1080::BtnWidth, p1920x1080::BtnHeight),
              vec2(p1920x1080::BtnWidth * 3 + BtnOffset, p1920x1080::BtnY),
              tsf::Green),
          "Settings",
          Monocraft,
          28,
          centerAlign);
      tsf::Button settingsBackBtn = tsf::genButton(
          tsf::genTile(
              vec2(p1920x1080::BtnWidth, p1920x1080::BtnHeight),
              vec2(20 + p1920x1080::BtnOffset, 20 + p1920x1080::BtnOffsetY),
              tsf::Red),
          "Back",
          Monocraft,
          28,
          centerAlign);
      tsf::Button creditsBtw = tsf::genButton(
          tsf::genTile(
              vec2(p1920x1080::BtnWidth, p1920x1080::BtnHeight),
              vec2(p1920x1080::BtnWidth * 5 + BtnOffset, p1920x1080::BtnY),
              tsf::Blue),
          "Credits",
          Monocraft,
          28,
          centerAlign);

      int framesDone = 0;
      int state = inMenu;
      int menuState = base;
      vec2 position = vec2(250, 250);
      tsf::Tile playerTile = tsf::genTile(
          vec2(50, 50),
          position,
          tsf::Blue);
      sf::RectangleShape glowObj;
      glowObj.setSize(vec2(240, 240));
      glowObj.setPosition(position);

      sf::Shader shader;
      if (!shader.loadFromFile("mainShader.vert", "mainShader.frag"))
            std::cout << "Failed to load shader" << std::endl;
      tsf::Tile line1[2];
      for (int i = 0; i < 2; i++)
      {
            line1[i] = tsf::genTile(
                vec2(150, 50),
                vec2(650 + 150 * i, 700),
                tsf::Red);
      }
      tsf::Tile line2[2];
      for (int i = 0; i < 2; i++)
      {
            line2[i] = tsf::genTile(
                vec2(150, 50),
                vec2(50 + 150 * i, 700),
                tsf::Red);
      }
      tsf::Tile line4[2];
      for (int i = 0; i < 2; i++)
      {
            line4[i] = tsf::genTile(
                vec2(150, 50),
                vec2(1700 + 150 * i, 700),
                tsf::Red);
      }
      tsf::Tile line3[2];
      for (int i = 0; i < 2; i++)
      {
            line3[i] = tsf::genTile(
                vec2(150, 50),
                vec2(1150 + 150 * i, 700),
                tsf::Green);
      }

      tsf::CollisionTable tbl1;
      for (int i = 0; i < 2; i++)
      {
            tbl1.tiles.push_back(&line1[i]);
      }
      tsf::CollisionTable tbl2;
      for (int i = 0; i < 2; i++)
      {
            tbl2.tiles.push_back(&line2[i]);
      }
      tsf::CollisionTable tbl3;
      for (int i = 0; i < 2; i++)
      {
            tbl3.tiles.push_back(&line3[i]);
      }
      tsf::CollisionTable tbl4;
      for (int i = 0; i < 2; i++)
      {
            tbl4.tiles.push_back(&line4[i]);
      }

      sf::Text renderText;
      sf::Text overheadText;
      sf::Text apprFPS;
      renderText.setFont(Monocraft);
      renderText.setFillColor(sf::Color::Black);
      renderText.setCharacterSize(20);
      renderText.setOutlineThickness(0.5);
      renderText.setPosition(vec2(5, 0));
      overheadText.setFont(Monocraft);
      overheadText.setFillColor(sf::Color::Black);
      overheadText.setCharacterSize(20);
      overheadText.setPosition(vec2(5, 20));
      overheadText.setOutlineThickness(0.5);
      apprFPS.setFont(Monocraft);
      apprFPS.setFillColor(sf::Color::Black);
      apprFPS.setCharacterSize(20);
      apprFPS.setPosition(vec2(5, 40));
      apprFPS.setOutlineThickness(0.5);
      int renderTime = 1;

      // didnt yet find a better way to do this
      sf::Clock clock;
      sf::Clock clock2;
      const int delay = 55000;
      const int delay2 = 20000;

      float velocity = 0;
      float blockVelocity = 2.0;
      mainloop
      {
            blockVelocity += 0.001;
            std::chrono::steady_clock::time_point gameNow = std::chrono::steady_clock::now();
            window.clear(sf::Color(100, 100, 100));

            int q = clock.getElapsedTime().asMicroseconds();
            int q2 = clock2.getElapsedTime().asMicroseconds();

            if (q / (delay + 5000) >= 1)
            {
                  renderText.setString("renderTime: " + std::to_string(renderTime) + "micros");
                  overheadText.setString("overhead: " + std::to_string(16666 - renderTime) + "micros");
                  apprFPS.setString("approx FPS: " + std::to_string(1000000 / renderTime));
            }

            window.draw(renderText);
            window.draw(overheadText);
            window.draw(apprFPS);

            if (state == inMenu)
            {
                  while (eventsLeft)
                  {
                        if (event.type == sf::Event::Closed)
                              window.close();
                  }
                  menuBkg.draw(&window);
                  menuText.draw(&window);

                  if (menuState == base)
                  {
                        playBtn.draw(&window);
                        settingsBtn.draw(&window);
                        creditsBtw.draw(&window);

                        /*
                        //placeholder
                        if (playBtn.isClicked(&window))
                              playBtn.tile.updGradient(tsf::Blue);
                        else
                        playBtn.tile.updGradient(tsf::Red);
                        */
                        if (playBtn.isClicked(&window))
                        {
                              state = inGame;
                        }
                        if (settingsBtn.isClicked(&window))
                        {
                              menuState = settings;
                        }

                        if (creditsBtw.isClicked(&window))
                        {
                              menuState = credits;
                        }
                  }
                  else if (menuState == settings)
                  {
                        settingsBackBtn.draw(&window);
                        if (settingsBackBtn.isClicked(&window))
                        {
                              menuState = base;
                        }
                  }
                  else if (menuState == credits)
                  {
                        settingsBackBtn.draw(&window);
                        if (settingsBackBtn.isClicked(&window))
                        {
                              menuState = base;
                        }
                  }
            }
            if (state == inGame)
            {
                  while (eventsLeft)
                  {
                        if (event.type == sf::Event::Closed)
                              window.close();
                  }
                  /*
                  if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && q/delay >= 1)
                  {
                        position.y -= 49;
                        playerTile.shell.setPosition(position);
                        if (tbl1.hits(&playerTile) || tbl2.hits(&playerTile) || tbl3.hits(&playerTile)|| tbl4.hits(&playerTile))
                              position.y += 49;
                        else
                              position.y -= 1;
                        playerTile.shell.setPosition(position);
                  }
                  */
                  if (velocity <= 50)
                        velocity += 0.5;

                  if (true) // q/delay >= 1
                  {
                        position.y += velocity - 1;
                        playerTile.shell.setPosition(position);
                        if (tbl1.hits(&playerTile) || tbl2.hits(&playerTile) || tbl3.hits(&playerTile) || tbl4.hits(&playerTile))
                        {
                              position.y -= velocity - 1;
                              velocity = 0;
                        }

                        else
                              position.y += 1;
                        playerTile.shell.setPosition(position);
                  }
                  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && position.y >= 650)
                        velocity -= 20;
                  if (position.y >= 700)
                        exit(69);

                  if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && position.x <= p1920x1080::width)
                  {
                        position.x += 8;
                  }
                  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && position.x <= p1920x1080::width)
                  {
                        position.x -= 8;
                  }

                  playerTile.shell.setPosition(position);

                  vec2 center = position - vec2(120 - 25, 120 - 25);
                  glowObj.setPosition(center);
                  shader.setUniform("middlePoint", vec2(position.x + 25, p1920x1080::height - position.y - 25));
                  window.draw(glowObj, &shader);

                  for (int i = 0; i < 2; i++)
                  {
                        line1[i].shell.setPosition(line1[i].shell.getPosition() - vec2(blockVelocity, 0));
                        line1[i].draw(&window);
                        if (line1[0].shell.getPosition().x <= -305)
                              for (int j = 0; j < 2; j++)
                              {
                                    line1[j].shell.setPosition(p1920x1080::width + j * 150, 700);
                              }
                  }
                  for (int i = 0; i < 2; i++)
                  {
                        line2[i].shell.setPosition(line2[i].shell.getPosition() - vec2(blockVelocity, 0));

                        line2[i].draw(&window);
                        if (line2[0].shell.getPosition().x <= -305)
                              for (int j = 0; j < 2; j++)
                              {
                                    line2[j].shell.setPosition(p1920x1080::width + j * 150, 700);
                              }
                  }

                  for (int i = 0; i < 2; i++)
                  {
                        line3[i].shell.setPosition(line3[i].shell.getPosition() - vec2(blockVelocity, 0));
                        line3[i].draw(&window);
                        if (line3[0].shell.getPosition().x <= -305)
                              for (int j = 0; j < 2; j++)
                              {
                                    line3[j].shell.setPosition(p1920x1080::width + j * 150, 700);
                              }
                  }
                  for (int i = 0; i < 2; i++)
                  {
                        line4[i].shell.setPosition(line4[i].shell.getPosition() - vec2(blockVelocity, 0));
                        line4[i].draw(&window);
                        if (line4[0].shell.getPosition().x <= -305)
                              for (int j = 0; j < 2; j++)
                              {
                                    line4[j].shell.setPosition(p1920x1080::width + j * 150, 700);
                              }
                  }

                  playerTile.draw(&window);
            }
            if (q / delay >= 1)
                  clock.restart();
            if (q2 / delay2 >= 1)
                  clock2.restart();
            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
            renderTime = std::chrono::duration_cast<std::chrono::nanoseconds>(end - gameNow).count() / 1000;

            window.display();
            ++framesDone;
      }

      return 0;
}