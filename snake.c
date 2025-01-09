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

snake* addSnake(snake* head){
    int dir = head->direction;
    printf("add\n");

    snake* head1 = (snake*)malloc(sizeof(snake));
    head1->next = head;
    head1->name = "head1""cc";
    head1->direction = head->direction;

    switch(dir){
        case W:
            head1->rect = (Rectangle){head->rect.x , head->rect.y, TILE_SIZE, TILE_SIZE};
            break;
        case A:
            head1->rect = (Rectangle){head->rect.x , head->rect.y, TILE_SIZE, TILE_SIZE};
            break;
        case S:
            head1->rect = (Rectangle){head->rect.x , head->rect.y, TILE_SIZE, TILE_SIZE};
            break;
        case D:
            head1->rect = (Rectangle){head->rect.x , head->rect.y, TILE_SIZE, TILE_SIZE};
            break;
        default:
            assert("FIXME!: Head doesn't have a direction");
            break;
    }
    return head1;
}

typedef struct Vec2{
    int x;
    int y;
}Vec2;

int main(void){ 
    snake* head = (snake*)malloc(sizeof(snake));
    head->rect = (Rectangle){TILE_SIZE, TILE_SIZE, TILE_SIZE, TILE_SIZE};
    head->name = "head";
    head->next = NULL;
    head->direction = D;
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

    head = addSnake(head);
    head = addSnake(head);
    head = addSnake(head);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake");
    float timer = 0.0f;
    int test = 0;
    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(RAYWHITE);
        if(!GameOver){
            float deltaTime = GetFrameTime();

            sprintf(scoreBuff,"%d",score);
            DrawText(scoreBuff, 600 - 20, 20, 20, RED);

            // movement
            timer += deltaTime;
            if(timer > (1.0 / speed)){
                timer = 0;
                snake* temp = head;
                temp = temp->next;
                while(temp != NULL){
                    if(CheckCollisionRecs(temp->rect, head->rect)){
                        GameOver = false;
                    }
                    if(temp->next != NULL && temp->next->next == NULL){
                        snake* free_temp = temp->next->next;
                        temp->next = NULL;
                        free(free_temp);
                        printf("free\n");
                        head = addSnake(head);
                    }
                    else if(temp->next == NULL){
                        head = addSnake(head);
                        head->next = NULL;
                    }
                    printf("head->next : %p\n", head->next);
                    temp = temp->next;
                    test++;
                }
                switch(head->direction){
                    case W:
                        head->rect.y -= TILE_SIZE;
                        break;
                    case A:
                        head->rect.x -= TILE_SIZE;
                        break;
                    case S:
                        head->rect.y += TILE_SIZE;
                        break;
                    case D:
                        head->rect.x += TILE_SIZE;
                        break;
                    default:
                        assert("Init direction");
                        break;
                }
            }
            if(IsKeyDown(KEY_W) && head->direction != S){
                head->direction = W;
                speed = 6 + speedMultiplier;
            }
            else if(IsKeyDown(KEY_A) && head->direction != D){
                head->direction = A;
                speed = 6 + speedMultiplier;
            }
            else if(IsKeyDown(KEY_S) && head->direction != W){
                head->direction = S;
                speed = 6 + speedMultiplier;
            }
            else if(IsKeyDown(KEY_D) && head->direction != A){
                head->direction = D;
                speed = 6 + speedMultiplier;
            }

            // reset speed
            if(IsKeyReleased(KEY_W) || IsKeyReleased(KEY_S) || IsKeyReleased(KEY_D) || IsKeyReleased(KEY_A)){
                speed = 2 + speedMultiplier;
            }

            //drawing
            snake* temp = head;
            int counter = 0;
            while(temp && (temp->rect.x >= 0 && temp->rect.x <= SCREEN_WIDTH) &&
                           (temp->rect.y >= 0 && temp->rect.y <= SCREEN_WIDTH) ){
                if(counter%2 == 0)
                    DrawRectangleRec(temp->rect, RED);
                else
                    DrawRectangleRec(temp->rect, BLACK);
                temp = temp->next;
                counter++;
            }

            // add score
            //check collision with fruit
            if( CheckCollisionRecs(fruit, head->rect))
            {
                // test++;
                if(flag == true)
                    head = addSnake(&head);
                printf("head : %p \nhead->next : %p\n", head, head->next);
                flag = false;
                speedMultiplier += 0.2;
                score++;
            }

            //go around
            if(head->rect.x > 600) head->rect.x = 0;
            if(head->rect.x < 0) head->rect.x = 600;
            if(head->rect.y > 600) head->rect.y = 0;
            if(head->rect.y < 0) head->rect.y = 600;

            if(flag)
                DrawRectangle(fruit.x, fruit.y, TILE_SIZE, TILE_SIZE, GREEN);
            if(flag == false){
                fruit.x = GetRandomValue(TILE_SIZE, SCREEN_WIDTH - TILE_SIZE);
                fruit.y = GetRandomValue(TILE_SIZE, SCREEN_HEIGHT - TILE_SIZE);
                flag = true;
            }
        }
        else{
            ClearBackground(RAYWHITE);
            DrawText("Game Over!\n Score: ", SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 2, 20, RED);
            DrawText(scoreBuff , SCREEN_WIDTH / 2 , SCREEN_HEIGHT / 2 + 22, 20, RED);
        }

            DrawFPS(50, 50);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
