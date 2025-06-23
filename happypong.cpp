#include <iostream>
#include <raylib.h>
#include <math.h>

using namespace std;

/*
Sound rebond_ballon = {0};
Sound petit_chat = {0};
Sound Chouette = {0};
*/

Texture2D zero64x64 = {0};
Texture2D one64x64 = {0};
Texture2D two64x64 = {0};
Texture2D three64x64 = {0};
Texture2D four64x64 = {0};
Texture2D five64x64 = {0};
Texture2D six64x64 = {0};
Texture2D seven64x64 = {0};
Texture2D eight64x64 = {0};
Texture2D nine64x64 = {0};
Texture2D HAPPY_PONG512x512 = {0};
Texture2D left_paw = {0};
Texture2D rabbit_eyes = {0};
Texture2D rabbitHeart = {0};
Texture2D right_paw = {0};
Texture2D spark = {0};
//Texture2D star128x512 = {0};
Texture2D sevenStars512x512 = {0};
//Texture2D you_lose256x256 = {0};
//Texture2D you_win256x256 = {0};

Texture2D credits512x64 = {0};
Texture2D QRCodeNB64x64 = {0};
Texture2D pinkCorners = {0};

// Les variables

//player variables
int player_score = 0;
float player_h = 64;
float player_w = 8;
float player_x = 48;
//float player_y = (GetScreenHeight() - player_h) / 2;
float player_y = 208;
int player_vymax = 16;
int player_vy = 0;
int player_timeoutmax = 220;
int player_timeout = 0;
float player_bump = 0;
int playerMode = 1; // 1 buttons, 2 stick or 3 trigger
float previous_player_y = 208;
//bool playerEffect = false;

//opponent variables
int opponent_score = 0;
float opponent_h = 64;
float opponent_w = 8;
//float opponent_x = GetScreenWidth() - opponent_w - 48;
float opponent_x = 584;
//float opponent_y = (GetScreenHeight() - opponent_h) / 2;
float opponent_y = 208;
int opponent_vymax = 16;
int opponent_vy = 0;
int opponent_timeoutmax = 220;
int opponent_timeout = 0;
float opponent_bump = 0;
int opponentMode = 1; // 1 buttons, 2 stick or 3 trigger
float previous_opponent_y = 208;
//bool opponentEffect = false;

//ball variables
float ball_size = 24;
//float ball_x = opponent_x - ball_size - 20;
float ball_x = 84;
//float ball_y = (GetScreenHeight() - ball_size) / 2;
float ball_y = 228;

int ball_vx = 8;
int ball_vy = 0;
int ball_vymax = 24;
int star_number = 1;
int rotate_counter = 0;

int previous_ball_vy = 0;

float starColor = 0; // 0 yellow, 1 orange, 2 red, 3 blue, 4 green

int specialScore = 0;

bool specialMove = false;
int dash_vx = 6;

//bool endEffect = false;

//rabbit eyes
float eyes_x = 0;
float eyes_y = 0;

// sparks
float sparkCounter = 30;
float xSpark = 288;
float ySpark = 224;
float xSpark1 = 288; 
float ySpark1 = 224;
float xSpark2 = 288;
float ySpark2 = 224;
float xSpark3 = 288;
float ySpark3 = 224;
float xSpark4 = 288;
float ySpark4 = 224;
float xSpark5 = 288;
float ySpark5 = 224;

#define PLAYER 1;
#define OPPONENT 0;

int winner;

float numberBump1 = 0;
float numberBump2 = 0;

int gameMode = 0;

bool clockwise = true;
float win_rotation = 0;

bool win_bigger = true;
float win_size = 0;

bool twoControllers = false;

//Rectangle rec_ball = {ball_x, ball_y, ball_size, ball_size};
//Rectangle rec_player = {player_x, player_y, player_w, player_h};
//Rectangle rec_opponent = {opponent_x, opponent_y, opponent_w, opponent_h};

// the functions

void checkPlayerButtons() {
  // up and down from left D-pad
  if((IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_UP)) || (IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN))) {
    playerMode = 1;
    player_timeout = player_timeoutmax;
  }
  // left stick
  if((GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y) < -0.2) || (GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y) > 0.2)) {
    playerMode = 2;
    player_timeout = player_timeoutmax;
  }
  // left trigger
  //if(GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_TRIGGER) > -0.8) {
  if(GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_TRIGGER) > 0.2) {
    playerMode = 3;
    player_timeout = player_timeoutmax;
  }
}

///////////////////////////////////// MOVE PLAYER
void movePlayer() {

  checkPlayerButtons();

  switch (playerMode) {
    // human control
    // up and down D-pad buttons
    case 1:
    if(IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_UP)) {
      player_vy -= 4;
    } else if(IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN)) {
      player_vy += 4;
    } else if (player_timeout > 0) {
      player_vy = player_vy / 2;
    }
    break;
    // left stick control
    case 2:
    previous_player_y = player_y;
    //player_y = (480 - player_h)/2 + (GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y)) * 240;
    player_y = ((480 - player_h)/2 + (GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y)) * 240 + previous_player_y) / 2;
    player_vy = player_y - previous_player_y;
    break;
    // left trigger control
    case 3:
    previous_player_y = player_y;
    //player_y = (-player_h)/2 + (GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_TRIGGER)) * 480;
    player_y = ((-player_h)/2 + (GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_TRIGGER)) * 480 + previous_player_y) / 2;
    player_vy = player_y - previous_player_y;
    break;
  }

  //computer control after timeout
  if (player_timeout > 0) {
    player_timeout--;
  } else if ((player_y + (player_h / 2)) < (ball_y + (ball_size / 2))) { //if the ball is below the opponent
    player_vy = player_vy + 4; // accelerate down
    playerMode = 1;
  } else {
    player_vy = player_vy - 4; // accelerate up
    playerMode = 1;
  }

