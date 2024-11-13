
#include "Game.h"
#include <iostream>

Game::Game(sf::RenderWindow& game_window)
  : window(game_window)
{
  srand(time(nullptr));
}

Game::~Game()
{
  delete player;
  player = nullptr;
  delete sling_front;
  sling_front = nullptr;
  delete bird;
  bird = nullptr;
  delete[] lives;
  lives = nullptr;
  delete[] pigs;
  pigs = nullptr;
}

bool Game::init()
{
  current_state = StartMenu;
  c_state = FWD;

  player = new Player;
  sling_front = new Player;
  bird = new Bird;
  lives = new Bird[player->getLives()];
  pigs = new Enemy[pig_grid];
  obstacles = new Obstacle[obs_array_size];

  // Start Screen
  setStartScreen();

  // Main Menu
  setMainMenu();

  // Pause Menu
  setPauseMenu();

  // End Screen
  setEndScreen();

  // Game Screen
  setGameScreen();

  // Initialise Textures
  initTextures();

  // Initialise Views
  initViews();

  // Initialise Player
  float player_posX = window.getSize().x/3;
  float player_posY = window.getSize().y/2 + window.getSize().y/10;

  initPlayer(player_posX, player_posY);

  // Initialise Score
  float score_posX = window.getSize().x - window.getSize().x / 10;
  float score_posY = window.getSize().y / 10;

  initScore(score_posX, score_posY);

  // Initialise Lives
  float lives_posX = window.getSize().x/20;
  float lives_posY = window.getSize().y - window.getSize().y/8;

  initLives(lives_posX, lives_posY);

  // Initialise Bird
  initBird(Bird::getOriginX(window, *player), Bird::getOriginY(window, *player));

  // Initialise Level One
  float lvl_bird_posX = game_sprite.getGlobalBounds().width - game_sprite.getGlobalBounds().width/3 + 100;
  float lvl_bird_posY = game_sprite.getGlobalBounds().height - game_sprite.getGlobalBounds().height/3;
  float lvl_obs_posX = game_sprite.getGlobalBounds().width - game_sprite.getGlobalBounds().width/3 + 20;
  float lvl_obs_posY = game_sprite.getGlobalBounds().height - game_sprite.getGlobalBounds().height/3 - 70;

  initLevelOne(lvl_bird_posX, lvl_bird_posY, lvl_obs_posX, lvl_obs_posY);

  return true;
}

void Game::update(float dt)
{
  runGame(dt);
}

void Game::render()
{
  // Start Menu
  renderStartMenu();

  // Main Menu
  renderMainMenu();

  // Pause Menu
  renderPauseMenu();

  // End Menu
  renderEndMenu();

  // Game
  renderGame();

  // Cinematic
  renderCinematic();
}

