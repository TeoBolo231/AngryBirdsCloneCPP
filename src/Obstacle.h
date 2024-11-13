
#ifndef ANGRYBIRDSSFML_OBSTACLE_H
#define ANGRYBIRDSSFML_OBSTACLE_H

#include "GameObject.h"

class Obstacle: public GameObject
{
 public:

  Obstacle();
  ~Obstacle();

  static void resetVis(Obstacle obstacles[], int array_size, bool state);
};

#endif // ANGRYBIRDSSFML_OBSTACLE_H
