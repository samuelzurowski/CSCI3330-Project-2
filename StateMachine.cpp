#include "StateMachine.hpp"

/**
 * @function start state machine
 * @desc starts doing each step.
 */
void StateMachine::start() {
    printInfo();
    for(auto val: memory) // update this later on.
        checkInfo();
}

/**
 * @function checkInfo
 * @desc perform all logic to perform the state machine
 */
void StateMachine::checkInfo() {
    stateOne();
    printInfo();
    stateTwo();
    printInfo();
    switch(state.opCode) {
        case 0: checkRType();   break; // rTypes 
        case 2: jump();         break; // J verify this
        case 4: branchInstr(2); /* check NUM */ break; // BEQZ
        case 5: branchInstr(2); break; // BNEZ
        case 8:  iType(0);      break; // ADDI verify iType
        case 13: iType(5);      break; // ORI
        case 32: loadInstr(2);  break; // LB
        case 33: loadInstr(1);  break; // LH
        case 35: loadInstr(0);  break; // LW
        case 40: storeInstr(2); break; // SB verify these 3
        case 41: storeInstr(1); break; // SH
        case 43: storeInstr(0); break; // SW
        default: break;
    }
    printInfo();
}
/**
 * @function Branch instruction state
 * @desc branch instruction for the state machine
 * @param op for alu
 */

// TODO: ZFlag?
void StateMachine::branchInstr(int op) {
    setAoe(1);
    setS2OP(0);
    setAluOP(op);
}
/**
 * @function Jump to specific instruction
 * @desc take the offset and update pc value.
 */
void StateMachine::jump() {
    setPCoeS1(getPC());
    iroeS2(1);
    setS2OP(5); // verify this
    setAluOP(0); // add s1 + s2
    setPCLoad();

}

/**
 * @function determine the rtype of instruction
 * @desc based on the rtype perform operands
 */
void StateMachine::checkRType() {
    switch(func) {
        case 4:  rType(8);  break; // SLL
        case 6:  rType(10); break; // SRL
        case 32: rType(0);  break; // ADD 
        case 34: rType(1);  break; // SUB
        case 37: rType(5);  break; // OR
        default: exit(0);
    }
}

void StateMachine::iType(int op) {
    setAoe(1);
    iroeS2(1);
    setS2OP(3);
    setAluOP(op);
    CLoad(1);
    printInfo();

    setREGSelect(1);
    updateReg(getREGSelect());
}

/**
 * @function stateone of every instr
 * @desc put into IR the value in memory
 */
void StateMachine::stateOne() {
    PCMARSelect(0);
    readMemory(1, 0);
    IRLoad();
}

/**
 * @function stateTwo for every instruction
 * @desc increment the pc and load rs1 and rs2 into a & b
 */
void StateMachine::stateTwo() {
    incrementPC();
    ALoad(1);
    BLoad(1);
}

/**
 * @function increment the value of the pc
 * @desc pc = pc + 4
 */
void StateMachine::incrementPC() {
    setPCoeS1(getPC());
    setS2OP(7);
    setAluOP(0);
    setPCLoad();
}
/**
 * @function set the a value onto the bus or not
 * @desc set the s1 bus to a's value
 * @param val if it can set the s1 value
 */ 
void StateMachine::setAoe(bool val) {
    if(val) setS1(regFile[rs1]);
}

/**
 * @function set the b value onto the bus or not
 * @desc set the s2 bus to b's value
 * @param val if it can set the s2 value
 */ 
void StateMachine::setBoe(bool val) {
    if(val) setS2(regFile[rs2]);
}
/**
 * @function rType arthimetic for all the instructions
 * @desc do the state machine for all rType instructions
 * @param alucode to be able to perform arithmetic
 */ 
void StateMachine::rType(int aluCode) {
    setAoe(1);
    setBoe(1);
    setS2OP(0);
    setAluOP(aluCode);
    CLoad(1);
    printInfo();

    setREGSelect(0);
    updateReg(getREGSelect());
}

/**
 * @function update the register file
 * @desc takes the operand to determine the reg to put it in
 * @param reg to determine which case it is.
 */ 
void StateMachine::updateReg(long reg) {
    switch(reg) {
        case 0: regFile[rd]  = c; break;
        case 1: regFile[rs2] = c; break;
        case 2: regFile[31]  = c; break;
        default: exit(0); 
    } 
}

/**
 * @function set ALU op to perform math
 * @desc based on alu puts result on dest bus
 * @param op choice
 */ 
void StateMachine::setAluOP(int op) {
    state.aluOP = op;
    switch(op) {
        case 0:  dest = s1 + s2;  break; // Add
        case 1:  dest = s1 - s2;  break; // Sub
        case 3:  dest = s2;       break; // pass s2
        case 5:  dest = s1 | s2;  break; // OR
        case 8:  dest = s1 << s2; break; // SLL
        case 10: dest = s1 >> s2; break; // SRL
        default: exit(0);
    }
}

/**
 * @function set Immediate
 * @desc makes the immediate signed by checking first bit
 * @param string to get immediate from
 */ 
void StateMachine::setImm(string s){
    imm  = stol(s.substr(16, 16), nullptr, 2);
    imm |= imm &  (1 << 15) ? (-(1<<16)) : 0;
}

/**
 * @function set Offset
 * @desc makes the offset signed by checking first bit
 * @param string to get immediate from
 */ 
void StateMachine::setOffset(string s) {
    offset = stol(s.substr(6, 26), nullptr, 2);
    offset |= offset & (1 << 25) ? (-(1<<26)) : 0;
}

