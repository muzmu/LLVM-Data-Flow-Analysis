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
    std::set<std::string> Variable;
    std::vector<std::string> Variables;

    BitVector* transfer(BasicBlock* block, std::map<BasicBlock*,block_info*>&  state);
    BitVector* meet(BasicBlock* block,std::map<BasicBlock*,block_info*>& state)
    {	std::vector<BitVector*> output;	
        BitVector* finalBitVector;
        BitVector* check;
        for (succ_iterator sit = succ_begin(block), set = succ_end(block); sit != set; ++sit){ //enlist all pred
            BasicBlock* b = *sit;
            if(b)
            {   check = new BitVector(*state[b]->input);
              //  *state[b]->input;
                for(auto I = b->begin();I!= b->end(); ++I){
                    if(PHINode* p = dyn_cast<PHINode>(I)){
                        for(unsigned int v = 0;v<p->getNumIncomingValues();++v){
                            if(p->getIncomingBlock(v) != block){
                                for(unsigned int s=0;s<Variables.size();s++){
                                    if(Variables[s] == getShortValueName(p->getIncomingValue(v))){
                                        check->reset(s);
                                    }
                                }
                            }
                        }
                    }
                }

                output.push_back(check);
            }
        }
        /* for (auto pred_block =pred_begin(block),et=pred_end(block);pred_block!=et;++pred_block)
           {
           output.push_back(state[*pred_block]->output);		
           }*/
        if (output.size()==0){
            return state[block]->output;		
        }
        finalBitVector=output[0];	
        for (std::size_t i=0,max=output.size();i!=max;++i){
            *(finalBitVector)|=*output[i];	
        }

        return finalBitVector;
    }

    
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
                errs()<<"\n \n \n";
                iterative_model avail(false,F,&meet,&transfer,Variables.size());
                avail.run_analysis();
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
            for(unsigned int j=0;j<Variables.size();j++){
                if(getShortValueName(V)==Variables[j]){
                    state[block]->input->reset(j);
                } 
            }
            for(auto op = I->op_begin(),e_op=I->op_end();op!=e_op;++op){
                Value* v = *op; 
                for(unsigned int j=0;j<Variables.size();j++){
                    if(getShortValueName(v)==Variables[j]){
                        state[block]->input->set(j);
                    }
                }
            }
            for(int LI=state[block]->input->find_first();LI>=0;LI=state[block]->input->find_next(LI)){
                errs()<<Variables[LI] << " ";
            }    
            errs()<< "------"<<  *I << "\n \n \n";

        }
        return state[block]->input;
    }

}
