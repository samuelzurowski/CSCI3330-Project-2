#include <iostream>
#pragma once

using namespace std;

struct State {
    int s2OP;
    int aluOP;
    int pc = 0;
    string ir;
    int zFlag;
    int cLoad;
    int regLoad;
    int aLoad;
    int bLoad;
    int boe;
    int aoe;
    int regSelect;
    int irLoad;
    int iroeS1;
    int iroeS2;
    int opCode;
    int pcLoad;
    int pcoeS1;
    int PCMARSelect;
    int marLoad;
    int memRead;
    int mdrLoad;
    int mdroeS2;
    int memwrite;
    int memOP;
    int reset;
};
