// ECE 5984 S20 Assignment 3: dataflow.h
// Group: Naga Sanjana Bikonda, Balvansh Heerekar
////////////////////////////////////////////////////////////////////////////////

#ifndef __CLASSICAL_DATAFLOW_H__
#define __CLASSICAL_DATAFLOW_H__
#include "llvm/ADT/BitVector.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/PostOrderIterator.h"
#include "llvm/ADT/SmallSet.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/ValueMap.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "available-support.h"

#include <cstdio>
#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include <iomanip>


namespace llvm {
// Add definitions (and code, depending on your strategy) for your dataflow
// abstraction here.

  struct BBResult {
    BitVector inBB;
    BitVector outBB;

    BitVector outputValue;
    std::map<BasicBlock*, BitVector> neighbors;
  };

  typedef std::map<BasicBlock*, BBResult> result;

  class DataFlowAnalysis{
  public:

    DataFlowAnalysis(bool direction, bool meetOperator):direction(direction), meetOperator(meetOperator)
    {}

      BitVector MeetOperation(std::vector<BitVector> inputs);

      virtual BBResult transferFunction(BitVector input, BasicBlock* block, std::vector<void*> domain)=0;

      result run(Function &F, BitVector boundaryValues, BitVector initialValues, std::vector<void*> domain);

  private:
    /*direction: true - forward; false - backward
      meetOperator: true - union; false - intersection*/
    bool direction;
    bool meetOperator;
  };

  void printBitVector(BitVector b);
}

#endif
