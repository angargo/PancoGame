#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

const int FRAMES_PER_SECOND = 60.f;

// Game Class
class Game{
  public:
    Game();
    void run();

  private:
    void update(Time dt);
    void processEvents();
    void render();
    void handlePlayerInput(Keyboard::Key key, bool isPressed);

  private:
    RenderWindow mWindow;
    // Texture and Sprite, should be array of both or sumething
    // now there's only one tho.
    Texture mTexture;
    Sprite mPlayer;
    // Store the Player Movement
    bool mIsMovingUp;
    bool mIsMovingDown;
    bool mIsMovingRight;
    bool mIsMovingLeft;
    float playerSpeed; // Pixels it should move in one second
    Time timePerFrame;
};

// Constructor
Game::Game()
:mWindow(VideoMode(640,480), "Mobility test")
,mTexture()
,mPlayer()
,mIsMovingUp(false)
,mIsMovingDown(false)
,mIsMovingLeft(false)
,mIsMovingRight(false)
,playerSpeed(100.f)
,timePerFrame(seconds(1.f/FRAMES_PER_SECOND)){
  // Loads only the needed part from the link.png textures file
  if (!mTexture.loadFromFile("./textures/link.png", IntRect(124,1028,17,24))){
    cerr << "Wololo! Link was not found :'(" << endl;
  }
  mPlayer.setTexture(mTexture);
  mPlayer.setPosition(100.f,100.f);
}

// Loop Handler
void Game::run(){
  Clock clock; // Main timer
  // By keeping time since last update fixed-time-steps
  // can be implemented
  Time timeSinceLastUpdate = Time::Zero;
  while(mWindow.isOpen()){
    processEvents();
    timeSinceLastUpdate += clock.restart();
    // If some loops are very quick, it keeps dropping them
    // It only executes updates if it has passed more time than
    // the timePerFrame. Then it keeps executing updates until
    // timeSinceLastUpdate is lower again. So if one loop takes
    // a lot of time, the program corrects it by executing some
    // inner loops. It may stutter the program (it first seems that
    // the game is frozen but then, in the next render, it seems
    // like a lot of frames have passed -they actually have-)
    // It may seem undesirable, but solves collision problems.
    while(timeSinceLastUpdate > timePerFrame){
      timeSinceLastUpdate -= timePerFrame;
      processEvents();
      update(timePerFrame);
    }
    render();
  }
}

// Process User Input
void Game::processEvents(){
  Event event;
  while(mWindow.pollEvent(event)){
    switch (event.type){
      case Event::KeyPressed :
        handlePlayerInput(event.key.code,true);
        break;
      case Event::KeyReleased :
        handlePlayerInput(event.key.code,false);
        break;
      case Event::Closed :
        mWindow.close();
    }
  }
}

// Handles User Input
void Game::handlePlayerInput(Keyboard::Key key, bool isPressed){
  if(key == Keyboard::W || key == Keyboard::Up){
    mIsMovingUp = isPressed;
  }
  else if(key == Keyboard::S || key == Keyboard::Down){
    mIsMovingDown = isPressed;
  }
  else if(key == Keyboard::A || key == Keyboard::Left){
    mIsMovingLeft = isPressed;
  }
  else if(key == Keyboard::D || key == Keyboard::Right){
    mIsMovingRight = isPressed;
  }
}

// Update Game Logic
void Game::update(Time dt){ // Uses dt to allow Frame-Independent Movement
  Vector2f movement (0.f,0.f);
  if(mIsMovingUp){
    movement.y -= playerSpeed;
  }
  if(mIsMovingDown){
    movement.y += playerSpeed;
  }
  if(mIsMovingLeft){
    movement.x -= playerSpeed;
  }
  if(mIsMovingRight){
    movement.x += playerSpeed;
  }
  
  mPlayer.move(movement * dt.asSeconds());
}

// Render Window
void Game::render(){
  mWindow.clear();
  mWindow.draw(mPlayer);
  mWindow.display();
}

int main(){
  Game game;
  game.run();
}
