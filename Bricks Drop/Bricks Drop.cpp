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

int main()
{
	ifstream usersFile("users.txt");
	int numberOfUsers = getNumberOfUsers(usersFile);
	User* users = new User[numberOfUsers];
	getUsers(usersFile, users, numberOfUsers);
	printUsers(users, numberOfUsers);
	User selectedUser = selectUser(users, numberOfUsers);
	cout << "Selected user:" << endl;
	printUser(selectedUser);

	usersFile.clear();
	usersFile.close();

	return 0;
}