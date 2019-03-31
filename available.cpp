// 15-745 S18 Assignment 2: available.cpp
// Group:
////////////////////////////////////////////////////////////////////////////////

#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/IR/Constants.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/CFG.h"

#include "dataflow.h"
#include "available-support.h"

using namespace llvm;
using namespace std;

namespace {

  BitVector* meet(BasicBlock* block,std::map<BasicBlock*,block_info*>& state)
  {	std::vector<BitVector*>inputs;	
	std::BitVector* finalBitVector;
	for (auto pred_block =pred_begin(block),et=pred_end(block);pred_block!=et;++pred_block)
	{
		inputs.push_back(state[pred_block]->output);	
	
	}
	if (inputs.size()==0){
	return finalBitVector;		
	}
	
	finalBitVector=inputs[0];//intiliazing bit vector
	
	for (int i=0;i<inputs.size();i++){
	finalBitVector&=inputs[i];	
	}
	return finalBitVector;


  }
  class AvailableExpressions : public FunctionPass {
    
  public:
    static char ID;
    
    AvailableExpressions() : FunctionPass(ID) { }
    
    virtual bool runOnFunction(Function& F) {
      
      // Here's some code to familarize you with the Expression
      // class and pretty printing code we've provided:
      
      vector<Expression> expressions;
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
  RegisterPass<AvailableExpressions> X("available",
				       "15745 Available Expressions");
}
