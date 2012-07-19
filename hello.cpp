#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/Support/IRBuilder.h"

int main()
{
  // コンテキストの生成
  llvm::LLVMContext& context = llvm::getGlobalContext();
  // モジュールの生成
  llvm::Module *module = new llvm::Module("top", context);
  // アセンブリビルダの生成
  llvm::IRBuilder<> builder(context);

  // main関数をつくる(中身はからっぽ)
  llvm::FunctionType *funcType =
      llvm::FunctionType::get(builder.getInt32Ty(), false);
  llvm::Function *mainFunc =
      llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "main", module);

  // エントリポイントを追加する
  llvm::BasicBlock *entry = llvm::BasicBlock::Create(context, "entrypoint", mainFunc);
  builder.SetInsertPoint(entry);

  // 文字列を定義する
  llvm::Value *helloWorld = builder.CreateGlobalStringPtr("hello world!\n");

  // puts をとりだしてつかえるようにセットアップする。
  std::vector<llvm::Type *> putsArgs;
  putsArgs.push_back(builder.getInt8Ty()->getPointerTo());
  llvm::ArrayRef<llvm::Type*>  argsRef(putsArgs);

  // puts の型
  llvm::FunctionType *putsType =
    llvm::FunctionType::get(builder.getInt32Ty(), argsRef, false);
  // puts 関数をいよいよとりだした!
  llvm::Constant *putsFunc = module->getOrInsertFunction("puts", putsType);

  // puts関数をよびだす
  builder.CreateCall(putsFunc, helloWorld);
  // return 0;
  builder.CreateRet(llvm::ConstantInt::get(builder.getInt32Ty(), 0));

  // できたアセンブリを標準エラー出力にだす。
  module->dump();
}
