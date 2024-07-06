// Game_Vyselica.cpp
//

#include <iostream>
#include <filesystem>
#include <fstream>
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


// Класс необходимый для работы с категориями и словами 
// (в том числе загрузка слов из файлов)
class WordBank {
public:
    std::map<std::string, std::vector<std::string>> categories;
    void loadWordsFromDirectory(const std::string& directoryPath) {
        for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
            if (entry.is_regular_file()) {
                string categoryName = entry.path().stem().string(); // Извлекаем имя файла без расширения как название категории
                vector<std::string> wordsInCategory;

                ifstream file(entry.path());
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


   string getRandomWord(const string& category) {
        srand(static_cast<unsigned int>(time(nullptr)));
        const auto& words = categories[category];
        int index = rand() % words.size();
        return words[index];
    }
};


class HangmanGame {
private:
    string wordToGuess;

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
    string selectcategory;
    string guessedLetters;   //

    HangmanGame();
    HangmanGame(int _maxTries,const std::string& word) : maxTries(_maxTries), wordToGuess(word), guessedLetters(word.length(), '_ ') {}
    HangmanGame(const std::string& category, int _maxTries) : maxTries(_maxTries) {
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
    string getWordToGuess() {
		return wordToGuess;
	}
};


// Класс необходимый для красивого вывода результатов в консоль
class Results {
public:
    void StartShow() {
        SetColor(LightGreen, Black); SetXY(19, 1);
        cout << "Welcome to Hangman!" << endl;
        SetColor(Cyan, Black); SetXY(12, 3); 
        cout << "To start the game, select a theme:" << endl;    
   }

    void clearScreen() {
        system("cls");
    }
    string Show_SetCategories(map<std::string, std::vector<std::string>> categories) {
        SetColor(LightCyan, Black);
        vector<char> keys; // Вектор с начальными буквами для categories
        int x = 12, y = 5; // Начальная позиция для вывода категорий
        for (const auto& category : categories) {
            SetXY(x, y);
            std::string categoryName = category.first;
            char key = static_cast<char>(std::toupper(categoryName[0])); // Получаем клавишу для категории по первой букве файла категории
            keys.push_back(key);
            // Выводим название категории и соответствующую ей клавишу
            cout << categoryName << " - '" << key << "'." << endl;

            // Изменяем позицию для следующей категории
            y += 2; // Переходим на две строки вниз после каждой категории для наглядного разделения
        }

        SetColor(DarkGray, Black);
        SetXY(10, y + 2); // Смещаем указатель ниже последней категории
        cout << "(To select, press the appropriate key)" << endl;

        char key;
        while (find(keys.begin(), keys.end(), toupper(key)) == keys.end()) {

            if (_kbhit()) {
                key = _getch();
                if (key == 'r' || key == 'R') {
                    key = keys[rand() % keys.size()];
                    break;
                }
            }
        }

        for (const auto& category : categories) {
            if (toupper(category.first[0]) == toupper(key)) {
                return category.first;
            }
            else return "";
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

    void printHangman(int attemptsLeft) {
        switch (attemptsLeft) {
        case 7:
            SetColor(Brown, Black);
            for (int m = 19; m < 26; m++) {
                SetXY(m, 6); cout << "_";
            }
            for (int m = 7; m < 13; m++) {
                SetXY(18, m); cout << "|";
            }
            break;
        case 6:
            SetColor(DarkGray, Black); SetXY(26, 7); cout << "|";
            break;
        case 5:
            SetColor(White, Black); SetXY(26, 8); cout << "0";
            break;
        case 4:
            SetColor(White, Black); SetXY(26, 9); cout << "|";
            break;
        case 3:
            SetColor(White, Black); SetXY(25, 9); cout << "/";
            break;
        case 2:
            SetColor(White, Black); SetXY(27, 9); cout << "\\";
            break;
        case 1:
            SetColor(White, Black); SetXY(25, 10); cout << "/";
            break;
        case 0:
            SetColor(White, Black); SetXY(27, 10); cout << "\\";
            break;
        default:
            // Handle invalid attemptsLeft values, if necessary
            break;
        }
    }
    void ShowCategoryWord(const string& category) {
        SetColor(Cyan, Black); SetXY(3, 17);
        cout << "Theme: "<< category<<"." << endl;
        }

    void ShowEnterletter() {
        SetXY(3, 18); cout << "Enter the letter: ";
    }
    void Showattempts(int attemptsLeft) {
        SetColor(Cyan, Black);
        SetXY(5, 2);
        cout << "You have " << attemptsLeft << " attempts to guess the word.";
    }
    void printGuessedWord(string guessedLetters) {
        SetColor(LightCyan, Black);
        int m = (23 - guessedLetters.length() / 2);
        SetXY(m, 15);
        cout << guessedLetters << " ";
    }
    char GetLetter() {
        int enterX = 20;
        char guess;
        SetColor(LightCyan, Black);
        SetXY(enterX, 18);
        cin >> guess;
        return guess;
    }
    
    void PrintResultGuessedLeter(bool flagguessletter) {
        SetXY(19, 4);
        if (flagguessletter) {
			SetColor(Green, Black);
			cout << "Correct!" << endl;
		}
		else {
			SetColor(Red, Black);
			cout << " Wrong! " << endl;
		}
    }
    void PrintResultGame(bool flagwin,string word) {
        if (flagwin) {

            int m = (23 - word.length()/ 2);
            SetColor(LightCyan, Black); SetXY(m, 15); cout << word;
            m = 58 - ((39 + word.length()) / 2);
            SetColor(Green, Black); SetXY(m, 8); cout << "Congratulations, you guessed the word: " << word << '.' << endl;
        }
        else
        {
            SetColor(Cyan, Black); SetXY(14, 2); cout << "0";
            int m = 58 - ((30 + word.length()) / 2);
            SetColor(Red, Black); SetXY(m, 8); cout << "You couldn't guess the word: " << word << '.' << endl;
        }
    }

    void PrintMessageNewGame() {
    SetColor(White, Black);
    SetXY(43, 10); cout << "To start a new game, press'S'." << endl;
    SetXY(48, 11); cout << "To exit, press 'Esc'." << endl;
    }
    
};


// Возможна загрузка программы с параметрами
// первый параметор слово
// второй параметр количество попыток
// g++ -o game game.cpp. / game mysteryword 5
void main(int argc, char* argv[]) {
    srand(time(nullptr));
    std::string guessedWord;
    int attempts = 7; // Default number of attempts
    WordBank wordBank;
    Results results;
    HangmanGame hangmanGame;
    results.StartShow();

    // Check if at least one argument is passed
    if (argc > 1) {
        guessedWord = argv[1]; // First argument is the guessed word
        // If a second argument is provided, parse it as the number of attempts
    if (argc > 2) {
            attempts = stoi(argv[2]); // Convert the second argument to an integer
        }
    }

    if (guessedWord.empty()) {
        // Show and selet Categories and user to enter category
        wordBank.loadWordsFromDirectory("\\categories");
        hangmanGame = HangmanGame(results.Show_SetCategories(wordBank.categories), attempts);

    }
    else
        hangmanGame = HangmanGame(attempts,guessedWord);


    while (true) {

        results.clearScreen();
        results.Walls();
        
        results.ShowCategoryWord(hangmanGame.selectcategory);
        
        results.ShowEnterletter();
        
        
        char guess,key;

        while (hangmanGame.isGameOver()) {

            results.printHangman(hangmanGame.getRemainingTries());
            results.Showattempts(hangmanGame.getRemainingTries());

            results.printGuessedWord(hangmanGame.guessedLetters);

            results.PrintResultGuessedLeter(hangmanGame.guess(results.GetLetter()));


            results.printHangman(hangmanGame.getRemainingTries());

            results.PrintResultGame(hangmanGame.isGameWon(), hangmanGame.getWordToGuess());

            results.PrintMessageNewGame();

            while (key != 's' && key != 'S' && key != 27) {
                if (_kbhit()) {
                    key = _getch();
                    results.clearScreen();
                }
            }
            if (key == 27) { break; }

            hangmanGame = HangmanGame(results.Show_SetCategories(wordBank.categories), attempts);
        }
}
