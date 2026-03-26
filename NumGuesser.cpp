#include <iostream>
#include <random>
class numGuesser{
    private:
    int _correctNum;
    public:
    int _guessedNum;
    numGuesser(int correctNum){
        _correctNum = correctNum;
    }
    bool checkForNum(int guessedNum){
        guessedNum = _guessedNum;
        if (guessedNum < _correctNum){
            std::cout << "Too low!" << std::endl;
            return false;
        }
        else if (guessedNum > _correctNum){
            std::cout << "Too high!" << std::endl;
            return false;
        }
        else {
            std::cout << "Correct!" << std::endl;
            return true;
        }

    }

};

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, 100);
    std::cout << "guess a number between 1 and 100:" << std::endl;
    int correctNum = dist(gen);
    numGuesser guesser(correctNum);
    while (!guesser.checkForNum(guesser._guessedNum)){
        std::cin >> guesser._guessedNum;
    }


    return 0;
}