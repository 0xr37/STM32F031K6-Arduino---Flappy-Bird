#include <stm32f031x6.h>
#include "display.h"
#include <stdio.h>
#include "prbs.h"
#include "serial.h"
#define BLUE 33020
#define MINSCREENX 0
#define MINSCREENY 0
#define MAXSCREENX 127
#define MAXSCREENY 159
#define PIPEAMT 3
#define PIPEWIDTH 14
#define spawnGap 40 / 2

int isInside(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t px, uint16_t py);
void initClock(void);
void initSysTick(void);
void SysTick_Handler(void);
void delay(volatile uint32_t dly);
void setupIO();
void enablePullUp(GPIO_TypeDef *Port, uint32_t BitNumber);
void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode);
volatile uint32_t milliseconds;


int upPressed();
int downPressed();
int leftPressed();
int rightPressed();
int anyButtonPressed();

void flapping();
void gameLoop();
void initPipe();
void movePipe();

void drawPipe();
void drawPipeTop();
void drawPipeBottom();
void endOfPipeCheck();
void initPipe();


struct pipe{
	int x;
	uint16_t y;
	uint16_t speed;
	uint16_t width;
	uint16_t height;
	uint16_t start;
	uint16_t gap;
	int oldy;
	int oldx;
	int active;
};

struct pipe pipes[PIPEAMT];


const uint16_t bird1[]=
{
	5536,5536,5536,5536,5536,5536,2625,2625,2625,2625,2625,2625,5536,5536,5536,5536,5536,5536,5536,5536,5536,2625,2625,65422,65422,65422,2625,24575,24575,2625,5536,5536,5536,5536,5536,5536,5536,2625,65422,65422,65333,65333,2625,24575,24575,24575,24575,2625,5536,5536,5536,5536,2625,2625,2625,2625,65333,65333,65333,2625,56015,24575,24575,2625,24575,2625,5536,5536,2625,24575,24575,24575,24575,2625,65333,65333,2625,56015,24575,24575,2625,24575,2625,5536,5536,2625,24575,24575,24575,24575,24575,2625,65333,65333,2625,56015,24575,24575,24575,2625,5536,5536,2625,65422,24575,24575,24575,65422,2625,65333,65333,65333,2625,2625,2625,2625,2625,2625,5536,5536,2625,65422,65422,65422,2625,65333,65333,65333,2625,7937,7937,7937,7937,7937,7937,2625,5536,5536,2625,2625,2625,7212,7212,7212,2625,7937,2625,2625,2625,2625,2625,2625,5536,5536,5536,2625,7212,7212,7212,7212,7212,7212,2625,7937,7937,7937,7937,7937,2625,5536,5536,5536,5536,2625,2625,7212,7212,7212,7212,7212,2625,2625,2625,2625,2625,5536,5536,5536,5536,5536,5536,5536,2625,2625,2625,2625,2625,5536,5536,5536,5536,5536,5536,5536,
};

