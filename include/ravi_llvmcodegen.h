/******************************************************************************
* Copyright (C) 2015 Dibyendu Majumdar
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
******************************************************************************/

#ifndef RAVI_LLVMCODEGEN_H
#define RAVI_LLVMCODEGEN_H

#ifdef USE_LLVM

#include "ravijit.h"
#include "ravi_llvm.h"

#ifdef __cplusplus
extern "C" {
#endif

// TODO we probably do not need all the headers
// below

#define LUA_CORE

#include "lprefix.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lua.h"

#include "ldebug.h"
#include "ldo.h"
#include "lfunc.h"
#include "lobject.h"
#include "lopcodes.h"
#include "lstate.h"
#include "lstring.h"
#include "ltable.h"
#include "lvm.h"

#ifdef __cplusplus
}
#endif

#include <array>
#include <iterator>
#include <type_traits>
#include <atomic>

namespace ravi {

enum LuaTypeCode {
  LUA__TNIL = LUA_TNIL,
  LUA__TBOOLEAN = LUA_TBOOLEAN,
  LUA__TLIGHTUSERDATA = LUA_TLIGHTUSERDATA,
  LUA__TNUMBER = LUA_TNUMBER,
  LUA__TSTRING = ctb(LUA_TSTRING),
  LUA__TTABLE = ctb(LUA_TTABLE),
  LUA__TFUNCTION = ctb(LUA_TFUNCTION),
  LUA__TUSERDATA = ctb(LUA_TUSERDATA),
  LUA__TTHREAD = ctb(LUA_TTHREAD),
  LUA__TLCL = ctb(LUA_TLCL),
  LUA__TLCF = LUA_TLCF,
  LUA__TCCL = ctb(LUA_TCCL),
  LUA__TSHRSTR = ctb(LUA_TSHRSTR),
  LUA__TLNGSTR = ctb(LUA_TLNGSTR),
  LUA__TNUMFLT = LUA_TNUMFLT,
  LUA__TNUMINT = LUA_TNUMINT
};

// All Lua types are gathered here
struct LuaLLVMTypes {

  LuaLLVMTypes(llvm::LLVMContext &context);
  void dump();

  llvm::Type *C_voidT;

  llvm::Type *C_doubleT;
  llvm::Type *C_intptr_t;
  llvm::Type *C_size_t;
  llvm::PointerType *C_psize_t;
  llvm::Type *C_ptrdiff_t;
  llvm::Type *C_int64_t;
  llvm::Type *C_shortT;

  llvm::Type *lua_NumberT;
  llvm::PointerType *plua_NumberT;
  llvm::PointerType *pplua_NumberT;

  llvm::Type *lua_IntegerT;
  llvm::PointerType *plua_IntegerT;
  llvm::PointerType *pplua_IntegerT;

  llvm::Type *lua_UnsignedT;
  llvm::Type *lua_KContextT;

  llvm::FunctionType *lua_CFunctionT;
  llvm::PointerType *plua_CFunctionT;

  llvm::FunctionType *lua_KFunctionT;
  llvm::PointerType *plua_KFunctionT;

  llvm::FunctionType *lua_HookT;
  llvm::PointerType *plua_HookT;

  llvm::FunctionType *lua_AllocT;
  llvm::PointerType *plua_AllocT;

  llvm::Type *l_memT;
  llvm::Type *lu_memT;

  llvm::Type *tmsT;

  llvm::Type *lu_byteT;
  llvm::Type *L_UmaxalignT;
  llvm::PointerType *C_pcharT;

  llvm::Type *C_intT;
  llvm::PointerType *C_pintT;

  llvm::StructType *lua_StateT;
  llvm::PointerType *plua_StateT;

  llvm::StructType *global_StateT;
  llvm::PointerType *pglobal_StateT;

  llvm::StructType *ravi_StateT;
  llvm::PointerType *pravi_StateT;

  llvm::StructType *GCObjectT;
  llvm::PointerType *pGCObjectT;

  llvm::StructType *ValueT;
  llvm::StructType *TValueT;
  llvm::PointerType *pTValueT;
  llvm::StructType *TStringT;
  llvm::PointerType *pTStringT;
  llvm::PointerType *ppTStringT;

  llvm::StructType *UdataT;
  llvm::StructType *RaviArrayT;
  llvm::StructType *TableT;
  llvm::PointerType *pTableT;
  llvm::PointerType *ppTableT;

  llvm::StructType *UpvaldescT;
  llvm::PointerType *pUpvaldescT;

  llvm::Type *ravitype_tT;
  llvm::StructType *LocVarT;
  llvm::PointerType *pLocVarT;

  llvm::Type *InstructionT;
  llvm::PointerType *pInstructionT;
  llvm::StructType *LClosureT;
  llvm::PointerType *pLClosureT;
  llvm::PointerType *ppLClosureT;
  llvm::PointerType *pppLClosureT;