/**
 * @function print info about data
 * @desc prints information such as registers, mar, etc
 */ 
void StateMachine::printInfo() {
    cout << "\nRegFile: " << endl;
    for(int i = 0; i < 32; i++) {
        cout << "r" << i << ": " << regFile[i] << ' ';
    }
    //remove later on
    cout << "\nMemory: " << endl;
    for(int i = 0; i < memory.size(); i++)
        cout << i << ' ' << memory[i] << '\n';

    cout << "\nMemory Register:\nMDR: " << mdr << " MAR: " << mar << endl;

    cout << "Hardware Registers:\nIR: " << getIR() << " PC: " << getPC() << endl; 

    cout << "Data Bus:\nS1: " << s1 << " S2: " << s2 << " DEST: " << dest 
         << " ADDR: " << addr << " DATA: " << data << endl;
    
    cout << "Register Buffers: " << endl;

    cout << "A: " << a;
    cout << " B: " << b;
    cout << " C: " << c << endl;

    cout << "Press [Enter] to continue...";
    // getchar();
}

/**
 * @function load memory into program
 * @desc takes the hexadecimal values and puts them in binary into memory
 */ 
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


/**
 * @function set the s2 op
 * @desc put information on the s2 bus
 * @param int op determine what is on the bus
 */ 
void StateMachine::setS2OP(int op) {
    switch(op) {
        case 0: break;
        case 3: s2 = imm; break;
        case 5: s2 = offset; break;
        case 6: s2 = 16; break;
        case 7: s2 = 4; break;
        default: exit(0);
    }
    state.s2OP = op;
}

/**
 * @function pc or mar select
 * @desc based on selection put information on address bus
 * @param op determine if PC or MAR onto the bus
 */ 
void StateMachine::PCMARSelect(bool op) {
    if(op) addr = mar;
    else addr = state.pc;
}

/**
 * @function read from memory
 * @desc put the data on the bus
 * @param bool read if can read
 * @param op for if halfword, word, or byte
 */ 
void StateMachine::readMemory(bool read, int op) {
    if(read) {
        state.memRead = 1;
        data = getMemSize(addr, op);
    } else state.memRead = 0;
}

/**
 * @function get the memory size
 * @desc pull from memory the binary string
 * @param addr for the memory location
 * @param type for if its word, halfword, or byte
 * @return the value pulled from memory
 */ 
string StateMachine::getMemSize(long addr, int type) {
    string val;
    double dec = 0;
    switch(type) {
        case 0: // word = 32 bits
            val = memory[addr / 4];
            break;
        case 1: // halfword = 16 bits
            if(addr % 4 == 0) 
                val = memory[addr / 4].substr(16, 16);
            else 
                val = memory[floor(addr / 4)].substr(0, 16);
            break;
        case 2: // byte = 8 bits
            dec = addr / 4;
            if(dec == 0) 
                val = memory[floor(dec)].substr(24, 8);
            else if(dec == .25) 
                val = memory[floor(dec)].substr(16, 8);
            else if(dec == .5)
                val = memory[floor(dec)].substr(8, 8);
            else  
                val = memory[floor(dec)].substr(0, 8);
            break;
        default:
            return 0;
    }
    return val;
}

/**
 * @function load instruction
 * @desc pull from memory the instruction into register file
 * @param opcode to determine if its a halfword, word, or byte
 */
void StateMachine::loadInstr(int op) {
    setAoe(1);
    iroeS2(1);
    setS2OP(3);
    setAluOP(0);
    marLoad(1);
    printInfo();

    PCMARSelect(1);
    readMemory(addr, op);
    mdrLoad(1);
    printInfo();

    mdroeS2(1);
    setS2OP(0);
    setAluOP(3);
    CLoad(1);
    printInfo();

    setREGSelect(1);
    updateReg(getREGSelect());
    printInfo();
}

/**
 * @function store instruction
 * @desc based on type word,halfword, or byte
 * @param op for determining size
 */
void StateMachine::storeInstr(int op) {
    setAoe(1);
    iroeS2(1);
    setS2OP(3);
    setAluOP(0);
    marLoad(1);
    printInfo();

    setBoe(1);
    setS2OP(0);
    setAluOP(3);
    mdrLoad(1);
    printInfo();

    setMemOP(op);
    PCMARSelect(1);
    memWrite(1);
}

/**
 * @function memory
 * @desc write to memory the data based on memop
 * @param boolean to determine if you can write to memory
 */
void StateMachine::memWrite(bool write) {
    if(write) {
        data = bitset<32>(mdr).to_string();
        cout << "\n\n" << data << endl;
        switch(getMemOP()) {
            // fix case num just testing!
            case 0: memory[mar] = data;  break; // word = 32 bits
            case 1: memory[mar] = data.substr(16, 16); break; // hw = 16 bits
            case 2: memory[mar] = data.substr(24, 8); break; // 1 byte = 8bits
            default: break;
        }
    }
}

/**
 * @function mdroes2
 * @desc put mdr on s2 bus
 * @param boolean if s2 gets mdr
 */
void StateMachine::mdroeS2(bool val) {
    if(val) s2 = mdr;
}

/**
 * @function load into mdr
 * @desc puts the data into mdr
 * @param boolean if mdr gets data bus
 */
void StateMachine::mdrLoad(bool val) {
    if(val) mdr = stol(data, nullptr, 2);
}

/**
 * @function load into mar
 * @desc puts the dest into mar
 * @param boolean if mar gets dest bus
 */
void StateMachine::marLoad(bool val) {
    if(val) mar = dest;
}

/**
 * @function load data into ir
 * @desc gets all info such as imm func opcode and all import information
 */
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