// limit the player speed
  if(player_vy < (-player_vymax)) {
    player_vy = -player_vymax;
  }
   if(player_vy > (player_vymax)) {
    player_vy = player_vymax;
  }

// move the player if stick and trigger aren't touched
  if((playerMode == 1) || (player_timeout == 0)) {
    player_y = player_y + player_vy;
  }
// keep the left paw inside the screen
   if(player_y < -(player_h/2)) {
    player_y = -(player_h/2);
  }
//   if(player_y > (GetScreenHeight() - (player_h/2))) {
//    player_y = GetScreenHeight() - (player_h/2);
//  }
  if(player_y > (480 - (player_h/2))) {
    player_y = 480 - (player_h/2);
  }

}

void colorTheStar() {
  switch (specialScore) {
    case -2:
      starColor = 3;
    break;
    case -1:
      starColor = 4;
    break;
    case 0:
      starColor = 0;
    break;
    case 1:
      starColor = 1;
    break;
    case 2:
      starColor = 2;
    break;
  }
}

void checkOpponentButtons() {
  if(twoControllers == true) {
    // buttons Y and A (Dreamcast or XBOX buttons positions)
    if((IsGamepadButtonDown(1, GAMEPAD_BUTTON_RIGHT_FACE_UP)) || (IsGamepadButtonDown(1, GAMEPAD_BUTTON_RIGHT_FACE_DOWN))) {
      opponentMode = 1;
      opponent_timeout = opponent_timeoutmax;
    }
    // right stick
    if((GetGamepadAxisMovement(1, GAMEPAD_AXIS_RIGHT_Y) < -0.2) || (GetGamepadAxisMovement(1, GAMEPAD_AXIS_RIGHT_Y) > 0.2)) {
      opponentMode = 2;
      opponent_timeout = opponent_timeoutmax;
    }
    // right trigger
    if(GetGamepadAxisMovement(1, GAMEPAD_AXIS_RIGHT_TRIGGER) > 0.2) { 
      opponentMode = 3;
      opponent_timeout = opponent_timeoutmax;
    }

  } else {
    
    // buttons Y and A (Dreamcast or XBOX buttons positions)
    if((IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_UP)) || (IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN))) {
      opponentMode = 1;
      opponent_timeout = opponent_timeoutmax;
    }
    // right stick
    if((GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_Y) < -0.2) || (GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_Y) > 0.2)) {
      opponentMode = 2;
      opponent_timeout = opponent_timeoutmax;
    }
    // right trigger
    if(GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_TRIGGER) > 0.2) { 
      opponentMode = 3;
      opponent_timeout = opponent_timeoutmax;
    }
  }
}

///////////////////////////////////// MOVE OPPONENT
void moveOpponent() {

  checkOpponentButtons();

  // human control
  switch (opponentMode) {

  // A and Y buttons control
    case 1:
      if(twoControllers == true) {
        if(IsGamepadButtonDown(1, GAMEPAD_BUTTON_RIGHT_FACE_UP)) {
          opponent_vy -= 4;
        } else if (IsGamepadButtonDown(1, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) {  
          opponent_vy += 4;
        } else if (opponent_timeout > 0) {
          opponent_vy = opponent_vy / 2;
        }
      } else {
        if(IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_UP)) {
          opponent_vy -= 4;
        } else if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) {  
          opponent_vy += 4;
        } else if (opponent_timeout > 0) {
          opponent_vy = opponent_vy / 2;
        }
      }
    break;

  // right stick control (yes)
    case 2:
      if(twoControllers == true) {
        previous_opponent_y = opponent_y;
        //opponent_y = (480 - opponent_h)/2 + (GetGamepadAxisMovement(1, GAMEPAD_AXIS_RIGHT_Y)) * 240;
        opponent_y = ((480 - opponent_h)/2 + (GetGamepadAxisMovement(1, GAMEPAD_AXIS_RIGHT_Y)) * 240 + previous_opponent_y) / 2;
        opponent_vy = opponent_y - previous_opponent_y;
      } else {
        previous_opponent_y = opponent_y;
        //opponent_y = (480 - opponent_h)/2 + (GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_Y)) * 240;
        opponent_y = ((480 - opponent_h)/2 + (GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_Y)) * 240 + previous_opponent_y) / 2;
        opponent_vy = opponent_y - previous_opponent_y;
      }
    break;

  // right trigger control
    case 3:
      if(twoControllers == true) {
        previous_opponent_y = opponent_y;
        //opponent_y = (-opponent_h)/2 + (GetGamepadAxisMovement(1, GAMEPAD_AXIS_RIGHT_TRIGGER)) * 480;
        opponent_y = ((-opponent_h)/2 + (GetGamepadAxisMovement(1, GAMEPAD_AXIS_RIGHT_TRIGGER)) * 480 + previous_opponent_y) / 2;
        opponent_vy = opponent_y - previous_opponent_y;
      } else {
        previous_opponent_y = opponent_y;
        //opponent_y = (-opponent_h)/2 + (GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_TRIGGER)) * 480;
        opponent_y = ((-opponent_h)/2 + (GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_TRIGGER)) * 480 + previous_opponent_y) / 2;
        opponent_vy = opponent_y - previous_opponent_y;
      }
    break;

  }

  //computer control after timeout
  if (opponent_timeout > 0) {
    opponent_timeout--;
  } else if ((opponent_y + (opponent_h / 2)) < (ball_y + (ball_size / 2))) { //if the ball is below the opponent
    opponent_vy = opponent_vy + 4; //move down
    opponentMode = 1;
  } else {
    opponent_vy = opponent_vy - 4; //move up
    opponentMode = 1;
  }

