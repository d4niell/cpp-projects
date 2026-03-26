#include <iostream>
#include <string>
#include <limits>
class RPS{
    public:
    std::string player1, player2;
    RPS(std::string p1, std::string p2){
        player1 = p1;
        player2 = p2;
        _isPlayerTurn = true;
    }
    int choicep1 = 0;
    int choicep2 = 0;
    bool _isPlayerTurn;
    bool Winner = false;
    bool getChoice(int choice){
        if (choice < 1 || choice > 3){
            std::cout << "Invalid choice. Use 1 (rock), 2 (paper), 3 (scissors)." << std::endl;
            return false;
        }

        if (_isPlayerTurn){
        choicep1 = choice;
        if (choicep1 == 1){
            std::cout << "Player 1 chose rock" << std::endl;
            _isPlayerTurn = false;
        }
        else if (choicep1 == 2){
            std::cout << "Player 1 chose paper" << std::endl;
            _isPlayerTurn = false;
        }
        else if (choicep1 == 3){
            std::cout << "Player 1 chose scissors" << std::endl;
            _isPlayerTurn = false;
        }
    }
    else if (!_isPlayerTurn){
        choicep2 = choice;
        if (choicep2 == 1){
            std::cout << "Player 2 chose rock" << std::endl;
            _isPlayerTurn = true;
        }
        else if (choicep2 == 2){
            std::cout << "Player 2 chose paper" << std::endl;
            _isPlayerTurn = true;
        }
        else if (choicep2 == 3){
            std::cout << "Player 2 chose scissors" << std::endl;
            _isPlayerTurn = true;
        }
        }
        return true;
    }

    bool roundReady() const{
        return _isPlayerTurn;
    }
    void CheckForWinner(int choice1, int choice2){
        if (choice1 == choice2){
            std::cout << "It's a tie!" << std::endl;
        }
        else if ((choice1 == 1 && choice2 == 3) || (choice1 == 2 && choice2 == 1) || (choice1 == 3 && choice2 == 2)){
            std::cout << "Player 1 wins!" << std::endl;
            Winner = true;
        }
        else{
            std::cout << "Player 2 wins!" << std::endl;
            Winner = true;
        }
    }
};

int main(){
    RPS rps("Pelaaja 1", "Pelaaja 2");
    std::string line;
    
    while (rps.Winner == false)
    {
        std::cout << (rps._isPlayerTurn ? "Player 1" : "Player 2")
                  << " enter choice (1 for rock, 2 for paper, 3 for scissors): ";

        std::getline(std::cin, line);
        int choice = 0;
        try{
            choice = std::stoi(line);
        }
        catch (...){
            std::cout << "Please enter a valid number." << std::endl;
            continue;
        }

        if (!rps.getChoice(choice)){
            continue;
        }

        if (rps.roundReady()){
            rps.CheckForWinner(rps.choicep1, rps.choicep2);
        }
    }

    return 0;
}