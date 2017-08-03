//Author: Sonika Finch

typedef unsigned int u32;
typedef unsigned short u16;

#define REG_DISPCTL *(unsigned short *)0x4000000
#define MODE3 3
#define BG2_ENABLE (1<<10)

#define SCANLINECOUNTER *(volatile unsigned short *)0x4000006

#define OFFSET(row, col, rowlen)  ((row)*(rowlen)+(col))

extern unsigned short *videoBuffer;
extern const unsigned short welcomeScreen[38400];
extern const unsigned short winScreen[38400];
extern const unsigned short lossScreen[38400];
extern const unsigned short trampoline[750];


#define COLOR(r, g, b) ((r) | (g)<<5 | (b)<<10)
#define L_GREEN COLOR(20, 29, 26)
#define GREEN COLOR(10, 20, 16)
#define PINK COLOR(29, 22, 20)
#define L_PINK COLOR(31, 26, 24)
#define RUST COLOR(20, 8, 6)
#define BLACK 0

// Buttons

#define BUTTON_A		(1<<0)
#define BUTTON_B		(1<<1)
#define BUTTON_SELECT	(1<<2)
#define BUTTON_START	(1<<3)
#define BUTTON_RIGHT	(1<<4)
#define BUTTON_LEFT		(1<<5)
#define BUTTON_UP		(1<<6)
#define BUTTON_DOWN		(1<<7)
#define BUTTON_R		(1<<8)
#define BUTTON_L		(1<<9)

#define KEY_DOWN_NOW(key)  (~(BUTTONS) & key)

#define BUTTONS *(volatile unsigned int *)0x4000130

/* DMA */

#define REG_DMA0SAD         *(volatile u32*)0x40000B0 		// source address
#define REG_DMA0DAD         *(volatile u32*)0x40000B4       // destination address
#define REG_DMA0CNT         *(volatile u32*)0x40000B8       // control register

// DMA channel 1 register definitions
#define REG_DMA1SAD         *(volatile u32*)0x40000BC 		// source address
#define REG_DMA1DAD         *(volatile u32*)0x40000C0       // destination address
#define REG_DMA1CNT         *(volatile u32*)0x40000C4       // control register

// DMA channel 2 register definitions
#define REG_DMA2SAD         *(volatile u32*)0x40000C8 		// source address
#define REG_DMA2DAD         *(volatile u32*)0x40000CC       // destination address
#define REG_DMA2CNT         *(volatile u32*)0x40000D0       // control register

// DMA channel 3 register definitions
#define REG_DMA3SAD         *(volatile u32*)0x40000D4 		// source address
#define REG_DMA3DAD         *(volatile u32*)0x40000D8       // destination address
#define REG_DMA3CNT         *(volatile u32*)0x40000DC   

typedef struct {
	const volatile void *src;
	volatile void *dst;
	unsigned int cnt;
} DMA_CONTROLLER;

#define DMA ((volatile DMA_CONTROLLER *) 0x040000B0)

// Defines
#define DMA_CHANNEL_0 0
#define DMA_CHANNEL_1 1
#define DMA_CHANNEL_2 2
#define DMA_CHANNEL_3 3

#define DMA_DESTINATION_INCREMENT (0 << 21)
#define DMA_DESTINATION_DECREMENT (1 << 21)
#define DMA_DESTINATION_FIXED (2 << 21)

#define DMA_SOURCE_INCREMENT (0 << 23)
#define DMA_SOURCE_DECREMENT (1 << 23)
#define DMA_SOURCE_FIXED (2 << 23)

#define DMA_REPEAT (1 << 25)

#define DMA_16 (0 << 26)
#define DMA_32 (1 << 26)

#define DMA_NOW (0 << 28)
#define DMA_AT_VBLANK (1 << 28)
#define DMA_AT_HBLANK (2 << 28)
#define DMA_AT_REFRESH (3 << 28)

#define DMA_IRQ (1 << 30)
#define DMA_ON (1 << 31)

//Slider
#define SLIDER_WIDTH 50
#define SLIDER_HEIGHT 15
#define SLIDER_SPEED 3

//Ball
#define BALL_SIZE 5
#define BALL_COLOR GREEN

//Screen
#define SCREEN_BOTTOM 130
#define SCREEN_TOP 0
#define SCREEN_LEFT 0
#define SCREEN_RIGHT 240
#define BGCOLOR PINK

//Bottom Bar
#define BAR_TOP 145
#define BAR_BOTTOM 160
#define BAR_LEFT 0
#define BAR_RIGHT 240
#define BAR_COLOR GREEN
#define TEXT_COLOR L_PINK

//Brick
#define BRICK_WIDTH 30
#define BRICK_HEIGHT 5
#define BRICK_COLOR RUST
#define NUM_BRICKS 18

typedef struct {
	int pos;
} SLIDER;

typedef struct {
	int row;
	int col;
	int rd;
	int cd;
} BALL;

typedef struct {
	int row;
	int col;
	int hit;
} BRICK;

//prototypes
void drawRect(int row, int col, int height, int width, unsigned short color);
void drawImage3(int row, int col, int width, int height, const unsigned short *image);
void setPixel(int row, int col, unsigned short color);
void waitForVblank();