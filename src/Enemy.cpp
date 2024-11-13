
#include "Enemy.h"

Enemy::Enemy()
{
  getSprite()->setScale(0.5, 0.5);
  setVisibility(true);
}

Enemy:: ~Enemy()
= default;

void Enemy::resetVis(Enemy enemies[], int array_size, bool state)
{
  for (int i = 0; i < array_size; ++i)
  {
    enemies[i].setVisibility(state);
  }
}
