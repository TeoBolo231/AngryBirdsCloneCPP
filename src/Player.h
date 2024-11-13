
#ifndef ANGRYBIRDSSFML_PLAYER_H
#define ANGRYBIRDSSFML_PLAYER_H

#include "GameObject.h"

class Player: public GameObject
{
 public:

  Player();
  ~Player();

  void resetScore();
  void addScore(int value);
  void redScore(int value);
  int getScore() const;

  void resetLives();
  void loseLife(int value);
  int getLives() const;

 protected:

  int const MAX_LIVES = 5;
  int lives;

  int score = 0;
};

#endif // ANGRYBIRDSSFML_PLAYER_H
