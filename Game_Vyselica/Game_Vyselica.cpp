// Game_Vyselica.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <ctime>
#include <windows.h>
#include <conio.h> 
#include <cstdlib>
#include <map>

using namespace std;

HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

void SetXY(short X, short Y) {
    COORD coord = { X, Y };
    SetConsoleCursorPosition(hStdOut, coord);
}
void SetColor(ConsoleColor text, ConsoleColor background) {
    SetConsoleTextAttribute(hStdOut, (WORD)((background) | text));
}
enum ConsoleColor {
    Black = 0,
    Blue = 1,
    Green = 2,
    Cyan = 3,
    Red = 4,
    Brown = 6,
    LightGray = 7,
    DarkGray = 8,
    LightBlue = 9,
    LightGreen = 10,
    LightCyan = 11,
    LightRed = 12,
    White = 15
};


/*
vector <int> wordsKey = { 1, 2 ,3 ,4 ,5 ,6};
vector <string> wordsAnimals = { "fish", "dog", "cat", "elephant", "lion", "tiger", "monkey", "zebra", "giraffe", "bear", "kangaroo", "koala", "penguin", "seal", "hippo", "chiken", "wolf", "panda", "frog", "owl", "bunny", "ladybug", "fox", "dragonfly", "sheep", "octopus", "butterfly", "whale", "sheep", "horse", "goat", "pig", "snake" };
vector <string> wordsPlants = { "flower", "tree", "grass", "sunflower", "rose", "cactus", "lily", "dandelion", "tulip",  "oak", "birch", "pine", "iris", "lotus", "violet", "dandelion" };
vector <string> wordsFoods = { "apple", "peach", "banana", "orange", "strawberry", "grapefruit", "carrot", "potato", "sandwich", "chicken", "beef", "salad", "cake" };
vector <string> wordsClothes = { "dress", "shirt", "pants", "skirt", "jeans", "sweater", "jacket", "hat", "shoes", "socks", "gloves", "scarf", "bra", "panties", "boots", "junper", "blouse" };
vector <string> wordsItems = { "pencil", "book", "chair", "table", "computer", "desk", "notebook", "pen", "calculator", "clock", "backpack", "ruler", "scissors", "marker", "board" };
vector <string> wordsCountries = { "Brazil", "Canada", "Germany", "Japan", "China", "Italy", "England", "Russia", "France", "Australia", "India", "Spain", "USA", "Mexico", "Poland", "Australia", "Cermany", "nepal", "thailand" };
*/


// Класс необходимый для работы с категориями и словами 
// (в том числе загрузка слов из файлов)
class WordBank {
private:
    std::map<std::string, std::vector<std::string>> categories;
public:
    void loadWordsFromDirectory(const std::string& directoryPath) {
        for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
            if (entry.is_regular_file()) {
                std::string categoryName = entry.path().stem().string(); // Извлекаем имя файла без расширения как название категории
                std::vector<std::string> wordsInCategory;

                std::ifstream file(entry.path());
                if (!file) {
                    std::cerr << "Failed to open file: " << entry.path() << std::endl;
                    continue; // Продолжаем обработку следующих файлов, даже если один не удалось открыть
                }
                std::string word;
                while (std::getline(file, word)) {
                    if (!word.empty()) {
                        wordsInCategory.push_back(word);
                    }
                }

                categories[categoryName] = wordsInCategory;
            }
        }
    }


    /*
    WordBank() {
        categories = {
            {"Animals", {"cat", "dolphin", "elephant", "kangaroo", "wolf"}},
            {"Countries", {"brazil", "canada", "germany", "nepal", "thailand"}},
            {"Foods", {"pizza", "sushi", "taco", "pasta", "salad"}}
        };
    }
    */

    string getRandomWord(const string& category) {
        srand(static_cast<unsigned int>(time(nullptr)));
        const auto& words = categories[category];
        int index = rand() % words.size();
        return words[index];
    }

    void displayCategories() {
        std::cout << "Available Categories:\n";
        for (const auto& category : categories) {
            std::cout << "- " << category.first << "\n";
        }
    }
};