  llvm::StructType *RaviJITProtoT;

  llvm::StructType *ProtoT;
  llvm::PointerType *pProtoT;
  llvm::PointerType *ppProtoT;

  llvm::StructType *UpValT;
  llvm::PointerType *pUpValT;

  llvm::StructType *CClosureT;
  llvm::PointerType *pCClosureT;

  llvm::StructType *TKeyT;
  llvm::PointerType *pTKeyT;

  llvm::StructType *NodeT;
  llvm::PointerType *pNodeT;

  llvm::StructType *lua_DebugT;
  llvm::PointerType *plua_DebugT;

  llvm::StructType *lua_longjumpT;
  llvm::PointerType *plua_longjumpT;

  llvm::StructType *MbufferT;
  llvm::StructType *stringtableT;

  llvm::PointerType *StkIdT;

  llvm::StructType *CallInfoT;
  llvm::StructType *CallInfo_cT;
  llvm::StructType *CallInfo_lT;
  llvm::PointerType *pCallInfoT;

  llvm::FunctionType *jitFunctionT;

  llvm::FunctionType *luaD_poscallT;
  llvm::FunctionType *luaD_precallT;
  llvm::FunctionType *luaD_callT;
  llvm::FunctionType *luaF_closeT;
  llvm::FunctionType *luaT_trybinTMT;
  llvm::FunctionType *luaG_runerrorT;
  llvm::FunctionType *luaV_equalobjT;
  llvm::FunctionType *luaV_lessthanT;
  llvm::FunctionType *luaV_lessequalT;
  llvm::FunctionType *luaV_forlimitT;
  llvm::FunctionType *luaV_tonumberT;
  llvm::FunctionType *luaV_tointegerT;
  llvm::FunctionType *luaV_modT;
  llvm::FunctionType *luaV_objlenT;
  llvm::FunctionType *luaV_divT;
  llvm::FunctionType *luaC_upvalbarrierT;

  llvm::FunctionType *luaV_executeT;
  llvm::FunctionType *luaV_gettableT;
  llvm::FunctionType *luaV_settableT;

  // Following are functions that handle specific bytecodes
  // We cheat for these bytecodes by calling the function that
  // implements it
  llvm::FunctionType *raviV_op_newarrayintT;
  llvm::FunctionType *raviV_op_newarrayfloatT;
  llvm::FunctionType *raviV_op_setlistT;
  llvm::FunctionType *raviV_op_newtableT;
  llvm::FunctionType *raviV_op_loadnilT;
  llvm::FunctionType *raviV_op_concatT;
  llvm::FunctionType *raviV_op_closureT;
  llvm::FunctionType *raviV_op_varargT;
  llvm::FunctionType *raviV_op_shrT;
  llvm::FunctionType *raviV_op_shlT;

  llvm::FunctionType *raviH_set_intT;
  llvm::FunctionType *raviH_set_floatT;

  llvm::FunctionType *ravi_dump_valueT;
  llvm::FunctionType *ravi_dump_stackT;
  llvm::FunctionType *ravi_dump_stacktopT;
  llvm::FunctionType *ravi_debug_traceT;

  std::array<llvm::Constant *, 256> kInt;
  std::array<llvm::Constant *, 21> kluaInteger;
  std::array<llvm::Constant *, 10> kByte;

  llvm::Constant *kFalse;