const uint16_t pipe1[]=
{
	2625,2625,2625,2625,2625,2625,2625,2625,2625,2625,2625,2625,2625,2625,2625,2625,2625,2625,2625,2625,2625,64391,31887,64655,31887,39815,14719,6263,22118,5214,29269,4429,28477,11828,60460,2852,18980,10788,27164,2625,2625,23167,56199,23695,48007,47743,39031,22382,46438,37982,62029,45125,3901,60724,52268,60196,10788,2596,18972,2625,2625,2625,2625,2625,2625,2625,2625,2625,2625,2625,2625,2625,2625,2625,2625,2625,2625,2625,2625,2625,5536,2625,53573,45909,54886,47222,23167,64135,55679,14190,13670,4693,61509,3644,19500,10788,59939,10788,2625,5536,5536,2625,62029,5214,22382,22911,64391,48271,31623,39031,30310,4950,53581,44604,52268,19236,2596,18972,2625,5536,5536,2625,53837,62558,14190,14719,56199,31887,14983,30839,22118,62293,45389,36412,44076,19236,2596,18972,2625,5536,5536,2625,53837,62558,14190,14719,56199,31887,14983,30839,22118,62293,45389,36412,44076,19236,2596,18972,2625,5536,5536,2625,62029,62558,14190,14719,56199,31887,14983,30839,22118,62293,45389,36412,44076,19236,2596,18972,2625,5536,5536,2625,53837,62558,14190,14719,56199,31887,14983,30839,22118,62293,45389,36412,44076,19236,2596,18972,2625,5536,5536,2625,53837,62558,14190,14719,56199,31887,14983,30839,22118,62293,45389,36412,44076,19236,2596,18972,2625,5536,5536,2625,53837,62558,14190,14719,56199,31887,14983,30839,22118,62293,45389,36412,44076,19236,2596,18972,2625,5536,5536,2625,53837,62558,14190,14719,56199,31887,14983,30839,22118,62293,45389,36412,44076,19236,2596,18972,2625,5536,5536,2625,53837,62558,14190,14719,56199,31887,14983,30839,22118,62293,45389,36412,44076,19236,2596,18972,2625,5536,5536,2625,53837,62558,14190,14719,56199,31887,14983,30839,22118,62293,45389,36412,44076,19236,2596,18972,2625,5536,5536,2625,53837,62558,14190,14719,56199,31887,14983,30839,22118,62293,45389,36412,44076,19236,2596,18972,2625,5536,5536,2625,53837,62558,14190,14719,56199,31887,14983,30839,22118,62293,45389,36412,44076,19236,2596,18972,2625,5536,5536,2625,53837,62558,14190,14719,56199,31887,14983,30839,22118,62293,45389,36412,44076,19236,2596,18972,2625,5536,5536,2625,53837,62558,14190,14719,56199,31887,14983,30839,22118,62293,45389,36412,44076,19236,2596,18972,2625,5536,5536,2625,53837,62558,14190,14719,56199,31887,14983,30839,22118,62293,45389,36412,44076,19236,2596,18972,2625,5536,5536,2625,53837,62558,14190,14719,56199,31887,14983,30839,22118,4950,53581,36412,44076,19236,2596,18972,2625,5536,5536,2625,53837,62558,14190,14719,56199,31887,14983,30839,22118,62293,45389,36412,44076,19236,2596,18972,2625,5536,5536,2625,53837,62558,14190,14719,56199,31887,14983,30839,22118,62293,45389,36412,44076,19236,2596,18972,2625,5536,5536,2625,53837,62558,14190,14719,56199,31887,14983,30839,22118,62293,45389,36412,44076,19236,2596,18972,2625,5536,5536,2625,53837,62558,14190,14719,56199,31887,14983,30839,22118,4950,45389,36412,44076,19236,2596,18972,2625,5536,5536,2625,53837,62558,14190,14719,56199,31887,14983,30839,22118,62293,45389,36412,44076,19236,2596,18972,2625,5536,5536,2625,53837,62558,14190,14719,56199,31887,14983,30839,22118,62293,45389,36412,44076,19236,2596,18972,2625,5536,5536,2625,53837,62558,14190,14719,56199,31887,14983,30839,22118,62293,45389,36412,44076,19236,2596,18972,2625,5536,5536,2625,53837,62558,14190,14719,56199,31887,14983,30839,22118,62293,45389,36412,44076,19236,2596,18972,2625,5536,5536,2625,53837,62558,14190,14719,56199,31887,14983,30839,22118,62293,45389,36412,44076,19236,2596,18972,2625,5536,5536,2625,53837,62558,14190,14719,56199,31887,14983,30839,22118,62293,45389,36412,44076,19236,2596,18972,2625,5536,5536,2625,53837,62558,14190,14719,56199,31887,14983,30839,22118,62293,45389,36412,44076,19236,2596,18972,2625,5536,5536,2625,53837,62558,14190,14719,56199,31887,14983,30839,22118,62293,45389,36412,44076,19236,2596,18972,2625,5536,5536,2625,53837,62558,14190,14719,56199,31887,14983,30839,22118,4950,53581,36412,44076,19236,2596,18972,2625,5536,5536,2625,53837,62558,14190,14719,56199,31887,14983,30839,22118,62293,45389,36412,44076,19236,2596,18972,2625,5536,5536,2625,53837,62558,14190,14719,56199,31887,14983,30839,22118,62293,45389,36412,44076,19236,2596,18972,2625,5536,5536,2625,53837,62558,14190,14719,56199,31887,14983,30839,22118,62293,45389,36412,44076,19236,2596,18972,2625,5536,5536,2625,53837,62558,14190,14719,56199,31887,14983,30839,22118,4950,45389,36412,44076,19236,2596,18972,2625,5536,5536,2625,53837,62558,14190,14719,56199,31887,14983,30839,22118,62293,45389,36412,44076,19236,2596,18972,2625,5536,5536,2625,53837,62558,14190,14719,56199,31887,14983,30839,22118,62293,45389,36412,44076,19236,2596,18972,2625,5536,5536,2625,53837,62558,14190,14719,56199,31887,14983,30839,22118,62293,45389,36412,44076,19236,2596,18972,2625,5536,5536,2625,53837,62558,14190,14719,56199,31887,14983,30839,22118,62293,45389,36412,44076,19236,2596,18972,2625,5536,5536,2625,53837,62558,14190,14719,56199,31887,14983,30839,22118,4950,45389,36412,44076,19236,2596,18972,2625,5536,5536,2625,53837,62558,14190,14719,56199,31887,14983,30839,22118,62293,45389,36412,44076,19236,2596,18972,2625,5536,5536,2625,53837,62558,14190,14719,56199,31887,14983,30839,22118,62293,45389,36412,44076,19236,2596,18972,2625,5536,5536,2625,53837,62558,14190,14719,56199,31887,14983,30839,22118,4950,45389,36412,44076,19236,2596,18972,2625,5536,5536,2625,53837,62558,14190,14719,56199,31887,14983,30839,22118,62293,45389,36412,44076,19236,2596,18972,2625,5536,5536,2625,53837,62558,14190,14719,56199,31887,14983,30839,22118,62293,45389,36412,44076,19236,2596,18972,2625,5536,5536,2625,53837,62558,14190,14719,56199,31887,14983,30839,22118,62293,45389,36412,44076,19236,2596,18972,2625,5536,5536,2625,62029,62558,14190,14719,56199,31887,14983,30839,22118,4950,45389,36412,44076,19236,2596,18972,2625,5536,
};

