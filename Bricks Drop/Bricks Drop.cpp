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
#include <cstdlib>
#include <time.h>

constexpr int MAX_USER_NAME_LENGTH = 100;
constexpr int MAX_INT_LENGTH = 10;
constexpr int MAX_BRICK_LENGTH = 4;
constexpr int DIFFERENT_BRICK_COLORS = 4;
constexpr int MAX_NUMBER_OF_BRICKS_IN_A_ROW = 8;
constexpr int NUMBER_OF_ROWS = 10;
constexpr int MAX_COMMAND_LENGTH = 7;

const int LEFT_DIRECTION = -1;
const int RIGHT_DIRECTION = 1;
const char LEFT_DIRECTION_CHAR = 'l';
const char RIGHT_DIRECTION_CHAR = 'r';
const char TEMPLATE_DIGIT_CHAR = '%';
const char TEMPLATE_DIRECTION_CHAR = '@';
const char TEMPLATE_MOVE_STRING[] = "% % @ %";
const char QUIT_COMMAND[] = "quit";
const char RESTART_COMMAND[] = "restart";
const int INDEX_OF_BRICK_ROW_IN_COMMAND = 0;
const int INDEX_OF_BRICK_COLUMN_IN_COMMAND = 2;
const int INDEX_OF_DIRECTION_IN_COMMAND = 4;
const int INDEX_OF_MOVE_POSITIONS_IN_COMMAND = 6;
const int SCORE_FOR_A_DELETED_ROW = 10;

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

bool isCharInInterval(char ch, char intervalStart, char intervalEnd)
{
	return ch >= intervalStart && ch <= intervalEnd;
}

int getNumberOfUsers(std::ifstream& usersFile)
{
	char numberOfUsersString[MAX_INT_LENGTH + 1];
	usersFile >> numberOfUsersString;
	return stringToNum(numberOfUsersString);
}

void getUsers(std::ifstream& userFile, User* users, size_t numberOfUsers)
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

void updateUsersFile(User* users, size_t numberOfUsers)
{
	std::ofstream usersFile("users.txt");
	usersFile << numberOfUsers << std::endl;
	for (size_t i = 0; i < numberOfUsers; i++)
		usersFile << users[i].name << " " << users[i].highScore << std::endl;

	//Closing the users file
	usersFile.clear();
	usersFile.close();
}

void printUser(User user)
{
	std::cout << "User: " << user.name << " High Score: " << user.highScore << std::endl;
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
	std::cout << "Select user: ";
	std::cin >> selectedUserName;
	while (!isSelectedUserValid(users, numberOfUsers, selectedUserName))
	{
		std::cout << "The selected user is not valid. Choose a different user: ";
		std::cin >> selectedUserName;
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
		std::cout << symbol;
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
	std::cout << " ";
	printSymbolNTymes('-', MAX_NUMBER_OF_BRICKS_IN_A_ROW + 2);
	std::cout << std::endl;
	for (int row = 0; row < NUMBER_OF_ROWS; row++)
	{
		//Labels for the rows
		std::cout << NUMBER_OF_ROWS - row - 1 << "|";
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
				std::cout << " ";
				currentCol++;
			}
		}
		std::cout << "|" << std::endl;
	}
	std::cout << " ";
	printSymbolNTymes('-', MAX_NUMBER_OF_BRICKS_IN_A_ROW + 2);
	//Labels for columns
	std::cout << std::endl << "  ";
	for (int i = 0; i < MAX_NUMBER_OF_BRICKS_IN_A_ROW; i++)
		std::cout << i;
	std::cout << std::endl;
}

void printGameScreen(const Canvas& canvas, int score)
{
	std::cout << "Score: " << score << std::endl;
	std::cout << "Current row: " << canvas.currentRow << std::endl;
	printCanvas(canvas);
}

bool isRowValid(int row)
{
	return row >= 0 && row < NUMBER_OF_ROWS;
}

void moveRow(Canvas& canvas, int rowToMove, int newLocation)
{
	if (!isRowValid(rowToMove) || !isRowValid(newLocation))
		return;
	for (int col = 0; col < MAX_NUMBER_OF_BRICKS_IN_A_ROW; col++)
		canvas.bricks[newLocation][col] = canvas.bricks[rowToMove][col];
}

void moveRowsUp(Canvas& canvas, int startingRow)
{
	if (!isRowValid(startingRow))
		return;
	if (!(startingRow > 0 && startingRow < NUMBER_OF_ROWS))
		return;

	for (int row = 0; row < startingRow; row++)
		moveRow(canvas, row + 1, row);

	emptyARow(canvas, startingRow);

	canvas.currentRow--;
}

