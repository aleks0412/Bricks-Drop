﻿/**
*
* Solution to course project # 3
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2023/2024
*
* @author Aleks Karabashev
* @idnumber 3MI0600368
* @compiler VC
*
* <Main file>
*
*/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <time.h>

using namespace std;

constexpr int MAX_USER_NAME_LENGTH = 100;
constexpr int MAX_INT_LENGTH = 10;
constexpr int MAX_BRICK_LENGTH = 4;
constexpr int DIFFERENT_BRICK_COLORS = 4;
constexpr int MAX_NUMBER_OF_BRICKS_IN_A_ROW = 8;
constexpr int NUMBER_OF_ROWS = 10;

struct User
{
	char name[MAX_USER_NAME_LENGTH + 1];
	int highScore;
};

int strLen(const char* str)
{
	if (str == nullptr)
		return 0;

	int result = 0;
	while (*(str++))
		result++;
	return result;
}

int strCmp(const char* str1, const char* str2)
{
	if (str1 == nullptr || str2 == nullptr)
		return 0;

	while (*str1 && *str2 && *str1 == *str2)
	{
		str1++;
		str2++;
	}
	return *str1 - *str2;
}

void strCpy(const char* source, char* destination)
{
	if (source == nullptr || destination == nullptr)
		return;

	while (*source)
	{
		*destination = *source;
		source++;
		destination++;
	}

	*destination = '\0';
}

int stringToNum(const char* str)
{
	if (str == nullptr)
		return -1;

	int result = 0;
	for (int i = strLen(str) - 1, mult = 1; i >= 0; i--, mult *= 10)
	{
		result += (str[i] - '0') * mult;
	}
	return result;
}

int getNumberOfUsers(ifstream& usersFile)
{
	char numberOfUsersString[MAX_INT_LENGTH + 1];
	usersFile >> numberOfUsersString;
	return stringToNum(numberOfUsersString);
}

void getUsers(ifstream& userFile, User* users, size_t numberOfUsers)
{
	if (users == nullptr)
		return;

	for (size_t i = 0; i < numberOfUsers; i++)
	{
		char currentUserName[MAX_USER_NAME_LENGTH + 1];
		userFile >> currentUserName;
		strCpy(currentUserName, users[i].name);
		char currentUserHighScore[MAX_INT_LENGTH + 1];
		userFile >> currentUserHighScore;
		users[i].highScore = stringToNum(currentUserHighScore);
	}
}

void printUser(User user)
{
	cout << "User: " << user.name << " High Score: " << user.highScore << endl;
}

void printUsers(User* users, size_t numberOfUsers)
{
	if (users == nullptr)
		return;

	for (int i = 0; i < numberOfUsers; i++)
		printUser(users[i]);
}

bool isSelectedUserValid(User* users, size_t numberOfUsers, const char* userName)
{
	if (users == nullptr)
		return false;

	for (size_t i = 0; i < numberOfUsers; i++)
		if (strCmp(users[i].name, userName) == 0)
			return true;
	return false;
}

int findIndexOfUser(User* users, size_t numberOfUsers, const char* userName)
{
	if (users == nullptr)
		return -1;

	for (size_t i = 0; i < numberOfUsers; i++)
		if (strCmp(users[i].name, userName) == 0)
			return i;
	return -1;
}

User selectUser(User* users, size_t numberOfUsers)
{
	char selectedUserName[MAX_USER_NAME_LENGTH + 1];
	User selectedUser;
	cout << "Select user: ";
	cin >> selectedUserName;
	while (!isSelectedUserValid(users, numberOfUsers, selectedUserName))
	{
		cout << "The selected user is not valid. Choose a different user: ";
		cin >> selectedUserName;
	}
	selectedUser = users[findIndexOfUser(users, numberOfUsers, selectedUserName)];
	return selectedUser;
}

struct Brick
{
	int length;
	char color;
};

void printSymbolNTymes(char symbol, int n)
{
	for (int i = 0; i < n; i++)
		cout << symbol;
}

void printBrick(Brick brick)
{
	printSymbolNTymes(brick.color, brick.length);
}

struct Canvas
{
	Brick* bricks[NUMBER_OF_ROWS][MAX_NUMBER_OF_BRICKS_IN_A_ROW];
	int currentRow = NUMBER_OF_ROWS - 1; //The row at the top is 0 and the lowest is numberOfRows - 1
};

void emptyARow(Canvas& canvas, int row)
{
	for (int col = 0; col < MAX_NUMBER_OF_BRICKS_IN_A_ROW; col++)
		canvas.bricks[row][col] = nullptr;
}

void emptyTheCanvas(Canvas& canvas)
{
	for (int row = 0; row < NUMBER_OF_ROWS; row++)
		emptyARow(canvas, row);
	canvas.currentRow = NUMBER_OF_ROWS - 1;
}

void printCanvas(const Canvas& canvas)
{
	printSymbolNTymes('-', MAX_NUMBER_OF_BRICKS_IN_A_ROW + 2);
	cout << endl;
	for (int row = 0; row < NUMBER_OF_ROWS; row++)
	{
		cout << "|";
		int currentCol = 0;
		while (currentCol < MAX_NUMBER_OF_BRICKS_IN_A_ROW)
		{
			if (canvas.bricks[row][currentCol] != nullptr)
			{
				printBrick(*canvas.bricks[row][currentCol]);
				currentCol += canvas.bricks[row][currentCol]->length;
			}
			else
			{
				cout << " ";
				currentCol++;
			}
		}
		cout << "|" << endl;
	}
	printSymbolNTymes('-', MAX_NUMBER_OF_BRICKS_IN_A_ROW + 2);
}

