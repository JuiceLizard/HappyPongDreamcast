#include <iostream>
#include <raylib.h>
#include <math.h>
#include <kos.h>
#include <dc/sound/sound.h>
#include <dc/sound/sfxmgr.h>

#include <sys/cdefs.h>
#include <arch/types.h>
#include <dc/maple.h>
#include <dc/maple/controller.h>

#include <dc/maple/vmu.h>
#include <dc/vmu_fb.h>
#include <dc/video.h>
#include <stdio.h>
#include <stdint.h>
#include <kos/init.h>
#include <dc/fmath.h>
#include <arch/arch.h>

using namespace std;

// VMU stuff
/*
static const char badger[] = {
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00111111111100000000111111111100,
    0b00111111111100000000111111111100,
    0b00110000001111000011110000001100,
    0b00110000001111000011110000001100,
    0b00110011110011000011001111001100,
    0b00110011110011000011001111001100,
    0b00110011110011111111001111001100,
    0b00110011110011111111001111001100,
    0b00111100111111000011111100111100,
    0b00111100111111000011111100111100,
    0b00110000111111000011111100001100,
    0b00110000111111000011111100001100,
    0b00110000110011000011001100001100,
    0b00110000110011000011001100001100,
    0b00111100001111000011110000111100,
    0b00111100001111000011110000111100,
    0b00001111000000000000000011110000,
    0b00001111000000000000000011110000,
    0b00000011110011111111001111000000,
    0b00000011110011111111001111000000,
    0b00000000111100111100111100000000,
    0b00000000111100111100111100000000,
    0b00000000001111111111110000000000,
    0b00000000001111111111110000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
};
*/

static vmufb_t vmufb;

maple_device_t *dev;
unsigned int vmu;


sfxhnd_t poc = {0};
sfxhnd_t bim = {0};
sfxhnd_t woosh = {0};
sfxhnd_t dash = {0};
sfxhnd_t split = {0};
sfxhnd_t glue = {0};
sfxhnd_t ghost = {0};
sfxhnd_t paw = {0};
sfxhnd_t pawpawpaw = {0};
sfxhnd_t happypong = {0};
sfxhnd_t ronron = {0};

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
Texture2D sevenStars512x512 = {0};
Texture2D credits512x64 = {0};
Texture2D QRCodeNB64x64 = {0};
Texture2D pinkCorners = {0};
Texture2D H128x128 = {0};
Texture2D A128x256 = {0};
Texture2D Pa128x256 = {0};
Texture2D Pb128x256 = {0};
Texture2D Y128x256 = {0};
Texture2D Pc128x256 = {0};
Texture2D O128x128 = {0};
Texture2D N128x128 = {0};
Texture2D G128x128 = {0};

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
int playerMode = 2; // 1 buttons, 2 stick or 3 trigger
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
int opponentMode = 2; // 1 buttons, 2 stick or 3 trigger
bool leftStickSecondPad = true;
float previous_opponent_y = 208;
//bool opponentEffect = false;

//ball variables
float ball_size = 24;
//float ball_x = opponent_x - ball_size - 20;
float ball_x = 84;
//float ball_y = (GetScreenHeight() - ball_size) / 2;
float ball_y = 228;

float fake_ball_y = 228;
int fake_ball_vy = 0;

int ball_vx = 8;
int ball_vy = 0;
int ball_vymax = 24;
int star_number = 1;
int rotate_counter = 0;

int previous_ball_vy = 0;

float starColor = 0; // 0 yellow, 1 orange, 2 red, 3 blue, 4 green

int specialScore = 0;
int previousSpecialScore = 0;
int pocBimWoosh = 0;

int specialMove = 0; // 0 no special move, 1 dash, 2 ghost, 3 glue, 4 split
int dash_vx = 0;
unsigned char starStealth = 255;
int transparencyCounter = 0;
int fakeStarUp = 1;

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

int ronron_loop_channel = -1;

// title letters animation
const int letterSizeMax = 12;
float Hsize = 0;
float Hspeed = 0;
float Asize = 0;
float Aspeed = 0;
float Pasize = 0;
float Paspeed = 0;
float Pbsize = 0;
float Pbspeed = 0;
float Ysize = 0;
float Yspeed = 0;
float Pcsize = 0;
float Pcspeed = 0;
float Osize = 0;
float Ospeed = 0;
float Nsize = 0;
float Nspeed = 0;
float Gsize = 0;
float Gspeed = 0;