/*----------------------------------------------------------------------------*/
// Inputs
// Keyboard
void Game::keyPressed(sf::Event event)
{
  // Start Menu
  if (current_state == StartMenu)
  {
    if ((event.key.code == sf::Keyboard::A) ||
        (event.key.code == sf::Keyboard::D))
    {
      play_selected = !play_selected;

      if(play_selected)
      {
        start_menu_play.setString(">Play<");
        start_menu_play.setStyle(sf::Text::Italic | sf::Text::Bold);
        start_menu_quit.setString("Quit");
        start_menu_quit.setStyle(sf::Text::Regular);
      }
      else
      {
        start_menu_play.setString("Play");
        start_menu_play.setStyle(sf::Text::Regular);
        start_menu_quit.setString(">Quit<");
        start_menu_quit.setStyle(sf::Text::Italic | sf::Text::Bold);
      }
    }

    else if (event.key.code == sf::Keyboard::Enter)
    {
      if(play_selected)
      {
        current_state = MainMenu;
      }
      else
      {
        window.close();
      }
    }
  }

  // Main Menu
  else if (current_state == MainMenu)
  {
    if ((event.key.code == sf::Keyboard::A) ||
        (event.key.code == sf::Keyboard::D))
    {
      play_selected = !play_selected;

      if(play_selected)
      {
        main_menu_play.setString(">Play<");
        main_menu_play.setStyle(sf::Text::Italic | sf::Text::Bold);
        main_menu_quit.setString("Quit");
        main_menu_quit.setStyle(sf::Text::Regular);
      }
      else
      {
        main_menu_play.setString("Play");
        main_menu_play.setStyle(sf::Text::Regular);
        main_menu_quit.setString(">Quit<");
        main_menu_quit.setStyle(sf::Text::Italic | sf::Text::Bold);
      }
    }

    else if (event.key.code == sf::Keyboard::Enter)
    {
      if(play_selected)
      {
        current_state = Cinematic;

      }
      else
      {
        window.close();
      }
    }
  }

  // Pause Menu
  else if (current_state == PauseMenu)
  {
    if (event.key.code == sf::Keyboard:: Escape)
    {
      current_state = GameRunning;
    }

    else if ((event.key.code == sf::Keyboard::A) ||
        (event.key.code == sf::Keyboard::D))
    {
      play_selected = !play_selected;

      if(play_selected)
      {
        pause_restart.setString(">Restart<");
        pause_restart.setStyle(sf::Text::Italic | sf::Text::Bold);
        pause_quit.setString("Quit");
        pause_quit.setStyle(sf::Text::Regular);
      }
      else
      {
        pause_restart.setString("Restart");
        pause_restart.setStyle(sf::Text::Regular);
        pause_quit.setString(">Quit<");
        pause_quit.setStyle(sf::Text::Italic | sf::Text::Bold);
      }
    }

    else if (event.key.code == sf::Keyboard::Enter)
    {
      if(play_selected)
      {
        current_state = Cinematic;
        player->resetLives();
        player->resetScore();
        Obstacle::resetVis(obstacles, obs_array_size, true);
        Enemy::resetVis(pigs, pig_grid, true);
        bird->resetPos(window, *player);
        bird->setState(Bird::Idle);
      }
      else
      {
        window.close();
      }
    }
  }

  // End Screen
  else if (current_state == EndMenu)
  {
    if ((event.key.code == sf::Keyboard::A) ||
        (event.key.code == sf::Keyboard::D))
    {
      play_selected = !play_selected;
      if(play_selected)
      {
        end_menu_play_again.setString(">Play Again<");
        end_menu_play_again.setStyle(sf::Text::Italic | sf::Text::Bold);
        end_menu_quit.setString("Quit");
        end_menu_quit.setStyle(sf::Text::Regular);
      }
      else
      {
        end_menu_play_again.setString("Play Again");
        end_menu_play_again.setStyle(sf::Text::Regular);
        end_menu_quit.setString(">Quit<");
        end_menu_quit.setStyle(sf::Text::Italic | sf::Text::Bold);
      }
    }

    else if (event.key.code == sf::Keyboard::Enter)
    {
      if(play_selected)
      {
        current_state = Cinematic;
        player->resetLives();
        player->resetScore();
        Obstacle::resetVis(obstacles, obs_array_size, true);
        Enemy::resetVis(pigs, pig_grid, true);
        bird->resetPos(window, *player);
        bird->setState(Bird::Idle);
      }
      else
      {
        window.close();
      }
    }
  }

  // Game
  else if (current_state == GameRunning)
  {
    // Pause
    if (event.key.code == sf::Keyboard:: Escape)
    {
      main_view.setCenter(window.getSize().x/2, window.getSize().y/2);
      current_state = PauseMenu;
    }
  }

  // Cinematic
  else if (current_state == Cinematic)
  {
    if (event.key.code == sf::Keyboard:: Escape)
    {
      current_state = GameRunning;
    }
  }
}

