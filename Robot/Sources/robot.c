#include "../Headers/robot.h"

void setup_robot(struct Robot *robot){
    robot->x = OVERALL_WINDOW_WIDTH/2-50;
    robot->y = OVERALL_WINDOW_HEIGHT-50;
    robot->true_x = 0;
    robot->true_y = 380;
    robot->width = ROBOT_WIDTH;
    robot->height = ROBOT_HEIGHT;
    robot->direction = 0;
    robot->targetTurnDirection = 0;
    robot->angle = 90;
    robot->next_angle = 0;
    robot->currentSpeed = 0;
    robot->crashed = 0;
    robot->auto_mode = 0;
    robot->initialPassed = 0;
    robot->foundWall = 0;

    printf("Press arrow keys to move manually, or enter to move automatically\n\n");
}
int robot_off_screen(struct Robot * robot){
    if(robot->x < 0 || robot-> y < 0){
        return 0;
    }
    if(robot->x > OVERALL_WINDOW_WIDTH || robot->y > OVERALL_WINDOW_HEIGHT){
        return 0;
    }
    return 1;
}

int checkRobotHitWall(struct Robot * robot, struct Wall * wall) {

    int overlap = checkOverlap(robot->x,robot->width,robot->y,robot->height,
                 wall->x,wall->width,wall->y, wall->height);

    return overlap;
}

int checkRobotHitWalls(struct Robot * robot, struct Wall_collection * head) {
   struct Wall_collection *ptr = head;
   int hit = 0;

   while(ptr != NULL) {
      hit = (hit || checkRobotHitWall(robot, &ptr->wall));
      ptr = ptr->next;
   }
   return hit;

}

int checkRobotReachedEnd(struct Robot * robot, int x, int y, int width, int height){

    int overlap = checkOverlap(robot->x,robot->width,robot->y,robot->height,
                 x,width,y,height);

    return overlap;
}

void robotCrash(struct Robot * robot) {
    robot->currentSpeed = 0;
    if (!robot->crashed)
        printf("Ouchies!!!!!\n\nPress space to start again\n");
    robot->crashed = 1;
}

void robotSuccess(struct Robot * robot, int msec) {
    robot->currentSpeed = 0;
    if (!robot->crashed){
        printf("Success!!!!!\n\n");
        printf("Time taken %d seconds %d milliseconds \n", msec/1000, msec%1000);
        printf("Press space to start again\n");
    }
    robot->crashed = 1;
}

int checkRobotSensor(int x, int y, int sensorSensitivityLength, struct Wall * wall)  {
    //viewing_region of sensor is a square of 2 pixels * chosen length of sensitivity
    int overlap = checkOverlap(x,2,y,sensorSensitivityLength,
                 wall->x,wall->width,wall->y, wall->height);

    return overlap;
}