int letterCounter = 0;
const int letterSpeed = 2;
const int letterDelay = 5;

int vmuPaws = 1;
int pawsCounter = 1;


// the functions

void resetSpecialMove() {
  specialMove = 0;
  dash_vx = 0;
  starStealth = 255;
  transparencyCounter = 0;
}

void setSpecialMove() {
  switch (specialMove) {
    case 1:
      dash_vx = 6;
    break;
    case 2:
      transparencyCounter = 4;
    break;
  }
}

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
    } else if(player_timeout > 0) {
      player_vy = player_vy / 2;
    }
    break;
    // left stick control
    case 2:
    previous_player_y = player_y;
    player_y = ((480 - player_h)/2 + (GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y)) * 240 + previous_player_y) / 2;
    player_vy = player_y - previous_player_y;
    break;
    // left trigger control
    case 3:
    previous_player_y = player_y;
    player_y = ((-player_h)/2 + (GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_TRIGGER)) * 480 + previous_player_y) / 2;
    player_vy = player_y - previous_player_y;
    break;
  }

  //computer control after timeout
  if (player_timeout > 0) {
    player_timeout--;
  } else if((player_y + (player_h / 2)) < (ball_y + (ball_size / 2))) { //if the ball is below the opponent
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
  if(twoControllers == true) { // second pad
    // buttons Y and A (Dreamcast or XBOX buttons positions)
    if((IsGamepadButtonDown(1, GAMEPAD_BUTTON_RIGHT_FACE_UP)) || (IsGamepadButtonDown(1, GAMEPAD_BUTTON_RIGHT_FACE_DOWN))) {
      opponentMode = 1;
      opponent_timeout = opponent_timeoutmax;
    }
    // right stick or second controller left stick
    if((GetGamepadAxisMovement(1, GAMEPAD_AXIS_RIGHT_Y) < -0.2) || (GetGamepadAxisMovement(1, GAMEPAD_AXIS_RIGHT_Y) > 0.2)) {
      opponentMode = 2;
      leftStickSecondPad = false;
      opponent_timeout = opponent_timeoutmax;
    } else if((GetGamepadAxisMovement(1, GAMEPAD_AXIS_LEFT_Y) < -0.2) || (GetGamepadAxisMovement(1, GAMEPAD_AXIS_LEFT_Y) > 0.2)) {
      opponentMode = 2;
      leftStickSecondPad = true;
      opponent_timeout = opponent_timeoutmax;
    }
    // right trigger
    if(GetGamepadAxisMovement(1, GAMEPAD_AXIS_RIGHT_TRIGGER) > 0.2) { 
      opponentMode = 3;
      opponent_timeout = opponent_timeoutmax;
    }

  } else {
    // first pad
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
        } else if(IsGamepadButtonDown(1, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) {  
          opponent_vy += 4;
        } else if(opponent_timeout > 0) {
          opponent_vy = opponent_vy / 2;
        }
      } else {
        if(IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_UP)) {
          opponent_vy -= 4;
        } else if(IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) {  
          opponent_vy += 4;
        } else if(opponent_timeout > 0) {
          opponent_vy = opponent_vy / 2;
        }
      }
    break;

  // stick control
    case 2:
      if(twoControllers == true) { // second pad
        previous_opponent_y = opponent_y;
        
        if(leftStickSecondPad == true) {
          opponent_y = ((480 - opponent_h)/2 + (GetGamepadAxisMovement(1, GAMEPAD_AXIS_LEFT_Y)) * 240 + previous_opponent_y) / 2;
        } else {
          opponent_y = ((480 - opponent_h)/2 + (GetGamepadAxisMovement(1, GAMEPAD_AXIS_RIGHT_Y)) * 240 + previous_opponent_y) / 2;
        }

        opponent_vy = opponent_y - previous_opponent_y;
      } else { // first pad
        previous_opponent_y = opponent_y;
        opponent_y = ((480 - opponent_h)/2 + (GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_Y)) * 240 + previous_opponent_y) / 2;
        opponent_vy = opponent_y - previous_opponent_y;
      }
    break;

  // right trigger control
    case 3:
      if(twoControllers == true) {
        previous_opponent_y = opponent_y;
        opponent_y = ((-opponent_h)/2 + (GetGamepadAxisMovement(1, GAMEPAD_AXIS_RIGHT_TRIGGER)) * 480 + previous_opponent_y) / 2;
        opponent_vy = opponent_y - previous_opponent_y;
      } else {
        previous_opponent_y = opponent_y;
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
    if(specialMove == 3) {
      ball_vy = 0;
    } else {
      ball_vy = -ball_vy;
    }
    //  PlaySound(poc);
    snd_sfx_play(poc, 128, (int)(ball_x/2.4));
  }

  //collision with the bottom border
  if ((ball_y + ball_size) > 480) {
    ball_y = 480 - ball_size;
    if(specialMove == 3) {
      ball_vy = 0;
    } else {
      ball_vy = -ball_vy;
    }
  
    // star collision sound
    snd_sfx_play(poc, 128, (int)(ball_x/2.4));
  }

  // fake ball collisions
    if(specialMove == 4) {
    // top border
    if(fake_ball_y < 0) {
      fake_ball_y = 0;
      fake_ball_vy = -fake_ball_vy;
    }
    // bottom border
    if(fake_ball_y + ball_size > 480) {
      fake_ball_y = 480 - ball_size;
      fake_ball_vy = -fake_ball_vy;
    }
  }

  //collision with the player
  if((ball_x + ball_size > player_x) && (ball_x < player_x + player_w) &&
  (ball_y + ball_size > player_y) && (ball_y < player_y + player_h)) {
    ball_x = player_x + player_w;
    ball_vx = -ball_vx;
    
    previous_ball_vy = ball_vy;
    
    resetSpecialMove();

    if(playerMode == 2) { // stick
      specialScore = specialScore - 1;
      if(specialScore < -2) {
        specialScore = 0;
        specialMove = GetRandomValue(1, 4);
        setSpecialMove();
      }
    }

    if(playerMode == 3) { // trigger
      specialScore = specialScore - 2;
      if(specialScore < -2) {
        specialScore = 0;
        specialMove = GetRandomValue(1, 4);
        setSpecialMove();
      }
    }

    if(player_timeout < 1) { // computer control

      previousSpecialScore = specialScore;

      switch (opponent_score) {
        case 2:
          specialScore = specialScore - GetRandomValue(0, 1);
        break;
        case 3:
          specialScore = specialScore - GetRandomValue(0, 1);
        break;
        case 4:
          specialScore = specialScore - GetRandomValue(0, 2);
        break;
        case 5:
          specialScore = specialScore - GetRandomValue(0, 2);
        break;
        case 6:
          specialScore = specialScore - GetRandomValue(1, 2);
        break;
        case 7:
          specialScore = specialScore - GetRandomValue(1, 2);
        break;
        case 8:
          specialScore = specialScore - 2;
        break;
        case 9:
          specialScore = specialScore - 2;
        break;
      }

      pocBimWoosh = previousSpecialScore - specialScore;

      if(specialScore < -2) {
        specialScore = 0;
        specialMove = GetRandomValue(1, 4);
        setSpecialMove();
      }
    }

    colorTheStar();

    //add some effet to the ball
    if(specialMove == 4) {
      fake_ball_y = ball_y;
      fakeStarUp = GetRandomValue(1, 2);
      if(fakeStarUp == 1) {
        ball_vy += player_vy / 2;
        fake_ball_vy = ball_vy + 2;
      } else {
        ball_vy += player_vy / 2;
        fake_ball_vy = ball_vy - 2;
      }
    } else {
      ball_vy += player_vy / 2;
    }

    // limit the vertical speed of the ball
    if(ball_vy < (-ball_vymax)) {
      ball_vy = -ball_vymax;
    }
    if(ball_vy > ball_vymax) {
      ball_vy = ball_vymax;
    }

    // star collision sound
    switch (specialMove) {
      case 0:
        switch (playerMode) {
          case 1:
            if(player_timeout > 0) {
              snd_sfx_play(poc, 128, 22);
            } else {
              switch (pocBimWoosh) {
                case 0:
                  snd_sfx_play(poc, 128, 22);
                break;
                case 1:
                  snd_sfx_play(bim, 128, 22);
                break;
                case 2:
                  snd_sfx_play(woosh, 128, 22);
                break;
              }
            }
            break;
          case 2:
            snd_sfx_play(bim, 128, 22);
          break;
            case 3:
            snd_sfx_play(woosh, 128, 22);
          break;
        }
      break;
      case 1:
        snd_sfx_play(dash, 128, 22);
      break;
      case 2:
        snd_sfx_play(ghost, 128, 22);
      break;
      case 3:
        snd_sfx_play(glue, 128, 22);
      break;
      case 4:
        snd_sfx_play(split, 128, 22);
      break;
    }
  }

