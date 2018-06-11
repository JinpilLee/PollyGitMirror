//===- SPDIRInfo.h - SPDIR generation ---------------------------*- C++ -*-===//
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

#ifndef POLLY_SPDIR_INFO
#define POLLY_SPDIR_INFO

#include "polly/ScopPass.h"
#include "polly/SPDIR.h"
#include <map>

namespace polly {
class SPDIRInfo : public ScopPass {
public:
  static char ID;
  SPDIRInfo() : ScopPass(ID) {}

  SPDIR *getIR(Scop *S) const;

  bool runOnScop(Scop &S) override;
  void getAnalysisUsage(AnalysisUsage &AU) const override;

private:
  std::map<Scop *, SPDIR *> IRTable;
};
} // namespace polly

namespace llvm {
void initializeSPDIRInfoPass(llvm::PassRegistry &);
} // namespace llvm

#endif // POLLY_SPDIR_INFO
