//===- SPDIRInfo.cpp - SPD code generation --------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This path generates SPDIR from SCoP/LLVM-IR.
//
//===----------------------------------------------------------------------===//

#include "polly/LinkAllPasses.h"
#include "polly/SPDIRInfo.h"

#define DEBUG_TYPE "polly-ast"

using namespace llvm;
using namespace polly;

SPDIR *SPDIRInfo::getIR(Scop *S) const {
  auto Iter = IRTable.find(S);
  if (Iter != IRTable.end()) {
    return Iter->second;
  }
  else {
    return nullptr;
  }
}

bool SPDIRInfo::runOnScop(Scop &S) {
  if (S.isToBeSkipped())
    return false;

  SPDIR *IR = new SPDIR(S);
  IRTable[&S] = IR;

  return false;
}

void SPDIRInfo::getAnalysisUsage(AnalysisUsage &AU) const {
  AU.addRequired<ScopInfoRegionPass>();
  AU.setPreservesAll();
}

char SPDIRInfo::ID = 0;

Pass *polly::createSPDIRInfoPass() {
  return new SPDIRInfo();
}

INITIALIZE_PASS_BEGIN(SPDIRInfo, "polly-spdirinfo",
                      "Polly - Generate SPDIR from SCoP/LLVM-IR",
                      false, false);
INITIALIZE_PASS_DEPENDENCY(ScopInfoRegionPass);
INITIALIZE_PASS_END(SPDIRInfo, "polly-spdirinfo",
                    "Polly - Generate SPDIR from SCoP/LLVM-IR",
                    false, false)