  // To allow better optimization we need to decorate the
  // LLVM Load/Store instructions with type information.
  // For this we need to construct tbaa metadata
  llvm::MDBuilder mdbuilder;
  llvm::MDNode *tbaa_root;
  llvm::MDNode *tbaa_charT;
  llvm::MDNode *tbaa_pcharT;
  llvm::MDNode *tbaa_shortT;
  llvm::MDNode *tbaa_pshortT;
  llvm::MDNode *tbaa_intT;
  llvm::MDNode *tbaa_pintT;
  llvm::MDNode *tbaa_longlongT;
  llvm::MDNode *tbaa_plonglongT;
  llvm::MDNode *tbaa_doubleT;
  llvm::MDNode *tbaa_pdoubleT;
  llvm::MDNode *tbaa_pointerT;
  llvm::MDNode *tbaa_ppointerT;
  llvm::MDNode *tbaa_CallInfo_lT;
  llvm::MDNode *tbaa_CallInfoT;
  llvm::MDNode *tbaa_luaStateT;
  llvm::MDNode *tbaa_luaState_ciT;
  llvm::MDNode *tbaa_luaState_ci_baseT;
  llvm::MDNode *tbaa_CallInfo_funcT;
  llvm::MDNode *tbaa_CallInfo_func_LClosureT;
  llvm::MDNode *tbaa_CallInfo_topT;
  llvm::MDNode *tbaa_CallInfo_jitstatusT;
  llvm::MDNode *tbaa_CallInfo_callstatusT;
  llvm::MDNode *tbaa_LClosureT;
  llvm::MDNode *tbaa_LClosure_pT;
  llvm::MDNode *tbaa_LClosure_upvalsT;
  llvm::MDNode *tbaa_ProtoT;
  llvm::MDNode *tbaa_Proto_kT;
  llvm::MDNode *tbaa_Proto_sizepT;
  llvm::MDNode *tbaa_TValueT;
  llvm::MDNode *tbaa_TValue_nT;
  llvm::MDNode *tbaa_TValue_hT;
  llvm::MDNode *tbaa_TValue_ttT;
  llvm::MDNode *tbaa_luaState_topT;
  llvm::MDNode *tbaa_UpValT;
  llvm::MDNode *tbaa_UpVal_vT;
  llvm::MDNode *tbaa_RaviArrayT;
  llvm::MDNode *tbaa_TableT;
  llvm::MDNode *tbaa_RaviArray_typeT;
  llvm::MDNode *tbaa_RaviArray_dataT;
  llvm::MDNode *tbaa_RaviArray_lenT;
};

class RAVI_API RaviJITStateImpl;

// Represents a JITed or JITable function
// Each function gets its own module and execution engine - this
// may change in future
// The advantage is that we can drop the function when the corresponding
// Lua object is garbage collected - with MCJIT this is not possible
// to do at function level
class RAVI_API RaviJITFunctionImpl : public RaviJITFunction {

  // The function is tracked by RaviJITState so we need to
  // tell RaviJITState when this function dies
  RaviJITStateImpl *owner_;

  // Unique name for the function
  std::string name_;

  // The execution engine responsible for compiling the
  // module
  llvm::ExecutionEngine *engine_;

  // Module within which the function will be defined
  llvm::Module *module_;

  // The llvm Function definition
  llvm::Function *function_;

  // Pointer to compiled function - this is only set when
  // the function
  void *ptr_;

public:
  RaviJITFunctionImpl(RaviJITStateImpl *owner, llvm::FunctionType *type,
                      llvm::GlobalValue::LinkageTypes linkage,
                      const std::string &name);
  virtual ~RaviJITFunctionImpl();

  // Compile the function if not already compiled and
  // return pointer to function
  virtual void *compile(bool doDump = false);

  // Add declaration for an extern function that is not
  // loaded dynamically - i.e., is part of the the executable
  // and therefore not visible at runtime by name
  virtual llvm::Function *addExternFunction(llvm::FunctionType *type,
                                            void *address,
                                            const std::string &name);

  virtual const std::string &name() const { return name_; }
  virtual llvm::Function *function() const { return function_; }
  virtual llvm::Module *module() const { return module_; }
  virtual llvm::ExecutionEngine *engine() const { return engine_; }
  virtual RaviJITState *owner() const;
  virtual void dump();
  virtual void dumpAssembly();
  void runpasses(bool dumpAsm = false);
};

// Ravi's JIT State
// All of the JIT information is held here
class RAVI_API RaviJITStateImpl : public RaviJITState {

  // map of names to functions
  std::map<std::string, RaviJITFunction *> functions_;

  llvm::LLVMContext *context_;

  // The triple represents the host target
  std::string triple_;

  // Lua type definitions
  LuaLLVMTypes *types_;

  // Should we auto compile what we can?
  bool auto_;

  // Is JIT enabled
  bool enabled_;

  // Optimizer level (LLVM PassManagerBuilder)
  int opt_level_;

  // Size level (LLVM PassManagerBuilder)
  int size_level_;

  // min code size for compilation
  int min_code_size_;

  // min execution count for compilation
  int min_exec_count_;

  // gc step size; defaults to 200
  int gc_step_;

public:
  RaviJITStateImpl();
  virtual ~RaviJITStateImpl();

  // Create a function of specified type and linkage
  virtual RaviJITFunction *
  createFunction(llvm::FunctionType *type,
                 llvm::GlobalValue::LinkageTypes linkage,
                 const std::string &name);

  // Stop tracking the named function - note that
  // the function is assumed to be destroyed by the user
  void deleteFunction(const std::string &name);

  void addGlobalSymbol(const std::string &name, void *address);

