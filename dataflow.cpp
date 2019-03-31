// 15-745 S18 Assignment 2: aataflow.cpp
// Group:
////////////////////////////////////////////////////////////////////////////////

#include "dataflow.h"

namespace llvm {
   iterative_model::iterative_model(bool direction, Function &p, BitVector* (*mt)(BasicBlock*, std::map<BasicBlock*, block_info*>&) ,  BitVector* (*trns)(BasicBlock*,std::map<BasicBlock*, block_info*>&),int number){
    fwd = direction;
    func = &p;
    meet = mt;
    transfer_function = trns;
       for(Function::iterator FI=func->begin(), FE=func->end(); FI!=FE; ++FI){
            BasicBlock* block = &*FI;
            block_info* info = new block_info(number);
            state[block] = info;
        }
    }


BitVector* iterative_model::run_analysis(){
    bool converged = false;
    if(fwd){
        BasicBlock* starting_point=&*func->begin();
        BitVector* out2;
        BitVector* out= state[starting_point]->input;
        while(!converged){
            for(Function::iterator FI=func->begin(), FE=func->end(); FI!=FE; ++FI){
                converged = false;
                BasicBlock* block = &*FI;
                state[block]->input = meet(block,state);
                out2 = transfer_function(block,state);
                if(out == out2){
                    converged = true;
                }
                state[block]->output=out2;
            }
            out = out2;
        }

    }else{}
        BitVector* toreturn = new BitVector(1);
        return toreturn;
   }
    // Add code for your dataflow abstraction here.
}




