#include "pch.h"
#include <iostream>
#include <conio.h>
#include <chrono>
#include <thread>
#include <windows.h>


using namespace winrt;
using namespace Windows::Foundation;
using namespace std;

bool gameOver;
const int width = 20;
const int height = 20;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int snakeLength;
int currentSpeed;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN};
string str = "fail";
eDirection dir;



int randNo(int max){
	srand((unsigned)time(NULL));
	int random = rand() % max;
	return random;
}

int randNo(int max, int seed) {
	srand((unsigned)seed);
	int random = rand() % max;
	return random;
}



void Setup() {
	dir = STOP;
	x = width / 2; // start in middle of screen
	y = height / 2;
	fruitX = randNo(width);
	int diffSeed = randNo(height, randNo(1000));
	fruitY = randNo(height, diffSeed);
	score = 0;
	snakeLength = 1;
	currentSpeed = 350;

}

int Speed() {
	if (score < 500) {
		currentSpeed = (currentSpeed - 25);
		return currentSpeed;
	}

	else if (score < 1000) {
		currentSpeed = (currentSpeed - 15);
		return currentSpeed;
	}
	else if (score < 1500) {
		currentSpeed = (currentSpeed - 10);
		return currentSpeed;
	}
	else {
		currentSpeed = (currentSpeed - 5);
		return currentSpeed;
	}
}

void Draw() {

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0,0 });
	for (int col = 0; col < width + 2; col++){
		cout << "#";
	}
	cout << endl;

	for (int row = 0; row < height; row++) {
		cout << "#";
		for (int col = 0; col < width; col++) {
			bool displayed = false;

			for (int i = 0; i < snakeLength; i++) {
				if (tailY[i] == row && tailX[i] == col) {
					cout << "O"; // display snake 
					displayed = true;
				}
			}

			if (fruitY == row && fruitX == col && !displayed) {
				cout << "@"; // display fruit
				displayed = true;
			}

			if (!displayed) {
				cout << " ";
			}

		}
		cout << "#" << endl;
	}

	for (int col = 0; col < width + 2; col++) {
		cout << "#";
	}

	cout << "\n" << "Score: " << score << endl;

}

void Input() {

	if (kbhit()) {
		char input = getch();
		switch (input) { // the real value
		case 'w':
			// up
			if (dir != DOWN) // can't move backward
				dir = UP;
			break;
		case 's':
			// down
			if (dir != UP)
			dir = DOWN;
			break;
		case 'd':
			// right
			if (dir != LEFT)
			dir = RIGHT;
			break;
		case 'a':
			// left
			if (dir != RIGHT)
			dir = LEFT;
			break;
		}
	}

	// arrow input code from user qwertz https://stackoverflow.com/questions/10463201/getch-and-arrow-codes

	//if (getch() == '\033') { // if the first value is esc
	//	getch(); // skip the [
	//	switch (getch()) { // the real value
	//	case 'A':
	//		//arrow up
	//		dir = UP;
	//		str =  "up";
	//		break;
	//	case 'B':
	//		//arrow down
	//		dir = DOWN;
	//		break;
	//	case 'C':
	//		//arrow right
	//		dir = RIGHT;
	//		break;
	//	case 'D':
	//		//arrow left
	//		dir = LEFT;
	//		break;
	//	}
	//}
}

bool CheckCollisions(){
	if (fruitX == x && fruitY == y){
		snakeLength++;
		fruitX = randNo(width);
		int diffSeed = randNo(height, randNo(1000));
		fruitY = randNo(height, diffSeed);
		score += 100;
		Speed();

	}

	for (int i = 1; i < snakeLength; i++) {
		if (tailY[i] == tailY[0] && tailX[i] == tailX[0]) {
			return true;
		}
		if (tailY[i] == fruitY && tailX[i] == fruitX) {
			fruitX = randNo(width);
			int diffSeed = randNo(height, randNo(1000));
			fruitY = randNo(height, diffSeed);
		}

	}

	if (x >= 20 || y >= 20){

		return true;
	}
	if (x < 0 || y < 0) {

		return true;
	}


	return false;


}

void Logic() {

	gameOver = CheckCollisions();

	int tailX1 = tailX[0];
	int tailY1 = tailY[0];
	int tailX2, tailY2;
	for (int i = 1; i < snakeLength; i++){
		tailX2 = tailX[i];
		tailY2 = tailY[i];
		tailX[i] = tailX1;
		tailY[i] = tailY1;
		tailX1 = tailX2;
		tailY1 = tailY2;
	}

	switch(dir){
	case UP:
		y--;
		break;
	case DOWN:
		//arrow down
		y++;
		break;
	case RIGHT:
		//arrow right
		x++;
		break;
	case LEFT:
		//arrow left
		x--;
		break;
	}

}




int main() {
	init_apartment();

	Setup();
	while (!gameOver){
		tailX[0] = x;
		tailY[0] = y;

		Input();
		Draw();
		std::this_thread::sleep_for(std::chrono::milliseconds(currentSpeed));
		Input();
		Logic();
	}
}
