#include <raylib.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600
#define TILE_SIZE 20

enum Direction{
	W = 0,
	A,
	S,
	D
};
typedef enum Direction Direction;

struct snake{
	Rectangle rec;
	Direction dir;
	struct snake* next;
};
typedef struct snake snake;

snake* addSnake(snake* head){
	snake* newHead = (snake*)malloc(sizeof(snake));
	newHead->next = head;
	newHead->dir = head->dir;
	newHead->rec.x = head->rec.x;
	newHead->rec.y = head->rec.y;
	switch(head->dir){
		case W:
			newHead->rec.y -= TILE_SIZE;
			break;
		case A:
			newHead->rec.x -= TILE_SIZE;
			break;
		case S:
			newHead->rec.y += TILE_SIZE;
			break;
		case D:
			newHead->rec.x += TILE_SIZE;
			break;
		default :
			assert("Direction not set in addSnake");
	}
	return newHead;
}


int main(void){
	int speed = 6;
	bool flag = true;

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "snake");
	snake* head = (snake*)malloc(sizeof(snake));
	head->rec.x = 20;
	head->rec.y = 20;
	head->dir= D;
	head->next = NULL;
	for(int i = 0; i < 4; i++){
		head = addSnake(head);
	}

	bool freeTheEnd = true;
	SetRandomSeed(time(NULL));
	Rectangle fruit;
	fruit.x = 560;
	fruit.y = 20;

	float timePassed = 0;
	while(!WindowShouldClose()){
		BeginDrawing();
		ClearBackground(RAYWHITE);

		flag = true;

		// INPUT
		if(IsKeyDown(KEY_W)){
			head->dir = W;
		}
		if(IsKeyDown(KEY_A)){
			head->dir = A;
		}
		if(IsKeyDown(KEY_S)){
			head->dir = S;
		}
		if(IsKeyDown(KEY_D)){
			head->dir = D;
		}

		int counter = 0;
		snake* temp = head;
		while(temp->next != NULL){
			temp = temp->next;
			if(counter % 2)
				DrawRectangle(temp->rec.x, temp->rec.y, 20, 20, RED);
			else
				DrawRectangle(temp->rec.x, temp->rec.y, 20, 20, BLACK);
			counter++;
		}

		timePassed += GetFrameTime();
		if(timePassed > 1.0f / speed){
			timePassed = 0;

				// perpetual motion
			head = addSnake(head);
			// if addSnake dont advance
			if(freeTheEnd){
				snake* iter = head;
				while(iter->next->next != NULL){
					iter = iter->next;
				}
				free(iter->next);
				iter->next = NULL;
			}
			freeTheEnd = true;
		}

		DrawRectangle(fruit.x, fruit.y, TILE_SIZE, TILE_SIZE, GREEN);
		if(fruit.x == head->rec.x && fruit.y == head->rec.y && flag){
			flag = false;
			addSnake(head);
			int newX = GetRandomValue(TILE_SIZE, SCREEN_WIDTH);
			int newY = GetRandomValue(TILE_SIZE, SCREEN_HEIGHT);
			fruit.x = newX - newX % TILE_SIZE;
			fruit.y = newY - newY % TILE_SIZE;
			freeTheEnd = false;
		}

		DrawFPS(20,20);
		EndDrawing();
	}
	return 0;
}
