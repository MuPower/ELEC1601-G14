#include "stdio.h"
#include "stdlib.h"
#include "sdl.h"
#include "SDL2_gfxPrimitives.h"
#include "time.h"

#include "../Headers/formulas.h"
#include "../Headers/wall.h"
#include "../Headers/robot.h"

int done = 0;


int main(int argc, char *argv[]) {
    SDL_Window *window;
    SDL_Renderer *renderer;

    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        return 1;
    }

    window = SDL_CreateWindow("Robot Maze", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, OVERALL_WINDOW_WIDTH, OVERALL_WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
    window = SDL_CreateWindow("Robot Maze", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, OVERALL_WINDOW_WIDTH, OVERALL_WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
    renderer = SDL_CreateRenderer(window, -1, 0);

    struct Robot robot;
    struct Wall_collection *head = NULL;
    int front_left_sensor, front_right_sensor, mid_left_sensor, mid_right_sensor, back_right_sensor, back_left_sensor = 0;
    clock_t start_time, end_time;
    int msec;

    // SETUP MAZE
    // You can create your own maze here. line of code is adding a wall.
    // You describe position of top left corner of wall (x, y), then width and height going down/to right
    // Relative positions are used (OVERALL_WINDOW_WIDTH and OVERALL_WINDOW_HEIGHT)
    // But you can use absolute positions. 10 is used as the width, but you can change this.

    // Template map
/*
    insertAndSetFirstWall(&head, 1,  OVERALL_WINDOW_WIDTH/2, OVERALL_WINDOW_HEIGHT/2, 10, OVERALL_WINDOW_HEIGHT/2);
    insertAndSetFirstWall(&head, 2,  OVERALL_WINDOW_WIDTH/2-100, OVERALL_WINDOW_HEIGHT/2+100, 10, OVERALL_WINDOW_HEIGHT/2-100);
    insertAndSetFirstWall(&head, 3,  OVERALL_WINDOW_WIDTH/2-250, OVERALL_WINDOW_HEIGHT/2+100, 150, 10);
    insertAndSetFirstWall(&head, 4,  OVERALL_WINDOW_WIDTH/2-150, OVERALL_WINDOW_HEIGHT/2, 150, 10);
    insertAndSetFirstWall(&head, 5,  OVERALL_WINDOW_WIDTH/2-250, OVERALL_WINDOW_HEIGHT/2-200, 10, 300);
    insertAndSetFirstWall(&head, 6,  OVERALL_WINDOW_WIDTH/2-150, OVERALL_WINDOW_HEIGHT/2-100, 10, 100);
    insertAndSetFirstWall(&head, 7,  OVERALL_WINDOW_WIDTH/2-250, OVERALL_WINDOW_HEIGHT/2-200, 450, 10);
    insertAndSetFirstWall(&head, 8,  OVERALL_WINDOW_WIDTH/2-150, OVERALL_WINDOW_HEIGHT/2-100, 250, 10);
    insertAndSetFirstWall(&head, 9,  OVERALL_WINDOW_WIDTH/2+200, OVERALL_WINDOW_HEIGHT/2-200, 10, 300);
    insertAndSetFirstWall(&head, 10,  OVERALL_WINDOW_WIDTH/2+100, OVERALL_WINDOW_HEIGHT/2-100, 10, 300);
    insertAndSetFirstWall(&head, 11,  OVERALL_WINDOW_WIDTH/2+100, OVERALL_WINDOW_HEIGHT/2+200, OVERALL_WINDOW_WIDTH/2-100, 10);
    insertAndSetFirstWall(&head, 12,  OVERALL_WINDOW_WIDTH/2+200, OVERALL_WINDOW_HEIGHT/2+100, OVERALL_WINDOW_WIDTH/2-100, 10);
*/

    // Jackson Q's map
/*
    int i = 0;
    while(i < 10){
        if(i%2 != 0){
            insertAndSetFirstWall(&head, i+1, i * OVERALL_WINDOW_WIDTH/10 - 10, 65, 10, OVERALL_WINDOW_HEIGHT/3);

            i = i + 1;
        } else {
            insertAndSetFirstWall(&head, i+1, i * OVERALL_WINDOW_WIDTH/10 - 10, 10, 10, OVERALL_WINDOW_HEIGHT/3*2);
            insertAndSetFirstWall(&head, i+1, i * OVERALL_WINDOW_WIDTH/10 - 10, OVERALL_WINDOW_HEIGHT/3*2 + OVERALL_WINDOW_WIDTH/10, 10, OVERALL_WINDOW_HEIGHT/3*2);
            i = i + 1;
        }
    }
    insertAndSetFirstWall(&head, 11, 0, 0, 10, OVERALL_WINDOW_HEIGHT);
    insertAndSetFirstWall(&head, 12, 0, 0, OVERALL_WINDOW_WIDTH, 10);
    insertAndSetFirstWall(&head, 13, OVERALL_WINDOW_WIDTH - 10, 0, 10, OVERALL_WINDOW_HEIGHT);
    insertAndSetFirstWall(&head, 14, 0, OVERALL_WINDOW_HEIGHT - 10, OVERALL_WINDOW_WIDTH - 60, 10);
    insertAndSetFirstWall(&head, 15, OVERALL_WINDOW_WIDTH - 60, OVERALL_WINDOW_HEIGHT/3*2, 10, OVERALL_WINDOW_HEIGHT/3);
    insertAndSetFirstWall(&head, 16, OVERALL_WINDOW_WIDTH/3 - 15, OVERALL_WINDOW_HEIGHT/3*2, 110, 10);
    insertAndSetFirstWall(&head, 17, OVERALL_WINDOW_WIDTH/3 + 125, OVERALL_WINDOW_HEIGHT/3*2 +60, 110, 10);
    insertAndSetFirstWall(&head, 18, OVERALL_WINDOW_WIDTH/3 + 125, OVERALL_WINDOW_HEIGHT/3*2 +60, 110, 10);
    insertAndSetFirstWall(&head, 18, OVERALL_WINDOW_WIDTH/3 + 125, OVERALL_WINDOW_HEIGHT/3*2 +60, 110, 10);
*/

    // Gabrielle's map 1
/*
    int box_x = 110;
    int box_y = OVERALL_WINDOW_HEIGHT/2 - 120;
    int box_size = 70;
    int x_start = 30;
    int y_start = OVERALL_WINDOW_HEIGHT/2 - 200;
    int gap_size = 80;
    int wall_width = x_start + gap_size * 4 + box_size * 3;
    int wall_height = 180;


    insertAndSetFirstWall(&head, 1,  1, OVERALL_WINDOW_HEIGHT/2 - 40, 30, 10);
    insertAndSetFirstWall(&head, 2,  1, OVERALL_WINDOW_HEIGHT/2 + 40, 30, 10);
    insertAndSetFirstWall(&head, 3,  x_start, y_start, 10, wall_height);
    insertAndSetFirstWall(&head, 3,  x_start, y_start, wall_width, 10);
    insertAndSetFirstWall(&head, 3,  x_start, (y_start + wall_height + gap_size - 10), 10, wall_height);
    insertAndSetFirstWall(&head, 3,  x_start, (y_start + wall_height * 2 + gap_size - 10), wall_width, 10);
    insertAndSetFirstWall(&head, 3,  (x_start + wall_width), y_start, 10, (wall_height - 10));
    insertAndSetFirstWall(&head, 3,  (x_start + wall_width), (y_start + wall_height + gap_size - 10), 10, (wall_height + 10));
    insertAndSetFirstWall(&head, 3,  (x_start + wall_width), (y_start + wall_height - 20), 200, 10);
    insertAndSetFirstWall(&head, 3,  (x_start + wall_width), (y_start + wall_height - 10 + gap_size), 200, 10);

    int box = 1;
    while (box <= 6) {
        insertAndSetFirstWall(&head, 4,  box_x, box_y, 10, box_size);
        insertAndSetFirstWall(&head, 4,  box_x + box_size, box_y, 10, box_size);
        insertAndSetFirstWall(&head, 4,  box_x, box_y, box_size, 10);
        insertAndSetFirstWall(&head, 4,  box_x, box_y + box_size, box_size + 10, 10);
        if (box % 3 == 1 || box % 3 == 2) {
            box_x += 160;
        } else {
            box_x = 110;
            box_y += 160;
        }
        box += 1;
    }
*/

    // Gabrielle's map 2
/*
    int y_start = OVERALL_WINDOW_HEIGHT/2 - 170;
    int x_start = 20;
    int gap = 60;

    int top_tjunction = 1;
    int tjunction_width = 180;
    int tjunction_height = 80;
    while (top_tjunction <= 2) {
        insertAndSetFirstWall(&head, 3, x_start, y_start, tjunction_width, 10);
        insertAndSetFirstWall(&head, 3, x_start, y_start, 10, gap);
        insertAndSetFirstWall(&head, 3, (x_start + tjunction_width), y_start, 10, gap);
        insertAndSetFirstWall(&head, 3, x_start, (y_start + gap), gap, 10);
        insertAndSetFirstWall(&head, 3, (x_start + gap*2), (y_start + gap), gap + 10, 10);
        insertAndSetFirstWall(&head, 3, (x_start + gap), (y_start + gap), 10, tjunction_height);
        insertAndSetFirstWall(&head, 3, (x_start + gap*2), (y_start + gap), 10, tjunction_height);
        insertAndSetFirstWall(&head, 3, (x_start + gap*2), (y_start + gap + tjunction_height), 150, 10);
        x_start += tjunction_width + 20;
        top_tjunction += 1;
    }

    int bottom_deadend = 1;
    x_start = 20 + gap;
    y_start += gap*2 + tjunction_height;
    while (bottom_deadend <= 3) {
        if (bottom_deadend < 3) {
            insertAndSetFirstWall(&head, 5, x_start, y_start, 10, gap);
            insertAndSetFirstWall(&head, 5, (x_start + gap), y_start, 10, gap);
            insertAndSetFirstWall(&head, 5, x_start, (y_start + gap), (gap + 10), 10);
            insertAndSetFirstWall(&head, 5, (x_start + gap), y_start, 40, 10);
        } else {
            insertAndSetFirstWall(&head, 6, x_start, y_start, 10, 250);
            insertAndSetFirstWall(&head, 6, (x_start + gap), y_start, 10, 250);
            insertAndSetFirstWall(&head, 6, (x_start + gap), y_start, 150, 10);
        }
        bottom_deadend += 1;
        x_start += gap + 40;
    }

    insertAndSetFirstWall(&head, 1,  0, OVERALL_WINDOW_HEIGHT/2 - 30, 90, 10);
    insertAndSetFirstWall(&head, 2,  0, OVERALL_WINDOW_HEIGHT/2 + 30, 90, 10);
    insertAndSetFirstWall(&head, 4, (x_start + 110), (y_start - gap), 10, gap + 10);
*/

    // Test maps
    insertAndSetFirstWall(&head, 2,  640-10-220, 400, 10, 80);
    insertAndSetFirstWall(&head, 2,  640-200-20, 400, 200, 10);
    insertAndSetFirstWall(&head, 2,  640-10-20, 50, 10, 350);
    insertAndSetFirstWall(&head, 2,  640-280-20, 50, 280, 10);
    insertAndSetFirstWall(&head, 2,  640-10-300, 50, 10, 100);
    insertAndSetFirstWall(&head, 2,  640-110-300, 150, 110, 10);
    insertAndSetFirstWall(&head, 2,  640-10-400, 50, 10, 100);
    insertAndSetFirstWall(&head, 2,  640-400-220, 50, 220, 10);
    insertAndSetFirstWall(&head, 2,  640-10-620, 50, 10, 290);
    insertAndSetFirstWall(&head, 2,  640-620-20, 340, 20, 10);


    insertAndSetFirstWall(&head, 1,  640-10-320, 300, 10, 180);
    insertAndSetFirstWall(&head, 2,  640-200-120, 300, 200, 10);
    insertAndSetFirstWall(&head, 2,  640-10-120, 150, 10, 150);
    insertAndSetFirstWall(&head, 2,  640-80-120, 150, 80, 10);
    insertAndSetFirstWall(&head, 2,  640-10-200, 150, 10, 100);
    insertAndSetFirstWall(&head, 2,  640-310-200, 250, 310, 10);
    insertAndSetFirstWall(&head, 2,  640-10-500, 150, 10, 100);
    insertAndSetFirstWall(&head, 2,  640-20-500, 150, 20, 10);
    insertAndSetFirstWall(&head, 2,  640-10-520, 150, 10, 290);
    insertAndSetFirstWall(&head, 2,  640-120-520, 440, 120, 10);




    // Code
    setup_robot(&robot);
    updateAllWalls(head, renderer);

    SDL_Event event;

    robot.auto_mode = 1;
    int initial = 1;

    while(!done){
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_RenderClear(renderer);

        //Move robot based on user input commands/auto commands
        if (robot.auto_mode == 1) {
            robotAutoMotorMove(&robot, initial, front_left_sensor, front_right_sensor, mid_left_sensor, mid_right_sensor, back_right_sensor, back_left_sensor);
            if (initial) {
                initial = 0;
            }
        }
        robotMotorMove(&robot);

        //Check if robot reaches endpoint. and check sensor values
        //if (checkRobotReachedEnd(&robot, OVERALL_WINDOW_WIDTH, OVERALL_WINDOW_HEIGHT/2+100, 10, 100)){
        if (checkRobotReachedEnd(&robot, 640-10-320, 480, 100, 10)){ //Maze 4
            end_time = clock();
            msec = (end_time-start_time) * 1000 / CLOCKS_PER_SEC;
            robotSuccess(&robot, msec);
        }
        else if(checkRobotHitWalls(&robot, head))
            robotCrash(&robot);
        //Otherwise compute sensor information
        else {
            front_left_sensor = checkRobotSensorFrontLeftAllWalls(&robot, head);
            if (front_left_sensor>0) {
                //printf("Getting close on the left. Score = %d\n", front_left_sensor);
            }

            front_right_sensor = checkRobotSensorFrontRightAllWalls(&robot, head);
            if (front_right_sensor>0) {
                //printf("Getting close on the right. Score = %d\n", front_right_sensor);
            }

            mid_left_sensor = checkRobotSensorMidLeftAllWalls(&robot, head);
            if (mid_left_sensor>0) {
                //printf("Close to the left. Score = %d\n", mid_left_sensor);
            }

            mid_right_sensor = checkRobotSensorMidRightAllWalls(&robot, head);
            if (mid_right_sensor>0) {
                //printf("Close to the right. Score = %d\n", mid_right_sensor);
            }

            back_right_sensor = checkRobotSensorBackRightAllWalls(&robot, head);
            if (back_right_sensor>0) {
                //printf("Close to the right. Score = %d\n", mid_right_sensor);
            }

            back_left_sensor = checkRobotSensorBackLeftAllWalls(&robot, head);
            if (back_left_sensor>0) {
                //printf("Close to the right. Score = %d\n", mid_right_sensor);
            }

        }
        robotUpdate(renderer, &robot);
        updateAllWalls(head, renderer);

        // Check for user input
        SDL_RenderPresent(renderer);
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                done = 1;
            }
            const Uint8 *state = SDL_GetKeyboardState(NULL);
            if(state[SDL_SCANCODE_UP] && robot.direction != DOWN){
                robot.direction = UP;
            }
            if(state[SDL_SCANCODE_DOWN] && robot.direction != UP){
                robot.direction = DOWN;
            }
            if(state[SDL_SCANCODE_LEFT] && robot.direction != RIGHT){
                robot.direction = LEFT;
            }
            if(state[SDL_SCANCODE_RIGHT] && robot.direction != LEFT){
                robot.direction = RIGHT;
            }
            if(state[SDL_SCANCODE_SPACE]){
                setup_robot(&robot);
            }
            if(state[SDL_SCANCODE_RETURN]){
                robot.auto_mode = 1;
                start_time = clock();
            }
        }

        SDL_Delay(120);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    printf("DEAD\n");
}