class HangmanGame {
private:
    string wordToGuess;   //word
    string guessedLetters;   //
    vector<char> incorrectGuesses;
    int maxTries;
    int length;
    int key;
    WordBank wordBank;

    void updateGuessedWord(char letter) {
        for (size_t i = 0; i < wordToGuess.length(); ++i) {
            if (wordToGuess[i] == letter) {
                guessedLetters[i] = letter;
            }
        }
    }

public:
    HangmanGame(const std::string& category) : maxTries(7) {
        wordToGuess = wordBank.getRandomWord(category);
        guessedLetters = string(wordToGuess.length(), '_ ');
    }

    bool guess(char letter) {
        if (wordToGuess.find(letter) != std::string::npos) {
            updateGuessedWord(letter);
            return true;
        }
        else {
            incorrectGuesses.push_back(letter);
            return false;
        }
    }

    bool isGameOver() {
        return incorrectGuesses.size() >= maxTries || guessedLetters == wordToGuess;
    }

    bool isGameWon() {
        return guessedLetters == wordToGuess;
    }

    void displayGame() {
        std::cout << "\nIncorrect Guesses: ";
        for (char letter : incorrectGuesses) {
            std::cout << letter << ' ';
        }
        std::cout << "\nWord to Guess: ";
        for (char letter : guessedLetters) {
            std::cout << letter << ' ';
        }
        std::cout << "\n";
    }

    int getRemainingTries() {
        return maxTries - incorrectGuesses.size();
    }
};


// Класс необходимый для вывода результатов в консоль
class Results {
    void GameStart() {
        SetColor(Cyan, Black); SetXY(12, 3); 
        cout << "To start the game, select a theme:" << endl;
        SetColor(LightCyan, Black);
   }
    void ShowCategories() {

    }
};








void Start() {
    
    SetXY(12, 5); cout << "Animals - 'A'.       Plants - 'P'." << endl;
    SetXY(24, 7); cout << "Food - 'F'." << endl;
    SetXY(12, 9); cout << "Clothes - 'C'.        Items - 'I'." << endl;
    SetXY(20, 11); cout << "Random theme - 'R'." << endl;
    SetColor(DarkGray, Black); SetXY(10, 13); cout << "(To select, press the appropriate key)" << endl;

    while (!(key == 'a' || key == 'A' || key == 'p' || key == 'P' || key == 'f' || key == 'F' || key == 'c' || key == 'C' || key == 'i' || key == 'I')) {
        if (_kbhit()) {
            key = _getch();
            if (key == 'r' || key == 'R') {
                key = wordsKey[rand() % wordsKey.size()];
                break;
            }
        }
    }
}

string printWithUnderlines(const string& text) {
    string u_text;
	for (char letter : text) {
		if (isalpha(letter)) {
			u_text += "_ ";
		}
		else {
			u_text += letter;
		}
	}
	return u_text;
}


void ChooseWord() {
    SetColor(Cyan, Black); SetXY(3, 17);
    if (key == 'a' || key == 'A' || key == 1) {
        word = wordsAnimal[rand() % wordsAnimal.size()];
        cout << "Theme: animals." <<  endl;
    }
    if (key == 'p' || key == 'P' || key == 2) {
        word = wordsP[rand() % wordsP.size()];
        
        cout << "Theme: plants." <<  endl;
    }
    if (key == 'f' || key == 'F' || key == 3) {
        word = wordsFruits[rand() % wordsF.size()];
      
        cout << "Theme: food." << endl;
    }
    if (key == 'c' || key == 'C' || key == 4) {
        word = wordsC[rand() % wordsC.size()];
        cout << "Theme: clothes." << endl;
    }
    if (key == 'i' || key == 'I' || key == 5) {
        word = wordsItems[rand() % wordsI.size()];
        cout << "Theme: item." <<  endl;
    }
}

void Walls() {
    SetColor(Blue, Black);

    for (int m = 15; m < 31; m++) {
        SetXY(m, 5); cout << "*";
        SetXY(m, 14); cout << "*";
    }

    for (int m = 6; m < 14; m++) {
        SetXY(15, m); cout << "*";
        SetXY(30, m); cout << "*";
    }
}

