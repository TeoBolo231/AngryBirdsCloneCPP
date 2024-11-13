
#ifndef ANGRYBIRDSSFML_BIRD_H
#define ANGRYBIRDSSFML_BIRD_H

#include "GameObject.h"
#include "Player.h"
#include "Enemy.h"
#include "Obstacle.h"
#include <cmath>

class Bird: public GameObject
{
 public:

  Bird();
  ~Bird();

  float getCenterX();
  float getCenterY();

  static float getOriginX(sf::RenderWindow& window, Player& player);
  static float getOriginY(sf::RenderWindow& window, Player& player);

  void setSpeed(float value);
  float getSpeed() const;
  void resetPos(sf::RenderWindow& window, Player& player);

  void resetDecay();

  void grab(sf::RenderWindow& window, Player& player, sf::Vector2i click);
  void launch(sf::RenderWindow& window,
              sf::Sprite& bgSprite,
              sf::View& view,
              Player& player,
              Enemy enemy[],
              int enemy_array_size,
              Obstacle obstacles[],
              int obs_array_size,
              float dt);
  void move(sf::RenderWindow& window, Player& player, float dt);

  enum States
  {
    Idle = 0 ,
    Held = 1,
    Launched = 2
  };

  States state;
  States getState() const;
  void setState(States state_enum);

  void enemyCollision(sf::RenderWindow& window,
                      sf::View& view,
                      Player& player,
                      Enemy enemies[],
                      int enemy_array_size);
  void obstacleCollision(sf::RenderWindow& window,
                         sf::View& view,
                         Player& player,
                         Obstacle obstacles[],
                         int obs_array_size);
  void edgeCollision(sf::RenderWindow& window,
                     sf::Sprite& bg_sprite,
                     sf::View& view,
                     Player& player);

 protected:

  float speed;

  float const MAX_DISTANCE = 150.f;
  float distance = 0;

  float const MIN_DECAY = 18.f;
  float const DECAY_RATE   = 4.f;
  float decay;
};

#endif // ANGRYBIRDSSFML_BIRD_H
