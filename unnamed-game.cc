// #define SFML_STATIC
#include "engine.hh"
#include <chrono>
#include <cmath>
#include <cstring>
#define fps 60
#pragma GCC diagnostic ignored "-Wwrite-strings"
#define times(x) for (int i = 0; i < x; i++)

constexpr int jumpBoostLuckFactor = 10;

enum p1920x1080
{
      width = 1920,
      height = 1080,
      BtnWidth = 183,
      BtnHeight = 85,
      BtnY = 540,
      BtnOffset = int((p1920x1080::width - p1920x1080::width / 1.5) / 2),
      BtnOffsetY = int((p1920x1080::height - p1920x1080::height / 1.5) / 2),
      HiScoreY = p1920x1080::BtnY + 150
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
tsf::Tile line1[2], line2[2], line3[2], line4[2];

float velocity, blockVelocity = 0;
int score = 0;

int max(std::vector<int> v)
{
      int max = v[0];
      for (int i = 1; i < v.size(); i++)
      {
            if (v[i] > max)
            {
                  max = v[i];
            }
      }
      return max;
}

int main()
{
      sf::Font Monocraft;
      if (!Monocraft.loadFromFile("src/Monocraft.otf"))
            std::cout << "Sources folder not found!\n";

      sf::Shader shader;
      if (!shader.loadFromFile("mainShader.vert", "mainShader.frag"))
            std::cout << "Failed to load shader" << std::endl;

      sf::RenderWindow window(sf::VideoMode(p1920x1080::width, p1920x1080::height), "Window Label", sf::Style::Fullscreen);
      window.setFramerateLimit(fps);

      sf::BlendMode blendMode(sf::BlendMode::SrcAlpha, sf::BlendMode::OneMinusSrcAlpha);

      sf::RenderStates renderStates;
      renderStates.blendMode = blendMode;
      renderStates.shader = &shader;

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

      sf::Texture arrow;
      if (!arrow.loadFromFile("src/arrow.png"))
            std::cout << "Cant find arrow.png in src folder" << std::endl;
      sf::Sprite arrowSprite;
      arrowSprite.setTexture(arrow);

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

      for (int i = 0; i < 2; i++)
      {
            line1[i] = tsf::genTile(
                vec2(150, 50),
                vec2(650 + 150 * i, 700),
                tsf::Red);
            line2[i] = tsf::genTile(
                vec2(150, 50),
                vec2(50 + 150 * i, 700),
                tsf::Red);
            line3[i] = tsf::genTile(
                vec2(150, 50),
                vec2(1150 + 150 * i, 700),
                tsf::Red);
            line4[i] = tsf::genTile(
                vec2(150, 50),
                vec2(1700 + 150 * i, 700),
                tsf::Red);
      }

      tsf::CollisionTable platforms;
      for (int i = 0; i < 2; i++)
      {
            platforms.tiles.push_back(&line1[i]);
            platforms.tiles.push_back(&line2[i]);
            platforms.tiles.push_back(&line3[i]);
            platforms.tiles.push_back(&line4[i]);
      }

      sf::Text renderText;
      sf::Text overheadText;
      sf::Text apprFPS;
      sf::Text scoreText;
      sf::Text HiScore;
      tsf::TextBox countDown = tsf::genTextBox( // made before built in text boxes were a thing
          tsf::genTile(
              vec2(p1920x1080::width, p1920x1080::height),
              vec2(0, 0),
              tsf::TransParent),
          "69",
          Monocraft,
          300,
          centerAlign);
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
      scoreText.setFont(Monocraft);
      scoreText.setFillColor(tsf::Blue.fillColor);
      scoreText.setCharacterSize(46);
      scoreText.setString("score: 100");
      scoreText.setPosition(vec2(p1920x1080::width - scoreText.getLocalBounds().width, 25));
      scoreText.setOutlineThickness(0.5);
      HiScore.setFont(Monocraft);
      HiScore.setFillColor(sf::Color::Black);
      HiScore.setCharacterSize(20);
      HiScore.setOutlineThickness(0.5);
      HiScore.setPosition(vec2(p1920x1080::BtnOffset + p1920x1080::BtnWidth, p1920x1080::HiScoreY));
      HiScore.setString("High score: 0");
      int renderTime = 1;

      // didnt yet find a better way to do this
      sf::Clock clock, clock2, clock3, secondTimer;
      constexpr int delay = 55000;
      constexpr int delay2 = 20000;
      std::vector<int> scores;
      tsf::Tile **lines;

      mainloop
      {
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
                        window.draw(HiScore);

                        if (playBtn.isClicked(&window))
                        {
                              state = inGame;
                              clock3.restart();
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
                  arrowSprite.setPosition(position.x, 0);
                  if (position.y < 0)
                        window.draw(arrowSprite);
                  window.draw(scoreText);
                  if (clock3.getElapsedTime().asSeconds() <= 3)
                  {
                        countDown.text = std::to_string(3 - int(floor(clock3.getElapsedTime().asSeconds())));
                        countDown.draw(&window);
                        scoreText.setString("score: 0");
                  }
                  if (floor(clock3.getElapsedTime().asSeconds()) == 3)
                  {
                        blockVelocity = 2.0;
                  }
                  if (clock3.getElapsedTime().asSeconds() >= 4)
                  {
                        blockVelocity += 0.001;
                        scoreText.setString("score: " + std::to_string(score));
                  }
                  score = int(floor(clock3.getElapsedTime().asSeconds())) - 3;
                  if (velocity <= 50)
                        velocity += 0.5;

                  if (true) // q/delay >= 1
                  {
                        position.y += velocity - 1;
                        playerTile.position = vec2(position);
                        playerTile.update();
                        if (platforms.hits(&playerTile))
                        {
                              position.y -= velocity - 1;
                              velocity = 0;
                        }

                        else
                              position.y += 1;
                  }
                  playerTile.position = vec2(position);
                  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) &&
                      (platforms.hits(&playerTile)))
                        if (platforms.hitsWReturn(&playerTile)->gradient == tsf::Yellow)
                              velocity -= 40;
                        else
                              velocity -= 20;
                  if (platforms.hits(&playerTile))
                        position.x -= blockVelocity;

                  if (position.y >= 955)
                  {
                        velocity = 0;
                        blockVelocity = 0.0;
                        times(2)
                        {
                              line1[i] = tsf::genTile(
                                  vec2(150, 50),
                                  vec2(650 + 150 * i, 700),
                                  tsf::Red);
                              line2[i] = tsf::genTile(
                                  vec2(150, 50),
                                  vec2(50 + 150 * i, 700),
                                  tsf::Red);
                              line4[i] = tsf::genTile(
                                  vec2(150, 50),
                                  vec2(1700 + 150 * i, 700),
                                  tsf::Red);
                              line3[i] = tsf::genTile(
                                  vec2(150, 50),
                                  vec2(1150 + 150 * i, 700),
                                  tsf::Red);
                        }
                        position = vec2(250, 250);
                        state = inMenu;
                        scores.push_back(score);
                        score = 0;
                        HiScore.setString("High score: " + std::to_string(max(scores)));
                  }

                  if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                  {
                        position.x += 8;
                  }
                  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                  {
                        position.x -= 8;
                  }

                  playerTile.position = vec2(position);
                  playerTile.update();

                  vec2 center = position - vec2(120 - 25, 120 - 25);
                  glowObj.setPosition(center);
                  shader.setUniform("middlePoint", vec2(position.x + 25, p1920x1080::height - position.y - 25));
                  window.draw(glowObj, renderStates);

                  for (int i = 0; i < 2; i++)
                  {
                        line1[i].position = vec2(line1[i].shell.getPosition() - vec2(blockVelocity, 0));
                        line1[i].update();
                        line1[i].draw(&window);
                        if (line1[0].shell.getPosition().x <= -305)
                        {
                              int yPos = tsf::random(550, 950);
                              bool jumpBoost = false;
                              if (tsf::random(0, jumpBoostLuckFactor) == 6)
                                    jumpBoost = true;

                              for (int j = 0; j < 2; j++)
                              {
                                    line1[j].position = vec2(p1920x1080::width + j * 150, yPos);
                                    if (jumpBoost)
                                          line1[j].gradient = tsf::Yellow;
                                    else
                                          line1[j].gradient = tsf::Red;
                                    line1[j].update();
                              }
                        }
                        line2[i].position = vec2(line2[i].shell.getPosition() - vec2(blockVelocity, 0));
                        line2[i].update();
                        line2[i].draw(&window);
                        if (line2[0].shell.getPosition().x <= -305)
                        {
                              int yPos = tsf::random(550, 950);
                              bool jumpBoost = false;
                              if (tsf::random(0, jumpBoostLuckFactor) == 6)
                                    jumpBoost = true;
                              for (int j = 0; j < 2; j++)
                              {
                                    line2[j].position = vec2(p1920x1080::width + j * 150, yPos);
                                    if (jumpBoost)
                                          line2[j].gradient = tsf::Yellow;
                                    else
                                          line2[j].gradient = tsf::Red;
                                    line2[j].update();
                              }
                        }
                        line3[i].position = vec2(line3[i].shell.getPosition() - vec2(blockVelocity, 0));
                        line3[i].update();
                        line3[i].draw(&window);
                        if (line3[0].shell.getPosition().x <= -305)
                        {
                              int yPos = tsf::random(550, 950);
                              bool jumpBoost = false;
                              if (tsf::random(0, jumpBoostLuckFactor) == 6)
                                    jumpBoost = true;
                              for (int j = 0; j < 2; j++)
                              {
                                    line3[j].position = vec2(p1920x1080::width + j * 150, yPos);
                                    if (jumpBoost)
                                          line3[j].gradient = tsf::Yellow;
                                    else
                                          line3[j].gradient = tsf::Red;
                                    line3[j].update();
                              }
                        }
                        line4[i].position = vec2(line4[i].shell.getPosition() - vec2(blockVelocity, 0));
                        line4[i].update();
                        line4[i].draw(&window);
                        if (line4[0].shell.getPosition().x <= -305)
                        {
                              int yPos = tsf::random(550, 950);
                              bool jumpBoost = false;
                              if (tsf::random(0, jumpBoostLuckFactor) == 6)
                                    jumpBoost = true;
                              for (int j = 0; j < 2; j++)
                              {
                                    line4[j].position = vec2(p1920x1080::width + j * 150, yPos);
                                    if (jumpBoost)
                                          line4[j].gradient = tsf::Yellow;
                                    else
                                          line4[j].gradient = tsf::Red;
                                    line4[j].update();
                              }
                        }
                  }

                  playerTile.draw(&window);
            }
            if (q / delay >= 1)
                  clock.restart();
            if (q2 / delay2 >= 1)
                  clock2.restart();
            if (secondTimer.getElapsedTime().asMicroseconds() > 999)
                  secondTimer.restart();
            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
            renderTime = std::chrono::duration_cast<std::chrono::nanoseconds>(end - gameNow).count() / 1000;

            window.display();
            ++framesDone;
      }

      return 0;
}