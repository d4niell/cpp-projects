#include <iostream>
#include "Windows.H"
#include <filesystem>
#include <chrono>
#include <vector>
#include <string>
class Expenses{
    public: 
    int id;
    std::string date;
    int category;
    double amount;
    std::string description;

    void addExpense(int _id, std::string _date, int _category, double _amount, std::string _description){
        id = _id;
        date = _date;
        category = _category;
        amount = _amount;
        description = _description;


    }
    void viewAllExpenses(){
        std::cout << "ID: " << id << std::endl;
        std::cout << "Date: " << date << std::endl;
        std::cout << "Category: " << category << std::endl;
        std::cout << "Amount: " << amount << std::endl;
        std::cout << "Description: " << description << std::endl;
    }
    void deleteExpense(){
        id = 0;
        date = "";
        category = 0;
        amount = 0.0;
        description = "";
    }
};
std::vector<Expenses> expenses;
int main() {
    Expenses exp;
    int choice;
    std::cout << "Welcome to the Expense Tracker!\n";
    std::cout << "1. Add expense\n";
    std::cout << "2. View all expenses\n";
    std::cout << "3. Delete expense\n";
    std::cout << "4. Exit\n";
    std::cin >> choice;

    switch (choice){
        case 1: {
            int id;
            std::string date;
            int category;
            double amount;
            std::string description;
            std::cout << "Enter ID: ";
            std::cin >> id;
            if (id <= 0 || std::cin.fail()) {
                std::cout << "Invalid ID. Please enter a positive integer." << std::endl;
                choice = 0;
                return 1; // Exit with an error code
            }
            std::cout << "Enter date: ";
            std::cin >> date;
            std::cout << "Enter category: ";
            std::cin >> category;
            std::cout << "Enter amount: ";
            std::cin >> amount;
            std::cout << "Enter description: ";
            std::cin >> description;
            Expenses expense;
            expense.addExpense(id, date, category, amount, description);
            expenses.push_back(expense);
            break;
        }
        case 2: {
            for (const auto& expense : expenses){
                exp.viewAllExpenses();
            }
            break;
        }
    }


    return 0;
}