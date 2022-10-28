/*---------------------------------------------------------------------------------

	Paddle by jacobo
	Copyright 2022
	No Rights Reserved.
	If you need a licence, this game uses the Unlicence

---------------------------------------------------------------------------------*/
#include <nds.h>
#include <stdio.h>
#include <gl2d.h>

volatile int frame = 0;

void Vblank(){
	frame++;
}

int main(void) {
	reset:
	touchPosition touchXY;
	int lost = 0;
	int score = 3;
	int highScore = 10;

	struct position{
		int x;
		int y;
	}playerPos, ballPos;
	struct rect{
		int x1;
		int y1;
		int x2;
		int y2;
	}playerRect, ballRect;
	playerPos.x = 10;
	playerPos.y = 1;
	playerRect.x1 = playerPos.x - 2;
	playerRect.x2 = playerPos.x + 2;
	playerRect.y1 = playerPos.y - 10;
	playerRect.y2 = playerPos.y + 10;
	ballPos.x = 45;
	ballPos.y = 96;
	ballRect.x1 = ballPos.x - 5;
	ballRect.x2 = ballPos.x + 5;
	ballRect.y1 = ballPos.y - 5;
	ballRect.y2 = ballPos.y + 5;
	int ballVelX = 1;
	int ballVelY = 1;

	videoSetMode( MODE_5_3D );
	glScreen2D();
	consoleDemoInit();
	irqSet(IRQ_VBLANK, Vblank);
	iprintf("PADDLE by jacobo\nCopyright 2022\nNo Rights Reserved\n\n");
	iprintf("How to Play:\n");
	iprintf("Drag stylus on bottom screen to move paddle up and down.\n");
	iprintf("Keep the ball from touching the left side of the screen.\n");
	iprintf("You start with 3 points.\n");
	iprintf("If you successfully block the\n ball, you gain points.\n");
	iprintf("If you fail to block the\n ball, you lose points.\n");
	iprintf("If you lose all your points, you lose.\n");

	while(1) {
		swiWaitForVBlank();
		lcdMainOnBottom();
        scanKeys();
        int pressed = keysDown();
        if(pressed & KEY_START) break;
        if((pressed & KEY_SELECT) && lost){
            goto reset;
        }
		if(!lost){
			int held = keysHeld();
			if((held & KEY_TOUCH) && !(lost)){
				touchRead(&touchXY);
				playerPos.y = touchXY.py;
			}else if(held & KEY_UP){
				playerPos.y -= 1;
			}else if(held & KEY_DOWN){
				playerPos.y += 1;
			}
		}

		playerRect.x1 = playerPos.x - 2;
		playerRect.x2 = playerPos.x + 2;
		playerRect.y1 = playerPos.y - 10;
		playerRect.y2 = playerPos.y + 10;
		ballPos.x += ballVelX;
		ballPos.y += ballVelY;
		ballRect.x1 = ballPos.x - 5;
		ballRect.x2 = ballPos.x + 5;
		ballRect.y1 = ballPos.y - 5;
		ballRect.y2 = ballPos.y + 5;

		if(ballPos.y >= 192){
			ballVelY = -1;
		}else if(ballPos.y <= 0){
			ballVelY = 1;
		}
		if(ballPos.x >= 256){
			ballVelX = -1;
		}else if(ballPos.x <= 0){
			if(!lost){
				score -= frame % 10;
				consoleClear();
				if(score < 0){
					lost = 1;
					iprintf("Your High Score was %d.\n", highScore);
					iprintf("You let the ball slip past too\n many times!\nYou Lost!\nPress Select to Restart.");
				}else{
					iprintf("High Score: %d;\n", highScore);
					iprintf("Score: %d;\n", score);
				}
			}
			ballVelX = 1;
		}
		if((playerRect.x1 < ballRect.x2) &&
		(playerRect.x2 > ballRect.x1) &&
		(playerRect.y1 < ballRect.y2) &&
		(playerRect.y2 > ballRect.y1)) {
			if(!lost){
				consoleClear();
				score += 1;
				if(highScore < score){
					highScore = score;
				}
				iprintf("High Score: %d;\n", highScore);
				iprintf("Score: %d;\n", score);
			}
			ballVelX = 1;
			if(ballPos.y > playerPos.y+2){
				ballVelY = 1;
			}else if(ballPos.y < playerPos.y-2){
				ballVelY = -1;
			}else{
				ballVelY = 0;
			}
		}

		glBegin2D();
		glBoxFilled(ballRect.x1, ballRect.y1, ballRect.x2, ballRect.y2, RGB15(255, 255, 255));
		glBoxFilled(playerRect.x1, playerRect.y1, playerRect.x2, playerRect.y2, RGB15(255, 0, 0));
		glEnd2D();
		glFlush(0);
	}

}