//collision with the opponent
  if((ball_x + ball_size > opponent_x) && (ball_x < opponent_x + opponent_w) &&
  (ball_y + ball_size > opponent_y) && (ball_y < opponent_y + opponent_h)) {
    ball_x = opponent_x - ball_size;
    ball_vx = -ball_vx;

    previous_ball_vy = ball_vy;


    resetSpecialMove();

    if(opponentMode == 2) { // stick
      specialScore = specialScore + 1;
      if(specialScore > 2) {
        specialScore = 0;
        specialMove = GetRandomValue(1, 4);
        setSpecialMove();
      }
    }

    if(opponentMode == 3) { // trigger
      specialScore = specialScore + 2;
      if(specialScore > 2) {
        specialScore = 0;
        specialMove = GetRandomValue(1, 4);
        setSpecialMove();
      }
    }

    if(opponent_timeout < 1) { // computer control

      previousSpecialScore = specialScore;

      switch (player_score) {
        case 2:
          specialScore = specialScore + GetRandomValue(0, 1);
        break;
        case 3:
          specialScore = specialScore + GetRandomValue(0, 1);
        break;
        case 4:
          specialScore = specialScore + GetRandomValue(0, 2);
        break;
        case 5:
          specialScore = specialScore + GetRandomValue(0, 2);
        break;
        case 6:
          specialScore = specialScore + GetRandomValue(1, 2);
        break;
        case 7:
          specialScore = specialScore + GetRandomValue(1, 2);
        break;
        case 8:
          specialScore = specialScore + 2;
        break;
        case 9:
          specialScore = specialScore + 2;
        break;
      }

      pocBimWoosh = specialScore - previousSpecialScore;

      if(specialScore > 2) {
        specialScore = 0;
        specialMove = GetRandomValue(1, 4);
        setSpecialMove();
      }
    }

    colorTheStar();

    //add some effet to the ball
    if(specialMove == 4) {
      fake_ball_y = ball_y;
      fakeStarUp = GetRandomValue(1, 2);
      if(fakeStarUp == 1) {
        ball_vy += opponent_vy / 2;
        fake_ball_vy = ball_vy + 2;
      } else {
        ball_vy += opponent_vy / 2;
        fake_ball_vy = ball_vy - 2;
      }
    } else {
      ball_vy += opponent_vy / 2;
    }

    // limit the vertical speed of the ball
    if(ball_vy < (-ball_vymax)) {
      ball_vy = -ball_vymax;
    }
    if(ball_vy > ball_vymax) {
      ball_vy = ball_vymax;
    }

  // star collision sound

    switch (specialMove) {
      case 0:
        switch (opponentMode) {
          case 1:
            if(opponent_timeout > 0) {
              snd_sfx_play(poc, 128, 233);
            } else {
              switch (pocBimWoosh) {
                case 0:
                  snd_sfx_play(poc, 128, 233);
                break;
                case 1:
                  snd_sfx_play(bim, 128, 233);
                break;
                case 2:
                  snd_sfx_play(woosh, 128, 233);
                break;
              }
            }
          break;
          case 2:
            snd_sfx_play(bim, 128, 233);
          break;
          case 3:
            snd_sfx_play(woosh, 128, 233);
          break;
        }
      break;
      case 1:
        snd_sfx_play(dash, 128, 233);
      break;
      case 2:
        snd_sfx_play(ghost, 128, 233);
      break;
      case 3:
        snd_sfx_play(glue, 128, 233);
      break;
      case 4:
        snd_sfx_play(split, 128, 233);
      break;
    }
  }