// Mouse
void Game::mouseClicked(sf::Event event)
{
  //get the click position
  sf::Vector2i click = sf::Mouse::getPosition(window);

  // Start Menu
  if (current_state == StartMenu)
  {
    if (collisionCheckMouse(click, start_menu_play))
    {
      current_state = MainMenu;
    }
    if (collisionCheckMouse(click, start_menu_quit))
    {
      window.close();
    }
  }

  // Main Menu
  else if (current_state == MainMenu)
  {
    if (collisionCheckMouse(click, main_menu_play))
    {
      current_state = Cinematic;
    }
    if (collisionCheckMouse(click, main_menu_quit))
    {
      window.close();
    }
  }

  // Pause Menu
  else if (current_state == PauseMenu)
  {
    if (collisionCheckMouse(click, pause_restart))
    {
      current_state = Cinematic;
      player->resetLives();
      player->resetScore();
      Obstacle::resetVis(obstacles, obs_array_size, true);
      Enemy::resetVis(pigs, pig_grid, true);
      bird->resetPos(window, *player);
      bird->setState(Bird::Idle);
    }
    if (collisionCheckMouse(click, pause_quit))
    {
      window.close();
    }
  }

  // End Screen
  else if (current_state == EndMenu)
  {
    if (collisionCheckMouse(click, end_menu_play_again))
    {
      current_state = Cinematic;
      player->resetLives();
      player->resetScore();
      Obstacle::resetVis(obstacles, obs_array_size, true);
      Enemy::resetVis(pigs, pig_grid, true);
      bird->resetPos(window, *player);
      bird->setState(Bird::Idle);
    }
    if (collisionCheckMouse(click, end_menu_quit))
    {
      window.close();
    }
  }

  // Game
  else if (current_state == GameRunning && bird->getState() == Bird::Idle)
  {
    if (collisionCheckMouse(click, *bird))
    {
      bird->setState(Bird::Held);
    }
  }
}
void Game::mouseReleased(sf::Event event)
{
  if (current_state == GameRunning && bird->getState() == Bird::Held)
  {
    bird->setState(Bird::Launched);
    bird->resetPos(window, *player);
  }
}

/*----------------------------------------------------------------------------*/
// Initialise Methods
// Start Screen
void Game::setStartScreen()
{
  // Background
  setBG(start_menu_texture, start_menu_sprite,
        "Data/Images/menu.jpg");

  // Play Option
  setTextOnScreen(font_game,
    start_menu_play,
                  "Data/Fonts/OpenSans-Bold.ttf",
                  ">Play<",
                  30,
                  window.getSize().x/3  + window.getSize().x/20,
                  window.getSize().y - window.getSize().y/5);

  // Quit Option
  setTextOnScreen(font_game,
    start_menu_quit,
                  "Data/Fonts/OpenSans-Bold.ttf",
                  "Quit",
                  30,
                  window.getSize().x - window.getSize().x/3 - window.getSize().x/20,
                  window.getSize().y - window.getSize().y/5);
}

// Main Menu
void Game::setMainMenu()
{
  // Background
  setBG(main_menu_texture, main_menu_sprite,
        "Data/Images/pause_bg.png");

  // Rules
  setTextOnScreen(font_game,
    main_menu_rules,
                  "Data/Fonts/OpenSans-Bold.ttf",
                  "CLICK and DRAG the bird to choose the trajectory.\n"
                  "Release to shot!\nIf you hit an obstacle or miss, you lose a life.\n"
                  "If you run out of lives, it's GAME OVER!\n"
                  "Hit all the enemies before you run out of lives to WIN!\n\n"
                  "(Press 'Esc' to skip cinematic or pause the game)",
                  30,
                  window.getSize().x/2,
                  window.getSize().y/2 - window.getSize().y/10);

  // Play Option
  setTextOnScreen(font_game,
    main_menu_play,
                  "Data/Fonts/OpenSans-Bold.ttf",
                  ">Play<",
                  30,
                  window.getSize().x/3  + window.getSize().x/20,
                  window.getSize().y - window.getSize().y/5);

  // Quit Option
  setTextOnScreen(font_game,
    main_menu_quit,
                  "Data/Fonts/OpenSans-Bold.ttf",
                  "Quit",
                  30,
                  window.getSize().x - window.getSize().x/3 - window.getSize().x/20,
                  window.getSize().y - window.getSize().y/5);
}

// Game Screen
void Game::setGameScreen()
{
  // BG GAME
  setGameBG();

}
void Game::setGameBG()
{
  loadTexture(game_texture, "Data/Images/lvl1.png");
  game_sprite.setTexture(game_texture);
  game_texture.setRepeated(true);

  game_sprite.setScale
    (
      window.getSize().x / game_sprite.getGlobalBounds().width,
      window.getSize().y / game_sprite.getGlobalBounds().height
    );

  game_sprite.setTextureRect(sf::IntRect(0.f, 0.f,
    game_sprite.getLocalBounds().width * 1.5f,
    game_sprite.getLocalBounds().height));

  game_sprite.setPosition(0, 0);
}

