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
      const Gradient RedGradient = { sf::Color(255,20,10,255), sf::Color(255,110,100,255) };
      const Gradient BlueGradient = { sf::Color(10,20,255,255), sf::Color(100,110,255,255) };
      const Gradient GreenGradient = { sf::Color(10,200,20,255), sf::Color(100,255,110,255) };
      Tile genTile(Vector2 size, Vector2 pos, Gradient gradient)
      {
            sf::RectangleShape temp;
            temp.setSize(sf::Vector2f(size.x, size.y));
            temp.setPosition(sf::Vector2f(pos.x, pos.y));
            temp.setOutlineColor(gradient.borderC);
            temp.setFillColor(gradient.fillC);
            temp.setOutlineThickness(5);
            return { gradient, temp };
      }
      void renderTile(Tile tile, sf::RenderWindow* window)
      {
            window->draw(tile.rectangle);
      }
}