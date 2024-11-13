
#include <iostream>
#include <SFML/Graphics.hpp>
#include "GameObject.h"

GameObject::GameObject()
{
  sprite = new sf::Sprite;
  setVisibility(false);
  setVisibility(false);
}

GameObject::~GameObject()
{
  delete sprite;
}

void GameObject::setVisibility(bool state)
{
  is_visible = state;
}

bool GameObject::getVisibility() const
{
  return is_visible;
}

sf::Sprite* GameObject::getSprite()
{
  return sprite;
}

void GameObject::setDirection(float x, float y)
{
  direction.x = x;
  direction.y = y;
}

Vector2 GameObject::getDirection()
{
  return direction;
}

void GameObject::normDirection()
{
  direction.normalise();
}

void GameObject::setTexture(sf::Texture& texture)
{
  sprite->setTexture(texture);
}