// limit the opponent speed
  if(opponent_vy < (-opponent_vymax)) {
    opponent_vy = -opponent_vymax;
  }
  if(opponent_vy > opponent_vymax) {
    opponent_vy = opponent_vymax;
  }

// move the opponent if stick and trigger aren't touched
  if((opponentMode == 1) || (opponent_timeout == 0)) {
    opponent_y = opponent_y + opponent_vy;
  }

// keep the right paw inside the screen
  if(opponent_y < -(opponent_h/2)) {
    opponent_y = -(opponent_h/2);
  }
//  if(opponent_y > (GetScreenHeight() - (opponent_h/2))) {
//    opponent_y = (GetScreenHeight() - (opponent_h/2));
//  }
  if(opponent_y > (480 - (opponent_h/2))) {
    opponent_y = (480 - (opponent_h/2));
  }
}

///////////////////////////////////// UPDATE COLLISIONS
void updateCollisions() {
  //check for the main ball's collisions
  
  //collision with the top border
  if (ball_y < 0) {
    ball_y = 0;
    ball_vy = -ball_vy;
  //  PlaySound(poc);
  }

  //collision with the bottom border
  if ((ball_y + ball_size) > 480) {
    ball_y = 480 - ball_size;
    ball_vy = -ball_vy;
  //  PlaySound(poc);
  }


  //collision with the player
  if((ball_x + ball_size > player_x) && (ball_x < player_x + player_w) &&
  (ball_y + ball_size > player_y) && (ball_y < player_y + player_h)) {
    ball_x = player_x + player_w;
    ball_vx = -ball_vx;
    
    previous_ball_vy = ball_vy;

    //add some effet to the ball
    ball_vy += player_vy / 2;

    // check if the player gives speed up effect to the ball
    /*
    if(previous_ball_vy == 0) {
      if(ball_vy != previous_ball_vy) {
        playerEffects = playerEffects + 1;
      }
    } else if(previous_ball_vy > 0) {
      if(ball_vy > previous_ball_vy) {
        playerEffects = playerEffects + 1;
      }
    } else {
      if(ball_vy < previous_ball_vy) {
        playerEffects = playerEffects + 1;
      }
    }
    */

    /*
    if(previous_ball_vy == 0) {
      if(ball_vy != previous_ball_vy) {
        playerEffect = true;
      } else {
        playerEffect = false;
      }
    } else if(previous_ball_vy > 0) {
      if(ball_vy > previous_ball_vy) {
        playerEffect = true;
      } else {
        playerEffect = false;
      }
    } else {
      if(ball_vy < previous_ball_vy) {
        playerEffect = true;
      } else {
        playerEffect = false;
      }
    }
    */

    /*
    if(playerEffect == true) {
      switch (playerMode) {
        case 1:
          starColor = 4; // green
        break;
        case 2:
          starColor = 3; // blue
        break;
        case 3:
          starColor = 2; // red
        break;
      }
    } else {
      starColor = 0; // yellow
    }
    */

    // last effect
    //if(playerEffects > 6) {
    //  playerEffects = 6;
    //}
    
    specialMove = false;

    if(playerMode == 2) {
      specialScore = specialScore - 1;
      if(specialScore < -2) {
        specialScore = 0;
        specialMove = true;
      }
    }

    if(playerMode == 3) {
      specialScore = specialScore - 2;
      if(specialScore < -2) {
        specialScore = 0;
        specialMove = true;
      }
    }

    colorTheStar();

    // limit the vertical speed of the ball
    if(ball_vy < (-ball_vymax)) {
      ball_vy = -ball_vymax;
    }
    if(ball_vy > ball_vymax) {
      ball_vy = ball_vymax;
    }

  // PlaySound(rebond_ballon);

    //endEffect = false;

  }

//collision with the opponent
  if((ball_x + ball_size > opponent_x) && (ball_x < opponent_x + opponent_w) &&
  (ball_y + ball_size > opponent_y) && (ball_y < opponent_y + opponent_h)) {
    ball_x = opponent_x - ball_size;
    ball_vx = -ball_vx;

   previous_ball_vy = ball_vy;

    //add some effet to the ball
    ball_vy += opponent_vy / 2;

    // check if the opponent gives speed up effect to the ball
    /*
    if(previous_ball_vy == 0) {
      if(ball_vy != previous_ball_vy) {
        opponentEffects = opponentEffects + 1;
      }
    } else if(previous_ball_vy > 0) {
      if(ball_vy > previous_ball_vy) {
        opponentEffects = opponentEffects + 1;
      }
    } else {
      if(ball_vy < previous_ball_vy) {
        opponentEffects = opponentEffects + 1;
      }
    }
    */

    /*
    if(previous_ball_vy == 0) {
      if(ball_vy != previous_ball_vy) {
        opponentEffect = true;
      } else {
        opponentEffect = false;
      }
    } else if(previous_ball_vy > 0) {
      if(ball_vy > previous_ball_vy) {
        opponentEffect = true;
      } else {
        opponentEffect = false;
      }
    } else {
      if(ball_vy < previous_ball_vy) {
        opponentEffect = true;
      } else {
        opponentEffect = false;
      }
    }
    */

    /*
    if(opponentEffect == true) {
      switch (opponentMode) {
        case 1:
          starColor = 4; // green
        break;
        case 2:
          starColor = 3; // blue
        break;
        case 3:
          starColor = 2; // red
        break;
      }
    } else {
      starColor = 0; // yellow
    }
    */


    // last effect
    //if(opponentEffects > 6) {
    //  opponentEffects = 6;
    //}

    specialMove = false;

    if(opponentMode == 2) {
      specialScore = specialScore + 1;
      if(specialScore > 2) {
        specialScore = 0;
        specialMove = true;
      }
    }

    if(opponentMode == 3) {
      specialScore = specialScore + 2;
      if(specialScore > 2) {
        specialScore = 0;
        specialMove = true;
      }
    }

    colorTheStar();

    // limit the vertical speed of the ball
    if(ball_vy < (-ball_vymax)) {
      ball_vy = -ball_vymax;
    }
    if(ball_vy > ball_vymax) {
      ball_vy = ball_vymax;
    }

  // PlaySound(rebond_ballon);

    //endEffect = false;

  }

//collision with the left side
  if (ball_x < 0) {
    opponent_score = opponent_score + 1;

   // playerEffect = false;
   // opponentEffect = false;

// opponent score bumps
    numberBump2 = 4;   
    
    if(opponent_score < 10) {
    //  PlaySound(petit_chat);
    }

    ball_x = opponent_x - 16;
    ball_vx = -abs(ball_vx);
    ball_vy = 0;
  //  ball_y = GetRandomValue(0, GetScreenHeight() - ball_size - 1);
    ball_y = GetRandomValue(0, 480 - ball_size - 1);

    specialScore = 0;
    colorTheStar();

    specialMove = false;
  }

//collision with the right side
  //if ((ball_x + ball_size) > GetScreenWidth()) {
  if ((ball_x + ball_size) > 640) {
    player_score = player_score + 1;

  //  playerEffect = false;
  //  opponentEffect = false;
    
// player score bumps
    numberBump1 = 4;

    if(player_score < 10) {
    //  PlaySound(petit_chat);
    }

    ball_x = player_x + player_w + 20;
    ball_vx = abs(ball_vx);
    ball_vy = 0;
  //  ball_y = GetRandomValue(0, GetScreenHeight() - ball_size - 1);
    ball_y = GetRandomValue(0, 480 - ball_size - 1);

    specialScore = 0;
    colorTheStar();

    specialMove = false;
  }
}

