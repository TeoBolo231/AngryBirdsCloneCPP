
#include "Obstacle.h"

Obstacle::Obstacle()
{
  setVisibility(true);
}

Obstacle::~Obstacle()
= default;

void Obstacle::resetVis(Obstacle* obstacles, int array_size, bool state)
{
  for (int i = 0; i < array_size; i++)
  {
    obstacles[i].setVisibility(state);
  }
}