  virtual void dump();
  virtual llvm::LLVMContext &context() { return *context_; }
  LuaLLVMTypes *types() const { return types_; }
  const std::string &triple() const { return triple_; }
  bool is_auto() const { return auto_; }
  void set_auto(bool value) { auto_ = value; }
  bool is_enabled() const { return enabled_; }
  void set_enabled(bool value) { enabled_ = value; }
  int get_optlevel() const { return opt_level_; }
  void set_optlevel(int value) {
    if (value >= 0 && value <= 3)
      opt_level_ = value;
  }
  int get_sizelevel() const { return size_level_; }
  void set_sizelevel(int value) {
    if (value >= 0 && value <= 2)
      size_level_ = value;
  }
  int get_mincodesize() const { return min_code_size_; }
  void set_mincodesize(int value) {
    min_code_size_ = value > 0 ? value : min_code_size_;
  }
  int get_minexeccount() const { return min_exec_count_; }
  void set_minexeccount(int value) {
    min_exec_count_ = value > 0 ? value : min_exec_count_;
  }
  int get_gcstep() const { return gc_step_; }
  void set_gcstep(int value) {
    gc_step_ = value > 0 ? value : gc_step_;
  }
};

// To optimise fornum loops
// i.e. OP_FORPREP and OP_FORLOOP instructions
// we use computed gotos to specialised
// jmp labels. Hence the 4 jmp targets.
// For other instructions only the first jump
// target is used
struct RaviBranchDef {
  // this field is used for all branches
  // forloop int step > 0
  llvm::BasicBlock *jmp1;
  // forloop int step < 0
  llvm::BasicBlock *jmp2;
  // forloop float step > 0
  llvm::BasicBlock *jmp3;
  // forlook float step < 0
  llvm::BasicBlock *jmp4;

  // These are local variables for a fornum
  // loop
  llvm::Value *ilimit;
  llvm::Value *istep;
  llvm::Value *iidx;
  llvm::Value *flimit;
  llvm::Value *fstep;
  llvm::Value *fidx;

  // This holds the branch to which the
  // loop body will jump to using a
  // IndirectBr instruction
  llvm::Value *forloop_branch;

  RaviBranchDef();
};

// This structure holds stuff we need when compiling a single
// function
struct RaviFunctionDef {
  RaviJITStateImpl *jitState;
  RaviJITFunctionImpl *raviF;
  llvm::Function *f;
  llvm::BasicBlock *entry;
  llvm::Value *L;
  LuaLLVMTypes *types;
  llvm::IRBuilder<> *builder;
  Proto *p;

  // Lua function declarations
  llvm::Function *luaD_poscallF;
  llvm::Function *luaD_precallF;
  llvm::Function *luaD_callF;
  llvm::Function *luaF_closeF;
  llvm::Function *luaG_runerrorF;
  llvm::Function *luaT_trybinTMF;
  llvm::Function *luaV_equalobjF;
  llvm::Function *luaV_lessthanF;
  llvm::Function *luaV_lessequalF;
  llvm::Function *luaV_forlimitF;
  llvm::Function *luaV_tonumberF;
  llvm::Function *luaV_tointegerF;
  llvm::Function *luaV_executeF;
  llvm::Function *luaV_gettableF;
  llvm::Function *luaV_settableF;
  llvm::Function *luaV_modF;
  llvm::Function *luaV_divF;
  llvm::Function *luaV_objlenF;
  llvm::Function *luaC_upvalbarrierF;

  // Some cheats - these correspond to OPCODEs that
  // are not inlined as of now
  llvm::Function *raviV_op_newarrayintF;
  llvm::Function *raviV_op_newarrayfloatF;
  llvm::Function *raviV_op_setlistF;
  llvm::Function *raviV_op_newtableF;
  llvm::Function *raviV_op_loadnilF;
  llvm::Function *raviV_op_concatF;
  llvm::Function *raviV_op_closureF;
  llvm::Function *raviV_op_varargF;
  llvm::Function *raviV_op_shrF;
  llvm::Function *raviV_op_shlF;

  // array setters
  llvm::Function *raviH_set_intF;
  llvm::Function *raviH_set_floatF;

  llvm::Function *ravi_dump_valueF;
  llvm::Function *ravi_dump_stackF;
  llvm::Function *ravi_dump_stacktopF;
  llvm::Function *ravi_debug_traceF;

  // standard C functions
  llvm::Constant *printfFunc;
  llvm::Constant *fmodFunc;
  llvm::Constant *floorFunc;
  llvm::Constant *powFunc;

  // Jump targets in the function
  std::vector<RaviBranchDef> jmp_targets;

  // Load pointer to proto
  llvm::Value *proto; // gep
  llvm::Instruction *proto_ptr;

  // Obtain pointer to Proto->k
  llvm::Value *proto_k;
  // Load pointer to Proto->k; this does not change
  llvm::Instruction *k_ptr;

  // Load L->ci
  llvm::Value *L_ci; // This is the GEP for L->ci
  llvm::Instruction *ci_val;

  // Pointer to ci->u.l.base
  llvm::Value *Ci_base; // This is the GEP for ci->u.l.base
  llvm::Instruction *base_ptr;

  // Pointer to LClosure
  llvm::Value *p_LClosure;
};

// This class is responsible for compiling Lua byte code
// to LLVM IR
class RaviCodeGenerator {
public:
  RaviCodeGenerator(RaviJITStateImpl *jitState);