void eyesPosition() {
    if(ball_x < 132) {
      eyes_x = -8;
    } else if(ball_x < 260){
      eyes_x = -4;
    } else if(ball_x < 388) {
      eyes_x = 0;
    } else if(ball_x < 516){
      eyes_x = 4;
    } else {
      eyes_x = 8;
    }

    if(ball_y < 112) {
      eyes_y = -8;
    } else if(ball_y < 208) {
      eyes_y = -4;
    } else if(ball_y < 304) {
      eyes_y = 0;
    } else if(ball_y < 400){
      eyes_y = 4;
    } else {
      eyes_y = 8;
    }
}

void bump() {
  if(((ball_y + ball_size) >= player_y) && (ball_y <= (player_y + player_h))) {
    if(ball_x == 64) {
      player_bump = -4;
    } else if(ball_x == 60) {
      player_bump = -8;
    } else if(ball_x == 56) {
      player_bump = -12;
    } else {
      player_bump = 0;
    }
  } else {
    player_bump = 0;
  }

  if(((ball_y + ball_size) >= opponent_y) && (ball_y <= (opponent_y + opponent_h))) {
    if((ball_x + ball_size) == 576) {
      opponent_bump = 4;
    } else if((ball_x + ball_size) == 580) {
      opponent_bump = 8;
    } else if((ball_x + ball_size) == 584) {
      opponent_bump = 12;
    } else {
      opponent_bump = 0;
    }
  } else {
    opponent_bump = 0;
  }
}

void drawStar() {
//the star rotates at different speeds
  if(ball_vx < 0) {
    if(ball_vy > 8) {
      star_number = star_number + 1;
    }
    if((ball_vy <= 8) && (ball_vy > 4) && ((rotate_counter % 2) == 0)) {
      star_number = star_number + 1;
    }
    if((ball_vy <= 4) && (ball_vy > 0) && ((rotate_counter % 4) == 0)) {
      star_number = star_number + 1;
    }
    if((ball_vy >= -4) && (ball_vy < 0) && ((rotate_counter % 4) == 0)) {
      star_number = star_number - 1;
    }
    if((ball_vy >= -8) && (ball_vy < -4) && ((rotate_counter % 2) == 0)) {
      star_number = star_number - 1;
    }
    if(ball_vy < -8) {
    star_number = star_number - 1;
    }
  } else {
    if(ball_vy > 8) {
      star_number = star_number - 1;
    }
    if((ball_vy <= 8) && (ball_vy > 4) && ((rotate_counter % 2) == 0)) {
      star_number = star_number - 1;
    }
    if((ball_vy <= 4) && (ball_vy > 0) && ((rotate_counter % 4) == 0)) {
      star_number = star_number - 1;
    }
    if((ball_vy >= -4) && (ball_vy < 0) && ((rotate_counter % 4) == 0)) {
      star_number = star_number + 1;
    }
    if((ball_vy >= -8) && (ball_vy < -4) && ((rotate_counter % 2) == 0)) {
      star_number = star_number + 1;
    }
    if(ball_vy < -8) {
    star_number = star_number + 1;
    }
  }

//the star animation loops

  if(star_number > 3) {
    star_number = 0;
  }
  if(star_number < 0) {
    star_number = 3;
  }

  //DrawTexturePro(star128x512, {0, ((float)star_number*72), 72, 72}, {(float)ball_x - 24, (float)ball_y - 24, 72, 72}, {0, 0}, 0, WHITE);
  if(ball_vx > 0) {
    DrawTexturePro(sevenStars512x512, {starColor*72, ((float)star_number*72), 72, 72}, {(float)ball_x - 24, (float)ball_y - 24, 72, 72}, {0, 0}, 0, WHITE);
  } else {
    DrawTexturePro(sevenStars512x512, {starColor*72, ((float)star_number*72), 72, 72}, {(float)ball_x - 24, (float)ball_y - 24, 72, 72}, {0, 0}, 0, WHITE);
  }
  //DrawRectangle(ball_x - 24, ball_y - 24, 72, 72, WHITE);
  //DrawCircle(ball_x + 12, ball_y + 12, 32, BLUE);

//loop the retate counter from 0 to 4
  rotate_counter = rotate_counter + 1;
  if(rotate_counter > 3) {
    rotate_counter = 0;
  }
}

