// 15-745 S18 Assignment 2: dataflow.h
// Group:
////////////////////////////////////////////////////////////////////////////////

#ifndef __CLASSICAL_DATAFLOW_H__
#define __CLASSICAL_DATAFLOW_H__

#include <stdio.h>
#include <iostream>
#include <queue>
#include <vector>
#include <map>



#include "llvm/IR/Instructions.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/IR/ValueMap.h"
#include "llvm/IR/CFG.h"

namespace llvm {

// Add definitions (and code, depending on your strategy) for your dataflow
// abstraction here.


// Make c a class for Basic block information


class block_info{
    public:
        BitVector* input;
        BitVector* output;
        block_info(int instructions){
            input = new BitVector(instructions, false);
            output = new BitVector(instructions, false);
        }
};

class iterative_model{
    private:
        bool fwd;
        Function* func;
        std::map<BasicBlock*,block_info*> state;
        BitVector* (*transfer_function)(BasicBlock*); 
        BitVector* (*meet)(BasicBlock*);

    public:
        bool run_flow(bool direction, Function &p, BitVector* (*mt)(BasicBlock*) ,  BitVector* (*trns)(BasicBlock*));

};








}

#endif
