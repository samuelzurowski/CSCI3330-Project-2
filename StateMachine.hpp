#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <map>
#include <sstream>
#include <math.h>
#include <fstream>
#include "State.hpp"
using namespace std;

class StateMachine {
    private:
        State state;
        string fileName;
        int addr = 0, dest = 0;
        long s1 = 0, s2 = 0;
        int rs1 = 0, rs2 = 0, rd = 0, func = 0, offset = 0, imm = 0, mar = 0, mdr = 0;
        int a = 0, b = 0, c = 0;
        vector<int> regFile {
            0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,
            18,19,20,21,22,23,24,25,26,27,28,29,30,31
        };
        string data;
        vector<string> memory;
    public:   
        StateMachine(string fileName): fileName(fileName){ loadMemory(); }

        void loadMemory();
        void stateOne();
        void stateTwo();
        void printInfo();

        void setAluOP(int op);
        int getAluOP() { return state.aluOP; }

        void setMemOP(int op) { state.memOP = op; }
        int getMemOP() { return state.memOP; }

        void setS2OP(int op);
        int getS2OP() { return state.s2OP; }

        void setREGSelect(int op) { state.regSelect = op; }
        int getREGSelect() { return state.regSelect; }
        void setPCLoad() { state.pc = dest; }

        void readMemory(bool read, int op);
        void PCMARSelect(bool op);
        string getMemSize(int addr, int type);

        void IRLoad();
        string getIR() { return state.ir; }
        void setIR(string s)  { state.ir = s; }

        void setOP(string s)  { state.opCode = stoi(s.substr(0, 6), nullptr, 2); }
        void setRS1(string s) { rs1 = stoi(s.substr(6, 5), nullptr, 2); }
        void setRS2(string s) { rs2 = stoi(s.substr(11, 5), nullptr, 2); }
        
        void setImm(string s);

        void setRd(string s)    { rd     = stoi(s.substr(16, 5), nullptr, 2); }
        void setFunc(string s)  { func   = stoi(s.substr(21, 11), nullptr, 2); }
        void setOffset(string s){ offset = stoi(s.substr(6, 26), nullptr, 2); }

        void setAoe(int val);
        void setBoe(int val);

        void setS1(long val) { s1 = val; }
        void setS2(long val) { s2 = val; }


        void incrementPC();
        int getPC() { return state.pc; }
        void setPCoeS1(int pc){ s1 = pc; } 

        void ALoad(bool val) { if(val) a = regFile[rs1]; }
        void BLoad(bool val) { if(val) b = regFile[rs2]; }
        void CLoad(bool val) { if(val) c = dest; }


        void iroeS1(bool val) { if(val) s1 = stol(getIR(), nullptr, 2); }
        void iroeS2(bool val){ if(val)  s2 = stol(getIR(), nullptr, 2); }

        void updateReg(int reg);
        void rType(int op);
        void checkRType();
        void checkInfo();
        void marLoad(bool op);
        void mdrLoad(bool op);

        void mdroeS2(bool op);

        void loadInstr(int op);
        void start();
};