void drawSparks() {
  sparkCounter = sparkCounter + 1;
  if(sparkCounter > 35) {
    sparkCounter = 0;
  }

  if(sparkCounter == 0) {
    xSpark = GetRandomValue(0, 576);
    ySpark = GetRandomValue(0, 416);
  }
  DrawTexturePro(spark, {0, (floor(sparkCounter/4))*64, 64, 64}, {xSpark, ySpark, 64, 64}, {0, 0}, 0, WHITE);

  if(sparkCounter == 6) {
    xSpark1 = GetRandomValue(0, 576);
    ySpark1 = GetRandomValue(0, 416);
  }
  DrawTexturePro(spark, {0, floor(((sparkCounter<6)?(sparkCounter+30):(sparkCounter-6))/4)*64, 64, 64}, {xSpark1, ySpark1, 64, 64}, {0, 0}, 0, WHITE);

  if(sparkCounter == 12) {
    xSpark2 = GetRandomValue(0, 576);
    ySpark2 = GetRandomValue(0, 416);
  }
  DrawTexturePro(spark, {0, floor(((sparkCounter<12)?(sparkCounter+24):(sparkCounter-12))/4)*64, 64, 64}, {xSpark2, ySpark2, 64, 64}, {0, 0}, 0, WHITE);

  if(sparkCounter == 18) {
    xSpark3 = GetRandomValue(0, 576);
    ySpark3 = GetRandomValue(0, 416);
  }
  DrawTexturePro(spark, {0, floor(((sparkCounter<18)?(sparkCounter+18):(sparkCounter-18))/4)*64, 64, 64}, {xSpark3, ySpark3, 64, 64}, {0, 0}, 0, WHITE);

  if(sparkCounter == 24) {
    xSpark4 = GetRandomValue(0, 576);
    ySpark4 = GetRandomValue(0, 416);
  }
  DrawTexturePro(spark, {0, floor(((sparkCounter<24)?(sparkCounter+12):(sparkCounter-24))/4)*64, 64, 64}, {xSpark4, ySpark4, 64, 64}, {0, 0}, 0, WHITE);


  if(sparkCounter == 30) {
    xSpark5 = GetRandomValue(0, 576);
    ySpark5 = GetRandomValue(0, 416);
  }
  DrawTexturePro(spark, {0, floor(((sparkCounter<30)?(sparkCounter+6):(sparkCounter-30))/4)*64, 64, 64}, {xSpark5, ySpark5, 64, 64}, {0, 0}, 0, WHITE);

}