const uint16_t pipeStart[]=
{
	2625,2625,2625,2625,2625,2625,2625,2625,2625,2625,2625,2625,2625,2625,2625,56463,56463,7047,22911,30574,21854,12877,4165,3380,27692,18980,35356,2625,2625,15239,15495,14983,47479,46702,54366,45645,44869,52532,19500,10788,27164,2625,2625,2625,2625,2625,2625,2625,2625,2625,2625,2625,2625,2625,2625,2625,
};

const uint16_t pipeEnd[]=
{
	5536,2625,13142,55150,14719,39559,39030,46174,45389,60980,43820,10788,2625,5536,
};



int main()
{

	initClock();
	initSysTick();
	setupIO();
	initSerial();
	// putImage(11,151,birdWidth,birdHeight,bird1,0,0); // x, y, width, height, image, hOrientation, vOrientation

	while (1){
		if(anyButtonPressed()){
			break;
		}
	}
	delay(500);
	gameLoop();

	return 0;
}



void movePipe(){
	fillRectangle(pipes[0].oldx, pipes[0].oldy, pipes[0].width, pipes[0].height, 0);
	pipes[0].oldx = pipes[0].x;
	pipes[0].oldy = pipes[0].y;
	pipes[0].x -= pipes[0].speed;
	putImage(pipes[0].x, pipes[0].y, pipes[0].width, pipes[0].height, pipe1, 0, 0);
	
	if (pipes[0].x < 1){
		pipes[0].x = MAXSCREENX - pipes[0].width;
		fillRectangle(MINSCREENX, MINSCREENY, pipes[0].width, pipes[0].height, 0);
	}
}

void drawPipeBottom(int i, int gap){
	fillRectangle(pipes[i].oldx, pipes[i].oldy + gap, pipes[i].width, pipes[i].height, 0);
	pipes[i].oldx = pipes[i].x;
	pipes[i].oldy = pipes[i].y;
	pipes[i].x -= pipes[i].speed;
	putImage(pipes[i].x, pipes[i].y + gap, pipes[i].width, pipes[i].height, pipeStart, 0, 0);
	
	int start = pipes[i].y + pipes[i].height + gap;

	for(int j = start; j <= MAXSCREENY - 1; j++){
		fillRectangle(pipes[i].oldx, j, pipes[i].width, 1, 0);
		putImage(pipes[i].x, j, pipes[i].width, 1, pipeEnd, 0, 0);
	}
}

void drawPipeTop(int i, int gap){

	fillRectangle(pipes[i].oldx, pipes[i].oldy - gap , pipes[i].width, pipes[i].height, 0);
	pipes[i].oldx = pipes[i].x;
	pipes[i].oldy = pipes[i].y;
	pipes[i].x -= pipes[i].speed;

	
	putImage(pipes[i].x, pipes[i].y - gap, pipes[i].width, pipes[i].height, pipeStart, 0, 0);
	
	int start = pipes[i].y - 1 - gap;

	for(int j = start; j >= MINSCREENY + 1; j--){
		fillRectangle(pipes[i].oldx, j, pipes[i].width, 1, 0);
		putImage(pipes[i].x, j, pipes[i].width, 1, pipeEnd, 0, 1);
	}
}

