#include <iostream>
#include <string>

struct Blue {
    std::string tok = "null";
    std::string reftok = "null";
};

Blue ref;

void signup(){
    
}

int main(void) {
    std::cout << "Ecliptix Blue V1\n";
    std::cout << "\nWhat do you want to do?\n[1] - Sign Up\n[2] - Log in\n[3...] - Exit";
    int ans;
    std::cin >> ans;

    switch (ans){
    case 1:
        signup();
    break;

    case 2:
    break;

    case 3:
    break;
    
    default:
        break;
    }

    return 0;
}