void drawScore() {
  switch (player_score) {
    case 0:
      DrawTexturePro(zero64x64, {0, 0, 64, 64}, {52 - numberBump1*4, 60 - numberBump1*4, 64 + (numberBump1*8), 64 + (numberBump1*8)}, {0, 0}, 0, WHITE);
    break;
    case 1:
      DrawTexturePro(one64x64, {0, 0, 64, 64}, {52 - numberBump1*4, 60 - numberBump1*4, 64 + (numberBump1*8), 64 + (numberBump1*8)}, {0, 0}, 0, WHITE);
    break;
    case 2:
      DrawTexturePro(two64x64, {0, 0, 64, 64}, {52 - numberBump1*4, 60 - numberBump1*4, 64 + (numberBump1*8), 64 + (numberBump1*8)}, {0, 0}, 0, WHITE);
    break;
    case 3:
      DrawTexturePro(three64x64, {0, 0, 64, 64}, {52 - numberBump1*4, 60 - numberBump1*4, 64 + (numberBump1*8), 64 + (numberBump1*8)}, {0, 0}, 0, WHITE);
    break;
    case 4:
      DrawTexturePro(four64x64, {0, 0, 64, 64}, {52 - numberBump1*4, 60 - numberBump1*4, 64 + (numberBump1*8), 64 + (numberBump1*8)}, {0, 0}, 0, WHITE);
    break;
    case 5:
      DrawTexturePro(five64x64, {0, 0, 64, 64}, {52 - numberBump1*4, 60 - numberBump1*4, 64 + (numberBump1*8), 64 + (numberBump1*8)}, {0, 0}, 0, WHITE);
    break;
    case 6:
      DrawTexturePro(six64x64, {0, 0, 64, 64}, {52 - numberBump1*4, 60 - numberBump1*4, 64 + (numberBump1*8), 64 + (numberBump1*8)}, {0, 0}, 0, WHITE);
    break;
    case 7:
      DrawTexturePro(seven64x64, {0, 0, 64, 64}, {52 - numberBump1*4, 60 - numberBump1*4, 64 + (numberBump1*8), 64 + (numberBump1*8)}, {0, 0}, 0, WHITE);
    break;
    case 8:
     DrawTexturePro(eight64x64, {0, 0, 64, 64}, {52 - numberBump1*4, 60 - numberBump1*4, 64 + (numberBump1*8), 64 + (numberBump1*8)}, {0, 0}, 0, WHITE);
    break;
    case 9:
      DrawTexturePro(nine64x64, {0, 0, 64, 64}, {52 - numberBump1*4, 60 - numberBump1*4, 64 + (numberBump1*8), 64 + (numberBump1*8)}, {0, 0}, 0, WHITE);
    break;
    case 10:
      DrawTexturePro(one64x64, {0, 0, 64, 64}, {52 - 24 - numberBump1*4, 60 - numberBump1*4, 64 + (numberBump1*8), 64 + (numberBump1*8)}, {0, 0}, 0, WHITE);
      DrawTexturePro(zero64x64, {0, 0, 64, 64}, {52 + 24 - numberBump1*4, 60 - numberBump1*4, 64 + (numberBump1*8), 64 + (numberBump1*8)}, {0, 0}, 0, WHITE);
    break;
  }

  if(numberBump1 > 0) {
    numberBump1 = numberBump1 - 1;
  }

  switch (opponent_score) {
    case 0:
      DrawTexturePro(zero64x64, {0, 0, 64, 64}, {524 - numberBump2*4, 60 - numberBump2*4, 64 + (numberBump2*8), 64 + (numberBump2*8)}, {0, 0}, 0, WHITE);
    break;
    case 1:
      DrawTexturePro(one64x64, {0, 0, 64, 64}, {524 - numberBump2*4, 60 - numberBump2*4, 64 + (numberBump2*8), 64 + (numberBump2*8)}, {0, 0}, 0, WHITE);
    break;
    case 2:
      DrawTexturePro(two64x64, {0, 0, 64, 64}, {524 - numberBump2*4, 60 - numberBump2*4, 64 + (numberBump2*8), 64 + (numberBump2*8)}, {0, 0}, 0, WHITE);
    break;
    case 3:
      DrawTexturePro(three64x64, {0, 0, 64, 64}, {524 - numberBump2*4, 60 - numberBump2*4, 64 + (numberBump2*8), 64 + (numberBump2*8)}, {0, 0}, 0, WHITE);
    break;
    case 4:
      DrawTexturePro(four64x64, {0, 0, 64, 64}, {524 - numberBump2*4, 60 - numberBump2*4, 64 + (numberBump2*8), 64 + (numberBump2*8)}, {0, 0}, 0, WHITE);
    break;
    case 5:
      DrawTexturePro(five64x64, {0, 0, 64, 64}, {524 - numberBump2*4, 60 - numberBump2*4, 64 + (numberBump2*8), 64 + (numberBump2*8)}, {0, 0}, 0, WHITE);
    break;
    case 6:
      DrawTexturePro(six64x64, {0, 0, 64, 64}, {524 - numberBump2*4, 60 - numberBump2*4, 64 + (numberBump2*8), 64 + (numberBump2*8)}, {0, 0}, 0, WHITE);
    break;
    case 7:
      DrawTexturePro(seven64x64, {0, 0, 64, 64}, {524 - numberBump2*4, 60 - numberBump2*4, 64 + (numberBump2*8), 64 + (numberBump2*8)}, {0, 0}, 0, WHITE);
    break;
    case 8:
      DrawTexturePro(eight64x64, {0, 0, 64, 64}, {524 - numberBump2*4, 60 - numberBump2*4, 64 + (numberBump2*8), 64 + (numberBump2*8)}, {0, 0}, 0, WHITE);
    break;
    case 9:
      DrawTexturePro(nine64x64, {0, 0, 64, 64}, {524 - numberBump2*4, 60 - numberBump2*4, 64 + (numberBump2*8), 64 + (numberBump2*8)}, {0, 0}, 0, WHITE);
    break;
    case 10:
      DrawTexturePro(one64x64, {0, 0, 64, 64}, {524 - 24 - numberBump2*4, 60 - numberBump2*4, 64 + (numberBump2*8), 64 + (numberBump2*8)}, {0, 0}, 0, WHITE);
      DrawTexturePro(zero64x64, {0, 0, 64, 64}, {524 + 24 - numberBump2*4, 60 - numberBump2*4, 64 + (numberBump2*8), 64 + (numberBump2*8)}, {0, 0}, 0, WHITE);
    break;
  }

  if(numberBump2 > 0) {
    numberBump2 = numberBump2 - 1;
  }
}