void drawPipe(int gap){
	gap = gap/2;
	for (int i = 0; i < PIPEAMT; i++){
		if (pipes[i].active){
			drawPipeBottom(i, gap);
			drawPipeTop(i, gap);
		}
	}
}

/// for removing the pipe once the pipe is at the end of the screen
void endOfPipeCheck(){
	for (int i = 0; i < PIPEAMT; i++){
		if (pipes[i].active){
			if (pipes[i].x < 1){
				pipes[i].x = MAXSCREENX - pipes[i].width;
				fillRectangle(MINSCREENX, MINSCREENY, pipes[i].width, MAXSCREENY, 0);
				pipes[i].active = 0;
			}
		}
	}
}

void initPipe(){

	for (int i = 0; i < PIPEAMT; i++){
		pipes[i].speed = 1;
		pipes[i].start = 80;
		pipes[i].width = 14; // 20
		pipes[i].height = 4; // 50 (full pipe height)
		pipes[i].x = MAXSCREENX - pipes[0].width;
		pipes[i].y = MAXSCREENY - pipes[0].start;
		pipes[i].oldx = pipes[0].x;
		pipes[i].oldy = pipes[0].y;
		pipes[i].active = 0;
		pipes[i].gap = 40 / 2;
	}

	pipes[0].active = 1;
}

void activatePipes(){
	int start = 60;
	int end = 100;
	for (int i = 0; i < PIPEAMT; i++) {
		if (pipes[i].active && pipes[(i + 1) % PIPEAMT].active == 0 && pipes[i].x <= (MAXSCREENX - pipes[i].spawnGap - PIPEWIDTH)) {
			pipes[(i + 1) % PIPEAMT].active = 1;
			pipes[(i + 1) % PIPEAMT].y = randomNum(60, 100);
		}
	}
}

// void activatePipes(int spawnGap){
// 	for (int i = 0; i < PIPEAMT; i++) {
// 		if (pipes[i].active && pipes[(i + 1) % PIPEAMT].active == 0 && pipes[i].x <= (MAXSCREENX - spawnGap - pipes[i].width)) {
// 			pipes[(i + 1) % PIPEAMT].active = 1;
// 			pipes[(i + 1) % PIPEAMT].start = randomNum(60, 100);
// 		}
// 	}
// }

void gameLoop(){
	int jumpStrength = 3;
	int birdWidth = 17;
	int birdHeight = 12;
	int toggle = 0;
	int gameCrashed = 0;
	int spawnGap = 50;
	int gap = 40;

	float gravity = -0.5;
	float y = 50.0;
	float oldy = y;
	float birdVelocity = 0.0;

	uint16_t x = 35;
	uint16_t oldx = x;

	initPipe();
	int rand = 0;
	int counter = 30;

	char str[20];
	
	while(1)
	{
		if (counter >= 30){
			rand = prbs();
			counter = 0;
		}
		counter +=1;

		if(rightPressed()){
			birdVelocity = jumpStrength;
		}

		if(gameCrashed == 0){
			birdVelocity = birdVelocity + gravity;

			y = y - birdVelocity;

			
			// sprintf(str, "%d\n\r", rand);

			// printNumber(rand, 74, 5, BLUE, 0);

			// eputs(str);

			drawPipe(gap);
			endOfPipeCheck();
			activatePipes(spawnGap);

			

			if ((y <= MAXSCREENY-birdHeight-1) && (y >= 1)){
				fillRectangle(oldx,oldy,birdWidth,birdHeight,0); // x, y, width, height, colour
				oldx = x;
				oldy = y;	

			

			if (toggle)
				putImage(x,y,birdWidth,birdHeight,bird1,0,0);
			else
				putImage(x,y,birdWidth,birdHeight,bird1,0,0);
			
			
			toggle = toggle ^ 1;
			
			}
			else{
				gameCrashed = 1;
			}
		}
		else if (y >= MAXSCREENY - birdHeight){
			delay(50);
			fillRectangle(oldx,oldy,birdWidth,birdHeight,0); // x, y, width, height, colour
			putImage(x,MAXSCREENY - birdHeight,birdWidth,birdHeight,bird1,0,0);
		}
		else{
			delay(50);
			fillRectangle(oldx,oldy,birdWidth,birdHeight,0); // x, y, width, height, colour
			putImage(x,0,birdWidth,birdHeight,bird1,0,0);
		}

		delay(50);
	}
}

int anyButtonPressed(){
	if (downPressed() || upPressed() || leftPressed() || rightPressed()){
		return 1;
	}
	else{
		return 0;
	}
}