//collision with the left side
  if (ball_x < 0) {
    opponent_score = opponent_score + 1;

// opponent score bumps
    numberBump2 = 4;   
    
    if(opponent_score < 10) {
    //  one point sound
      snd_sfx_play(paw, 128, 0);
    }

    ball_x = opponent_x - 16;
    ball_vx = -abs(ball_vx);
    ball_vy = 0;
  //  ball_y = GetRandomValue(0, GetScreenHeight() - ball_size - 1);
    ball_y = GetRandomValue(0, 480 - ball_size - 1);

    specialScore = 0;
    colorTheStar();

    resetSpecialMove();
  }

//collision with the right side
  //if ((ball_x + ball_size) > GetScreenWidth()) {
  if ((ball_x + ball_size) > 640) {
    player_score = player_score + 1;
    
// player score bumps
    numberBump1 = 4;

    if(player_score < 10) {
    //  one point sound
      snd_sfx_play(paw, 128, 255);
    }

    ball_x = player_x + player_w + 20;
    ball_vx = abs(ball_vx);
    ball_vy = 0;
  //  ball_y = GetRandomValue(0, GetScreenHeight() - ball_size - 1);
    ball_y = GetRandomValue(0, 480 - ball_size - 1);

    specialScore = 0;
    colorTheStar();

    resetSpecialMove();
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

  if((specialMove == 2) && (starStealth > 3)) {
    starStealth = starStealth - transparencyCounter;
  }

  // draw the fake star if special move is SPLIT
  if(specialMove == 4) {
    DrawTexturePro(sevenStars512x512, {0, 0, 72, 72}, {(float)ball_x - 24, (float)fake_ball_y - 24, 72, 72}, {0, 0}, 0, WHITE);
  }

  //DrawTexturePro(star128x512, {0, ((float)star_number*72), 72, 72}, {(float)ball_x - 24, (float)ball_y - 24, 72, 72}, {0, 0}, 0, WHITE);
  if(ball_vx > 0) {
    DrawTexturePro(sevenStars512x512, {starColor*72, ((float)star_number*72), 72, 72}, {(float)ball_x - 24, (float)ball_y - 24, 72, 72}, {0, 0}, 0, {255, 255, 255, starStealth});
  } else {
    DrawTexturePro(sevenStars512x512, {starColor*72, ((float)star_number*72), 72, 72}, {(float)ball_x - 24, (float)ball_y - 24, 72, 72}, {0, 0}, 0, {255, 255, 255, starStealth});
  }

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

  //draw the ball

  drawStar();

  //draw the player

  DrawTexturePro(left_paw, {0, 0, 104, 152}, {player_x - 48 + player_bump, player_y - 20, 104, 152}, {0, 0}, 0, WHITE);
}

