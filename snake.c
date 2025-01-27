#include <raylib.h>
#include <stddef.h>
#include <assert.h>
#include <stdio.h>
#include <sys/mman.h>

#define SCREEN_HEIGHT 600
#define SCREEN_WIDTH 600
#define TILE_SIZE 20
#define MEGABYTE 1024 * 1024

typedef struct{
	void* memory;
	size_t capacity;
	size_t used;
}Arena;

Arena arena_commit(size_t size){
	void* memory = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, 0, 0);
	assert(memory != NULL);
	return (Arena){
		.memory = memory,
		.capacity = size,
		.used = 0
	};
}

void* arena_alloc(Arena* arena, size_t size){
	if(arena->used + size > arena->capacity){
		return NULL;
	}
	void* pointer =  (char*)arena->memory + arena->used;
	arena->used += size;
	return pointer;
}

void arena_reset(Arena* arena){
	arena->capacity = 0;
	arena->used = 0;
	arena->memory = NULL;
}

void arena_destroy(Arena* arena){
	arena_reset(arena);
	munmap(arena, arena->capacity);
}

typedef enum{
	W, A, S, D
} Direction;

typedef struct snake{
	Rectangle rec;
	Direction dir;
	struct snake* next;
	struct snake* prev; 
}snake;

snake* addSnake(Arena* arena, snake* head){
	snake* newHead = arena_alloc(arena, sizeof(snake));
	newHead->rec = head->rec;
	newHead->prev = NULL;
	newHead->next = head;
	newHead->dir = head->dir;
	head->prev = newHead;
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
	}
	return newHead;
}

void delSnake(snake* head, snake* last){
	if(!last->prev){
		return;
	}
	last->prev->next = NULL;
}

int main(void){
	Arena arena = arena_commit(MEGABYTE);
	snake* head = arena_alloc(&arena, sizeof(snake));
	float speed = 2;
	head->rec = (Rectangle){.x = 20, .y = 20, .width = TILE_SIZE, .height = TILE_SIZE};
	head->next = NULL;
	head->prev = NULL;
	head->dir = D;
	snake* last = head;
	Rectangle fruit = {.x = 60, .y = 20, .width = TILE_SIZE, .height = TILE_SIZE};
	int score = 0;
	int GameOver = 0;
	char* buff = arena_alloc(&arena, 18);

	float time = 0;
	InitWindow(SCREEN_HEIGHT, SCREEN_WIDTH ,"snake");
	InitAudioDevice();
	SetMasterVolume(0.3);
	Sound food = LoadSound("/home/sujith/Documents/snake-c/music/food.mp3");
	Sound background= LoadSound("/home/sujith/Documents/snake-c/music/music.mp3");
	Sound gameOver = LoadSound("/home/sujith/Documents/snake-c/music/gameover.mp3");
	Sound move = LoadSound("/home/sujith/Documents/snake-c/music/move.mp3");
	SetSoundVolume(background, 0.05f);

	
	PlaySound(background);

	head = addSnake(&arena, head);
	while(!WindowShouldClose()){
		BeginDrawing();
		ClearBackground(RAYWHITE);

		if(!GameOver){
			time += GetFrameTime();
			if(IsKeyDown(KEY_W) && (head->dir != S && head->dir != W)){
				head->dir = W;
				PlaySound(move);
			}
			if(IsKeyDown(KEY_A) && (head->dir != D && head->dir != A)){
				head->dir = A;
				PlaySound(move);
			}
			if(IsKeyDown(KEY_S) && (head->dir != W && head->dir != S)){
				head->dir = S;
				PlaySound(move);
			}
			if(IsKeyDown(KEY_D) && (head->dir != A && head->dir != D)){
				head->dir = D;
				PlaySound(move);
			}

			if(head->rec.x > SCREEN_WIDTH)
				head->rec.x = 0;
			if(head->rec.y < 0)
				head->rec.y = SCREEN_HEIGHT;
			if(head->rec.x < 0)
				head->rec.x = SCREEN_WIDTH;
			if(head->rec.y < 0)
				head->rec.y = SCREEN_HEIGHT;

			if(time > 1/speed){
				head = addSnake(&arena, head);
				last = last->prev;
				time = 0;
			}

			snake* temp = head;
			while(temp != NULL){
				DrawRectangle(temp->rec.x, temp->rec.y, TILE_SIZE, TILE_SIZE, RED);
				temp = temp->next;
				delSnake(head, last);
				if(temp != NULL && CheckCollisionRecs(temp->rec, head->rec)){
					GameOver = 1;
					PlaySound(gameOver);
				}
			}

			if(CheckCollisionRecs(fruit, head->rec)){
				int tempX = GetRandomValue(TILE_SIZE, SCREEN_WIDTH - TILE_SIZE);
				tempX = tempX - tempX % TILE_SIZE;
				int tempY = GetRandomValue(TILE_SIZE, SCREEN_WIDTH - TILE_SIZE);
				tempY = tempY - tempY % TILE_SIZE;
				fruit.x = tempX;
				fruit.y = tempY;
				head = addSnake(&arena, head);
				score++;
				sprintf(buff, "%d", score);
				speed += 1;
				PlaySound(food);
				StopSound(background);
			}
			DrawRectangle(fruit.x, fruit.y, TILE_SIZE, TILE_SIZE, GREEN);
			DrawText(buff, SCREEN_WIDTH - 60, 20, TILE_SIZE, RED);
		}
		else{
			DrawText(buff, SCREEN_WIDTH/2 - 2, SCREEN_HEIGHT / 2 - TILE_SIZE / 2, TILE_SIZE, RED);
		}
		EndDrawing();
	}
	CloseAudioDevice();
	arena_destroy(&arena);
	return 0;
}

