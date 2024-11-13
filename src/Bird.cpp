
#include "Bird.h"

Bird::Bird()
{
  setVisibility(true);
  setState(Idle);
  setSpeed(11.f);
  getSprite()->setScale(0.5,0.5);
  resetDecay();
}

Bird::~Bird()
= default;

float Bird::getCenterX()
{
    return getSprite()->getGlobalBounds().width/2;
}

float Bird::getCenterY()
{
    return getSprite()->getGlobalBounds().height/2;
}

float Bird::getOriginX(sf::RenderWindow& window, Player& player)
{
  return player.getSprite()->getPosition().x + window.getSize().x/50;
}

float Bird::getOriginY(sf::RenderWindow& window, Player& player)
{
  return  player.getSprite()->getPosition().y;
}

Bird::States Bird::getState() const
{
  return state;
}

void Bird::setState(States state_enum)
{
  state = state_enum;
}

void Bird::setSpeed(float value)
{
  speed = value;
}

float Bird::getSpeed() const
{
  return speed;
}

void Bird::resetPos(sf::RenderWindow& window, Player& player)
{
  getSprite()->setPosition(player.getSprite()->getPosition().x + window.getSize().x/60,
                           player.getSprite()->getPosition().y - window.getSize().y/100);
}

void Bird::resetDecay()
{
  decay = MIN_DECAY;
}

void Bird::enemyCollision(sf::RenderWindow& window,
                          sf::View& view,
                          Player& player,
                          Enemy enemies[],
                          int enemy_array_size)
{
  for (int i = 0; i < enemy_array_size; ++i)
  {
    if (getSprite()->getGlobalBounds().intersects(
        enemies[i].getSprite()->getGlobalBounds()) &&
      enemies[i].getVisibility())
    {
      player.addScore(20);
      enemies[i].setVisibility(false);
      resetDecay();
      setState(Idle);
      resetPos(window, player);
      view.setCenter(window.getSize().x/2, window.getSize().y/2);
    }

  }
}

void Bird::obstacleCollision(sf::RenderWindow& window,
                             sf::View& view,
                             Player& player,
                             Obstacle obstacles[],
                             int obs_array_size)
{
  for (int i = 0; i < obs_array_size; ++i)
  {
    if (getSprite()->getGlobalBounds().intersects(
        obstacles[i].getSprite()->getGlobalBounds()) &&
      obstacles[i].getVisibility())
    {
      player.loseLife(1);
      player.redScore(10);
      obstacles[i].setVisibility(false);
      resetDecay();
      setState(Idle);
      resetPos(window, player);
      view.setCenter(window.getSize().x/2, window.getSize().y/2);
    }

  }
}

void Bird::edgeCollision(sf::RenderWindow& window,
                         sf::Sprite& bg_sprite,
                         sf::View& view,
                         Player& player)
{
  float bird_posX = getSprite()->getPosition().x;
  float bird_posY = getSprite()->getPosition().y;
  float screen_borderX =
    bg_sprite.getGlobalBounds().width - getSprite()->getGlobalBounds().width;
  float screen_borderY_bottom = window.getSize().y - window.getSize().y/6;
  if (
    bird_posX >= screen_borderX ||
    bird_posY >= screen_borderY_bottom)
  {
    setState(Idle);
    player.loseLife(1);
    resetDecay();
    resetPos(window, player);
    view.setCenter(window.getSize().x/2, window.getSize().y/2);
  }
}

void Bird::grab(sf::RenderWindow& window, Player& player, sf::Vector2i click)
{
  if (getState() == Bird::Held)
  {
    getSprite()->setPosition(click.x - getCenterX(), click.y - getCenterY());

    float current_posX = getOriginX(window, player) - getSprite()->getPosition().x;
    float current_posY = getOriginY(window, player) - getSprite()->getPosition().y;
    float const ANGLE = -1.8f;
    float maxDistY = current_posX * ANGLE;
    distance = sqrt(pow(current_posX, 2) + pow(current_posY, 2));

    setDirection(current_posX, current_posY);
    normDirection();

    // Limit Distance
    if (distance > MAX_DISTANCE)
    {
      getSprite()->setPosition(getOriginX(window, player) - (MAX_DISTANCE * direction.x),
                               getOriginY(window, player) - (MAX_DISTANCE * direction.y));

      current_posX = getOriginX(window, player) - getSprite()->getPosition().x;
      current_posY = getOriginY(window, player) - getSprite()->getPosition().y;
      maxDistY = current_posX * -1.5;
      distance = sqrt(pow(current_posX, 2) + pow(current_posY, 2));

      setDirection(current_posX, current_posY);
      normDirection();
    }

    if (current_posY < maxDistY)
    {
      getSprite()->setPosition(getOriginX(window, player) - current_posX, getOriginY(window, player) - maxDistY);
    }


    // Limit Right
    if (getSprite()->getPosition().x >= getOriginX(window, player))
    {
      getSprite()->setPosition(getOriginX(window, player), getSprite()->getPosition().y);
    }

    // Limit Top
    if (getSprite()->getPosition().y <= getOriginY(window, player))
    {
      getSprite()->setPosition(getSprite()->getPosition().x, getOriginY(window, player));
    }
  }
}

void Bird::launch(sf::RenderWindow& window,
                  sf::Sprite& bgSprite,
                  sf::View& view,
                  Player& player,
                  Enemy enemy[],
                  int enemy_array_size,
                  Obstacle obstacles[],
                  int obs_array_size,
                  float dt)
{
  if (getState() == Bird::Launched)
  {
    move(window, player, dt);
    edgeCollision(window,bgSprite, view, player);
    enemyCollision(window,view,player, enemy, enemy_array_size);
    obstacleCollision(window, view, player, obstacles, obs_array_size);
  }
}

void Bird::move(sf::RenderWindow& window, Player& player, float dt)
{
  float speed_mod     = distance/ MAX_DISTANCE;

  float moveX  = getDirection().x * speed_mod * getSpeed();
  float moveY  = getDirection().y * speed_mod * getSpeed();

  decay += DECAY_RATE;

  float new_moveY = moveY + decay * dt;

  getSprite()->move(moveX, new_moveY);
}