  // Compile given function if possible
  // The p->ravi_jit structure will be updated
  // Note that if a function fails to compile then
  // a flag is set so that it doesn't get compiled again
  void compile(lua_State *L, Proto *p, ravi_compile_options_t *options);

  // We can only compile a subset of op codes
  // and not all features are supported
  bool canCompile(Proto *p);

  // Create a unique function name in the context
  // of this generator
  const char *unique_function_name();

  // Create the prologue of the JIT function
  // Argument will be named L
  // Initial BasicBlock will be created
  // int func(lua_State *L) {
  std::unique_ptr<RaviJITFunctionImpl>
  create_function(llvm::IRBuilder<> &builder, RaviFunctionDef *def);

  llvm::CallInst *CreateCall1(llvm::IRBuilder<> *builder, llvm::Value *func,
                              llvm::Value *arg1);
  llvm::CallInst *CreateCall2(llvm::IRBuilder<> *builder, llvm::Value *func,
                              llvm::Value *arg1, llvm::Value *arg2);
  llvm::CallInst *CreateCall3(llvm::IRBuilder<> *builder, llvm::Value *func,
                              llvm::Value *arg1, llvm::Value *arg2,
                              llvm::Value *arg3);
  llvm::CallInst *CreateCall4(llvm::IRBuilder<> *builder, llvm::Value *func,
                              llvm::Value *arg1, llvm::Value *arg2,
                              llvm::Value *arg3, llvm::Value *arg4);
  llvm::CallInst *CreateCall5(llvm::IRBuilder<> *builder, llvm::Value *func,
                              llvm::Value *arg1, llvm::Value *arg2,
                              llvm::Value *arg3, llvm::Value *arg4,
                              llvm::Value *arg5);

  void emit_raise_lua_error(RaviFunctionDef *def, const char *str);

  // Add extern declarations for Lua functions we need to call
  void emit_extern_declarations(RaviFunctionDef *def);

  // Retrieve the proto->sizep
  llvm::Instruction *emit_load_proto_sizep(RaviFunctionDef *def);

  // Store lua_Number or lua_Integer
  llvm::Instruction *emit_store_local_n(RaviFunctionDef *def, llvm::Value *src,
                                        llvm::Value *dest);

  // Load lua_Number or lua_Integer
  llvm::Instruction *emit_load_local_n(RaviFunctionDef *def, llvm::Value *src);

  // Store int
  llvm::Instruction *emit_store_local_int(RaviFunctionDef *def,
                                          llvm::Value *src, llvm::Value *dest);

  // Load int
  llvm::Instruction *emit_load_local_int(RaviFunctionDef *def,
                                         llvm::Value *src);

  // Test if value type is of specific Lua type
  // Value_type should have been obtained by emit_load_type()
  // The Lua typecode to check must be in lua_typecode
  // The return value is a boolean type as a result of
  // integer comparison result which is i1 in LLVM
  llvm::Value *emit_is_value_of_type(RaviFunctionDef *def,
                                     llvm::Value *value_type,
                                     LuaTypeCode lua_typecode,
                                     const char *varname = "value.typeof");

  // Test if value type is NOT of specific Lua type
  // Value_type should have been obtained by emit_load_type()
  // The Lua typecode to check must be in lua_typecode
  // The return value is a boolean type as a result of
  // integer comparison result which is i1 in LLVM
  llvm::Value *
  emit_is_not_value_of_type(RaviFunctionDef *def, llvm::Value *value_type,
                            LuaTypeCode lua_typecode,
                            const char *varname = "value.not.typeof");

  // emit code for (LClosure *)ci->func->value_.gc
  llvm::Value *emit_gep_ci_func_value_gc_asLClosure(RaviFunctionDef *def);

  llvm::Value *emit_gep(RaviFunctionDef *def, const char *name, llvm::Value *s,
                        int arg1, int arg2);

  llvm::Value *emit_gep(RaviFunctionDef *def, const char *name, llvm::Value *s,
                        int arg1, int arg2, int arg3);

  // emit code for &ptr[offset]
  llvm::Value *emit_array_get(RaviFunctionDef *def, llvm::Value *ptr,
                              int offset);

  // emit code to load pointer L->ci->u.l.base
  void emit_load_base(RaviFunctionDef *def);

  llvm::Value *emit_load_ci(RaviFunctionDef *def);

  // emit code to obtain address of register at location A
  llvm::Value *emit_gep_register(RaviFunctionDef *def, int A);

  // emit code to obtain address of register or constant at location B
  llvm::Value *emit_gep_register_or_constant(RaviFunctionDef *def, int B);

  // emit code to obtain address of constant at locatiion B
  llvm::Value *emit_gep_constant(RaviFunctionDef *def, int B);

  llvm::Value *emit_is_jit_call(RaviFunctionDef *def, llvm::Value *ci);
  llvm::Value *emit_ci_is_Lua(RaviFunctionDef *def, llvm::Value *ci);

