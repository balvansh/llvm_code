// ECE 5984 S20 Assignment 3: loop_invariant_code_motion.cpp
// Group: nagasanjana, balvansh
////////////////////////////////////////////////////////////////////////////////

#include <vector>

#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/ValueTracking.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/LegacyPassManagers.h"

#include "dataflow.h"
#include "dominators.h"

using namespace std;

namespace llvm{

  class LICM : public LoopPass{
  public:
    static char ID;

    LICM() : LoopPass(ID) {}

    virtual void getAnalysisUsage(AnalysisUsage& AU) const {
      AU.addRequired<LoopInfoWrapperPass>();
    }

    bool checkInvariance(Loop* L, std::vector<Instruction*> invarantStats, Instruction* insn){

      if(!isSafeToSpeculativelyExecute(insn) || isa<LandingPadInst>(insn) || insn->mayReadFromMemory())
        return false;

        for (User::op_iterator OI = insn->op_begin(), OE = insn->op_end(); OI != OE; ++OI) {
  				Value *op = *OI;

  				if (Instruction* op_inst = dyn_cast<Instruction>(op)) {
  					if (L->contains(op_inst) && std::find(invarantStats.begin(), invarantStats.end(), op_inst) == invarantStats.end()) {
  						return false;
  					}
  				}
  			}

    }

    virtual bool runOnLoop(Loop *L, LPPassManager &no_use){

      bool modified = false;
      std::vector<void* > domain;

      //Ignoring loops without any pre-preheader
      BasicBlock* preheader = L->getLoopPreheader();
      if(!preheader)
        return false;

      LoopInfo& LI = getAnalysis<LoopInfoWrapperPass>().getLoopInfo();

      Function* F = L->getBlocks().front()->getParent();
        for (Function::iterator FI = F->begin(), FE = F->end(); FI != FE; ++FI) {
          BasicBlock* block = &*FI;
          domain.push_back(block);
        }


      std::map<BasicBlock*, BasicBlock*> domMap;
      DominatorsAnalysis doms;
      domMap = doms.computeDom(F, domain);
      doms.printIdom(domMap, L);

      DomTree *tree = doms.getDominatorsTree(domMap, L);

      std::vector<DomTreeNode*> worklist;
			worklist.push_back(tree->root);

			std::vector<Instruction*> invarantStats;

      while (!worklist.empty()) {
				DomTreeNode* n = worklist.back();
				BasicBlock* b = n->block;
				worklist.pop_back();

				if (LI.getLoopFor(b) != L) {
					return false;
				}

				// Iterate through all the intructions.
				for (BasicBlock::iterator II = b->begin(), IE = b->end(); II != IE; ++II) {
					Instruction* insn = &(*II);
					bool invariance = checkInvariance(L, invarantStats, insn);
					if (invariance) {
						invarantStats.push_back(insn);
					}
				}

				for (int i = 0; i < n->children.size(); ++i) {
					worklist.push_back(n->children[i]);
				}
			}


			// Checking conditions for code motion
			for (int j = 0; j < invarantStats.size(); ++j) {
				Instruction* inst = invarantStats[j];
				BasicBlock* b = inst->getParent();

				bool all_dominate = true;

				if (all_dominate) {
					Instruction* end = &(preheader->back());
					inst->moveBefore(end);
					if (!modified) {
						modified = true;
					}
				}
			}

      return modified;
    }
  };

  char LICM::ID = 0;
  RegisterPass<LICM> X("loop-invariant-code-motion",
    "ECE 5984 Loop Invariant Code Motion");
}
