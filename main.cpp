#include "StateMachine.hpp"

using namespace std;
/*
    You will only be creating the simulator for the following list of instructions:
    ADD     ADDI      AND      
    BEQZ    BNEZ      J       LB 
    LH      LW       
    OR      ORI       SLL     SRL  
    SB      SH        SW      SUB  
*/

int main(int argc, char* argv[]) {
    if(argc == 2 || argc == 3) {
        StateMachine s(argv[1]);
        s.start();
    } else cout << "Incorrect arguments" << endl;
    return 1;
}