  // obtain address of L->top
  llvm::Value *emit_gep_L_top(RaviFunctionDef *def);

  // (int)(L->top - ra)
  llvm::Value *emit_num_stack_elements(RaviFunctionDef *def, llvm::Value *ra);

  // Load a register or constant - if constant is int then return a direct LLVM
  // constant
  llvm::Value *emit_load_register_or_constant_i(RaviFunctionDef *def, int K);

  // Load a register or constant - if constant is int then return a direct LLVM
  // constant
  llvm::Value *emit_load_register_or_constant_n(RaviFunctionDef *def, int K);

  // emit code to load the lua_Number value from register
  llvm::Instruction *emit_load_reg_n(RaviFunctionDef *def, llvm::Value *ra);

  // emit code to load the lua_Integer value from register
  llvm::Instruction *emit_load_reg_i(RaviFunctionDef *def, llvm::Value *rb);

  // emit code to load the boolean value from register
  llvm::Instruction *emit_load_reg_b(RaviFunctionDef *def, llvm::Value *ra);

  // emit code to load the table value from register
  llvm::Instruction *emit_load_reg_h(RaviFunctionDef *def, llvm::Value *ra);

  // emit code to load pointer to int array
  llvm::Instruction *emit_load_reg_h_intarray(RaviFunctionDef *def,
                                              llvm::Instruction *ra);

  // emit code to load pointer to double array
  llvm::Instruction *emit_load_reg_h_floatarray(RaviFunctionDef *def,
                                                llvm::Instruction *ra);

  // emit code to store lua_Number value into register
  void emit_store_reg_n(RaviFunctionDef *def, llvm::Value *value,
                        llvm::Value *dest_ptr);
  void emit_store_reg_n_withtype(RaviFunctionDef *def, llvm::Value *value,
                                 llvm::Value *dest_ptr);

  // emit code to store lua_Integer value into register
  void emit_store_reg_i(RaviFunctionDef *def, llvm::Value *value,
                        llvm::Value *dest_ptr);
  void emit_store_reg_i_withtype(RaviFunctionDef *def, llvm::Value *value,
                                 llvm::Value *dest_ptr);

  // emit code to store bool value into register
  void emit_store_reg_b(RaviFunctionDef *def, llvm::Value *value,
                        llvm::Value *dest_ptr);
  void emit_store_reg_b_withtype(RaviFunctionDef *def, llvm::Value *value,
                                 llvm::Value *dest_ptr);

  // emit code to set the type in the register
  void emit_store_type_(RaviFunctionDef *def, llvm::Value *value, int type);

  // emit code to load the type from a register
  llvm::Instruction *emit_load_type(RaviFunctionDef *def, llvm::Value *value);

  // emit code to load the array type
  llvm::Instruction *emit_load_ravi_arraytype(RaviFunctionDef *def,
                                              llvm::Value *value);

  // emit code to load the array length
  llvm::Instruction *emit_load_ravi_arraylength(RaviFunctionDef *def,
                                                llvm::Value *value);

  // TValue assign
  void emit_assign(RaviFunctionDef *def, llvm::Value *ra, llvm::Value *rb);

  // Get &upvals[offset] from LClosure
  llvm::Value *emit_gep_upvals(RaviFunctionDef *def, int offset);

  // Load the &upvals[offset] -> result is UpVal*
  llvm::Instruction *emit_load_pupval(RaviFunctionDef *def,
                                      llvm::Value *ppupval);

  // Get &upval->v
  llvm::Value *emit_gep_upval_v(RaviFunctionDef *def,
                                llvm::Instruction *pupval);

  // Load upval->v
  llvm::Instruction *emit_load_upval_v(RaviFunctionDef *def,
                                       llvm::Instruction *pupval);

  // Get &upval->value -> result is TValue *
  llvm::Value *emit_gep_upval_value(RaviFunctionDef *def,
                                    llvm::Instruction *pupval);

  // isnil(reg) || isboolean(reg) && reg.value == 0
  // !(isnil(reg) || isboolean(reg) && reg.value == 0)
  llvm::Value *emit_boolean_testfalse(RaviFunctionDef *def, llvm::Value *reg,
                                      bool donot);

  llvm::Instruction *emit_tointeger(RaviFunctionDef *def, llvm::Value *reg);

  llvm::Instruction *emit_tofloat(RaviFunctionDef *def, llvm::Value *reg);

  // L->top = ci->top
  // ci is passed as the def->ci_val may be stale
  void emit_refresh_L_top(RaviFunctionDef *def, llvm::Value *ci);

  // L->top = R(B)
  void emit_set_L_top_toreg(RaviFunctionDef *def, int B);

  void debug_printf(RaviFunctionDef *def, const char *str);

