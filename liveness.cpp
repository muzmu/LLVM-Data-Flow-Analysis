// 15-745 S18 Assignment 2: liveness.cpp
// Group:
////////////////////////////////////////////////////////////////////////////////

#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "dataflow.h"
#include "available-support.h"
#include <string>
#include <set>
using namespace llvm;

namespace {

    BitVector* transfer(BasicBlock* block, std::map<BasicBlock*,block_info*>&  state);
    BitVector* meet(BasicBlock* block,std::map<BasicBlock*,block_info*>& state)
    {	std::vector<BitVector*> output;	
        BitVector* finalBitVector;
        for (succ_iterator sit = succ_begin(block), set = succ_end(block); sit != set; ++sit){ //enlist all pred
            BasicBlock* b = *sit;
            if(b)
            {
                output.push_back(state[b]->input);
            }
        }
        /* for (auto pred_block =pred_begin(block),et=pred_end(block);pred_block!=et;++pred_block)
           {
           output.push_back(state[*pred_block]->output);		
           }*/
        if (output.size()==0){
            errs()<<"No predecssor"<<"\n";
            return state[block]->output;		
        }	
        finalBitVector=output[0];	
        for (std::size_t i=0,max=output.size();i!=max;++i){
            *(finalBitVector)|=*(output[i]);	
        }
        return finalBitVector;
    }

    std::set<std::string> Variable;
    std::vector<std::string> Variables;

    class Liveness : public FunctionPass {
        public:
            static char ID;
            Liveness() : FunctionPass(ID) { }
            virtual bool runOnFunction(Function& F) {
                for(auto FI=F.begin(),FE=F.end();FI!=FE;++FI){
                   BasicBlock* b=&*FI;
                   for(auto i=b->begin(),e=b->end();i!=e;++i){
                        Instruction* I=&*i;
                        for(auto op = I->op_begin(),e_op=I->op_end();op!=e_op;++op){
                            Value* v = *op;
                            if(isa<Instruction>(v)||isa<Argument>(v)){
                                Variable.insert(getShortValueName(v));
                            }
                        }
                   }
                   
                }
                // Did not modify the incoming Function.
                for(auto i=Variable.begin();i!=Variable.end();i++){
                    Variables.push_back(*i);
                    errs()<<*i << " ";
                }
                errs()<<"\n";
                return false;
            }
            virtual void getAnalysisUsage(AnalysisUsage& AU) const {
                AU.setPreservesAll();
            }   
        private:
    };

    char Liveness::ID = 0;
    RegisterPass<Liveness> X("liveness", "15745 Liveness");


    BitVector* transfer(BasicBlock* block, std::map<BasicBlock*,block_info*>& state){
        *(state[block]->input)=*(state[block]->output);
        for(auto i = block->rbegin(),e=block->rend();i!=e;++i){
            Instruction* I = &*i;
            Value* V = &*i;
            if(PHINode* phi_i=dyn_cast<PHINode>(I)){
               for(auto op = I->op_begin(),e_op=I->op_end();op!=e_op;++op){
                    Value* v = *op; 
                                      
               }
            }
        }
    }

}