void deleteRow(Canvas& canvas, int row)
{
	if (!isRowValid(row))
		return;
	for (int col = 0; col < MAX_NUMBER_OF_BRICKS_IN_A_ROW; col++)
		if (canvas.bricks[row][col] != nullptr)
		{
			delete canvas.bricks[row][col];
			canvas.bricks[row][col] = nullptr;
		}
}

bool isRowEmpty(Canvas& canvas, int row)
{
	if (!isRowValid(row))
		return false;
	for (int col = 0; col < MAX_NUMBER_OF_BRICKS_IN_A_ROW; col++)
		if (canvas.bricks[row][col] != nullptr)
			return false;
	return true;
}

bool isRowFull(Canvas& canvas, int row)
{
	if (!isRowValid(row))
		return false;
	int currentCol = 0;
	while (currentCol < MAX_NUMBER_OF_BRICKS_IN_A_ROW)
	{
		if (canvas.bricks[row][currentCol] == nullptr)
			return false;
		currentCol += canvas.bricks[row][currentCol]->length;
	}
	return true;
}

//Returns the number of deleted rows
int deleteFullRows(Canvas& canvas)
{
	int rowsDeleted = 0;
	for (int row = canvas.currentRow; row < NUMBER_OF_ROWS; row++)
		if (isRowFull(canvas, row))
		{
			deleteRow(canvas, row);
			rowsDeleted++;
		}
	return rowsDeleted;
}

void dropAllRowsAboveGivenRow(Canvas& canvas, int row)
{
	for (int currentRow = row; currentRow > canvas.currentRow; currentRow--)
	{
		if (currentRow != 0)
			moveRow(canvas, currentRow - 1, currentRow);
		else
			emptyARow(canvas, currentRow);
	}
	canvas.currentRow++;
}

void dropRows(Canvas& canvas)
{
	int currentRow = NUMBER_OF_ROWS - 1;
	while (currentRow > canvas.currentRow)
	{
		if (isRowEmpty(canvas, currentRow))
			dropAllRowsAboveGivenRow(canvas, currentRow);
		else
			currentRow--;
	}
}

int emptySpaceInDirection(Canvas& canvas, int row, int col, int direction)
{
	int emptySpace = 0;
	switch (direction)
	{
	case RIGHT_DIRECTION:
		if (canvas.bricks[row][col])
			emptySpace -= canvas.bricks[row][col]->length;
		break;
	case LEFT_DIRECTION:
		emptySpace--;
		break;
	default:
		break;
	}
	int currentCol = col;
	do
	{
		emptySpace++;
		currentCol += direction;
	} while (currentCol >= 0 && currentCol < MAX_NUMBER_OF_BRICKS_IN_A_ROW && !canvas.bricks[row][currentCol]);
	if (direction == LEFT_DIRECTION && currentCol >= 0 && currentCol < MAX_NUMBER_OF_BRICKS_IN_A_ROW && canvas.bricks[row][currentCol])
		emptySpace -= canvas.bricks[row][currentCol]->length - 1;
	return emptySpace;
}

void addBrickToBoolArray(Canvas& canvas, int brickRow, int brickCol, bool* arr, size_t size)
{
	if (!canvas.bricks[brickRow][brickCol])
		return;
	for (int i = 0; i < canvas.bricks[brickRow][brickCol]->length; i++)
		if (brickCol + i < size)
			arr[brickCol + i] = true;
}

bool canBrickDrop(Canvas& canvas, int brickRow, int brickCol)
{
	if (!canvas.bricks[brickRow][brickCol])
		return false;
	if (brickRow == NUMBER_OF_ROWS - 1)
		return false;
	bool lowerRowBoolRepresentation[MAX_NUMBER_OF_BRICKS_IN_A_ROW]{ 0 };
	for (int col = 0; col < MAX_NUMBER_OF_BRICKS_IN_A_ROW; col++)
		if (canvas.bricks[brickRow + 1][col])
			addBrickToBoolArray(canvas, brickRow + 1, col, lowerRowBoolRepresentation, MAX_NUMBER_OF_BRICKS_IN_A_ROW);
	for (int i = brickCol; i < brickCol + canvas.bricks[brickRow][brickCol]->length; i++)
		if (lowerRowBoolRepresentation[i])
			return false;
	return true;
}

