#include <stdio.h>
#include <stdlib.h>

#include "arbero/engine.h"
#include "pd_api.h"
#include "math.h"

static int update(void* userdata);

PlaydateAPI* pd;

//RENDERING
LCDBitmap* screenBuffer;
uint8_t* screenBufferData;


float mtx17c[4][4] ={
	{16.0/17,  8.0/17, 14.0/17,  6.0/17},
	{4.0/17, 12.0/17,  2.0/17, 10.0/17},
	{13.0/17,  5.0/17, 15.0/17,  7.0/17},
	{1.0/17,  9.0/17,  3.0/17, 11.0/17}
};

float mtx9c[4][4] = {
	{8.0/9, 4.0/9, 7.0/9, 3.0/9},
	{2.0/9, 6.0/9, 1.0/9, 5.0/9},
	{7.0/9, 3.0/9, 8.0/9, 4.0/9},
	{1.0/9, 5.0/9, 2.0/9, 6.0/9}
};


//Delta time
unsigned int lastFrame;
int deltaTimeMS;


void drawPixel(int x, int y){
	int byteIndex = 52 * y + x / 8; //52 Rowbytes
	int bitIndex = 7 - x % 8;
	screenBufferData[byteIndex] &= ~(1 << bitIndex);
}

int eventHandler(PlaydateAPI* playdate, PDSystemEvent event, uint32_t arg)
{
	(void)arg;
	
	if ( event == kEventInit )
	{
		pd = playdate;
		pd->system->setUpdateCallback(update, pd);
		screenBuffer = pd->graphics->newBitmap(400, 240, kColorWhite);
		pd->graphics->getBitmapData(screenBuffer, NULL, NULL, NULL, NULL, &screenBufferData);
		pd->display->setRefreshRate(0);
		
		for (int i = 0; i < 400; i++)
		{
			for (int j = 0; j < 240; j++)
			{
				if(i%2 == 0 && j%2 == 0){
					drawPixel(i, j);
				} 
			}
		}
		
	}
	return 0;
}








static int update(void* userdata){
	//Check player location
	deltaTimeMS = pd->system->getCurrentTimeMilliseconds() - lastFrame;
	lastFrame = pd->system->getCurrentTimeMilliseconds();
   
   
    //TODO Optimize by  only update what went from black to white and the oppostite
	//screenBuffer = pd->graphics->newBitmap(400, 240, kColorWhite);
	//pd->graphics->getBitmapData(screenBuffer, NULL, NULL, NULL, NULL, &screenBufferData);
	


	pd->graphics->drawBitmap(screenBuffer, 0, 0, 0);
	pd->system->drawFPS(10,10);
	pd->system->resetElapsedTime();
	return 1;
}