void moveRow(Canvas& canvas, int rowToMove, int newLocation)
{
	for (int col = 0; col < MAX_NUMBER_OF_BRICKS_IN_A_ROW; col++)
		canvas.bricks[newLocation][col] = canvas.bricks[rowToMove][col];
}

void moveRowsUp(Canvas& canvas, int startingRow)
{
	if (!(startingRow > 0 && startingRow < NUMBER_OF_ROWS))
		return;

	for (int row = 0; row < startingRow; row++)
		moveRow(canvas, row + 1, row);

	emptyARow(canvas, startingRow);

	canvas.currentRow--;
}

void deleteRow(Canvas& canvas, int row)
{
	for(int col = 0; col < MAX_NUMBER_OF_BRICKS_IN_A_ROW; col++)
		if (canvas.bricks[row][col] != nullptr)
		{
			delete canvas.bricks[row][col];
			canvas.bricks[row][col] = nullptr;
		}
}

bool isRowEmpty(Canvas& canvas, int row)
{
	for (int col = 0; col < MAX_NUMBER_OF_BRICKS_IN_A_ROW; col++)
		if (canvas.bricks[row][col] != nullptr)
			return false;
	return true;
}

bool isRowFull(Canvas& canvas, int row)
{
	int currentCol = 0;
	while (currentCol < MAX_NUMBER_OF_BRICKS_IN_A_ROW)
	{
		if (canvas.bricks[row][currentCol] == nullptr)
			return false;
		currentCol += canvas.bricks[row][currentCol]->length;
	}
	return true;
}

void deleteFullRows(Canvas& canvas)
{
	for (int row = canvas.currentRow; row < NUMBER_OF_ROWS; row++)
		if (isRowFull(canvas, row))
			deleteRow(canvas, row);
}

void dropRows(Canvas& canvas)
{
	int currentRow = canvas.currentRow;
	while (currentRow <= NUMBER_OF_ROWS - 1)
	{
		if (isRowEmpty(canvas, currentRow))
		{
			for (int row = currentRow - 1; row >= canvas.currentRow; row--)
				moveRow(canvas, row, row + 1);
			emptyARow(canvas, canvas.currentRow++);
		}
		currentRow++;
	}
}

char colorOfLeftBrick(const Canvas& canvas, int row, int col)
{
	for (int i = col - 1; i >= 0 && i > col - MAX_BRICK_LENGTH; i--)
		if (canvas.bricks[row][i] != nullptr)
			return canvas.bricks[row][i]->color;
	return ' ';
}

void generateRandomRow(Canvas& canvas)
{
	int bottomRow = NUMBER_OF_ROWS - 1;
	moveRowsUp(canvas, bottomRow);
	int currentCol = 0;
	while (currentCol < MAX_NUMBER_OF_BRICKS_IN_A_ROW)
	{
		int newBrickLength = rand() % MAX_BRICK_LENGTH;
		cout << newBrickLength << " " << currentCol << endl;
		if (newBrickLength == 0)
			currentCol++;
		else
		{
			if (currentCol + newBrickLength > MAX_NUMBER_OF_BRICKS_IN_A_ROW)
			{
				newBrickLength = MAX_NUMBER_OF_BRICKS_IN_A_ROW - currentCol;
			}
			Brick* newBrick = new Brick;
			newBrick->length = newBrickLength;
			char newBrickColor;
			char lastBrickColor = colorOfLeftBrick(canvas, bottomRow, currentCol);
			do
			{
				newBrickColor = (rand() % DIFFERENT_BRICK_COLORS) + 'a';
			} while (newBrickColor == lastBrickColor);
			newBrick->color = newBrickColor;
			canvas.bricks[bottomRow][currentCol] = newBrick;
			currentCol += newBrickLength;
		}
	}
}

int main()
{
	//Selecting the user
	ifstream usersFile("users.txt");
	int numberOfUsers = getNumberOfUsers(usersFile);
	User* users = new User[numberOfUsers];
	getUsers(usersFile, users, numberOfUsers);
	printUsers(users, numberOfUsers);
	User selectedUser = selectUser(users, numberOfUsers);
	cout << "Selected user:" << endl;
	printUser(selectedUser);

	//Creating the canvas
	Canvas canvas;
	emptyTheCanvas(canvas);
	printCanvas(canvas);

	//Filling the canvas with temporary bricks
	cout << endl;
	Brick* tempBrick1 = new Brick;
	tempBrick1->color = 'a';
	tempBrick1->length = 4;
	canvas.bricks[canvas.currentRow][0] = tempBrick1;

	Brick* tempBrick2 = new Brick;
	tempBrick2->color = 'b';
	tempBrick2->length = 4;
	canvas.bricks[canvas.currentRow--][4] = tempBrick2;

	Brick* tempBrick3 = new Brick;
	tempBrick3->color = 'c';
	tempBrick3->length = 4;
	canvas.bricks[canvas.currentRow][0] = tempBrick3;

	Brick* tempBrick4 = new Brick;
	tempBrick4->color = 'a';
	tempBrick4->length = 4;
	canvas.bricks[canvas.currentRow][4] = tempBrick4;

	printCanvas(canvas);

	//Generating a random row
	cout << endl;
	srand(time(0));
	generateRandomRow(canvas);
	printCanvas(canvas);

	//Deleting the full rows
	cout << endl;
	deleteFullRows(canvas);
	printCanvas(canvas);
	cout << endl;
	cout << "The highest row is row " << canvas.currentRow << endl;
	dropRows(canvas);
	//Before dropping the rows the highest row doesnt change!
	cout << "The highest row is row " << canvas.currentRow << endl;

	usersFile.clear();
	usersFile.close();

	return 0;
}