int checkRobotSensorFrontRightAllWalls(struct Robot * robot, struct Wall_collection * head) {
    struct Wall_collection *ptr, *head_store;
    int i;
    double xDir, yDir;
    int robotCentreX, robotCentreY, xTL, yTL;
    int score, hit;

    int sensorSensitivityLength =  floor(SENSOR_VISION/5);

    head_store = head;
    robotCentreX = robot->x+ROBOT_WIDTH/2;
    robotCentreY = robot->y+ROBOT_HEIGHT/2;
    score = 0;

    for (i = 0; i < 5; i++)
    {
        ptr = head_store;
        xDir = round(robotCentreX+(ROBOT_WIDTH/2-2)*cos((robot->angle)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*sin((robot->angle)*PI/180));
        yDir = round(robotCentreY+(ROBOT_WIDTH/2-2)*sin((robot->angle)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*cos((robot->angle)*PI/180));
        xTL = (int) xDir;
        yTL = (int) yDir;
        hit = 0;

        while(ptr != NULL) {
            hit = (hit || checkRobotSensor(xTL, yTL, sensorSensitivityLength/2, &ptr->wall));
            ptr = ptr->next;
        }
        if (hit)
            score = i;
    }
    return score;
}

int checkRobotSensorFrontLeftAllWalls(struct Robot * robot, struct Wall_collection * head) {
    struct Wall_collection *ptr, *head_store;
    int i;
    double xDir, yDir;
    int robotCentreX, robotCentreY, xTL, yTL;
    int score, hit;
    int sensorSensitivityLength;

    head_store = head;
    robotCentreX = robot->x+ROBOT_WIDTH/2;
    robotCentreY = robot->y+ROBOT_HEIGHT/2;
    score = 0;
    sensorSensitivityLength =  floor(SENSOR_VISION/5);

    for (i = 0; i < 5; i++)
    {
        ptr = head_store;
        xDir = round(robotCentreX+(-ROBOT_WIDTH/2)*cos((robot->angle)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*sin((robot->angle)*PI/180));
        yDir = round(robotCentreY+(-ROBOT_WIDTH/2)*sin((robot->angle)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*cos((robot->angle)*PI/180));
        xTL = (int) xDir;
        yTL = (int) yDir;
        hit = 0;

        while(ptr != NULL) {
            hit = (hit || checkRobotSensor(xTL, yTL, sensorSensitivityLength/2, &ptr->wall));
            ptr = ptr->next;
        }
        if (hit)
            score = i;
    }
    return score;
}

int checkRobotSensorMidRightAllWalls(struct Robot * robot, struct Wall_collection * head) {
    struct Wall_collection *ptr, *head_store;
    int i;
    double xDir, yDir;
    int robotCentreX, robotCentreY, xTL, yTL;
    int score, hit;

    int sensorSensitivityLength =  floor(SIDE_SENSOR_VISION/5);

    head_store = head;
    robotCentreX = robot->x+ROBOT_WIDTH/2;
    robotCentreY = robot->y+ROBOT_HEIGHT/2;
    score = 0;

    for (i = 0; i < 5; i++)
    {
        ptr = head_store;
        xDir = round(robotCentreX-(ROBOT_WIDTH/2-2)*cos((robot->angle+90)*PI/180)-(-ROBOT_HEIGHT/2-SIDE_SENSOR_VISION+sensorSensitivityLength*i)*sin((robot->angle+90)*PI/180));
        yDir = round(robotCentreY-(ROBOT_WIDTH/2)*sin((robot->angle+90)*PI/180)+(-ROBOT_HEIGHT/2-SIDE_SENSOR_VISION+sensorSensitivityLength*i)*cos((robot->angle+90)*PI/180));
        xTL = (int) xDir;
        yTL = (int) yDir;
        hit = 0;

        while(ptr != NULL) {
            hit = (hit || checkRobotSensor(xTL, yTL, sensorSensitivityLength, &ptr->wall));
            ptr = ptr->next;
        }
        if (hit)
            score = i;
    }
    return score;
}

int checkRobotSensorBackRightAllWalls(struct Robot * robot, struct Wall_collection * head) {
    struct Wall_collection *ptr, *head_store;
    int i;
    double xDir, yDir;
    int robotCentreX, robotCentreY, xTL, yTL;
    int score, hit;

    int sensorSensitivityLength =  floor(SIDE_SENSOR_VISION/5);

    head_store = head;
    robotCentreX = robot->x+ROBOT_WIDTH/2;
    robotCentreY = robot->y+ROBOT_HEIGHT/2;
    score = 0;

    for (i = 0; i < 5; i++)
    {
        ptr = head_store;
        xDir = round(robotCentreX+(ROBOT_WIDTH/2-2)*cos((robot->angle+90)*PI/180)-(-ROBOT_HEIGHT/2-SIDE_SENSOR_VISION+sensorSensitivityLength*i)*sin((robot->angle+90)*PI/180)) - 2;
        yDir = round(robotCentreY+(ROBOT_WIDTH/2)*sin((robot->angle+90)*PI/180)+(-ROBOT_HEIGHT/2-SIDE_SENSOR_VISION+sensorSensitivityLength*i)*cos((robot->angle+90)*PI/180));
        yTL = (int) yDir;
        hit = 0;

        while(ptr != NULL) {
            hit = (hit || checkRobotSensor(xTL, yTL, sensorSensitivityLength, &ptr->wall));
            ptr = ptr->next;
        }
        if (hit)
            score = i;
    }
    return score;
}

int checkRobotSensorBackLeftAllWalls(struct Robot * robot, struct Wall_collection * head) {
    struct Wall_collection *ptr, *head_store;
    int i;
    double xDir, yDir;
    int robotCentreX, robotCentreY, xTL, yTL;
    int score, hit;

    int sensorSensitivityLength =  floor(SIDE_SENSOR_VISION/5);

    head_store = head;
    robotCentreX = robot->x+ROBOT_WIDTH/2;
    robotCentreY = robot->y+ROBOT_HEIGHT/2;
    score = 0;

    for (i = 0; i < 5; i++)
    {
        ptr = head_store;
        xDir = round(robotCentreX-(ROBOT_WIDTH/2-2)*cos((robot->angle-90)*PI/180)-(-ROBOT_HEIGHT/2-SIDE_SENSOR_VISION+sensorSensitivityLength*i)*sin((robot->angle-90)*PI/180));
        yDir = round(robotCentreY-(ROBOT_WIDTH/2)*sin((robot->angle-90)*PI/180)+(-ROBOT_HEIGHT/2-SIDE_SENSOR_VISION+sensorSensitivityLength*i)*cos((robot->angle-90)*PI/180));
        xTL = (int) xDir;
        yTL = (int) yDir;
        hit = 0;

        while(ptr != NULL) {
            hit = (hit || checkRobotSensor(xTL, yTL, sensorSensitivityLength, &ptr->wall));
            ptr = ptr->next;
        }
        if (hit)
            score = i;
    }
    return score;
}

int checkRobotSensorMidLeftAllWalls(struct Robot * robot, struct Wall_collection * head) {
    struct Wall_collection *ptr, *head_store;
    int i;
    double xDir, yDir;
    int robotCentreX, robotCentreY, xTL, yTL;
    int score, hit;

    int sensorSensitivityLength =  floor(SIDE_SENSOR_VISION/5);

    head_store = head;
    robotCentreX = robot->x+ROBOT_WIDTH/2;
    robotCentreY = robot->y+ROBOT_HEIGHT/2;
    score = 0;

    for (i = 0; i < 5; i++)
    {
        ptr = head_store;
        xDir = round(robotCentreX+(ROBOT_WIDTH/2-2)*cos((robot->angle-90)*PI/180)-(-ROBOT_HEIGHT/2-SIDE_SENSOR_VISION+sensorSensitivityLength*i)*sin((robot->angle-90)*PI/180)) - 2;
        yDir = round(robotCentreY+(ROBOT_WIDTH/2)*sin((robot->angle-90)*PI/180)+(-ROBOT_HEIGHT/2-SIDE_SENSOR_VISION+sensorSensitivityLength*i)*cos((robot->angle-90)*PI/180));
        xTL = (int) xDir;
        yTL = (int) yDir;
        hit = 0;

        while(ptr != NULL) {
            hit = (hit || checkRobotSensor(xTL, yTL, sensorSensitivityLength, &ptr->wall));
            ptr = ptr->next;
        }
        if (hit)
            score = i;
    }
    return score;
}

void robotUpdate(struct SDL_Renderer * renderer, struct Robot * robot){
    double xDir, yDir;

    int robotCentreX, robotCentreY, xTR, yTR, xTL, yTL, xBR, yBR, xBL, yBL;
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);

    /*
    //Other Display options:
    // The actual square which the robot is tested against (not so nice visually with turns, but easier
    // to test overlap
    SDL_Rect rect = {robot->x, robot->y, robot->height, robot->width};
    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
    SDL_RenderDrawRect(renderer, &rect);
    SDL_RenderFillRect(renderer, &rect);
    */
    /*
    //Center Line of Robot. Line shows the direction robot is facing
    xDir = -30 * sin(-robot->angle*PI/180);
    yDir = -30 * cos(-robot->angle*PI/180);
    xDirInt = robot->x+ROBOT_WIDTH/2+ (int) xDir;
    yDirInt = robot->y+ROBOT_HEIGHT/2+ (int) yDir;
    SDL_RenderDrawLine(renderer,robot->x+ROBOT_WIDTH/2, robot->y+ROBOT_HEIGHT/2, xDirInt, yDirInt);
    */

    //Rotating Square
    //Vector rotation to work out corners x2 = x1cos(angle)-y1sin(angle), y2 = x1sin(angle)+y1cos(angle)
    robotCentreX = robot->x+ROBOT_WIDTH/2;
    robotCentreY = robot->y+ROBOT_HEIGHT/2;

    xDir = round(robotCentreX+(ROBOT_WIDTH/2)*cos((robot->angle)*PI/180)-(-ROBOT_HEIGHT/2)*sin((robot->angle)*PI/180));
    yDir = round(robotCentreY+(ROBOT_WIDTH/2)*sin((robot->angle)*PI/180)+(-ROBOT_HEIGHT/2)*cos((robot->angle)*PI/180));
    xTR = (int) xDir;
    yTR = (int) yDir;

    xDir = round(robotCentreX+(ROBOT_WIDTH/2)*cos((robot->angle)*PI/180)-(ROBOT_HEIGHT/2)*sin((robot->angle)*PI/180));
    yDir = round(robotCentreY+(ROBOT_WIDTH/2)*sin((robot->angle)*PI/180)+(ROBOT_HEIGHT/2)*cos((robot->angle)*PI/180));
    xBR = (int) xDir;
    yBR = (int) yDir;

    xDir = round(robotCentreX+(-ROBOT_WIDTH/2)*cos((robot->angle)*PI/180)-(ROBOT_HEIGHT/2)*sin((robot->angle)*PI/180));
    yDir = round(robotCentreY+(-ROBOT_WIDTH/2)*sin((robot->angle)*PI/180)+(ROBOT_HEIGHT/2)*cos((robot->angle)*PI/180));
    xBL = (int) xDir;
    yBL = (int) yDir;

    xDir = round(robotCentreX+(-ROBOT_WIDTH/2)*cos((robot->angle)*PI/180)-(-ROBOT_HEIGHT/2)*sin((robot->angle)*PI/180));
    yDir = round(robotCentreY+(-ROBOT_WIDTH/2)*sin((robot->angle)*PI/180)+(-ROBOT_HEIGHT/2)*cos((robot->angle)*PI/180));
    xTL = (int) xDir;
    yTL = (int) yDir;

    SDL_RenderDrawLine(renderer,xTR, yTR, xBR, yBR);
    SDL_RenderDrawLine(renderer,xBR, yBR, xBL, yBL);
    SDL_RenderDrawLine(renderer,xBL, yBL, xTL, yTL);
    SDL_RenderDrawLine(renderer,xTL, yTL, xTR, yTR);

    //Front Right Sensor
    int sensor_sensitivity =  floor(SENSOR_VISION/5);
    int side_sensor_sensitivity =  floor(SIDE_SENSOR_VISION/5);
    int i;
    for (i = 0; i < 5; i++)
    {
        xDir = round(robotCentreX+(ROBOT_WIDTH/2-1)*cos((robot->angle)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensor_sensitivity*i)*sin((robot->angle)*PI/180));
        yDir = round(robotCentreY+(ROBOT_WIDTH/2)*sin((robot->angle)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensor_sensitivity*i)*cos((robot->angle)*PI/180));
        xTL = (int) xDir;
        yTL = (int) yDir;

        SDL_Rect rect = {xTL, yTL, 2, sensor_sensitivity/2};
        SDL_SetRenderDrawColor(renderer, 80+(20*(5-i)), 80+(20*(5-i)), 80+(20*(5-i)), 255);
        SDL_RenderDrawRect(renderer, &rect);
        SDL_RenderFillRect(renderer, &rect);
    }

    //Front Left Sensor
    for (i = 0; i < 5; i++)
    {
        xDir = round(robotCentreX+(-ROBOT_WIDTH/2)*cos((robot->angle)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensor_sensitivity*i)*sin((robot->angle)*PI/180));
        yDir = round(robotCentreY+(-ROBOT_WIDTH/2)*sin((robot->angle)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensor_sensitivity*i)*cos((robot->angle)*PI/180));
        xTL = (int) xDir;
        yTL = (int) yDir;

        SDL_Rect rect = {xTL, yTL, 2, sensor_sensitivity/2};
        SDL_SetRenderDrawColor(renderer, 80+(20*(5-i)), 80+(20*(5-i)), 80+(20*(5-i)), 255);
        SDL_RenderDrawRect(renderer, &rect);
        SDL_RenderFillRect(renderer, &rect);
    }

    //Mid Right Sensor
    for (i = 0; i < 5; i++)
    {
        xDir = round(robotCentreX-(ROBOT_WIDTH/2-2)*cos((robot->angle+90)*PI/180)-(-ROBOT_HEIGHT/2-SIDE_SENSOR_VISION+side_sensor_sensitivity*i)*sin((robot->angle+90)*PI/180));
        yDir = round(robotCentreY-(ROBOT_WIDTH/2)*sin((robot->angle+90)*PI/180)+(-ROBOT_HEIGHT/2-SIDE_SENSOR_VISION+side_sensor_sensitivity*i)*cos((robot->angle+90)*PI/180));
        xTL = (int) xDir;
        yTL = (int) yDir;

        SDL_Rect rect = {xTL, yTL, 2, side_sensor_sensitivity};
        SDL_SetRenderDrawColor(renderer, 80+(20*(5-i)), 80+(20*(5-i)), 80+(20*(5-i)), 255);
        SDL_RenderDrawRect(renderer, &rect);
        SDL_RenderFillRect(renderer, &rect);
    }

    //Back Right Sensor
    for (i = 0; i < 5; i++)
    {
        xDir = round(robotCentreX+(ROBOT_WIDTH/2-2)*cos((robot->angle+90)*PI/180)-(-ROBOT_HEIGHT/2-SIDE_SENSOR_VISION+side_sensor_sensitivity*i)*sin((robot->angle+90)*PI/180)) - 2;
        yDir = round(robotCentreY+(ROBOT_WIDTH/2)*sin((robot->angle+90)*PI/180)+(-ROBOT_HEIGHT/2-SIDE_SENSOR_VISION+side_sensor_sensitivity*i)*cos((robot->angle+90)*PI/180));
        xTL = (int) xDir;
        yTL = (int) yDir;

        SDL_Rect rect = {xTL, yTL, 2, side_sensor_sensitivity};
        SDL_SetRenderDrawColor(renderer, 80+(20*(5-i)), 80+(20*(5-i)), 80+(20*(5-i)), 255);
        SDL_RenderDrawRect(renderer, &rect);
        SDL_RenderFillRect(renderer, &rect);
    }

    //Back Left Sensor
    for (i = 0; i < 5; i++)
    {
        xDir = round(robotCentreX-(ROBOT_WIDTH/2-2)*cos((robot->angle-90)*PI/180)-(-ROBOT_HEIGHT/2-SIDE_SENSOR_VISION+side_sensor_sensitivity*i)*sin((robot->angle-90)*PI/180));
        yDir = round(robotCentreY-(ROBOT_WIDTH/2)*sin((robot->angle-90)*PI/180)+(-ROBOT_HEIGHT/2-SIDE_SENSOR_VISION+side_sensor_sensitivity*i)*cos((robot->angle-90)*PI/180));
        xTL = (int) xDir;
        yTL = (int) yDir;

        SDL_Rect rect = {xTL, yTL, 2, side_sensor_sensitivity};
        SDL_SetRenderDrawColor(renderer, 80+(20*(5-i)), 80+(20*(5-i)), 80+(20*(5-i)), 255);
        SDL_RenderDrawRect(renderer, &rect);
        SDL_RenderFillRect(renderer, &rect);
    }

    //Mid Left Sensor
    for (i = 0; i < 5; i++)
    {
        xDir = round(robotCentreX+(ROBOT_WIDTH/2-2)*cos((robot->angle-90)*PI/180)-(-ROBOT_HEIGHT/2-SIDE_SENSOR_VISION+side_sensor_sensitivity*i)*sin((robot->angle-90)*PI/180)) - 2;
        yDir = round(robotCentreY+(ROBOT_WIDTH/2)*sin((robot->angle-90)*PI/180)+(-ROBOT_HEIGHT/2-SIDE_SENSOR_VISION+side_sensor_sensitivity*i)*cos((robot->angle-90)*PI/180));
        xTL = (int) xDir;
        yTL = (int) yDir;

        SDL_Rect rect = {xTL, yTL, 2, side_sensor_sensitivity};
        SDL_SetRenderDrawColor(renderer, 80+(20*(5-i)), 80+(20*(5-i)), 80+(20*(5-i)), 255);
        SDL_RenderDrawRect(renderer, &rect);
        SDL_RenderFillRect(renderer, &rect);
    }
}



void robotMotorMove(struct Robot * robot) {
    double x_offset, y_offset;
    //printf("Direction: %d\n", robot->direction);
    switch(robot->direction){
        case UP :
            robot->currentSpeed += DEFAULT_SPEED_CHANGE;
            if (robot->currentSpeed > MAX_ROBOT_SPEED)
                robot->currentSpeed = MAX_ROBOT_SPEED;
            break;
        case DOWN :
            //printf("--- SLOWED DOWN\n\n");
            robot->currentSpeed -= DEFAULT_SPEED_CHANGE;
            if (robot->currentSpeed < -MAX_ROBOT_SPEED)
                robot->currentSpeed = -MAX_ROBOT_SPEED;
            break;
        case LEFT :
            robot->angle = (robot->angle+360-DEFAULT_ANGLE_CHANGE)%360;
            //printf("--- TURNED LEFT\n\n");
            break;
        case RIGHT :
            robot->angle = (robot->angle+DEFAULT_ANGLE_CHANGE)%360;
            //printf("--- TURNED RIGHT\n\n");
            break;
    }
    //robot->direction = 0;
    x_offset = (-robot->currentSpeed * sin(-robot->angle*PI/180));
    y_offset = (-robot->currentSpeed * cos(-robot->angle*PI/180));

    robot->true_x += x_offset;
    robot->true_y += y_offset;

    x_offset = round(robot->true_x);
    y_offset = round(robot->true_y);

    robot->x = (int) x_offset;
    robot->y = (int) y_offset;
}

void robotAutoMotorMove(struct Robot * robot, int initial, int front_left_sensor, int front_right_sensor, int mid_left_sensor, int mid_right_sensor, int back_right_sensor, int back_left_sensor) {

    if (initial == 1) {
        robot->next_angle = robot->angle;
    }

    int changeSpeed() {
        if (robot->currentSpeed > robot->targetSpeed) { // Robot is moving too fast
            robot->direction = DOWN;
            return 1;
        } else if (robot->currentSpeed < robot->targetSpeed) { // Robot is moving too slow
            robot->direction = UP;
            return 1;
        } else { // Robot is the correct speed
            robot->direction = 0;
            return 0;
        }
    }

    int changeDirection() {
        if (back_left_sensor > 0) {
            robot->initialPassed = 1;
        }
        if (robot->angle != robot->next_angle) { // There is a different desired angle, turn
            robot->direction = robot->targetTurnDirection;
            return 1;
        }
        return 0;
    }

    if (changeSpeed() == 0) { // Current speed is 0, turns are allowed
        if (changeDirection() == 1) { // Current speed is 0, turns are allowed
            return;
        };
    }

    if (robot->currentSpeed != 0) {
        if (back_left_sensor > 0) {
            robot->foundWall = 1;
        }
    }

    if (initial == 1) { // First frame

        if (back_left_sensor > 0) { // Left wall found, move forward
            robot->initialPassed = 1;
            robot->targetSpeed = 1;
            robot->foundWall = 1;

        } else { // Left wall not found, turn left
            robot->targetSpeed = 0;
            robot->targetTurnDirection = LEFT;
            robot->next_angle = (robot->angle+360 - 90) % 360;
        }

        return;
    }

    if (robot->initialPassed == 0) { // First left wall not found

        if (front_left_sensor == 0 && front_right_sensor == 0) { // Path ahead clear, move fast
            robot->targetSpeed = 6;

        } else if (front_left_sensor >= 4 || front_right_sensor >= 4) { // Wall ahead very close, stop moving and then turn right
            robot->targetSpeed = 0;
            robot->targetTurnDirection = RIGHT;
            robot->next_angle = (robot->angle + 90) % 360;

        } else if (front_left_sensor > 0 || front_right_sensor > 0) { // Wall ahead, slow down
            robot->targetSpeed = 2;
        }

        return;
    }

    if (back_left_sensor == 0 && robot->foundWall == 1) { // Left wall gone, turn left
        robot->foundWall = 0;
        robot->targetSpeed = 0;
        robot->targetTurnDirection = LEFT;
        robot->next_angle = (robot->angle+360 - 90) % 360;

    } else if (front_right_sensor >= 4 && mid_right_sensor > 3) {
        robot->foundWall = 0;
        robot->targetSpeed = 0;
        robot->targetTurnDirection = LEFT;
        robot->next_angle = (robot->angle+360 - 90) % 360;

    } else if (mid_left_sensor == 0 && robot->foundWall == 1) { // Left wall almost gone, slow down
        robot->targetSpeed = 2;

    } else if (front_left_sensor >= 4 || front_right_sensor >= 4) { // Wall ahead very close, stop moving and then turn right
        robot->targetSpeed = 0;
        robot->targetTurnDirection = RIGHT;
        robot->next_angle = (robot->angle + 45) % 360;

    } else if (mid_left_sensor == 0 && robot->foundWall == 0) {
        if (robot->currentSpeed == 0) {
            if (front_left_sensor == 0 && front_left_sensor == 0) {
                robot->targetSpeed = 6;
            } else {
                robot->targetSpeed = 2;
            }
        }

    } else if (front_left_sensor > 0 || front_right_sensor > 0) { // Wall ahead, slow down
        robot->targetSpeed = 2;

    } else if (front_left_sensor == 0 && front_right_sensor == 0) {
        robot->targetSpeed = 7;

    }


    // LEFT HAND RULE - with proper deceleration attempt / not complete
    /*if (front_left_sensor > 0) {
        printf("Front sensor hit %d\n", front_left_sensor);
    }
    if (back_left_sensor > 0) {
        printf("Back sensor hit %d\n", back_left_sensor);
    }

    if (robot->currentSpeed > robot->targetSpeed) {
        robot->currentSpeed -= 1;
        printf("Slow down, at %d / %d\n", robot->currentSpeed, robot->targetSpeed);
    } else if (robot->currentSpeed < robot->targetSpeed) {
        if (front_left_sensor <= 4 && front_right_sensor <= 4 && (robot->initialPassed == 0 || (mid_left_sensor == 0 && back_left_sensor == 0))) {
            robot->targetSpeed = 2;
            printf("Speed up v1, at %d / %d\n", robot->currentSpeed, robot->targetSpeed);
        } else if (front_left_sensor == 0 && front_right_sensor == 0 && (robot->initialPassed == 1 || (mid_left_sensor == 0 && back_left_sensor == 0))) {
            robot->currentSpeed += 1;
            printf("Speed up v2, at %d / %d\n", robot->currentSpeed, robot->targetSpeed);
        } else {
            robot->targetSpeed = 0;
            if (robot->currentSpeed > 0) {
                robot->currentSpeed -= 1;
            }
            printf("Halt, at %d / %d\n", robot->currentSpeed, robot->targetSpeed);
        }
    } else {

    if (robot->initialPassed == 0) {
            if (robot->angle == robot->next_angle || initial == 1) {
                if (back_left_sensor == 0 && initial == 1) {
                    printf("B \n");
                    robot->direction = LEFT;
                    robot->targetSpeed = 0;
                    robot->next_angle = (robot->angle+360 - 90) % 360;
                } else if (back_left_sensor > 0 && front_left_sensor == 0 && front_right_sensor == 0) { //done with this block
                    printf("C \n");
                    robot->initialPassed = 1;
                    robot->foundWall = 1;
                    robot->direction = 0;
                    robot->targetSpeed = 5;
                } else {
                    if (front_right_sensor > 4 || front_left_sensor > 4) {
                        printf("E \n");
                        robot->targetSpeed = 0;
                        robot->direction = RIGHT;
                        robot->next_angle = (robot->angle + 90) % 360;
                    } else {
                        printf("F \n");
                        robot->direction = 0;
                        robot->targetSpeed = 5;
                    }
                }
            }
        } else {
            if (robot->angle == robot->next_angle) {
                if (back_left_sensor > 0) {
                    //printf("Back left fired");
                    robot->foundWall = 1;
                }

                if (back_left_sensor == 0 && robot->foundWall == 1) {
                    robot->foundWall = 0;
                    robot->direction = LEFT;
                    robot->targetSpeed = 0;
                    robot->next_angle = (robot->angle+360 - 90) % 360;
                } else if ((front_right_sensor > 4 || front_left_sensor > 4)) {
                    robot->targetSpeed = 0; //0
                    if ((mid_right_sensor > 2)) {
                        robot->foundWall = 0;
                        robot->direction = LEFT;
                        robot->next_angle = (robot->angle+360 - 90) % 360;
                    } else {
                        printf("right??\n");
                        robot->direction = RIGHT;
                    robot->next_angle = (robot->angle + 90) % 360;
                    }
                } else {
                    robot->direction = 0;

                    if (front_left_sensor > 0 || front_right_sensor > 0  || mid_left_sensor == 0) {
                        robot->targetSpeed = 1;
                    } else {
                        robot->targetSpeed = 5;
                    }
                    //printf("%d\n", robot->currentSpeed);
                }
            }
        }

    }*/

    /*
    Check if robot is alongside the right wall
    - If not, set the speed to 0, turn the robot 90 degrees right, move forward until the front right sensor hits the right wall, then set speed to 0 and turn 90 degrees left
    - If it is then move forward
    Detect changes until either
    - Mid right == 0: turn right
    - Front right == 1, mid right == 1: turn left
    Repeat
    */

    // RIGHT HAND RULE (without proper acc)
    /*if (robot->initialPassed == 0) {
        if (robot->angle == robot->next_angle || initial == 1) {
            //printf("%d\n", robot->angle);
            if (back_right_sensor == 0 && initial == 1) {
                printf("B \n");
                robot->direction = RIGHT;
                robot->currentSpeed = 0;
                robot->next_angle = (robot->angle + 90) % 360;
            } else if (back_right_sensor > 0 && front_left_sensor == 0 && front_right_sensor == 0) { //done with this block
                printf("C \n");
                robot->initialPassed = 1;
                robot->foundWall = 1;
                robot->direction = 0;
                robot->currentSpeed = 25;
            } else {
                if (front_right_sensor > 0 || front_left_sensor > 0) {
                    printf("E \n");
                    robot->currentSpeed = 0;
                    robot->direction = LEFT;
                    robot->next_angle = (robot->angle+360 - 90) % 360;
                } else {
                    printf("F \n");
                    robot->direction = 0;
                    robot->currentSpeed = 25;
                }
            }
        }
    } else {
        if (robot->angle == robot->next_angle) {
            if (back_right_sensor > 0) {
                //printf("Back right fired");
                robot->foundWall = 1;
            }

            //if (front_right_sensor <= 2) {
                //printf("Front right %d\n", front_right_sensor); //0
                //printf("Mid right %d\n", mid_right_sensor);//3
                //printf("Back right %d\n\n", back_right_sensor);//2
            //}



            if (back_right_sensor == 0 && front_right_sensor == 0 && robot->foundWall == 1) { //foundWall
                printf("Front left %d\n", front_left_sensor);
                printf("Front right %d\n", front_right_sensor);
                printf("Mid right %d\n", mid_right_sensor);
                printf("Back right %d\n", back_right_sensor);
                printf("RIGHT\n\n");
                robot->foundWall = 0;
                robot->direction = RIGHT;
                robot->currentSpeed = 0;
                robot->next_angle = (robot->angle + 90) % 360;
            } else if (front_right_sensor > 2 || front_left_sensor > 2) {
                printf("Front left %d\n", front_left_sensor);
                printf("Front right %d\n", front_right_sensor);
                printf("Mid right %d\n", mid_right_sensor);
                printf("Back right %d\n", back_right_sensor);
                printf("LEFT\n\n");
                robot->currentSpeed = 0; //0
                robot->direction = LEFT;
                robot->next_angle = (robot->angle+360 - 45) % 360;
            } else {
                robot->direction = 0;
                if (front_right_sensor == 5) {
                    robot->currentSpeed = 1;
                } else if (front_right_sensor == 4) {
                    robot->currentSpeed = 2;
                } else if (front_right_sensor == 3) {
                    robot->currentSpeed = 4;
                } else if (front_right_sensor == 2) {
                    robot->currentSpeed = 6;
                } else if (front_right_sensor == 1  || mid_right_sensor == 0) {
                    robot->currentSpeed = 8;
                } else if (robot->currentSpeed < 12) {
                    robot->currentSpeed = 12;
                } else if (robot->currentSpeed < 16) {
                    robot->currentSpeed = 16;
                } else {
                    robot->currentSpeed = 24;
                }
                //printf("%d\n", robot->currentSpeed);
            }
        }
    }*/


    // LEFT HAND RULE (without proper dec)
    /*if (robot->initialPassed == 0) {
        if (robot->angle == robot->next_angle || initial == 1) {
            if (back_left_sensor == 0 && initial == 1) {
                printf("B \n");
                robot->direction = LEFT;
                robot->currentSpeed = 0;
                robot->next_angle = (robot->angle+360 - 90) % 360;
            } else if (back_left_sensor > 0 && front_left_sensor == 0 && front_right_sensor == 0) { //done with this block
                printf("C \n");
                robot->initialPassed = 1;
                robot->foundWall = 1;
                robot->direction = 0;
                robot->currentSpeed = 25;
            } else {
                if (front_right_sensor > 0 || front_left_sensor > 0) {
                    printf("E \n");
                    robot->currentSpeed = 0;
                    robot->direction = RIGHT;
                    robot->next_angle = (robot->angle + 45) % 360;
                } else {
                    printf("F \n");
                    robot->direction = 0;
                    robot->currentSpeed = 23;
                }
            }
        }
    } else {
        if (robot->angle == robot->next_angle) {
            if (back_left_sensor > 0) {
                //printf("Back right fired");
                robot->foundWall = 1;
            }

            if (back_left_sensor == 0 && robot->foundWall == 1) { //i removed  'front_right_sensor == 0 &&'
                robot->foundWall = 0;
                robot->direction = LEFT;
                robot->currentSpeed = 0;
                robot->next_angle = (robot->angle+360 - 90) % 360;
            } else if ((front_right_sensor > 2 || front_left_sensor > 2)) {
                robot->currentSpeed = 0; //0
                if ((mid_right_sensor > 2)) {
                    robot->foundWall = 0;
                    robot->direction = LEFT;
                    robot->next_angle = (robot->angle+360 - 90) % 360;
                } else {
                    robot->direction = RIGHT;
                robot->next_angle = (robot->angle + 45) % 360;
                }
            } else {
                robot->direction = 0;

                if (front_left_sensor > 0 || front_right_sensor > 0  || mid_left_sensor == 0) {
                    robot->currentSpeed = 8;
                } else if (robot->currentSpeed < 12) {
                    robot->currentSpeed = 12;
                } else if (robot->currentSpeed < 16) {
                    robot->currentSpeed = 16;
                } else {
                    robot->currentSpeed = 24;
                }
                //printf("%d\n", robot->currentSpeed);
            }
        }
    }*/



}