// End Screen
void Game::setEndScreen()
{
  // BG
  setBG(end_menu_texture, end_menu_sprite,
        "Data/Images/end_bg.png");

  // Text
  setTextOnScreen(font_game,
    end_menu_text,
                  "Data/Fonts/OpenSans-Bold.ttf",
                  "GAME OVER",
                  80,
                  window.getSize().x/2,
                  window.getSize().y/3 );

  // Play Again Option
  setTextOnScreen(font_game,
    end_menu_play_again,
                  "Data/Fonts/OpenSans-Bold.ttf",
                  ">Play Again<",
                  30,
                  window.getSize().x / 3,
                  window.getSize().y - window.getSize().y / 3);

  // Quit option
  setTextOnScreen(font_game,
    end_menu_quit,
                  "Data/Fonts/OpenSans-Bold.ttf",
                  "Quit",
                  30,
                  window.getSize().x - window.getSize().x / 3,
                  window.getSize().y - window.getSize().y / 3);
}

// Pause Screen
void Game::setPauseMenu()
{
  // Background
  setBG(pause_texture, pause_sprite,
        "Data/Images/pause_bg.png");

  // Pause Text
  setTextOnScreen(font_game,
    pause_text,
                  "Data/Fonts/OpenSans-Bold.ttf",
                  "PAUSE",
                  80,
                  window.getSize().x / 2,
                  window.getSize().y / 3);

  // Restart Option
  setTextOnScreen(font_game,
    pause_restart,
                  "Data/Fonts/OpenSans-Bold.ttf",
                  ">Restart<",
                  30,
                  window.getSize().x / 3,
                  window.getSize().y - window.getSize().y / 3);
  // Quit option
  setTextOnScreen(font_game,
    pause_quit,
                  "Data/Fonts/OpenSans-Bold.ttf",
                  "Quit",
                  30,
                  window.getSize().x - window.getSize().x / 3,
                  window.getSize().y - window.getSize().y / 3);
}

// Textures
void Game::initTextures()
{
  // Load Bird Texture
  loadTexture(bird_texture, "Data/Images/kenney_animalpackredux/PNG/Round (outline)/parrot.png");

  // Load Player Texture
  loadTexture(sling_texture_back, "Data/Images/slingshot_full.png");
  loadTexture(sling_texture_front, "Data/Images/slingshot_leftbranch.png");

  // Load Pig Texture
  loadTexture(pig_texture, "Data/Images/kenney_animalpackredux/PNG/Round (outline)/pig.png");

  // Load Obstacle Texture
  loadTexture(obstacle_texture, "Data/Images/kenney_physicspack/PNG/Wood elements/elementWood019.png");
}

// Views
void Game::initViews()
{
  main_view.setCenter(window.getSize().x/2, window.getSize().y/2);
  main_view.setSize(window.getSize().x, window.getSize().y);

  small_view.setCenter(
    game_sprite.getLocalBounds().width/2,
    game_sprite.getLocalBounds().height/2);
  small_view.setSize(
    game_sprite.getLocalBounds().width, game_sprite.getLocalBounds().height);

  main_view.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
  small_view.setViewport(sf::FloatRect(0.f, 0.f, 0.5f, 0.25f));
}

// Player
void Game::initPlayer(float posX, float posY)
{
  player->setTexture(sling_texture_back);
  player->getSprite()->setPosition(posX, posY);
  sling_front->setTexture(sling_texture_front);
  sling_front->getSprite()->setPosition(posX,posY);
}

// Score
void Game::initScore(float posX, float posY)
{
  setTextOnScreen(font_game,
                  score,
                  "Data/Fonts/OpenSans-Bold.ttf",
                  "Score: ",
                  30,
                  posX,
                  posY);
}
void Game::updateScore()
{
  score.setString("Score:" + std::to_string(player->getScore()));
}

// Lives
void Game::initLives(float posX, float posY)
  {
    for (int i = 0; i < player->getLives(); i++)
    {
      lives[i].setTexture(bird_texture);
      lives[i].getSprite()->setPosition(posX * i, posY);
    }
  }

// Bird
void Game::initBird(float posX, float posY)
{
  bird->setTexture(bird_texture);
  bird->getSprite()->setPosition(posX, posY);
}