void resetTitleAnim() {
  Hsize = 0;
  Hspeed = 0;
  Asize = 0;
  Aspeed = 0;
  Pasize = 0;
  Paspeed = 0;
  Pbsize = 0;
  Pbspeed = 0;
  Ysize = 0;
  Yspeed = 0;
  Pcsize = 0;
  Pcspeed = 0;
  Osize = 0;
  Ospeed = 0;
  Nsize = 0;
  Nspeed = 0;
  Gsize = 0;
  Gspeed = 0;

 letterCounter = 0;
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
  starColor = 0;
  specialScore = 0;
  previousSpecialScore = 0;
  pocBimWoosh = 0;
      
  resetSpecialMove();

  resetTitleAnim();

}

void changeLettersSize() {

  // frame counter
  letterCounter = letterCounter + 1;
  if(letterCounter > 150) {
    letterCounter = 0;
  }
  
  // letter H
  if(letterCounter == letterDelay) {
    Hspeed = letterSpeed;
  }
  if(Hsize > letterSizeMax) {
    Hspeed = -letterSpeed;
  }
  if(Hsize < 0) {
    Hsize = 0;
    Hspeed = 0;
  }
  Hsize = Hsize + Hspeed;

  // letter A
  if(letterCounter == letterDelay*2) {
    Aspeed = letterSpeed;
  }
  if(Asize > letterSizeMax) {
    Aspeed = -letterSpeed;
  }
  if(Asize < 0) {
    Asize = 0;
    Aspeed = 0;
  }
  Asize = Asize + Aspeed;

  // letter P (first)
  if(letterCounter == letterDelay*3) {
    Paspeed = letterSpeed;
  }
  if(Pasize > letterSizeMax) {
    Paspeed = -letterSpeed;
  }
  if(Pasize < 0) {
    Pasize = 0;
    Paspeed = 0;
  }
  Pasize = Pasize + Paspeed;

  // letter P (second)
  if(letterCounter == letterDelay*4) {
    Pbspeed = letterSpeed;
  }
  if(Pbsize > letterSizeMax) {
    Pbspeed = -letterSpeed;
  }
  if(Pbsize < 0) {
    Pbsize = 0;
    Pbspeed = 0;
  }
  Pbsize = Pbsize + Pbspeed;

  // letter Y
  if(letterCounter == letterDelay*5) {
    Yspeed = letterSpeed;
  }
  if(Ysize > letterSizeMax) {
    Yspeed = -letterSpeed;
  }
  if(Ysize < 0) {
    Ysize = 0;
    Yspeed = 0;
  }
  Ysize = Ysize + Yspeed;

  // letter P (third)
  if(letterCounter == letterDelay*7) {
    Pcspeed = letterSpeed;
  }
  if(Pcsize > letterSizeMax) {
    Pcspeed = -letterSpeed;
  }
  if(Pcsize < 0) {
    Pcsize = 0;
    Pcspeed = 0;
  }
  Pcsize = Pcsize + Pcspeed;

  // letter O
  if(letterCounter == letterDelay*8) {
    Ospeed = letterSpeed;
  }
  if(Osize > letterSizeMax) {
    Ospeed = -letterSpeed;
  }
  if(Osize < 0) {
    Osize = 0;
    Ospeed = 0;
  }
  Osize = Osize + Ospeed;

  // letter N
  if(letterCounter == letterDelay*9) {
    Nspeed = letterSpeed;
  }
  if(Nsize > letterSizeMax) {
    Nspeed = -letterSpeed;
  }
  if(Nsize < 0) {
    Nsize = 0;
    Nspeed = 0;
  }
  Nsize = Nsize + Nspeed;

  // letter G
  if(letterCounter == letterDelay*10) {
    Gspeed = letterSpeed;
  }
  if(Gsize > letterSizeMax) {
    Gspeed = -letterSpeed;
  }
  if(Gsize < 0) {
    Gsize = 0;
    Gspeed = 0;
  }
  Gsize = Gsize + Gspeed;
}

