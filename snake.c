#include <raylib.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600
#define SPEED 2
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
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "snake");
	snake* head = (snake*)malloc(sizeof(snake));
	head->rec.x = 20;
	head->rec.y = 20;
	head->dir= D;
	head->next = NULL;

	bool advanceInTick = true;
	SetRandomSeed(time(NULL));
	Rectangle fruit;
	fruit.x = 40;
	fruit.y = 20;

	float timePassed = 0;
	while(!WindowShouldClose()){
		BeginDrawing();
		ClearBackground(RAYWHITE);


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
			DrawRectangle(temp->rec.x, temp->rec.y, 20, 20, RED);
			printf("counter: %d x : %lf, y:  %lf\n", counter, temp->rec.x, temp->rec.y);
			counter++;
		}

		timePassed += GetFrameTime();
		if(timePassed > 0.5){
			timePassed = 0;

			// if addSnake dont advance
			if(advanceInTick){
				// perpetual motion
				head = addSnake(head);
				while(temp->next != NULL){
					temp = temp->next;
				}
				free(temp->next);
				temp->next = NULL;
			}
		}

		//fruit
		// DrawRectangle(fruit.x, fruit.y, TILE_SIZE, TILE_SIZE, GREEN);
		if(CheckCollisionRecs(fruit, head->rec)){
			printf("Collision!\n");
		}
		EndDrawing();
	}
	return 0;
}
