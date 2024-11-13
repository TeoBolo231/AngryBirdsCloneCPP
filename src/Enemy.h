
#ifndef ANGRYBIRDSSFML_ENEMY_H
#define ANGRYBIRDSSFML_ENEMY_H

#include "GameObject.h"

class Enemy: public GameObject
{
 public:

  Enemy();
  ~Enemy();

  static void resetVis(Enemy enemies[], int array_size, bool state);
};

#endif // ANGRYBIRDSSFML_ENEMY_H