// First Level
void Game::initLevelOne(float birds_posX, float bird_posY,
                        float obs_posX, float obs_posY)
{
  for (int i = 0; i < pig_gridX; ++i)
  {
    for (int j = 0; j < pig_gridY; ++j)
    {
      pigs[i * pig_gridY + j].setTexture(pig_texture);
      pigs[i * pig_gridY + j].getSprite()->setPosition(
        birds_posX +80.f * i, bird_posY + 80.f * j);
    }
  }

  for (int i = 0; i < obs_array_size; ++i)
  {
    obstacles[i].setTexture(obstacle_texture);
    obstacles[i].getSprite()->setPosition(obs_posX + 325* i, obs_posY);
  }
}

/*----------------------------------------------------------------------------*/
// Game Methods
// Text Collision
bool Game::collisionCheckMouse(sf::Vector2i click, const sf::Text& text)
{
  if (click.x > text.getPosition().x
      && click.y > text.getPosition().y
      && click.x < text.getPosition().x + text.getGlobalBounds().width
      && click.y < text.getPosition().y + text.getGlobalBounds().height)
  {
    return true;
  }

  else
  {
    return false;
  }
}
bool Game::collisionCheckMouse(sf::Vector2i click, Bird& bird)
{
  if (click.x > bird.getSprite()->getPosition().x
      && click.y > bird.getSprite()->getPosition().y
      && click.x < bird.getSprite()->getPosition().x + bird.getSprite()->getGlobalBounds().width
      && click.y < bird.getSprite()->getPosition().y + bird.getSprite()->getGlobalBounds().height)
  {
    return true;
  }

  else
  {
    return false;
  }
}

// Load Texture
void Game::loadTexture(sf::Texture& texture, const std::string& filename)
{
  if (!texture.loadFromFile(filename) )
  {
    std::cout <<"Texture not loaded\n";
  }
}

// Set BG
void Game::setBG(sf::Texture& texture,
                 sf::Sprite& sprite,
                 const std::string& img_filename)
{
  loadTexture(texture, img_filename);
  sprite.setTexture(texture);

  float scaleX = window.getSize().x/sprite.getGlobalBounds().width;
  float scaleY = window.getSize().y/sprite.getGlobalBounds().height;

  sprite.setScale(scaleX, scaleY);
  sprite.setPosition(0, 0);
}

// Set Text
void Game::setTextOnScreen(sf::Font& font,
                           sf::Text& text,
                           const std::string& font_filename,
                           const std::string& text_on_screen,
                           int font_size,
                           float posX,
                           float posY)
{
  if (!font.loadFromFile(font_filename))
  {
    std::cout<<"Font not loaded\n";
  }
  text.setString(text_on_screen);
  text.setStyle(sf::Text::Italic | sf::Text::Bold);
  text.setFont(font);
  text.setCharacterSize(font_size);
  text.setFillColor(sf::Color(0, 0, 0, 255));

  float text_centerX = text.getGlobalBounds().width/2;
  float text_centerY = text.getGlobalBounds().height/2;
  text.setPosition(posX - text_centerX, posY - text_centerY);
}
// Game Manager
void Game::endGameManager()
{
  int enemy_visible = 0;

  for (int i = 0; i < pig_grid; ++i)
  {

    if (!pigs[i].getVisibility())
    {
      enemy_visible += 1;
    }

  }

  if (player->getLives() == 0 || enemy_visible == pig_grid) // add pig win condition
  {
    current_state = EndMenu;
  }
}

// Camera
void Game::cameraMovement(sf::Text& text)
{
  float bird_posX   = bird->getSprite()->getPosition().x;
  float bg_size     = game_sprite.getGlobalBounds().width;
  float win_centerX = window.getSize().x/2;
  float win_centerY = window.getSize().y/2;
  float text_centerX = text.getGlobalBounds().width/2;
  float text_centerY = text.getGlobalBounds().height/2;

  if (current_state == Cinematic)
  {
    float const CAM_MOVE_SPEED = 3.f;

    if (c_state == FWD)
    {
      main_view.move(CAM_MOVE_SPEED, 0.f);

      if (main_view.getCenter().x >= bg_size - win_centerX)
      {
        c_state = BKW;
      }
    }

    if (c_state == BKW)
    {
      main_view.move(-CAM_MOVE_SPEED, 0.f);

      if ( main_view.getCenter().x <= win_centerX )
      {
        c_state = FWD;
        current_state = GameRunning;
      }
    }
  }

  if (current_state == GameRunning)
  {
    if (bird_posX < win_centerX)
    {
      main_view.setCenter(win_centerX, win_centerY);
    }
    else if (bird_posX >= win_centerX && bird_posX <= bg_size - win_centerX)
    {
      main_view.setCenter(bird_posX, win_centerY);
    }
    else
    {
      main_view.setCenter((bg_size - win_centerX), win_centerY);
    }
    text.setPosition(
      main_view.getCenter().x + win_centerX - window.getSize().x / 10 -
        text_centerX, window.getSize().y / 10 - text_centerY);
    for (int i = 0; i < player->getLives(); i++)
    {
      lives[i].getSprite()->setPosition(
        main_view.getCenter().x - win_centerX + window.getSize().x/20 * i, lives[i].getSprite()->getPosition().y);
    }
  }
  else
  {
    if (current_state == GameRunning)
    {
      main_view.setCenter(win_centerX, win_centerY);
    }

  }
}

