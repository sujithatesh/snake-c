#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600

#define TILE_SIZE 20

enum Direction {
	W = 0,
	A,
	S,
	D
};

typedef struct snake{
	Rectangle rec;
	Direction dir;
	struct snake* prev;
	struct snake* next;
}snake;

snake* addSnake(snake* head){
	snake* newHead = (snake*)malloc(sizeof(snake));
	newHead->rec = head->rec;
	newHead->next = head;
	newHead->prev = NULL;
	newHead->dir = head->dir;
	switch(newHead->dir){
		case W:
			break;
		case A:
			break;
		case S:
			break;
		case D:
			break;
	}
	return newHead;
}

int main(void){
	snake* head = (snake*)malloc(sizeof(snake));
	head->rec.x = 20;
	head->rec.y = 20;
	head->next = NULL;
	head->prev = NULL;
	head->dir = D;
	SetRandomSeed(time(NULL));
	double accumTime = 0;

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake");
	while(!WindowShouldClose()){
		BeginDrawing();
		ClearBackground(RAYWHITE);

		accumTime += GetFrameTime();
		//Keyboard movement: 
		if(IsKeyDown(KEY_W)){
			head->rec.y -= TILE_SIZE;
		}
		if(IsKeyDown(KEY_A)){
			head->rec.x -= TILE_SIZE;
		}
		if(IsKeyDown(KEY_S)){
			head->rec.y += TILE_SIZE;
		}
		if(IsKeyDown(KEY_D)){
			head->rec.x += TILE_SIZE;
		}
		if(accumTime > 0.5){
			accumTime = 0;
		}
		DrawRectangle(head->rec.x, head->rec.y, TILE_SIZE, TILE_SIZE, RED);
		EndDrawing();
	}
	return 0;
}