bool HappyPongShouldClose() {
  if((IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_UP))
  && (IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN))
  && (IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_LEFT))
  && (IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT))
  && (IsGamepadButtonDown(0, GAMEPAD_BUTTON_MIDDLE_RIGHT))) {
    return true;
  } else {
    return false;
  }
}

void drawVmuPaws(){
pawsCounter = pawsCounter + 1;

if(pawsCounter > 10) {
  vmuPaws = vmuPaws + 1;
  if(vmuPaws > 8) {
    vmuPaws = 1;
  }
  pawsCounter = 1;
}


switch (vmuPaws) {
      case 1:
        vmufb_print_string_into(&vmufb, NULL, 10, 14, 48, 6, 0, "D   + C");
      break;
      case 2:
        vmufb_print_string_into(&vmufb, NULL, 10, 14, 48, 6, 0, "D    +C");
      break;
      case 3:
        vmufb_print_string_into(&vmufb, NULL, 10, 14, 48, 6, 0, "D   +C ");
      break;
      case 4:
        vmufb_print_string_into(&vmufb, NULL, 10, 14, 48, 6, 0, "D  +  C");
      break;
      case 5:
        vmufb_print_string_into(&vmufb, NULL, 10, 14, 48, 6, 0, "D +   C");
      break;
      case 6:
        vmufb_print_string_into(&vmufb, NULL, 10, 14, 48, 6, 0, "D+    C");
      break;
      case 7:
        vmufb_print_string_into(&vmufb, NULL, 10, 14, 48, 6, 0, " D+   C");
      break;
      case 8:
        vmufb_print_string_into(&vmufb, NULL, 10, 14, 48, 6, 0, "D  +  C");
      break;
  }
}

