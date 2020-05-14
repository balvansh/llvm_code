// ECE 5984 S20 Assignment 3: dominators.h
// Group: nagasanjana, balvansh
////////////////////////////////////////////////////////////////////////////////

#ifndef __DOMINANCE_H
#define __DOMINANCE_H

#include <set>
#include <queue>
#include <vector>

#include "llvm/ADT/DenseMap.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/Constants.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/LoopPass.h"

#include "dataflow.h"

using namespace std;

namespace llvm {

  class DomTreeNode {
    public:
      std::vector<DomTreeNode*> children;
      DomTreeNode* parent;
      BasicBlock* block;
      DomTreeNode(BasicBlock* b, DomTreeNode* p) { block = b; parent = p; }
  };

  // Dominance Tree
  class DomTree {
    public:
      std::vector<DomTreeNode*> nodes;
      DomTreeNode* root;
      DomTree() {}
  };


  class DominatorsAnalysis : public DataFlowAnalysis {
    public:
    DominatorsAnalysis() : DataFlowAnalysis(false, false) {     }
    DominatorsAnalysis(bool direction, bool meet_op) : DataFlowAnalysis(direction, meet_op){}
    void printbitVector(BitVector bitvector) {
      for (unsigned i = 0; i < bitvector.size(); i++) {
        if (bitvector[i])
          errs() << "1 ";
        else
          errs() << "0 ";
      }
      errs() << "\n";
    }

    BBResult transferFunction(BitVector input, BasicBlock * BB, std::vector<void*> domain) {
      BBResult bbresult;
      std::map<int, void*> domainToIndex;
      int domain_size = domain.size();
      BitVector GenSet(domain_size);
        for (int i = 0; i < domain.size(); i++){
          //errs()<<"debug: "<<domain[i]<<"\n";
          domainToIndex[i] = domain[i];
        }
      for (auto it = domainToIndex.begin(); it != domainToIndex.end(); ++it){
        if (it->second == BB)
          GenSet.set(it->first);
      }

      bbresult.outputValue = input;
      bbresult.outputValue |= GenSet;

      // errs() << "Gen[BB] : ";
      // printbitVector(GenSet);
      // errs() << "In[BB] : ";
      // printbitVector(input);
      // errs() << "Out[BB] : ";
      // printbitVector(bbresult.outputValue);

      return bbresult;

    }
    std::map<BasicBlock*, BasicBlock*> computeDom(Function* F, vector<void *> domain){

      result output;
      std::map<int, void*> domainToIndex;
      Function::BasicBlockListType &BBs = F->getBasicBlockList();

      for (int i = 0; i < domain.size(); i++){
        domainToIndex[i] = domain[i];
      }

      BitVector boundCond(domain.size(), false);
      BitVector initCond(domain.size(), true);



      std::map<BasicBlock*, BasicBlock*>domMap;

      output = run(*F, boundCond, initCond, domain);

      for(int block2 = 0; block2 < domain.size(); ++block2 ){

        BasicBlock* B = (BasicBlock*)domain[block2];

        for(int block1 = 0; block1 < domain.size(); ++block1){

          if(block1 == block2){
            continue;
          }
          BasicBlock* A = (BasicBlock*)domain[block1];

          if(output[B].inBB[block1] == false)
            continue;

          bool immediateDOM = true;

          for(int block3=0; block3 < domain.size(); ++block3){

            if (block3 == block1 || block3 == block2)
                continue;
            BasicBlock* C = (BasicBlock*)domain[block3];
            if (output[C].inBB[block1] && output[B].inBB[block3]) {
                immediateDOM = false;
                break;
            }
          }

          if (immediateDOM) {
              domMap[B] = A;
          }
        }

      }

      return domMap;

    }

    void printIdom(std::map<BasicBlock*, BasicBlock*> domMap, Loop* L) {
        std::vector<BasicBlock*> BB = L->getBlocks();
        errs() << "\n\nNew Loop\n";
        for (int i=0; i < BB.size(); ++i) {
            errs() << domMap[BB[i]]->getName() << " idom " << BB[i]->getName() << "\n";
        }
        return;
    }

    DomTree* getDominatorsTree(std::map<BasicBlock*, BasicBlock*> idom, Loop* L) {
    	DomTree* tree = new DomTree();
 		std::vector<BasicBlock*> blocks = L->getBlocks();
 		std::map<BasicBlock*,DomTreeNode*> lookup;
 		for(std::vector<BasicBlock*>::iterator BI = blocks.begin(), BE = blocks.end(); BI != BE; ++BI) {
 			BasicBlock* b = *BI;
 			BasicBlock* p;
 			if (BI==blocks.begin())	{ // Header
 				p = NULL;
 			}
 			else {
 				p = idom[b];
 			}

 			// Create a node for this if it doesn't exist already.
 			DomTreeNode* n;
 			if (lookup.find(b) != lookup.end()) {
 				n = lookup[b];
 			}
 			else {
 				n = new DomTreeNode(b,NULL);
	 			lookup[b] = n;
 			}

 			// Same for the parent
 			DomTreeNode* pn;
 			if (lookup.find(p) != lookup.end()) {
 				pn = lookup[p];
 			}
 			else {
 				pn = new DomTreeNode(p,NULL);
	 			lookup[p] = pn;
 			}

			n->parent = pn;				// Add parent to this node
			pn->children.push_back(n);	// Add this node to the children of the parent
 			tree->nodes.push_back(n);

 		}

 		tree->root = lookup[L->getBlocks().front()];
 		return tree;
    }

  };

}
#endif
