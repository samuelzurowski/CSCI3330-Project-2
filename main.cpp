#include "StateMachine.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    if(argc == 3) {
        StateMachine s(argv[1], argv[2]);
        s.start();
    } else cout << "Incorrect arguments" << endl;
    return 1;
}