int main() {

  const int screen_width = 640;
  const int screen_height = 480;

  InitWindow(screen_width, screen_height, "Happy Pong Dreamcast");
  SetTargetFPS(30);

  snd_init();


// CD
// sounds
  poc = snd_sfx_load("/cd/sounds/poc.wav");
  bim = snd_sfx_load("/cd/sounds/bim.wav");
  woosh = snd_sfx_load("/cd/sounds/woosh.wav");
  dash = snd_sfx_load("/cd/sounds/dash.wav");
  split = snd_sfx_load("/cd/sounds/split.wav");
  glue = snd_sfx_load("/cd/sounds/glue.wav");
  ghost = snd_sfx_load("/cd/sounds/ghost.wav");
  paw = snd_sfx_load("/cd/sounds/paw.wav");
  pawpawpaw = snd_sfx_load("/cd/sounds/pawpawpaw.wav");
  happypong = snd_sfx_load("/cd/sounds/happypong.wav");
  ronron = snd_sfx_load("/cd/sounds/ronron.wav");

// sprites
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
  credits512x64 = LoadTexture("/cd/sprites/credits512x64.png");
  QRCodeNB64x64 = LoadTexture("/cd/sprites/QRCodeNB64x64.png");
  pinkCorners = LoadTexture("/cd/sprites/pinkCorners.png");
  H128x128 = LoadTexture("/cd/sprites/H128x128.png");
  A128x256 = LoadTexture("/cd/sprites/A128x256.png");
  Pa128x256 = LoadTexture("/cd/sprites/Pa128x256.png");
  Pb128x256 = LoadTexture("/cd/sprites/Pb128x256.png");
  Y128x256 = LoadTexture("/cd/sprites/Y128x256.png");
  Pc128x256 = LoadTexture("/cd/sprites/Pc128x256.png");
  O128x128 = LoadTexture("/cd/sprites/O128x128.png");
  N128x128 = LoadTexture("/cd/sprites/N128x128.png");
  G128x128 = LoadTexture("/cd/sprites/G128x128.png");

  // play the purring cat sound repeatedly
  sfx_play_data_t data = {0};
	ronron_loop_channel = snd_sfx_chn_alloc();
	data.chn = ronron_loop_channel;
	data.idx = ronron;
	data.vol = 128;
	data.pan = 128;
	data.loop = 1;
	data.loopstart = 0;
  data.loopend = 0;

	snd_sfx_play_ex(&data);

  snd_sfx_play(happypong, 128, 128);

///////////////////////////////////// LOOP

  while(HappyPongShouldClose() == false) {

    // VMU display ASCII art playing cat
    vmufb_clear(&vmufb);
 
    vmufb_print_string_into(&vmufb, NULL, 2, 0, 48, 32, 1, "   /\\-/\\\n  ( ^x^ )\n\n (_)___(_)");

    drawVmuPaws();
    
    for(vmu = 0; !!(dev = maple_enum_type(vmu, MAPLE_FUNC_LCD)); vmu++) {
      vmufb_present(&vmufb, dev);
    }

    BeginDrawing();
    ClearBackground({255, 153, 229});


    switch (gameMode) {

      case 0: // titlescreen

        changeLettersSize();

        DrawTexturePro(H128x128, {0, 0, 128, 128}, {161, 173, 128 + Hsize, 128 + Hsize}, {64 + (Hsize/2), 64 + (Hsize/2)}, 0, WHITE);
        DrawTexturePro(A128x256, {0, 0, 128, 256}, {243, 167, 128 + Asize, 256 + (Asize*2)}, {64 + (Asize/2), 128 + Asize}, 0, WHITE);
        DrawTexturePro(Pa128x256, {0, 0, 128, 256}, {321, 165, 128 + Pasize, 256 + (Pasize*2)}, {64 + (Pasize/2), 128 + Pasize}, 0, WHITE);
        DrawTexturePro(Pb128x256, {0, 0, 128, 256}, {401, 172, 128 + Pbsize, 256 + (Pbsize*2)}, {64 + (Pbsize/2), 128 + Pbsize}, 0, WHITE);
        DrawTexturePro(Y128x256, {0, 0, 128, 256}, {476, 185, 128 + Ysize, 256 + (Ysize*2)}, {64 + (Ysize/2), 128 + Ysize}, 0, WHITE);

        DrawTexturePro(Pc128x256, {0, 0, 128, 256}, {174, 313, 128 + Pcsize, 256 + (Pcsize*2)}, {64 + (Pcsize/2), 128 + Pcsize}, 0, WHITE);
        DrawTexturePro(O128x128, {0, 0, 128, 128}, {250, 321, 128 + Osize, 128 + Osize}, {64 + (Osize/2), 64 + (Osize/2)}, 0, WHITE);
        DrawTexturePro(N128x128, {0, 0, 128, 128}, {343, 318, 128 + Nsize, 128 + Nsize}, {64 + (Nsize/2), 64 + (Nsize/2)}, 0, WHITE);
        DrawTexturePro(G128x128, {0, 0, 128, 128}, {449, 329, 128 + Gsize, 128 + Gsize}, {64 + (Gsize/2), 64 + (Gsize/2)}, 0, WHITE);
 
        if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_RIGHT)) {
          gameMode = 1;
          twoControllers = false;
        } else if(IsGamepadButtonPressed(1, GAMEPAD_BUTTON_MIDDLE_RIGHT)){
          gameMode = 1;
          twoControllers = true;
        }
        
      break;
      
      case 1: // game

    movePlayer();
    moveOpponent();

    //move the ball
    if(ball_vx > 0) {
      ball_x = ball_x + ball_vx + dash_vx;
    } else {
      ball_x = ball_x + ball_vx - dash_vx;
    }

    ball_y = ball_y + ball_vy;

    fake_ball_y = fake_ball_y + fake_ball_vy;
    
    updateCollisions();

    // go to winner screen if 10 points is reached
    if (player_score == 10) {
      winner = PLAYER;

      gameMode = 2;
    //  ten points sound
    snd_sfx_play(pawpawpaw, 128, 128);
    }
    if (opponent_score == 10) {
      winner = OPPONENT;

      gameMode = 2;
    //  ten points sound
    snd_sfx_play(pawpawpaw, 128, 128);
    }

    draw();

// go back to title screen and reset the values
    if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_RIGHT)) {
      gameMode = 0;
      resetValues();
      twoControllers = false;
      snd_sfx_play(happypong, 128, 128);
    } else if(IsGamepadButtonPressed(1, GAMEPAD_BUTTON_MIDDLE_RIGHT)){
      gameMode = 0;
      resetValues();
      twoControllers = true;
      snd_sfx_play(happypong, 128, 128);
    }

    break;

    case 2: // winner screen

    // draw Juice Lizard linktree QR code with rounded corners
    DrawTexturePro(QRCodeNB64x64, {0, 0, 64, 64}, {160, 80, 320, 320}, {0, 0}, 0, WHITE);
    
    DrawTexturePro(pinkCorners, {0, 0, 8, 8}, {175, 95, 8, 8}, {0, 0}, 0, WHITE);
    DrawTexturePro(pinkCorners, {8, 0, 8, 8}, {457, 95, 8, 8}, {0, 0}, 0, WHITE);
    DrawTexturePro(pinkCorners, {0, 8, 8, 8}, {175, 377, 8, 8}, {0, 0}, 0, WHITE);
    DrawTexturePro(pinkCorners, {8, 8, 8, 8}, {457, 377, 8, 8}, {0, 0}, 0, WHITE);

    DrawTexturePro(credits512x64, {0, 0, 512, 64}, {64, 400, 512, 64}, {0, 0}, 0, WHITE);

    drawScore();

  // go back to the titlescreen
    if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_RIGHT)) {
      gameMode = 0;
      resetValues();
      twoControllers = false;
      snd_sfx_play(happypong, 128, 128);
    } else if(IsGamepadButtonPressed(1, GAMEPAD_BUTTON_MIDDLE_RIGHT)) {
      gameMode = 0;
      resetValues();
      twoControllers = true;
      snd_sfx_play(happypong, 128, 128);
    }
    
    }

    EndDrawing();
  }

  // unload sounds
  snd_sfx_unload_all();

  snd_sfx_stop(ronron_loop_channel);
	snd_sfx_chn_free(ronron_loop_channel);
	ronron_loop_channel = -1;

  // unload textures
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

  UnloadTexture(credits512x64);
  UnloadTexture(QRCodeNB64x64);
  UnloadTexture(pinkCorners);
  UnloadTexture(H128x128);
  UnloadTexture(A128x256);
  UnloadTexture(Pa128x256);
  UnloadTexture(Pb128x256);
  UnloadTexture(Y128x256);
  UnloadTexture(Pc128x256);
  UnloadTexture(O128x128);
  UnloadTexture(N128x128);
  UnloadTexture(G128x128);

  snd_shutdown();

// go back to Dreamcast menu
  arch_menu();

  return 0;
}
