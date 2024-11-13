
#ifndef ANGRYBIRDS_GAME_H
#define ANGRYBIRDS_GAME_H

#include "Bird.h"
#include "Enemy.h"
#include "Player.h"
#include "Obstacle.h"
#include <SFML/Graphics.hpp>

class Game
{
 public:

  explicit Game(sf::RenderWindow& window);
  ~Game();

  bool init();
  void update(float dt);
  void render();

  void mouseClicked(sf::Event event);
  void mouseReleased(sf::Event event);
  void keyPressed(sf::Event event);

 private:

  static bool collisionCheckMouse(sf::Vector2i click, const sf::Text& text);
  static bool collisionCheckMouse(sf::Vector2i click, Bird& bird);

  static void loadTexture(sf::Texture& texture, const std::string& filename);
  void setBG(sf::Texture& texture, sf::Sprite& sprite, const std::string& img_filename);
  static void setTextOnScreen(sf::Font& font,
                       sf::Text& text,
                       const std::string& font_filename,
                       const std::string& text_on_screen,
                       int font_size, float posX, float posY);

  void cameraMovement(sf::Text& text);

  void endGameManager();

  void runGame(float dt);

  void setStartScreen();
  void setMainMenu();
  void setGameScreen();
  void setGameBG();
  void setEndScreen();
  void setPauseMenu();
  void initTextures();
  void initViews();
  void initPlayer(float posX, float posY);
  void initScore(float posX, float posY);
  void updateScore();
  void initLives(float posX, float posY);
  void initBird(float posX, float posY);
  void initLevelOne(float birds_posX, float bird_posY, float obs_posX, float obs_posY);

  void renderStartMenu();
  void renderMainMenu();
  void renderPauseMenu();
  void renderEndMenu();
  void renderGame();
  void renderCinematic();

  // Objects
  Bird* bird = nullptr;
  Bird* lives = nullptr;
  Enemy* pigs = nullptr;
  Player* player = nullptr;
  Player* sling_front = nullptr;
  Obstacle* obstacles = nullptr;

  int pig_gridX  = 3;
  int pig_gridY  = 2;
  int pig_grid = pig_gridX * pig_gridY;

  int obs_array_size = 2;

  // Active Selection
  bool play_selected = true;

  // Game States
  enum States{
    StartMenu = 0,
    MainMenu = 1,
    PauseMenu = 2,
    EndMenu = 3,
    GameRunning = 4,
    Cinematic = 5
  };
  States current_state;

  enum CStates
  {
    FWD = 0,
    BKW = 1
  };
  CStates c_state;

  // Views
  sf::View main_view;
  sf::View small_view;
  sf::RenderWindow& window;

  // Font
  sf::Font font_game;

  // Start Menu
  sf::Sprite start_menu_sprite;
  sf::Texture start_menu_texture;
  sf::Text start_menu_play;
  sf::Text start_menu_quit;

  //Main Menu
  sf::Sprite main_menu_sprite;
  sf::Texture main_menu_texture;
  sf::Text main_menu_rules;
  sf::Text main_menu_play;
  sf::Text main_menu_quit;

  // Pause Menu
  sf::Sprite pause_sprite;
  sf::Texture pause_texture;
  sf::Text pause_text;
  sf::Text pause_restart;
  sf::Text pause_quit;

  // End Screen
  sf::Sprite end_menu_sprite;
  sf::Texture end_menu_texture;
  sf::Text end_menu_text;
  sf::Text end_menu_play_again;
  sf::Text end_menu_quit;

  // Game
  sf::Sprite game_sprite;
  sf::Text score;

  sf::Texture game_texture;
  sf::Texture sling_texture_back;
  sf::Texture sling_texture_front;
  sf::Texture bird_texture;
  sf::Texture pig_texture;
  sf::Texture obstacle_texture;
};

#endif // ANGRYBIRDS_GAME_H
