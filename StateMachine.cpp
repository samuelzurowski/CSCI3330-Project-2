#include "StateMachine.hpp"

void StateMachine::start() {
    int i = 0;
    printInfo();
    for(auto val: memory)
        checkInfo();
}

void StateMachine::checkInfo() {
    stateOne();
    printInfo();
    stateTwo();
    printInfo();

    switch(state.opCode) {
        case 0: checkRType(); break; 
        case 2: /* TODO: J */ break;
        case 4: /* TODO: BEQZ*/ break;
        case 5: /* TODO: BNEZ*/ break;
        case 8: /* TODO: ADDI */break;
        case 13: /* TODO: ORI */ break;
        case 32: /* TODO: LB */ break;
        case 33: /* TODO: LH */ break;
        case 35: /* TODO: LW */ break;
        case 40: /* TODO: SB */ break;
        case 41: /* TODO: SH */ break;
        case 43: /* TODO: SW */ break;
        default:
            break;
    }
    printInfo();
}
void StateMachine::checkRType() {
    switch(func) {
        case 4:  rType(8);  break; // SLL
        case 6:  rType(10); break; // SRL
        case 32: rType(0); break; // ADD 
        case 34: rType(1); break; // SUB
        case 37: rType(5); break; // OR
        default: exit(0);
    }
}
void StateMachine::stateOne() {
    PCMARSelect(0);
    readMemory(1, 0);
    IRLoad();
}

void StateMachine::stateTwo() {
    incrementPC();
    ALoad(1);
    BLoad(1);
}

void StateMachine::incrementPC() {
    setPCoeS1(getPC());
    setS2OP(7);
    setAluOP(0);
    setPCLoad();
}

void StateMachine::setAoe(int val) {
    if(val) setS1(regFile[rs1]);
}

void StateMachine::setBoe(int val) {
    if(val) setS2(regFile[rs2]);
}
void StateMachine::rType(int aluCode) {
    setAoe(1);
    setBoe(1);
    setS2OP(0);
    setAluOP(aluCode);
    CLoad(1);
    printInfo();
    setREGSelect(0);
    switch(getREGSelect()) {
        case 0: regFile[rd]  = c; break;
        case 1: regFile[rs2] = c; break;
        case 2: regFile[31]  = c; break;
        default: exit(0); 
    }

}

void StateMachine::setAluOP(int op) {
    state.aluOP = op;
    switch(op) {
        case 0:  dest = s1 + s2;  break; // Add
        case 1:  dest = s1 - s2;  break; // Sub
        case 5:  dest = s1 | s2;  break; // OR
        case 8:  dest = s1 << s2; break; // SLL
        case 10: dest = s1 >> s2; break; // SRL
        default: exit(0);
    }
}

void StateMachine::setImm(string s){
    imm  = stoi(s.substr(16, 16), nullptr, 2);
    imm |= imm &  (1 << 15)  ? ( -(1<<16) ) : 0;
}

void StateMachine::printInfo() {
    cout << "\nRegFile: " << endl;
    for(int i = 0; i < 32; i++) cout << "r" << i << ": " << regFile[i] << ' ';

    cout << "\nMemory Register:\nMDR: " << mdr << " MAR: " << mar << endl;

    cout << "Hardware Registers:\nIR: " << getIR() << " PC: " << getPC() << endl; 

    cout << "Data Bus:\nS1: " << s1 << " S2: " << s2 << " DEST: " << dest 
         << " ADDR: " << addr << " DATA: " << data << endl;
    
    cout << "Register Buffers: " << endl;

    cout << "A: " << a;
    cout << " B: " << b;
    cout << " C: " << c << endl;

    // cout << "OP:  " << state.opCode << endl;
    // cout << "RS1: " << rs1          << endl;
    // cout << "RS2: " << rs2          << endl;
    // cout << "IMM: " << imm          << endl;
    // cout << "IR : " << getIR()      << endl;
    cout << "Press [Enter] to continue...";
    getchar();
}
void StateMachine::loadMemory() {
    ifstream ifs(fileName);
    if(!ifs.is_open()) {
        cout << "File not found aborting.";
        exit(0);
    }
    string s;
    while(getline(ifs, s)) {
        smatch sm;
        regex_search(s, sm, regex("x(.*)"));

        stringstream ss;
        ss << hex << sm.str(1);
        unsigned n;
        ss >> n;

        bitset<32> b(n);
        memory.push_back(b.to_string());
    } 
}
void StateMachine::setS2OP(int op) {
    switch(op) {
        case 0: break;
        case 6: s2 = 16; break;
        case 7: s2 = 4; break;
        default: exit(0);
    }
    state.s2OP = op;
}

void StateMachine::PCMARSelect(bool op) {
    if(op) addr = mar;
    else addr = state.pc;
}

void StateMachine::readMemory(bool read, int op) {
    if(read) {
        state.memRead = 1;
        data = getMemSize(addr, op);
    } else state.memRead = 0;
}
string StateMachine::getMemSize(int addr, int type) {
    string val;
    switch(type) {
        case 0: // word
            val = memory[addr / 4];
            break;
        case 1: // hw
            val = memory[addr / 4].substr(0, 16);
            break;
        case 2: // byte
            val = memory[addr / 4].substr(0, 8);
            break;
        default:
            return 0;
    }
    return val;
}

void StateMachine::IRLoad() {
    setIR(data);
    setOP(getIR());
    setRS1(getIR());
    setRS2(getIR());
    setRd(getIR());
    setOffset(getIR());
    setFunc(getIR());
    setImm(getIR());
}