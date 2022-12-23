#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
namespace engine {
      typedef struct Vector2 {
            float x;
            float y;
      } Vector2;
      typedef struct Gradient {
            sf::Color borderC;
            sf::Color fillC;
      } Gradient;
      typedef struct Tile {
            Gradient gradient;
            sf::RectangleShape rectangle;
      } Tile;
      typedef struct Button {
            Vector2 position;
            Vector2 size;
            Tile tile;
            std::string text;
            sf::Font font;
            int textSize;
            Gradient gradient;
            Vector2 offset;
      } Button;
      typedef struct TexturedTile {
            Gradient gradient;
            sf::RectangleShape rectangle;
            sf::Texture texture;
      } TexturedTile;
      typedef struct TexturedButton {
            Vector2 position;
            Vector2 size;
            TexturedTile tile;
            std::string text;
            sf::Font font;
            int textSize;
            Gradient gradient;
            Vector2 offset;
      } TexturedButton;
      const Gradient RedGradient = { sf::Color(255,20,10,255), sf::Color(255,110,100,255) }; // border, insides
      const Gradient BlueGradient = { sf::Color(10,20,255,255), sf::Color(100,110,255,255) };
      const Gradient GreenGradient = { sf::Color(10,200,20,255), sf::Color(100,255,110,255) };
      const Gradient GrayGradient = { sf::Color(25,20,10,255), sf::Color(69,69,69,255) };
      Tile genTile(Vector2 size, Vector2 pos, Gradient gradient)
      {
            /*
            `size` Size of the tile (engine::Vector2)
            `position` Position of the tile (engine::Vector2)
            `gradient` Gradient of the tile (engine::Gradient)
            */
            sf::RectangleShape temp;
            temp.setSize(sf::Vector2f(size.x, size.y));
            temp.setPosition(sf::Vector2f(pos.x, pos.y));
            temp.setOutlineColor(gradient.borderC);
            temp.setFillColor(gradient.fillC);
            temp.setOutlineThickness(5);
            return { gradient, temp };
      }
      void setButton(Button* button){
            button->tile = genTile(button->size, button->position, button->gradient);
      }
      
      void renderTile(Tile* tile, sf::RenderWindow* window)
      {
            window->draw(tile->rectangle);
      }
      void renderTile(TexturedTile* tile, sf::RenderWindow* window)
      {
            window->draw(tile->rectangle);
      }
      void renderTile(Button* button, sf::RenderWindow* window)
      {
            window->draw(button->tile.rectangle);
            sf::Text temp;
            temp.setFont(button->font);
            temp.setPosition(button->position.x+button->offset.x, button->position.y+button->offset.y);
            temp.setCharacterSize(button->textSize);
            temp.setString(button->text);
            window->draw(temp);
      }
      bool isClicked(Button* button)
      {
            sf::Vector2i temp = sf::Mouse::getPosition();
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                  if (temp.x > button->position.x && temp.x < button->position.x + button->size.x && \
                        temp.y > button->position.y && temp.y < button->position.y + button->size.y
                        ) return true;
            }
            return false;
      }
}