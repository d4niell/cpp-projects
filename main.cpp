#include <iostream>

class Calculator {
    public:
        bool add(int a, int b);
        bool subtract(int a, int b);
        bool multiply(int a, int b);
        bool divide(int a, int b);
    int a, b;

};

bool Calculator::add(int a, int b) { return a + b; }
bool Calculator::subtract(int a, int b) { return a - b; }
bool Calculator::multiply(int a, int b) { return a * b; }
bool Calculator::divide(int a, int b) { return a / b; }
int main() {
    Calculator calc;
    std::cout << "enter number 1:";
    std::cin >> calc.a;
    std::cout << "enter number 2:";
    std::cin >> calc.b;
    std::cout << "sum: " << calc.add(calc.a, calc.b) << std::endl;
    std::cout << "difference: " << calc.subtract(calc.a, calc.b) << std::endl;
    std::cout << "product: " << calc.multiply(calc.a, calc.b) << std::endl;
    std::cout << "quotient: " << calc.divide(calc.a, calc.b) << std::endl;
    return 0;
}