  void debug_printf1(RaviFunctionDef *def, const char *str, llvm::Value *arg1);

  void debug_printf2(RaviFunctionDef *def, const char *str, llvm::Value *arg1,
                     llvm::Value *arg2);

  void debug_printf3(RaviFunctionDef *def, const char *str, llvm::Value *arg1,
                     llvm::Value *arg2, llvm::Value *arg3);

  void debug_printf4(RaviFunctionDef *def, const char *str, llvm::Value *arg1,
                     llvm::Value *arg2, llvm::Value *arg3, llvm::Value *arg4);

  void emit_dump_stack(RaviFunctionDef *def, const char *str);
  void emit_dump_stacktop(RaviFunctionDef *def, const char *str);
  void emit_debug_trace(RaviFunctionDef *def, int opCode, int pc);

  // Look for Lua bytecodes that are jump targets and allocate
  // a BasicBlock for each such target in def->jump_targets.
  // The BasicBlocks are not inserted into the function until later
  // but having them created allows rest of the code to insert
  // branch instructions
  void scan_jump_targets(RaviFunctionDef *def, Proto *p);

  // Should be called before processing a Lua OpCode
  // This function checks if a new block should be started
  // and links in the new block
  void link_block(RaviFunctionDef *def, int pc);

  void emit_CONCAT(RaviFunctionDef *def, int A, int B, int C, int pc);

  void emit_CLOSURE(RaviFunctionDef *def, int A, int Bx, int pc);

  void emit_VARARG(RaviFunctionDef *def, int A, int B, int pc);

  void emit_LOADNIL(RaviFunctionDef *def, int A, int B, int pc);

  void emit_LOADFZ(RaviFunctionDef *def, int A, int pc);

  void emit_LOADIZ(RaviFunctionDef *def, int A, int pc);

  void emit_LOADBOOL(RaviFunctionDef *def, int A, int B, int C, int j, int pc);

  void emit_ARITH(RaviFunctionDef *def, int A, int B, int C, OpCode op, TMS tms,
                  int pc);

  void emit_MOD(RaviFunctionDef *def, int A, int B, int C, int pc);

  void emit_IDIV(RaviFunctionDef *def, int A, int B, int C, int pc);

  void emit_POW(RaviFunctionDef *def, int A, int B, int C, int pc);

  void emit_UNM(RaviFunctionDef *def, int A, int B, int pc);

  void emit_UNMF(RaviFunctionDef *def, int A, int B, int pc);

  void emit_UNMI(RaviFunctionDef *def, int A, int B, int pc);

  void emit_ADDFF(RaviFunctionDef *def, int A, int B, int C, int pc);

  void emit_ADDFI(RaviFunctionDef *def, int A, int B, int C, int pc);

  void emit_ADDII(RaviFunctionDef *def, int A, int B, int C, int pc);

  void emit_SUBFF(RaviFunctionDef *def, int A, int B, int C, int pc);

  void emit_SUBFI(RaviFunctionDef *def, int A, int B, int C, int pc);

  void emit_SUBIF(RaviFunctionDef *def, int A, int B, int C, int pc);

  void emit_SUBII(RaviFunctionDef *def, int A, int B, int C, int pc);

  void emit_MULFF(RaviFunctionDef *def, int A, int B, int C, int pc);

  void emit_MULFI(RaviFunctionDef *def, int A, int B, int C, int pc);

  void emit_MULII(RaviFunctionDef *def, int A, int B, int C, int pc);

  void emit_DIVFF(RaviFunctionDef *def, int A, int B, int C, int pc);

  void emit_DIVFI(RaviFunctionDef *def, int A, int B, int C, int pc);

  void emit_DIVIF(RaviFunctionDef *def, int A, int B, int C, int pc);

  void emit_DIVII(RaviFunctionDef *def, int A, int B, int C, int pc);

  void emit_LOADK(RaviFunctionDef *def, int A, int Bx, int pc);

  void emit_RETURN(RaviFunctionDef *def, int A, int B, int pc);

  void emit_CALL(RaviFunctionDef *def, int A, int B, int C, int pc);

  void emit_JMP(RaviFunctionDef *def, int A, int j, int pc);

  void emit_iFORPREP(RaviFunctionDef *def, int A, int sBx, int step_one,
                     int pc);

  void emit_iFORLOOP(RaviFunctionDef *def, int A, int sBx, RaviBranchDef &b,
                     int step_one, int pc);

  void emit_FORPREP(RaviFunctionDef *def, int A, int sBx, int pc);

  void emit_FORLOOP(RaviFunctionDef *def, int A, int sBx, int pc);

  void emit_FORPREP2(RaviFunctionDef *def, int A, int sBx, int pc);

  void emit_FORLOOP2(RaviFunctionDef *def, int A, int sBx, RaviBranchDef &b,
                     int pc);

