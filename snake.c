#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>
#include <time.h>
#include <sys/mman.h>
#include <unistd.h>

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600
#define TILE_SIZE 20
#define ARENA_SIZE (1024 * 1024)

typedef enum Direction {
    W = 0, A, S, D
} Direction;

typedef struct snake {
    Rectangle rec;
    Direction dir;
    struct snake* prev;
    struct snake* next;
} snake;

typedef struct {
    void* memory;
    size_t size;
    size_t used;
} Arena;

Arena arena_create(size_t size) {
    void* memory = mmap(NULL, size, 
                        PROT_READ | PROT_WRITE, 
                        MAP_PRIVATE | MAP_ANONYMOUS, 
                        -1, 0);
    return (Arena){ 
        .memory = memory, 
        .size = size, 
        .used = 0 
    };
}

void* arena_alloc(Arena* arena, size_t size) {
    size = (size + 7) & ~7;  // Align to 8 bytes
    
    if (arena->used + size > arena->size) {
        return NULL;
    }
    
    void* ptr = (char*)arena->memory + arena->used;
    arena->used += size;
    return ptr;
}

void arena_reset(Arena* arena) {
    arena->used = 0;
}

void arena_destroy(Arena* arena) {
    munmap(arena->memory, arena->size);
    arena->memory = NULL;
    arena->size = 0;
    arena->used = 0;
}

snake* addSnake(snake* head, Arena* arena) {
    snake* newHead = arena_alloc(arena, sizeof(snake));
    newHead->rec = head->rec;
    newHead->next = head;
    newHead->prev = NULL;
    newHead->dir = head->dir;
    
    switch(newHead->dir){
        case W: newHead->rec.y -= TILE_SIZE; break;
        case A: newHead->rec.x -= TILE_SIZE; break;
        case S: newHead->rec.y += TILE_SIZE; break;
        case D: newHead->rec.x += TILE_SIZE; break;
    }
    
    return newHead;
}

int main(void) {
    Arena arena = arena_create(ARENA_SIZE);
    
    snake* head = arena_alloc(&arena, sizeof(snake));
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
    InitAudioDevice();
    Sound food = LoadSound("/home/sujith/Documents/snake-c/music/food.mp3");
    Sound music = LoadSound("/home/sujith/Documents/snake-c/music/music.mp3");
    Sound gameOverSound = LoadSound("/home/sujith/Documents/snake-c/music/gameover.mp3");
    Sound move = LoadSound("/home/sujith/Documents/snake-c/music/move.mp3");

    SetSoundVolume(music, 0.1);
    PlaySound(music);
    
    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(RAYWHITE);
        accumTime += GetFrameTime();
        
        if(IsKeyDown(KEY_W)){
            head->dir = W;
            PlaySound(move);
        }
        if(IsKeyDown(KEY_A)){
            head->dir = A;
            PlaySound(move);
        }
        if(IsKeyDown(KEY_S)){
            head->dir = S;
            PlaySound(move);
        }
        if(IsKeyDown(KEY_D)){
            head->dir = D;
            PlaySound(move);
        }
        
        if(accumTime > 1 / speed){
            accumTime = 0;
            head = addSnake(head, &arena);
            first->prev = head;
            head->next = first;
            first = head;
            
            if(last->prev != NULL && !stop){
                last = last->prev;
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
                PlaySound(food);
                score++;
                speed += 0.5;
                stop = true;
            }
        }
        else{
            DrawText("Game Over!!", SCREEN_WIDTH / 2 - 5 * TILE_SIZE, SCREEN_HEIGHT / 2 - TILE_SIZE, TILE_SIZE, RED);
            PlaySound(gameOverSound);
        }
        
        sprintf(buff, "Score : %d", score);
        DrawText(buff, SCREEN_WIDTH - 12 * TILE_SIZE, TILE_SIZE, TILE_SIZE, RED);
        DrawFPS(20,20);
        EndDrawing();
    }
    
    arena_destroy(&arena);
    CloseWindow();
    
    return 0;
}