bool canABrickDrop(Canvas& canvas)
{
	for (int row = NUMBER_OF_ROWS - 1; row > canvas.currentRow; row--)
		for (int col = 0; col < MAX_NUMBER_OF_BRICKS_IN_A_ROW; col++)
			if (!canvas.bricks[row][col] && canBrickDrop(canvas, row, col))
				return true;
	return false;
}

void moveBrick(Canvas& canvas, int brickRow, int brickCol, int newBrickRow, int newBrickCol)
{
	if (canvas.bricks[brickRow][brickCol] && !canvas.bricks[newBrickRow][newBrickCol])
	{
		canvas.bricks[newBrickRow][newBrickCol] = canvas.bricks[brickRow][brickCol];
		canvas.bricks[brickRow][brickCol] = nullptr;
	}
}

bool isCharInTemplate(char ch, char templateCh)
{
	if (templateCh == TEMPLATE_DIGIT_CHAR)
		return ch >= '0' && ch <= '9';
	if (templateCh == TEMPLATE_DIRECTION_CHAR)
		return ch == LEFT_DIRECTION_CHAR || ch == RIGHT_DIRECTION_CHAR;
	return ch == templateCh;
}

bool isStringInTemplate(const char* str, const char* templateStr)
{
	if (!str || !templateStr)
		return false;
	if (strLen(str) != strLen(templateStr))
		return false;
	while (*str)
	{
		if (!isCharInTemplate(*str, *templateStr))
			return false;
		str++;
		templateStr++;
	}
	return true;
}

bool isValidMoveCommand(Canvas& canvas, const char* moveCommand)
{
	if(!(isStringInTemplate(moveCommand, TEMPLATE_MOVE_STRING)))
		return false;
	int brickRow = moveCommand[INDEX_OF_BRICK_ROW_IN_COMMAND] - '0';
	brickRow = NUMBER_OF_ROWS - brickRow - 1;
	int brickCol = moveCommand[INDEX_OF_BRICK_COLUMN_IN_COMMAND] - '0';
	if (!canvas.bricks[brickRow][brickCol])
	{
		std::cout << "There isn't a brick at the given coordinates!" << std::endl;
		return false;
	}
	char moveDirection = moveCommand[INDEX_OF_DIRECTION_IN_COMMAND];
	int movePositions = moveCommand[INDEX_OF_MOVE_POSITIONS_IN_COMMAND] - '0';
	int emptySpace = 0;
	switch (moveDirection)
	{
	case LEFT_DIRECTION_CHAR:
		emptySpace = emptySpaceInDirection(canvas, brickRow, brickCol, LEFT_DIRECTION);
		if (movePositions > emptySpace)
		{
			std::cout << "There are " << emptySpace << " empty space int the given direction!" << std::endl;
			return false;
		}
		break;
	case RIGHT_DIRECTION_CHAR:
		emptySpace = emptySpaceInDirection(canvas, brickRow, brickCol, RIGHT_DIRECTION);
		if (movePositions > emptySpace)
		{
			std::cout << "There are " << emptySpace << " empty space int the given direction!" << std::endl;
			return false;
		}
		break;
	default:
		break;
	}
	return true;
}

void getCommand(Canvas& canvas, bool& isGameOver)
{
	char command[MAX_COMMAND_LENGTH + 1]{ 0 };
	do
	{
		std::cin.getline(command, MAX_COMMAND_LENGTH + 1);
	} while (!(strCmp(command, QUIT_COMMAND) == 0 || (strCmp(command, RESTART_COMMAND) == 0 && isGameOver) || isValidMoveCommand(canvas, command)));
	if (strCmp(command, QUIT_COMMAND) == 0)
	{
		isGameOver = true;
		return;
	}
	if (strCmp(command, RESTART_COMMAND) == 0)
	{
		std::cout << "RESET";
		isGameOver = false;
		return;
	}
	if (isStringInTemplate(command, TEMPLATE_MOVE_STRING))
	{
		int brickRow = command[INDEX_OF_BRICK_ROW_IN_COMMAND] - '0';
		brickRow = NUMBER_OF_ROWS - brickRow - 1;
		int brickCol = command[INDEX_OF_BRICK_COLUMN_IN_COMMAND] - '0';
		char moveDirection = command[INDEX_OF_DIRECTION_IN_COMMAND];
		int movePositions = command[INDEX_OF_MOVE_POSITIONS_IN_COMMAND] - '0';
		switch (moveDirection)
		{
		case LEFT_DIRECTION_CHAR:
			moveBrick(canvas, brickRow, brickCol, brickRow, brickCol - movePositions);
			break;
		case RIGHT_DIRECTION_CHAR:
			moveBrick(canvas, brickRow, brickCol, brickRow, brickCol + movePositions);
			break;
		default:
			break;
		}
	}
}

