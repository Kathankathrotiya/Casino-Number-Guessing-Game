#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <map>
#include <fstream>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

using namespace std;

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    cout << "\033[H\033[J";
#endif
}

void rules()
{
    clearScreen();
    cout << "\t\t======CASINO NUMBER GUESSING RULES!======\n\n";
    cout << "\t1. Choose a number between 1 to 10\n";
    cout << "\t2. Winner gets 10 times the money bet\n";
    cout << "\t3. Wrong bet, and you lose the amount you bet\n\n";
}

void saveUserData(const map<string, pair<int, int>> &userDatabase)
{
    ofstream file("userdata.txt");
    if (file.is_open())
    {
        for (const auto &entry : userDatabase)
        {
            file << entry.first << " " << entry.second.first << " " << entry.second.second << endl;
        }
        file.close();
    }
}

map<string, pair<int, int>> loadUserData()
{
    map<string, pair<int, int>> userDatabase;
    ifstream file("userdata.txt");
    if (file.is_open())
    {
        string name;
        int won, lost;
        while (file >> name >> won >> lost)
        {
            userDatabase[name] = {won, lost};
        }
        file.close();
    }
    return userDatabase;
}

int main()
{
    map<string, pair<int, int>> userDatabase = loadUserData();
    string playerName;
    int playerBalance;

    srand(time(0));

    cout << "\n\t\t========WELCOME TO DARK CASINO=======\n\n";
    cout << "\n\nEnter Your Name : ";
    cin >> playerName;

    if (userDatabase.find(playerName) == userDatabase.end())
    {
        cout << "Welcome, " << playerName << "! Please enter your starting balance: $";
        cin >> playerBalance;
        if (playerBalance <= 0)
        {
            cout << "Invalid starting balance. Please enter a positive amount." << endl;
            return 1;
        }
        userDatabase[playerName] = {playerBalance, 0};
    }
    else
    {
        cout << "Welcome back, " << playerName << "!\n";
        cout << "Previous Progress: Won $" << userDatabase[playerName].first << " Lost $" << userDatabase[playerName].second << endl;
        playerBalance = userDatabase[playerName].first - userDatabase[playerName].second; // Initialize with the remaining balance
    }
    char choice;

    int wonInPlay = 0;
    int lostInPlay = 0;
    do
    {
        int bettingAmount;
        int guesses;
        int random;
        clearScreen();
        rules();
        cout << "\n\nYour Current Balance is $ " << playerBalance << "\n";

        do
        {
            cout << "Hello, " << playerName << ", Enter Amount you want to bet for : $";
            cin >> bettingAmount;
            if (cin.fail())
            {
                cout << "Invalid input. Please enter a valid number." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            else if (bettingAmount <= 0 || bettingAmount > playerBalance)
            {
                cout << "Invalid bet amount. Please enter a valid amount." << endl;
            }
        } while (bettingAmount <= 0 || bettingAmount > playerBalance);

        do
        {
            cout << "Guess any betting number between 1 & 10 :";
            cin >> guesses;
            if (cin.fail())
            {
                cout << "Invalid input. Please enter a valid number." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            else if (guesses <= 0 || guesses > 10)
            {
                cout << "Number should be between 1 to 10. Please re-enter." << endl;
            }
        } while (guesses <= 0 || guesses > 10);

        random = rand() % 10 + 1;
        if (random == guesses)
        {
            cout << "\n\nCongratulations!!\nYou are Lucky!!\nYou have won $ " << bettingAmount * 10;
            wonInPlay = bettingAmount * 10;
            playerBalance += wonInPlay;
            userDatabase[playerName].first += wonInPlay;
        }
        else
        {
            cout << "Oops, better luck next time !! You lost $ " << bettingAmount << "\n";
            lostInPlay = bettingAmount;
            playerBalance -= lostInPlay;
            userDatabase[playerName].second += lostInPlay;
        }

        cout << "\nWinning Number was : " << random << "\n";
        cout << "\n"
             << playerName << ", You have Balance of $ " << playerBalance << "\n";

        if (playerBalance == 0)
        {
            cout << "Sorry, You have no money to play.";
            break;
        }

        do
        {
            cout << "\n\n-->Do you want to play again (y/n)? ";
            cin >> choice;
        } while (choice != 'Y' && choice != 'y' && choice != 'N' && choice != 'n');

    } while (choice == 'Y' || choice == 'y');

    clearScreen();
    cout << "\n\nThanks for playing Dark Casino. Your Final Balance is $ " << playerBalance << "\n\n";
    cout << "You won $ " << wonInPlay << " and lost $ " << lostInPlay << " in this game session.\n";
    saveUserData(userDatabase);

    return 0;
}
