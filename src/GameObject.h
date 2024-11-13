
#ifndef ANGRYBIRDSSFML_GAMEOBJECT_H
#define ANGRYBIRDSSFML_GAMEOBJECT_H

#include "Vector2.h"
#include <SFML/Graphics.hpp>

class GameObject
{
 public:

  GameObject();
  ~GameObject();

  void setVisibility(bool state);
  bool getVisibility() const;

  sf::Sprite* getSprite();

  void setDirection(float x, float y);
  Vector2 getDirection();
  void normDirection();

  void setTexture(sf::Texture& texture);

 protected:

  bool is_visible;

  Vector2 direction = {0, 0};

  sf::Sprite* sprite;
};

#endif // ANGRYBIRDSSFML_GAMEOBJECT_H