void draw() {
// draw the sparks
  drawSparks();
  
//rabbit head with heart
    DrawTexturePro(rabbitHeart, {0, 0, 256, 256}, {192, 112, 256, 256}, {0, 0}, 0, WHITE);
    eyesPosition();
    DrawTexturePro(rabbit_eyes, {0, 0, 128, 64}, {256 + eyes_x, 224 + eyes_y, 128, 64}, {0, 0}, 0, WHITE);

  drawScore();

  bump();

  //draw the opponent

  DrawTexturePro(right_paw, {0, 0, 104, 152}, {opponent_x - 48 + opponent_bump, opponent_y - 20, 104, 152}, {0, 0}, 0, WHITE);

  //DrawRectangle(opponent_x, opponent_y, opponent_w, opponent_h, WHITE);

  //draw the ball

  drawStar();

  //draw the player

  DrawTexturePro(left_paw, {0, 0, 104, 152}, {player_x - 48 + player_bump, player_y - 20, 104, 152}, {0, 0}, 0, WHITE);
  
  //DrawRectangle(player_x, player_y, player_w, player_h, WHITE);


// test affichage rectangles player et opponent
//  DrawRectangle(player_x, player_y, player_w, player_h, GREEN);
//  DrawRectangle(opponent_x, opponent_y, opponent_w, opponent_h, RED);
//  DrawRectangle(ball_x, ball_y, ball_size, ball_size, BLUE);
}

void resetValues() {
      player_score = 0;
      opponent_score = 0;
      player_y = 208;
      opponent_y = 208;
      player_vy = 0;
      player_timeout = 0;
      player_bump = 0;
      opponent_vy = 0;
      opponent_timeout = 0;
      opponent_bump = 0;
      ball_x = 84;
      ball_y = 228;
      ball_vx = 8;
      ball_vy = 0;
      playerMode = 1;
      opponentMode = 1;
      //playerEffect = false;
      //opponentEffect = false;
      //endEffect = false;
      starColor = 0;
      specialScore = 0;
      specialMove = false;
}


int main() {
//  cout << "Happy Pong for Dreamcast" << endl;
  const int screen_width = 640;
  const int screen_height = 480;

  InitWindow(screen_width, screen_height, "Happy Pong Dreamcast");

  /*
  InitAudioDevice();

  rebond_ballon = LoadSound("sounds/rebond_ballon.wav");
  petit_chat = LoadSound("sounds/petit_chat.wav");
  Chouette = LoadSound("sounds/Chouette.wav");
  */


// CD
  //star128x512 = LoadTexture("/cd/sprites/star128x512.png");
  sevenStars512x512 = LoadTexture("/cd/sprites/sevenStars512x512.png");
  zero64x64 = LoadTexture("/cd/sprites/zero64x64.png");
  one64x64 = LoadTexture("/cd/sprites/one64x64.png");
  two64x64 = LoadTexture("/cd/sprites/two64x64.png");
  three64x64 = LoadTexture("/cd/sprites/three64x64.png");
  four64x64 = LoadTexture("/cd/sprites/four64x64.png");
  five64x64 = LoadTexture("/cd/sprites/five64x64.png");
  six64x64 = LoadTexture("/cd/sprites/six64x64.png");
  seven64x64 = LoadTexture("/cd/sprites/seven64x64.png");
  eight64x64 = LoadTexture("/cd/sprites/eight64x64.png");
  nine64x64 = LoadTexture("/cd/sprites/nine64x64.png");
  HAPPY_PONG512x512 = LoadTexture("/cd/sprites/HAPPY_PONG512x512.png");
  left_paw = LoadTexture("/cd/sprites/left_paw.png");
  right_paw = LoadTexture("/cd/sprites/right_paw.png");
  rabbitHeart = LoadTexture("/cd/sprites/rabbitHeart.png");
  spark = LoadTexture("/cd/sprites/spark.png");
  rabbit_eyes = LoadTexture("/cd/sprites/rabbit_eyes.png");
  //you_lose256x256 = LoadTexture("/cd/sprites/you_lose256x256.png");
  //you_win256x256 = LoadTexture("/cd/sprites/you_win256x256.png");
  credits512x64 = LoadTexture("/cd/sprites/credits512x64.png");
  QRCodeNB64x64 = LoadTexture("/cd/sprites/QRCodeNB64x64.png");
  pinkCorners = LoadTexture("/cd/sprites/pinkCorners.png");

  SetTargetFPS(30);

  while(WindowShouldClose() == false) {
    BeginDrawing();

///////////////////////////////////// LOOP

    switch (gameMode) {

      case 0: // titlescreen
        ClearBackground({255, 153, 229, 255});

        DrawTexturePro(HAPPY_PONG512x512, {0, 16, 512, 480}, {64, 0, 512, 480}, {0, 0}, 0, WHITE);

        //DrawText(TextFormat("HAPPY PONG"), 60, 210, 80, WHITE);

        //if(IsKeyPressed(KEY_ENTER)) {
        if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_RIGHT)) {
          gameMode = 1;
          twoControllers = false;
        } else if(IsGamepadButtonPressed(1, GAMEPAD_BUTTON_MIDDLE_RIGHT)){
          gameMode = 1;
          twoControllers = true;
        }
        
      break;
      
      case 1: // game
    
// light pink background
    ClearBackground({255, 153, 229, 255});

    movePlayer();
    moveOpponent();

    //move the ball
    if(specialMove == true) {
      if(ball_vx > 0) {
        ball_x = ball_x + ball_vx + dash_vx;
      } else {
        ball_x = ball_x + ball_vx - dash_vx;
      }
    } else {
      ball_x = ball_x + ball_vx;
    }
    ball_y = ball_y + ball_vy;

    /*
    if(endEffect == false) {
      if(ball_vx > 0){
        if((ball_x + (ball_size / 2)) > 320) {
          endEffect = true;
          starColor = 0;
          playerEffect = false;
        }
      } else {
        if((ball_x + (ball_size / 2)) < 320) {
          endEffect = true;
          starColor = 0;
          opponentEffect = false;
        }
      }
    }
    */
    
    updateCollisions();

    // go to winner screen if 10 points is reached
    if (player_score == 10) {
      winner = PLAYER;
//    winner_timer = winner_timermax;
//      player_score = 0;
//      opponent_score = 0;
      gameMode = 2;
    //  PlaySound(Chouette);
    }
    if (opponent_score == 10) {
      winner = OPPONENT;
//      winner_timer = winner_timermax;
//      player_score = 0;
//      opponent_score = 0;
      gameMode = 2;
    //  PlaySound(Chouette);
    }

    draw();

// go back to title screen and reset the values
    if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_RIGHT)) {
      gameMode = 0;
      resetValues();
      twoControllers = false;
    } else if(IsGamepadButtonPressed(1, GAMEPAD_BUTTON_MIDDLE_RIGHT)){
      gameMode = 0;
      resetValues();
      twoControllers = true;
    }

    break;

    case 2: // winner screen

    // light pink background
    ClearBackground({255, 153, 229, 255});
 
    //DrawTexturePro(rabbitHeart, {0, 0, 256, 256}, {192, 112, 256, 256}, {0, 0}, 0, WHITE);

    // draw Juice Lizard linktree QR code with rounded corners
    DrawTexturePro(QRCodeNB64x64, {0, 0, 64, 64}, {160, 80, 320, 320}, {0, 0}, 0, WHITE);
    DrawTexturePro(pinkCorners, {0, 0, 8, 8}, {175, 95, 8, 8}, {0, 0}, 0, WHITE);
    DrawTexturePro(pinkCorners, {8, 0, 8, 8}, {457, 95, 8, 8}, {0, 0}, 0, WHITE);
    DrawTexturePro(pinkCorners, {0, 8, 8, 8}, {175, 377, 8, 8}, {0, 0}, 0, WHITE);
    DrawTexturePro(pinkCorners, {8, 8, 8, 8}, {457, 377, 8, 8}, {0, 0}, 0, WHITE);
