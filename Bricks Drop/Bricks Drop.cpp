/**
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

using namespace std;

constexpr int maxUserNameLength = 100;
constexpr int maxIntLength = 10;
constexpr int maxNumberOfBricksInARow = 8;
constexpr int numberOfRows = 10;

struct User
{
	char name[maxUserNameLength + 1];
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
	char numberOfUsersString[maxIntLength + 1];
	usersFile >> numberOfUsersString;
	return stringToNum(numberOfUsersString);
}

void getUsers(ifstream& userFile, User* users, size_t numberOfUsers)
{
	if (users == nullptr)
		return;

	for (size_t i = 0; i < numberOfUsers; i++)
	{
		char currentUserName[maxUserNameLength + 1];
		userFile >> currentUserName;
		strCpy(currentUserName, users[i].name);
		char currentUserHighScore[maxIntLength + 1];
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
	char selectedUserName[maxUserNameLength + 1];
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

void printBrick(Brick brick)
{
	for (int i = 0; i < brick.length; i++)
		cout << brick.color;
}

struct Canvas
{
	Brick* bricks[numberOfRows][maxNumberOfBricksInARow];
	int currentRow = numberOfRows - 1; //The row at the top is 0 and the lowest is numberOfRows - 1
};

void emptyTheCanvas(Canvas& canvas)
{
	for (int row = 0; row < numberOfRows; row++)
		for (int col = 0; col < maxNumberOfBricksInARow; col++)
			canvas.bricks[row][col] = nullptr;
	canvas.currentRow = numberOfRows - 1;
}

void printCanvas(const Canvas canvas)
{
	for (int i = 0; i < maxNumberOfBricksInARow + 2; i++)
		cout << "-";
	cout << endl;
	for (int row = 0; row < numberOfRows; row++)
	{
		cout << "|";
		int currentCol = 0;
		while (currentCol < maxNumberOfBricksInARow)
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
	for (int i = 0; i < maxNumberOfBricksInARow + 2; i++)
		cout << "-";
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
	Brick tempBrick1;
	tempBrick1.color = 'a';
	tempBrick1.length = 4;
	canvas.bricks[canvas.currentRow][0] = &tempBrick1;

	Brick tempBrick2;
	tempBrick2.color = 'b';
	tempBrick2.length = 3;
	canvas.bricks[canvas.currentRow--][5] = &tempBrick2;

	Brick tempBrick3;
	tempBrick3.color = 'c';
	tempBrick3.length = 2;
	canvas.bricks[canvas.currentRow][0] = &tempBrick3;

	Brick tempBrick4;
	tempBrick4.color = 'c';
	tempBrick4.length = 4;
	canvas.bricks[canvas.currentRow--][4] = &tempBrick4;

	printCanvas(canvas);

	usersFile.clear();
	usersFile.close();

	return 0;
}