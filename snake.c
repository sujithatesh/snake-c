#include <raylib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#define TILE_SIZE 20
#define SCREEN_WIDTH 600 
#define SCREEN_HEIGHT 600

int speed = 2;

typedef enum dir{
    W = 0,
    A,
    S,
    D
}dir;

typedef struct snake{
    Rectangle rect;
    struct snake* next;
    dir direction;
    char* name;
}snake;

void addSnake(snake* head){
    int counter = 1;
    int headx = head->rect.x;
    int heady = head->rect.y;

    while(head->next != NULL){
        counter++;
        head = head->next;
    }

    snake* head1 = (snake*)malloc(sizeof(snake));
    head1->next = NULL;
    head1->name = "head1""cc";

    head->next = head1;
    head1->rect = (Rectangle){headx, heady, TILE_SIZE, TILE_SIZE};
}

typedef struct Vec2{
    int x;
    int y;
}Vec2;

int main(void){ 
    snake head;
    head.rect = (Rectangle){TILE_SIZE, TILE_SIZE, TILE_SIZE, TILE_SIZE};
    head.name = "head";
    head.next = NULL;
    head.direction = D;
    bool flag = true;
    float speedMultiplier = 0;
    char scoreBuff[10] = "0";
    int score = 0;
    bool GameOver = false;

    time_t seedTime = time(NULL);
    SetRandomSeed((int)seedTime);
    Rectangle fruit;
    fruit.x = GetRandomValue(TILE_SIZE, 600 - TILE_SIZE);
    fruit.y = GetRandomValue(TILE_SIZE, 600 - TILE_SIZE);
    fruit.width = TILE_SIZE;
    fruit.height = TILE_SIZE;

    //debug
    fruit.x = 40;
    fruit.y = 19;

    addSnake(&head);

    int test = 0;
    InitWindow(600,600, "Snake");
    float timer = 0.0f;
    while(!WindowShouldClose()){
        BeginDrawing();
        if(GameOver){
            ClearBackground(RAYWHITE);
            DrawText("GAME OVER!", SCREEN_WIDTH / 2 - 70, SCREEN_HEIGHT / 2, 20, RED);
        }
        else{
            ClearBackground(RAYWHITE);
            float deltaTime = GetFrameTime();

            sprintf(scoreBuff,"%d",score);
            DrawText(scoreBuff, 600 - 20, 20, 20, RED);

            // movement
            timer += deltaTime;
            if(timer > (1.0 / speed)){
                timer = 0;
                snake* temp = &head;
                Rectangle prev = temp->rect;
                Rectangle curr = temp->rect;
                temp = temp->next;
                while(temp != NULL){
                    if(CheckCollisionRecs(head.rect, temp->rect)){
                        // GameOver = true;
                        printf("collision: \n \
                            head.rect.x : %f, head.rect.y : %f\n \
                            temp.rect.x : %f, temp.rect.y : %f\n", head.rect.x, head.rect.y, temp->rect.x, temp->rect.y);
                        break;
                    }
                    curr = temp->rect;
                    temp->rect = prev;
                    prev = curr;
                    temp = temp->next;
                }
                switch(head.direction){
                    case W:
                        head.rect.y -= TILE_SIZE;
                        break;
                    case A:
                        head.rect.x -= TILE_SIZE;
                        break;
                    case S:
                        head.rect.y += TILE_SIZE;
                        break;
                    case D:
                        head.rect.x += TILE_SIZE;
                        break;
                    default:
                        assert("Init direction");
                        break;
                }
            }
            if(IsKeyDown(KEY_W)){
                head.direction = W;
                speed = 6 + speedMultiplier;
            }
            else if(IsKeyDown(KEY_A)){
                head.direction = A;
                speed = 6 + speedMultiplier;
            }
            else if(IsKeyDown(KEY_S)){
                head.direction = S;
                speed = 6 + speedMultiplier;
            }
            else if(IsKeyDown(KEY_D)){
                head.direction = D;
                speed = 6 + speedMultiplier;
            }

            // reset speed
            if(IsKeyReleased(KEY_W) || IsKeyReleased(KEY_S) || IsKeyReleased(KEY_D) || IsKeyReleased(KEY_A)){
                speed = 2 + speedMultiplier;
            }

            //drawing
            snake* temp = &head;
            int counter = 0;
            while(temp){
                if(counter%2 == 0)
                    DrawRectangleRec(temp->rect, RED);
                else
                    DrawRectangleRec(temp->rect, BLACK);
                temp = temp->next;
                counter++;
            }

            //check collision
            if( CheckCollisionRecs(fruit, head.rect))
            {
                test++;
                if(flag == true)
                    addSnake(&head);
                flag = false;
                speedMultiplier += 0.2;
                score++;
            }

            //go around
            if(head.rect.x > 600) head.rect.x = 0;
            if(head.rect.x < 0) head.rect.x = 600;
            if(head.rect.y > 600) head.rect.y = 0;
            if(head.rect.y < 0) head.rect.y = 600;

            if(flag)
                DrawRectangle(fruit.x, fruit.y, TILE_SIZE, TILE_SIZE, GREEN);
            if(flag == false){
                fruit.x = GetRandomValue(TILE_SIZE, 600 - TILE_SIZE);
                fruit.y = GetRandomValue(TILE_SIZE, 600 - TILE_SIZE);
                flag = true;
            }
        }

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
