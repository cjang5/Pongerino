/*void collision(float x, float y) {
//wall collisions
if (y < 0.0) {
ballMovingUp = false;
}
else if ((y + 50) > SCREEN_HEIGHT) {
ballMovingUp = true;
}


//left bar collisions
if ((x = leftBarRectPosX + 99) && ((y >= leftBarRectPosY && y <= leftBarRectPosY + 299) || (y + 49 >= leftBarRectPosY && y + 49 <= leftBarRectPosY + 299))) {
//coming in from top right
if (!ballMovingRight && !ballMovingUp) {
ballMovingRight = true;
ballMovingUp = false;
}
//coming in from bottom right
if (!ballMovingRight && ballMovingUp) {
ballMovingRight = true;
ballMovingRight = true;
}
}
if ((x < leftBarRectPosX + 99 && x > leftBarRectPosX + 49) && (y + 49 >= leftBarRectPosY)) {
ballMovingRight = true;
ballMovingUp = true;
}
if ((x + 49 <= leftBarRectPosX + 49) && (y + 49 >= leftBarRectPosY)) {
ballMovingRight = false;
ballMovingUp = true;
}
if ((x < leftBarRectPosX + 99 && x > leftBarRectPosX + 49) && (y <= leftBarRectPosY + 299)) {
ballMovingRight = true;
ballMovingUp = false;
}
if ((x + 49 <= leftBarRectPosX + 49) && (y <= leftBarRectPosY + 299)) {
ballMovingRight = false;
ballMovingUp = false;
}

//right bar collisions
if ((x + 50 == rightBarRectPosX) && (isBetween(y, rightBarRectPosY, rightBarRectPosY + 300) ||
isBetween(y + 50, rightBarRectPosY, rightBarRectPosY + 300))) {
if (ballMovingRight && !ballMovingUp) {
ballMovingRight = false;
ballMovingUp = false;
return;
}
if (ballMovingRight && ballMovingUp) {
ballMovingRight = false;
ballMovingUp = false;
return;
}
}
if ((x + 50 > rightBarRectPosX && x + 50 <= rightBarRectPosX + 50) && (y == rightBarRectPosY + 300)) {
ballMovingRight = false;
ballMovingUp = false;
return;
}
if ((x + 50 > rightBarRectPosX + 50) && (y == rightBarRectPosY + 300)) {
ballMovingRight = true;
ballMovingUp = false;
return;
}
if ((x + 50 > rightBarRectPosX && x + 50 <= rightBarRectPosX + 50) && (y + 50 == rightBarRectPosY)) {
ballMovingRight = false;
ballMovingUp = true;
return;
}
if ((x + 50 > rightBarRectPosX + 50) && (y + 50 == rightBarRectPosY)) {
ballMovingRight = true;
ballMovingUp = true;
return;
}

//WIN CONDITIONS
if (x < 0.0)
updateScore(1); //give opponent a point
else if (x + 50 > SCREEN_WIDTH)
updateScore(0); //give yourself a point
} */