  void emit_MOVE(RaviFunctionDef *def, int A, int B, int pc);

  void emit_MOVEI(RaviFunctionDef *def, int A, int B, int pc);

  void emit_MOVEF(RaviFunctionDef *def, int A, int B, int pc);

  void emit_TOINT(RaviFunctionDef *def, int A, int pc);

  void emit_TOFLT(RaviFunctionDef *def, int A, int pc);

  void emit_LEN(RaviFunctionDef *def, int A, int B, int pc);

  void emit_SETTABLE(RaviFunctionDef *def, int A, int B, int C, int pc);

  void emit_GETTABLE(RaviFunctionDef *def, int A, int B, int C, int pc);

  void emit_SELF(RaviFunctionDef *def, int A, int B, int C, int pc);

  void emit_GETUPVAL(RaviFunctionDef *def, int A, int B, int pc);

  void emit_SETUPVAL(RaviFunctionDef *def, int A, int B, int pc);

  void emit_GETTABUP(RaviFunctionDef *def, int A, int B, int C, int pc);

  void emit_SETTABUP(RaviFunctionDef *def, int A, int B, int C, int pc);

  void emit_NEWARRAYINT(RaviFunctionDef *def, int A, int pc);

  void emit_NEWARRAYFLOAT(RaviFunctionDef *def, int A, int pc);

  void emit_NEWTABLE(RaviFunctionDef *def, int A, int B, int C, int pc);

  void emit_SETLIST(RaviFunctionDef *def, int A, int B, int C, int pc);

  void emit_NOT(RaviFunctionDef *def, int A, int B, int pc);

  // Emit code for OP_EQ, OP_LT and OP_LE
  // The callee parameter should be luaV_equalobj, luaV_lessthan and
  // luaV_lessequal
  // respectively
  // A, B, C must be operands of the OP_EQ/OP_LT/OP_LE instructions
  // j must be the jump target (offset of the code to which we need to jump to)
  // jA must be the A operand of the jump instruction
  void emit_EQ(RaviFunctionDef *def, int A, int B, int C, int j, int jA,
               llvm::Constant *callee, OpCode opCode, int pc);

  // OP_TEST is followed by a OP_JMP instruction - both are handled
  // together
  // A, B, C must be operands of the OP_TEST instruction
  // j must be the jump target (offset of the code to which we need to jump to)
  // jA must be the A operand of the jump instruction
  void emit_TEST(RaviFunctionDef *def, int A, int B, int C, int j, int jA,
                 int pc);

  // OP_TESTSET is followed by a OP_JMP instruction - both are handled
  // together
  // A, B, C must be operands of the OP_TESTSET instruction
  // j must be the jump target (offset of the code to which we need to jump to)
  // jA must be the A operand of the jump instruction
  void emit_TESTSET(RaviFunctionDef *def, int A, int B, int C, int j, int jA,
                    int pc);

  void emit_TFORCALL(RaviFunctionDef *def, int A, int B, int C, int j, int jA,
                     int pc);

  void emit_TFORLOOP(RaviFunctionDef *def, int A, int j, int pc);

  void emit_GETTABLE_AF(RaviFunctionDef *def, int A, int B, int C,
                        bool omitArrayGetRangeCheck, int pc);

  void emit_GETTABLE_AI(RaviFunctionDef *def, int A, int B, int C,
                        bool omitArrayGetRangeCheck, int pc);

  void emit_SETTABLE_AF(RaviFunctionDef *def, int A, int B, int C,
                        bool known_int, int pc);

  void emit_SETTABLE_AI(RaviFunctionDef *def, int A, int B, int C,
                        bool known_float, int pc);

  void emit_MOVEAI(RaviFunctionDef *def, int A, int B, int pc);

  void emit_MOVEAF(RaviFunctionDef *def, int A, int B, int pc);

  void emit_TOARRAY(RaviFunctionDef *def, int A, int array_type_expected,
                    const char *errmsg, int pc);

  void emit_BITWISE_BINARY_OP(RaviFunctionDef *def, OpCode op, int A, int B,
                              int C, int pc);

  void emit_BITWISE_SHIFT_OP(RaviFunctionDef *def, OpCode op, int A, int B,
                             int C, int pc);

  void emit_BNOT_I(RaviFunctionDef *def, int A, int B, int pc);

  void emit_bitwise_shiftl(RaviFunctionDef *def, llvm::Value *ra, int B,
                           lua_Integer y);

private:
  RaviJITStateImpl *jitState_;
  char temp_[31]; // for name
  int id_;        // for name
};
}

struct ravi_State {
  ravi::RaviJITState *jit;
  ravi::RaviCodeGenerator *code_generator;
};

#define RaviJIT(L) ((ravi::RaviJITStateImpl *)G(L)->ravi_state->jit)

#define RAVI_CODEGEN_FORPREP2 0

#endif

#endif
