#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/Support/IRBuilder.h"

int main()
{
  // コンテキストの生成
  auto& context = llvm::getGlobalContext();
  // モジュールの生成
  auto module = new llvm::Module("top", context);
  // アセンブリビルダの生成
  llvm::IRBuilder<> builder(context);

  // main関数をつくる(中身はからっぽ)
  auto funcType =
      llvm::FunctionType::get(builder.getInt32Ty(), false);
  auto mainFunc =
      llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "main", module);

  // エントリポイントを追加する
  auto entry = llvm::BasicBlock::Create(context, "entrypoint", mainFunc);
  builder.SetInsertPoint(entry);

  // 文字列を定義する
  auto helloWorld = builder.CreateGlobalStringPtr("hello world!\n");

  // puts をとりだしてつかえるようにセットアップする。
  std::vector<llvm::Type *> putsArgs;
  putsArgs.push_back(builder.getInt8Ty()->getPointerTo());
  llvm::ArrayRef<llvm::Type*>  argsRef(putsArgs);

  // puts の型
  auto putsType =
    llvm::FunctionType::get(builder.getInt32Ty(), argsRef, false);
  // puts 関数をいよいよとりだした!
  auto putsFunc = module->getOrInsertFunction("puts", putsType);

  // puts関数をよびだす
  builder.CreateCall(putsFunc, helloWorld);
  // return 0;
  builder.CreateRet(llvm::ConstantInt::get(builder.getInt32Ty(), 0));

  // できたアセンブリを標準エラー出力にだす。
  module->dump();
}