int downPressed(){
	if ( (GPIOA->IDR & (1 << 11)) == 0){
		return 1;
	}
	else{
		return 0;
	}
}

int upPressed(){
	if ( (GPIOA->IDR & (1 << 8)) == 0){
		return 1;
	}
	else{
		return 0;
	}
}

int rightPressed(){
	if ((GPIOB->IDR & (1 << 4))==0){
		return 1;
	}
	else{
		return 0;
	}
}

int leftPressed(){
	if ((GPIOB->IDR & (1 << 5))==0){
		return 1;
	}
	else{
		return 0;
	}
}

void initSysTick(void)
{
	SysTick->LOAD = 48000;
	SysTick->CTRL = 7;
	SysTick->VAL = 10;
	__asm(" cpsie i "); // enable interrupts
}
void SysTick_Handler(void)
{
	milliseconds++;
}
void initClock(void)
{
// This is potentially a dangerous function as it could
// result in a system with an invalid clock signal - result: a stuck system
        // Set the PLL up
        // First ensure PLL is disabled
        RCC->CR &= ~(1u<<24);
        while( (RCC->CR & (1 <<25))); // wait for PLL ready to be cleared
        
// Warning here: if system clock is greater than 24MHz then wait-state(s) need to be
// inserted into Flash memory interface
				
        FLASH->ACR |= (1 << 0);
        FLASH->ACR &=~((1u << 2) | (1u<<1));
        // Turn on FLASH prefetch buffer
        FLASH->ACR |= (1 << 4);
        // set PLL multiplier to 12 (yielding 48MHz)
        RCC->CFGR &= ~((1u<<21) | (1u<<20) | (1u<<19) | (1u<<18));
        RCC->CFGR |= ((1<<21) | (1<<19) ); 

        // Need to limit ADC clock to below 14MHz so will change ADC prescaler to 4
        RCC->CFGR |= (1<<14);

        // and turn the PLL back on again
        RCC->CR |= (1<<24);        
        // set PLL as system clock source 
        RCC->CFGR |= (1<<1);
}
void delay(volatile uint32_t dly)
{
	uint32_t end_time = dly + milliseconds;
	while(milliseconds != end_time)
		__asm(" wfi "); // sleep
}

void enablePullUp(GPIO_TypeDef *Port, uint32_t BitNumber)
{
	Port->PUPDR = Port->PUPDR &~(3u << BitNumber*2); // clear pull-up resistor bits
	Port->PUPDR = Port->PUPDR | (1u << BitNumber*2); // set pull-up bit
}
void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode)
{
	/*
	*/
	uint32_t mode_value = Port->MODER;
	Mode = Mode << (2 * BitNumber);
	mode_value = mode_value & ~(3u << (BitNumber * 2));
	mode_value = mode_value | Mode;
	Port->MODER = mode_value;
}
int isInside(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t px, uint16_t py)
{
	// checks to see if point px,py is within the rectange defined by x,y,w,h
	uint16_t x2,y2;
	x2 = x1+w;
	y2 = y1+h;
	int rvalue = 0;
	if ( (px >= x1) && (px <= x2))
	{
		// ok, x constraint met
		if ( (py >= y1) && (py <= y2))
			rvalue = 1;
	}
	return rvalue;
}

void setupIO()
{
	RCC->AHBENR |= (1 << 18) + (1 << 17); // enable Ports A and B
	display_begin();
	pinMode(GPIOB,4,0);
	pinMode(GPIOB,5,0);
	pinMode(GPIOA,8,0);
	pinMode(GPIOA,11,0);
	enablePullUp(GPIOB,4);
	enablePullUp(GPIOB,5);
	enablePullUp(GPIOA,11);
	enablePullUp(GPIOA,8);
}


void exampleRendering(){

	const uint16_t rgb[]=
	{
		7936,7942,59655,49401,7936,7942,59655,49401,49401,59655,7942,7936,49401,59655,7942,7936,7942,7936,49401,59655,7942,7936,49401,59655,59655,49401,7936,7942,59655,49401,7936,7942,
	};

	int minx = 0;
	int miny = 0;
	int maxx = 127;
	int maxy = 159;
	int widthRGB = 8;
	int heightRGB = 4;

	putImage(minx,miny,widthRGB,heightRGB,rgb,0,0); // top left
	putImage(maxx-widthRGB,miny,widthRGB,heightRGB,rgb,0,0); // top right
	putImage(minx,maxy-heightRGB,widthRGB,heightRGB,rgb,0,0); // bottom left
	putImage(maxx-widthRGB,maxy-heightRGB,widthRGB,heightRGB,rgb,0,0); // bottom right
}