#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600

#define TILE_SIZE 20

typedef enum Direction {
	W = 0,
	A,
	S,
	D
} Direction;

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
	}
	head = newHead;
	return head;
}

int main(void){
	snake* head = (snake*)malloc(sizeof(snake));
	snake* first = head; 
	snake* last = head;
	bool stop = false;
	bool gameOver = false;
	float speed = 3;
	Rectangle fruit = {.x = 40, .y = 20, .width = TILE_SIZE, .height = TILE_SIZE};
	int score = 0;
	char buff[18];

	head->rec.x = 20;
	head->rec.y = 20;
	head->rec.height = TILE_SIZE;
	head->rec.width = TILE_SIZE;
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
		if(accumTime > 1 / speed){
			accumTime = 0;
			head = addSnake(head);
			first->prev = head;
			head->next = first;
			first = head;
			if(last->prev != NULL && !stop){
				last = last->prev;
				free(last->next);
				last->next = NULL;
			}
			stop = false;
		}

		snake* temp = head;

		if(!gameOver){
			while(temp != NULL ){
				if(CheckCollisionRecs(temp->rec, head->rec) && temp !=head){
					gameOver = true;
				}
				DrawRectangle(temp->rec.x, temp->rec.y, TILE_SIZE, TILE_SIZE, RED);
				temp = temp->next;
			}

			DrawRectangle(fruit.x, fruit.y, TILE_SIZE, TILE_SIZE, GREEN);
			if(CheckCollisionRecs(fruit, head->rec)){
				int tempX = GetRandomValue(TILE_SIZE, SCREEN_WIDTH - TILE_SIZE);
				tempX -= tempX % TILE_SIZE;
				int tempY = GetRandomValue(TILE_SIZE, SCREEN_WIDTH - TILE_SIZE);
				tempY -= tempY % TILE_SIZE;
				fruit.x = tempX;
				fruit.y = tempY;
				score++;
				speed += 0.5;
				stop = true;
			}
		}
		else{
			DrawText("Game Over!!", SCREEN_WIDTH / 2 - 5 * TILE_SIZE, SCREEN_HEIGHT / 2 - TILE_SIZE, TILE_SIZE, RED);
		}
		sprintf(buff, "Score : %d", score);
		DrawText(buff, SCREEN_WIDTH - 12 * TILE_SIZE, TILE_SIZE, TILE_SIZE, RED);

		DrawFPS(20,20);
		EndDrawing();
	}
	return 0;
}