//returns true if a brick has dropped
bool dropBricks(Canvas& canvas)
{
	bool hasBrickDropped = false;
	for (int row = NUMBER_OF_ROWS - 1; row > canvas.currentRow; row--)
	{
		for (int col = 0; col < MAX_NUMBER_OF_BRICKS_IN_A_ROW; col++)
		{
			if (canvas.bricks[row][col] != nullptr && canBrickDrop(canvas, row, col))
			{
				moveBrick(canvas, row, col, row + 1, col);
				hasBrickDropped = true;
			}
		}
	}
	for(int row = canvas.currentRow + 1; row < NUMBER_OF_ROWS; row++)
		if (isRowEmpty(canvas, row))
			canvas.currentRow++;
	return hasBrickDropped;
}

//this function allows bricks to drop more than 1 rows
bool dropBricksWhilePossible(Canvas& canvas)
{
	bool hasBrickDropped = dropBricks(canvas);
	while (dropBricks(canvas));
	return hasBrickDropped;
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

void generateNotFullRandomRow(Canvas& canvas, int& score)
{
	int bottomRow = NUMBER_OF_ROWS - 1;
	bool successfulGeneration = false;
	while(!successfulGeneration)
	{
		generateRandomRow(canvas);
		std::cout << std::endl;
		printGameScreen(canvas, score);
		if (isRowFull(canvas, bottomRow))
		{
			score += SCORE_FOR_A_DELETED_ROW;
			deleteRow(canvas, bottomRow);
			std::cout << std::endl;
			printGameScreen(canvas, score);
			dropRows(canvas);
			std::cout << std::endl;
			printGameScreen(canvas, score);
		}
		else
			successfulGeneration = true;
	}
}

void setup(Canvas& canvas, User*& users, size_t& numberOfUsers, User& selectedUser)
{
	//Opening the users file
	std::ifstream usersFile("users.txt");

	//Selecting the user
	numberOfUsers = getNumberOfUsers(usersFile);
	users = new User[numberOfUsers];
	getUsers(usersFile, users, numberOfUsers);
	std::cout << "All users: " << std::endl;
	printUsers(users, numberOfUsers);
	selectedUser = selectUser(users, numberOfUsers);
	std::cout << "Selected user:" << std::endl;
	printUser(selectedUser);
	std::cout << "Type quit to exit." << std::endl;

	//Creating the canvas
	emptyTheCanvas(canvas);
	printCanvas(canvas);

	//Setting the seed for the random number generator
	srand(time(0));

	//Closing the users file
	usersFile.clear();
	usersFile.close();
}

void gameLoop(Canvas& canvas, int& score, bool& isGameOver)
{
	while (!isGameOver)
	{
		std::cout << std::endl;
		generateNotFullRandomRow(canvas, score);
		std::cout << std::endl;
		if(dropBricksWhilePossible(canvas))
			printGameScreen(canvas, score);
		int deletedRows = deleteFullRows(canvas);
		if(deletedRows > 0)
		{
			score += deletedRows * SCORE_FOR_A_DELETED_ROW;
			dropRows(canvas);
			printGameScreen(canvas, score);
		}
		if(isRowValid(canvas.currentRow))
			getCommand(canvas, isGameOver);
		else
			isGameOver = true;
	}
}

int main()
{
	Canvas canvas;
	User* users = nullptr;
	size_t numberOfUsers = 0;
	User selectedUser;
	int score = 0;
	bool isGameOver = false;

	while(!isGameOver)
	{
		setup(canvas, users, numberOfUsers, selectedUser);
		gameLoop(canvas, score, isGameOver);

		std::cout << "Final score: " << score << std::endl;
		if (score > selectedUser.highScore)
		{
			std::cout << "NEW HIGH SCORE!!!" << std::endl;
			users[findIndexOfUser(users, numberOfUsers, selectedUser.name)].highScore = score;
			updateUsersFile(users, numberOfUsers);
		}

		std::cout << "Type restart to play again or quit to exit the game:" << std::endl;
		getCommand(canvas, isGameOver);
	}

	delete[] users;

	return 0;
}