// Game
void Game::runGame(float dt)
{
  // Get the click position
  sf::Vector2i click = sf::Mouse::getPosition(window);

  if (current_state == GameRunning)
  {
    bird->grab(window, *player, click);

    bird->launch(window,
      game_sprite,
      main_view,*player, pigs,
      pig_grid, obstacles,
      obs_array_size, dt);

    updateScore();

    endGameManager();
  }
  cameraMovement(score);
}

// Render
void Game::renderStartMenu()
{
  if (current_state == StartMenu)
  {
    window.setView(main_view);
    window.draw(start_menu_sprite);
    window.draw(start_menu_play);
    window.draw(start_menu_quit);
  }
}
void Game::renderMainMenu()
{
  if (current_state == MainMenu)
  {
    window.setView(main_view);
    window.draw(main_menu_sprite);
    window.draw(main_menu_rules);
    window.draw(main_menu_play);
    window.draw(main_menu_quit);
  }
}
void Game::renderPauseMenu()
{
  if (current_state == PauseMenu)
  {
    window.setView(main_view);
    window.draw(pause_sprite);
    window.draw(pause_text);
    window.draw(pause_restart);
    window.draw(pause_quit);
  }
}
void Game::renderEndMenu()
{
  if (current_state == EndMenu)
  {
    window.setView(main_view);
    window.draw(end_menu_sprite);
    window.draw(end_menu_text);
    window.draw(end_menu_play_again);
    window.draw(end_menu_quit);
  }
}
void Game::renderGame()
{
  if (current_state == GameRunning)
  {
    // Main View
    window.setView(main_view);
    window.draw(game_sprite);
    window.draw(score);
    window.draw(*player->getSprite());
    window.draw(*bird->getSprite());
    window.draw(*sling_front->getSprite());

    for (int i = 0; i < player->getLives(); i++)
    {
      window.draw(*lives[i].getSprite());
    }

    for (int i = 0; i < pig_grid; i++)
    {
      if (pigs[i].getVisibility())
      {
        window.draw(*pigs[i].getSprite());
      }
    }

    for (int i = 0; i < obs_array_size; i++)
    {
      if (obstacles[i].getVisibility())
      {
        window.draw(*obstacles[i].getSprite());
      }
    }

    // Small View
    window.setView(small_view);
    window.draw(game_sprite);
    window.draw(*player->getSprite());
    window.draw(*bird->getSprite());
    window.draw(*sling_front->getSprite());

    for (int i = 0; i < pig_grid; i++)
    {
      if (pigs[i].getVisibility())
      {
        window.draw(*pigs[i].getSprite());
      }
    }

    for (int i = 0; i < obs_array_size; i++)
    {
      if (obstacles[i].getVisibility())
      {
        window.draw(*obstacles[i].getSprite());
      }
    }
  }
}
void Game::renderCinematic()
{
  if (current_state == Cinematic)
  {
    window.setView(main_view);
    window.draw(game_sprite);
    window.draw(*player->getSprite());
    window.draw(*bird->getSprite());
    window.draw(*sling_front->getSprite());

    for (int i = 0; i < pig_grid; i++)
    {
      if (pigs[i].getVisibility())
      {
        window.draw(*pigs[i].getSprite());
      }
    }

    for (int i = 0; i < obs_array_size; i++)
    {
      if (obstacles[i].getVisibility())
      {
        window.draw(*obstacles[i].getSprite());
      }
    }
  }

}
