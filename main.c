//Brick Breaker
//Author: Sonika Finch

#include <stdlib.h>
#include <stdio.h>
#include "mylib.h"
#include "text.h"
#include "welcomeScreen.h"
#include "winScreen.h"
#include "lossScreen.h"
#include "trampoline.h"

int main() {


	REG_DISPCTL = MODE3 | BG2_ENABLE;

	//checks
	int state = 0;
	int screenDrawn = 0;
	int roundStarted = 0;

	//Initialize slider
	SLIDER currSlider;
	currSlider.pos = ((SCREEN_RIGHT - SCREEN_LEFT) / 2) - (SLIDER_WIDTH / 2);
	
	SLIDER oldSlider = currSlider;


	//Initialize ball
	int cDels[] = {-1, 1};
	int numcDels = sizeof(cDels) / sizeof(cDels[0]);

	BALL currBall;
	currBall.row = 0;
	currBall.col = 0;
	currBall.rd = 0;
	currBall.cd = 0;
	BALL oldBall = currBall;

	//Initialize bricks
	int ballHitBrick = 0;
	int ballHitTopBot = 0;
	int numActiveBricks = NUM_BRICKS;
	BRICK bricks[NUM_BRICKS];

	int brickRows[] = {10, 30, 50};
	int brickCols[] = {10, 46, 82, 118, 154, 190};

	//Initialize lives
	int lives = 3;
	char livesBuffer[20];
	sprintf(livesBuffer, "Lives: %d", lives);

	//Initialize instuction text
	char* instBuffer = "Press A to play";


	while (1) {
		switch(state) {

			//welcome screen
			case 0:
				if (!screenDrawn) {
					drawImage3(0, 0, 240, 160, welcomeScreen_data);
					screenDrawn = 1;
				}
				//set up game play state
				if (KEY_DOWN_NOW(BUTTON_START)) {
					//set up background
					drawRect(SCREEN_TOP, SCREEN_LEFT, SCREEN_BOTTOM - SCREEN_TOP, SCREEN_RIGHT, BGCOLOR);
					//set up bottom bar
					lives = 3;
					drawRect(BAR_TOP, BAR_LEFT, BAR_BOTTOM - BAR_TOP, BAR_RIGHT, BAR_COLOR);
					drawString(BAR_TOP + 3, BAR_LEFT + 3, livesBuffer, TEXT_COLOR);

					//set up inbetween space
					drawRect(SCREEN_BOTTOM, SCREEN_LEFT, BAR_TOP - SCREEN_BOTTOM, SCREEN_RIGHT, BGCOLOR);


					//signify round
					roundStarted = 0;

					//set up ball
					currBall.row = SCREEN_BOTTOM - SLIDER_HEIGHT - BALL_SIZE;
					currBall.col = currSlider.pos + (SLIDER_WIDTH / 2) - (BALL_SIZE / 2);
					currBall.rd = 0;
					currBall.cd = 0;

					oldBall = currBall;

					//set up bricks
					numActiveBricks = NUM_BRICKS;
					for (int i = 0; i < 6; i++) { //1st row of bricks
						bricks[i].hit = 0;
						bricks[i].row = brickRows[0];
						bricks[i].col = brickCols[i];
					}
					for (int i = 6; i < 12; i++) { //2nd row of bricks
						bricks[i].hit = 0;
						bricks[i].row = brickRows[1];
						bricks[i].col = brickCols[i - 6];
					}
					for (int i = 12; i < 18; i++) { //3rd row of bricks
						bricks[i].hit = 0;
						bricks[i].row = brickRows[2];
						bricks[i].col = brickCols[i - 12];
					}

					//change to game play state
					state = 1;

				}

			break;

			//game play screen
			case 1:

				//before game starts
				if (!roundStarted) {

					//keep ball on slider
					currBall.row = SCREEN_BOTTOM - SLIDER_HEIGHT - BALL_SIZE;
					currBall.col = currSlider.pos + (SLIDER_WIDTH / 2) - (BALL_SIZE / 2);
					drawString(BAR_TOP + 3, BAR_LEFT + 145, instBuffer, TEXT_COLOR);

				}

				//start of round
				if (KEY_DOWN_NOW(BUTTON_A) && !roundStarted) {
					currBall.rd = 1;
					currBall.cd = cDels[rand() % numcDels];
					roundStarted = 1;
				}

				//slider moves left
				if (KEY_DOWN_NOW(BUTTON_LEFT)) {
					currSlider.pos -= SLIDER_SPEED;	//slider moves to the left
					if(currSlider.pos < SCREEN_LEFT) {  //check for going off the page
						currSlider.pos = SCREEN_LEFT;
					}
				}

				//slider moves right
				if (KEY_DOWN_NOW(BUTTON_RIGHT)) {	//user presses right
					currSlider.pos += SLIDER_SPEED;	//slider moves to the right
					if(currSlider.pos + SLIDER_WIDTH > SCREEN_RIGHT) {  //check for going off the page
						currSlider.pos = SCREEN_RIGHT - SLIDER_WIDTH;
					}
				}

				//move ball
				currBall.row += currBall.rd;
				currBall.col += currBall.cd;

				//ball hits top
				if (currBall.row <= SCREEN_TOP) {
					currBall.row = SCREEN_TOP;
					currBall.rd = -currBall.rd;
				}

				//ball hits bottom
				if(currBall.row + BALL_SIZE >= SCREEN_BOTTOM - SLIDER_HEIGHT) {
					//check for not slider
					if (((currBall.col + BALL_SIZE) < currSlider.pos)
					 || (currBall.col > (currSlider.pos + SLIDER_WIDTH))) {
						lives--;
						roundStarted = 0;
					} else {
						//ball changes directions if hits the first or last third of the slider
						int ballMid = currBall.col + (BALL_SIZE / 2);
						int firstThird = currSlider.pos + (SLIDER_WIDTH / 3);
						int lastThird = (currSlider.pos + ((SLIDER_WIDTH / 3) * 2));
						if ((ballMid < firstThird) || (ballMid > lastThird)) {
							currBall.cd = -currBall.cd;
						}

						currBall.row = SCREEN_BOTTOM - SLIDER_HEIGHT - BALL_SIZE;
						currBall.rd = -currBall.rd;
					}
				}

				//ball hits right
				if (currBall.col + BALL_SIZE >= SCREEN_RIGHT) {
					currBall.col = SCREEN_RIGHT - BALL_SIZE;
					currBall.cd = -currBall.cd;
				}

				//ball hits left
				if (currBall.col <= SCREEN_LEFT) {
					currBall.col = SCREEN_LEFT;
					currBall.cd = -currBall.cd;
				}

				//ball hits brick
				for (int i = 0; i < NUM_BRICKS; i++) {
					if (!bricks[i].hit) {
						ballHitBrick = 0;
						ballHitTopBot = 0;

						if (!((currBall.col + BALL_SIZE) < bricks[i].col)) {
							if (!(currBall.col > (bricks[i].col + BRICK_WIDTH))) {
								if ((currBall.row + BALL_SIZE) == bricks[i].row) {
									//ball hit top side of brick
									ballHitBrick = 1;
									ballHitTopBot = 1;
								} else if (currBall.row == (bricks[i].row + BRICK_HEIGHT)) {
									//ball hit bottom side of brick
									ballHitBrick = 1;
									ballHitTopBot = 1;
								}
							}
						} else if (!(currBall.row > (bricks[i].row + BRICK_HEIGHT))) {
							if (!((currBall.row + BALL_SIZE) < bricks[i].row)) {
								if ((currBall.col + BALL_SIZE) == bricks[i].col) {
									//ball hit left side of brick
									ballHitBrick = 1;
								} else if (currBall.col == (bricks[i].col + BRICK_WIDTH)) {
									//ball hit right side of brick
									ballHitBrick = 1;
								}
							}
						}

						if (ballHitBrick) {
							bricks[i].hit++;
							numActiveBricks--;
							if (ballHitTopBot) {
								currBall.rd = -currBall.rd;
							} else {
								currBall.cd = -currBall.cd;
							}
						}
					}
				}

				waitForVblank();

				//Erase old bricks and draw new ones
				for (int i = 0; i < NUM_BRICKS; i++) {
					if (bricks[i].hit) {
						drawRect(bricks[i].row, bricks[i].col, BRICK_HEIGHT, BRICK_WIDTH, BGCOLOR);
					} else {
						drawRect(bricks[i].row, bricks[i].col, BRICK_HEIGHT, BRICK_WIDTH, BRICK_COLOR);
					}
				}

				//Erase old slider
				drawRect(SCREEN_BOTTOM - SLIDER_HEIGHT, oldSlider.pos, SLIDER_HEIGHT, SLIDER_WIDTH, BGCOLOR);
				//Create new slider
				//drawRect(SCREEN_BOTTOM - SLIDER_HEIGHT, currSlider.pos, SLIDER_HEIGHT, SLIDER_WIDTH, SLIDER_COLOR);
				drawImage3(SCREEN_BOTTOM - SLIDER_HEIGHT, currSlider.pos, SLIDER_WIDTH, SLIDER_HEIGHT, trampoline_data);

				oldSlider = currSlider;

				//Erase old ball
				drawRect(oldBall.row, oldBall.col, BALL_SIZE, BALL_SIZE, BGCOLOR);
				//Create new ball
				drawRect(currBall.row, currBall.col, BALL_SIZE, BALL_SIZE, BALL_COLOR);
				oldBall = currBall;

				sprintf(livesBuffer, "Lives: %d", lives);
				drawRect(BAR_TOP, BAR_LEFT, BAR_BOTTOM - BAR_TOP, BAR_RIGHT, BAR_COLOR);
				drawString(BAR_TOP + 3, BAR_LEFT + 5, livesBuffer, TEXT_COLOR);


				//end of game
				if (!numActiveBricks || !lives || KEY_DOWN_NOW(BUTTON_SELECT)) {
					//win
					if (!numActiveBricks) {
						state = 2;
						screenDrawn = 0;

					//lose
					} else if (!lives) {
						state = 3;
						screenDrawn = 0;
					}

					//reset
					if (KEY_DOWN_NOW(BUTTON_SELECT)) {
						state = 0;
						screenDrawn = 0;
					}
				}
			break;

			//win screen
			case 2:
				//set up
				if (!screenDrawn) {
					drawImage3(0, 0, 240, 160, winScreen_data);
					screenDrawn = 1;
				}

				//play again
				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					state = 0;
					screenDrawn = 0;
				}

			break;

			//loss screen
			case 3:
				//set up
				if (!screenDrawn) {
					drawImage3(0, 0, 240, 160, lossScreen_data);
					screenDrawn = 1;
				}
				//play again 
				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					state = 0;
					screenDrawn = 0;
				}
		}
	}
}
