// 15-745 S18 Assignment 2: available.cpp
// Group:
////////////////////////////////////////////////////////////////////////////////

#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/IR/Constants.h"
#include "llvm/Support/raw_ostream.h"
#include  <algorithm>
#include "dataflow.h"
#include "available-support.h"

using namespace llvm;
using namespace std;

namespace {
  vector<Expression> expressions;
  class AvailableExpressions : public FunctionPass {
  public:
    static char ID;
    AvailableExpressions() : FunctionPass(ID) { }
    virtual bool runOnFunction(Function& F) {
      // Here's some code to familarize you with the Expression
      // class and pretty printing code we've provided:
      for (Function::iterator FI = F.begin(), FE = F.end(); FI != FE; ++FI) {
	        BasicBlock* block = &*FI;
	        for (BasicBlock::iterator i = block->begin(), e = block->end(); i!=e; ++i) {
	            Instruction* I = &*i;
	  // We only care about available expressions for BinaryOperators
	            if (BinaryOperator *BI = dyn_cast<BinaryOperator>(I)) {
	    // Create a new Expression to capture the RHS of the BinaryOperator
	                  expressions.push_back(Expression(BI));
	            }
        	}
        }
      // Print out the expressions used in the function
      outs() << "Expressions used by this function:\n";
      printSet(&expressions);
      // Did not modify the incoming Function.
      return false;
    }
        virtual void getAnalysisUsage(AnalysisUsage& AU) const {
        AU.setPreservesAll();
    }
        private:
  };
     char AvailableExpressions::ID = 0;
        RegisterPass<AvailableExpressions> X("available","15745 Available Expressions");

    BitVector* transfer(BasicBlock* block, std::map<BasicBlock*,block_info*>&  state){
        *(state[block]->output) = *(state[block]->input);
        for (BasicBlock::iterator i = block->begin(), e = block->end(); i!=e; ++i) {
	            Instruction* I = &*i;
                Value* V = &*i;
	  // We only care about available expressions for BinaryOperators
	        if (dyn_cast<BinaryOperator>(I)) {
	    // Create a new Expression to capture the RHS of the BinaryOperator
             //   expressions.push_back(Expression(BI));
                int index = 0;
                Expression now = Expression(I);
                for(unsigned int i=0 ;i<expressions.size();i++){
                    if(expressions[i]== now){
                        index = i;
                    }
                }
                state[block]->output->set(index);
                string name = getShortValueName(V);
                for(unsigned int i=0;i<expressions.size();i++){
                    if(getShortValueName(expressions[i].v1)==name || getShortValueName(expressions[i].v2)==name){
                        state[block]->output->reset(i);
                    }
                }
            }
        }
        return state[block]->output;
    }








}