/*
    if(clockwise) {
      win_rotation = win_rotation + 2;
      if(win_rotation > 15) {
        clockwise = false;
      }
    } else {
      win_rotation = win_rotation - 2;
      if(win_rotation < -15) {
        clockwise = true;
      }
    }

    if(win_bigger) {
      win_size = win_size + 4;
      if(win_size > 16) {
        win_bigger = false;
      }
    } else {
      win_size = win_size - 4;
      if(win_size < -16) {
        win_bigger = true;
      }
    }
*/

    /*
    if(winner) {
      DrawTexturePro(you_lose256x256, {0, 0, 160, 160}, {520, 120, 160, 160}, {80, 80}, 0, WHITE);
      DrawTexturePro(you_win256x256, {0, 0, 160, 160}, {120, 120, 160 + win_size, 160 + win_size}, {80 + (win_size/2), 80 + (win_size/2)}, win_rotation, WHITE);
     // DrawTexturePro(rabbit_eyes, {0, 0, 128, 64}, {256 - 8, 224 - 8, 128, 64}, {0, 0}, 0, WHITE);
    } else {
      DrawTexturePro(you_lose256x256, {0, 0, 160, 160}, {120, 120, 160, 160}, {80, 80}, 0, WHITE);
      DrawTexturePro(you_win256x256, {0, 0, 160, 160}, {520, 120, 160 + win_size, 160 + win_size}, {80 + (win_size/2), 80 + (win_size/2)}, win_rotation, WHITE);
     // DrawTexturePro(rabbit_eyes, {0, 0, 128, 64}, {256 + 8, 224 - 8, 128, 64}, {0, 0}, 0, WHITE);
    }
    */

    DrawTexturePro(credits512x64, {0, 0, 512, 64}, {64, 400, 512, 64}, {0, 0}, 0, WHITE);

    drawScore();

// go back to the titlescreen
  //  if(IsKeyPressed(KEY_ENTER)) {
    if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_RIGHT)) {
      gameMode = 0;
      resetValues();
      twoControllers = false;
    } else if(IsGamepadButtonPressed(1, GAMEPAD_BUTTON_MIDDLE_RIGHT)) {
      gameMode = 0;
      resetValues();
      twoControllers = true;
    }
    
    }

    EndDrawing();
  }

  /*
  UnloadSound(rebond_ballon);
  UnloadSound(petit_chat);
  UnloadSound(Chouette);
  */

  //UnloadTexture(star128x512);
  UnloadTexture(sevenStars512x512);
  UnloadTexture(one64x64);
  UnloadTexture(two64x64);
  UnloadTexture(three64x64);
  UnloadTexture(four64x64);
  UnloadTexture(five64x64);
  UnloadTexture(six64x64);
  UnloadTexture(seven64x64);
  UnloadTexture(eight64x64);
  UnloadTexture(nine64x64);
  UnloadTexture(zero64x64);
  UnloadTexture(HAPPY_PONG512x512);
  UnloadTexture(left_paw);
  UnloadTexture(right_paw);
  UnloadTexture(rabbit_eyes);
  UnloadTexture(rabbitHeart);
  UnloadTexture(spark);
  //UnloadTexture(you_lose256x256);
  //UnloadTexture(you_win256x256);
  UnloadTexture(credits512x64);
  UnloadTexture(QRCodeNB64x64);
  UnloadTexture(pinkCorners);

  //CloseAudioDevice();

  CloseWindow();
  return 0;
}