bool isWordGuessed() {
    for (char c : word) {
        if (guessedLetters.find(c) == string::npos) {
            return false;
        }
    }
    return true;
}

void printHangman(int attemptsLeft) {
    SetColor(Brown, Black);
    if (attemptsLeft == 7) {
        for (int m = 19; m < 26; m++) {
            SetXY(m, 6); cout << "_";
        }

        for (int m = 7; m < 13; m++) {
            SetXY(18, m); cout << "|";
        }
    }
    else if (attemptsLeft == 6) {
        SetColor(DarkGray, Black); SetXY(26, 7); cout << "|" << endl;
    }
    else if (attemptsLeft == 5) {
        SetColor(White, Black); SetXY(26, 8); cout << "0" << endl;
    }
    else if (attemptsLeft == 4) {
        SetColor(White, Black); SetXY(26, 9); cout << "|" << endl;
    }
    else if (attemptsLeft == 3) {
        SetColor(White, Black); SetXY(25, 9); cout << "/" << endl;
    }
    else if (attemptsLeft == 2) {
        SetColor(White, Black); SetXY(27, 9); cout << "\\" << endl;
    }
    else if (attemptsLeft == 1) {
        SetColor(White, Black); SetXY(25, 10); cout << "/" << endl;
    }
    else if (attemptsLeft == 0) {
        SetColor(White, Black); SetXY(27, 10); cout << "\\" << endl;
    }
}

void printGuessedWord() {
    SetColor(LightCyan, Black);
    int m = (23 - length / 2);
    SetXY(m, 15);
    for (char c : word) {
        if (guessedLetters.find(c) != string::npos) {
            cout << c << " ";
        }
        else { cout << "_ "; }
    }
}

void clearScreen() {
    system("cls");
}

int main() {

    srand(time(0));
    SetColor(LightGreen, Black); SetXY(19, 1); 
    cout << "Welcome to Hangman!" << endl;

    while (true) {

        Start();
        clearScreen();

        Walls();
        SetColor(LightRed, Black); SetXY(3, 50); cout << "The author of this something - Mustafaev Emil aka DarkmMoonDm.";

        ChooseWord();
        length = word.length();

        int attemptsLeft = 7;

        SetXY(3, 18); cout << "Enter the letter: ";

        int enterX = 20;
        char guess;

        while (attemptsLeft > 0 && !isWordGuessed()) {

            SetColor(Cyan, Black); SetXY(5, 2); cout << "You have   attempts to guess the word.";
            SetXY(14, 2); cout << attemptsLeft;

            printHangman(attemptsLeft);

            SetColor(Cyan, Black);
            printGuessedWord();

            enterX++;
            SetColor(LightCyan, Black); SetXY(enterX, 18);
            cin >> guess;

            if (word.find(guess) != string::npos) {
                SetColor(Green, Black); SetXY(19, 4); cout << "Correct!" << endl;
                guessedLetters += guess;
            }
            else {
                SetColor(Red, Black); SetXY(19, 4); cout << " Wrong! " << endl;
                attemptsLeft--;
            }
        }

        printHangman(attemptsLeft);

        if (isWordGuessed()) {
            int m = (23 - length / 2);
            SetColor(LightCyan, Black); SetXY(m, 15); cout << word;
            m = 58 - ((39 + length) / 2);
            SetColor(Green, Black); SetXY(m, 8); cout << "Congratulations, you guessed the word: " << word << '.' << endl;
        }
        else
        {
            SetColor(Cyan, Black); SetXY(14, 2); cout << "0";
            int m = 58 - ((30 + length) / 2);
            SetColor(Red, Black); SetXY(m, 8); cout << "You couldn't guess the word: " << word << '.' << endl;
        }

        SetColor(White, Black);
        SetXY(43, 10); cout << "To start a new game, press'S'." << endl;
        SetXY(48, 11); cout << "To exit, press 'Esc'." << endl;
        while (key != 's' && key != 'S' && key != 27) {
            if (_kbhit()) {
                key = _getch();
                clearScreen();
            }
        }
        if (key == 27) { break; }

        length, key = 0;
        word = "";
        guessedLetters = "";
    }
}
