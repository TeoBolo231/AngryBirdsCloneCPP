
#include "Player.h"

Player::Player()
{
  resetLives();
  setVisibility(true);
}

Player::~Player()
= default;

void Player::resetScore()
{
  score = 0;
}
void Player::addScore(int value)
{
  score += value;
}
void Player::redScore(int value)
{
  score -= value;
}
int Player::getScore() const
{
  return score;
}

void Player::resetLives()
{
  lives = MAX_LIVES;
}
void Player::loseLife(int value)
{
  lives -= value;
}
int Player::getLives() const
{
  return lives;
}