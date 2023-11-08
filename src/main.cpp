#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>

#include "sim.h"


using namespace llvm;


[[noreturn]]
static void stack_chk_fail_mock() {
    throw std::runtime_error("stack check failed");
}

static Function * generate_main(Module * const module) {
    LLVMContext & context = module->getContext();

    IRBuilder<> builder(context);

    // ; Function Attrs: nounwind sspstrong uwtable
    // define dso_local i32 @main() local_unnamed_addr #0 {
    Function * const main_ = Function::Create(
            FunctionType::get(builder.getInt32Ty(), false),
            Function::ExternalLinkage,
            "main",
            module
    );

    main_->setDSOLocal(true);
    main_->setUnnamedAddr(llvm::GlobalValue::UnnamedAddr::Local);

    // attributes #0 = { nounwind sspstrong uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "probe-stack"="inline-asm" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
    main_->addFnAttr(Attribute::AttrKind::NoUnwind);
    main_->addFnAttr(Attribute::AttrKind::StackProtectStrong);
    main_->setUWTableKind(UWTableKind::Default);
    main_->addFnAttr("min-legal-vector-width", "0");
    main_->addFnAttr("no-trapping-math", "true");
    main_->addFnAttr("probe-stack", "inline-asm");
    main_->addFnAttr("stack-protector-buffer-size", "8");
    main_->addFnAttr("target-cpu", "x86-64");
    main_->addFnAttr("target-features", "+cx8,+fxsr,+mmx,+sse,+sse2,+x87");
    main_->addFnAttr("tune-cpu", "generic");

    // declare void @llvm.lifetime.start.p0(i64 immarg, ptr nocapture) #1
    Function * const llvm_lifetime_start_p0 =
            Intrinsic::getDeclaration(module, Intrinsic::lifetime_start, { builder.getPtrTy() });

    // ; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
    llvm_lifetime_start_p0->setMustProgress();

    // declare i32 @sim_rand() local_unnamed_addr #3
    FunctionCallee sim_rand = module->getOrInsertFunction("sim_rand", builder.getInt32Ty());

    // ; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
    // declare void @llvm.lifetime.end.p0(i64 immarg, ptr nocapture) #1
    Function * const llvm_lifetime_end_p0 =
            Intrinsic::getDeclaration(module, Intrinsic::lifetime_end, { builder.getPtrTy() });

    // attributes #2 = { mustprogress nocallback nofree nounwind willreturn memory(argmem: write) }
    llvm_lifetime_end_p0->setMustProgress();

    // declare void @sim_set_pixel(i32 noundef, i32 noundef, i32 noundef) local_unnamed_addr #3
    const AttributeList ssp_attrs = AttributeList::get(context, {}, {}, {
        AttributeSet::get(context, { Attribute::get(context, Attribute::AttrKind::NoUndef) }),
        AttributeSet::get(context, { Attribute::get(context, Attribute::AttrKind::NoUndef) }),
        AttributeSet::get(context, { Attribute::get(context, Attribute::AttrKind::NoUndef) }),
    });

    const FunctionCallee sim_set_pixel = module->getOrInsertFunction(
            "sim_set_pixel",
            ssp_attrs,
            builder.getVoidTy(),
            builder.getInt32Ty(),
            builder.getInt32Ty(),
            builder.getInt32Ty()
    );

    // declare void @sim_flush() local_unnamed_addr #3
    const FunctionCallee sim_flush = module->getOrInsertFunction("sim_flush", builder.getVoidTy());

    // attributes #3 = { "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
    // attributes #4 = { nounwind }
    // TODO

    BasicBlock * const BB0 = BasicBlock::Create(context, "", main_);
    BasicBlock * const BB3 = BasicBlock::Create(context, "", main_);
    BasicBlock * const BB6 = BasicBlock::Create(context, "", main_);
    BasicBlock * const BB9 = BasicBlock::Create(context, "", main_);
    BasicBlock * const BB12 = BasicBlock::Create(context, "", main_);
    BasicBlock * const BB27 = BasicBlock::Create(context, "", main_);
    BasicBlock * const BB30 = BasicBlock::Create(context, "", main_);
    BasicBlock * const BB38 = BasicBlock::Create(context, "", main_);
    BasicBlock * const BB44 = BasicBlock::Create(context, "", main_);
    BasicBlock * const BB51 = BasicBlock::Create(context, "", main_);
    BasicBlock * const BB54 = BasicBlock::Create(context, "", main_);
    BasicBlock * const BB57 = BasicBlock::Create(context, "", main_);
    BasicBlock * const BB67 = BasicBlock::Create(context, "", main_);
    BasicBlock * const BB69 = BasicBlock::Create(context, "", main_);
    BasicBlock * const BB75 = BasicBlock::Create(context, "", main_);
    BasicBlock * const BB82 = BasicBlock::Create(context, "", main_);
    BasicBlock * const BB84 = BasicBlock::Create(context, "", main_);
    BasicBlock * const BB175 = BasicBlock::Create(context, "", main_);
    BasicBlock * const BB179 = BasicBlock::Create(context, "", main_);
    BasicBlock * const BB182 = BasicBlock::Create(context, "", main_);
    BasicBlock * const BB185 = BasicBlock::Create(context, "", main_);
    BasicBlock * const BB200 = BasicBlock::Create(context, "", main_);
    BasicBlock * const BB201 = BasicBlock::Create(context, "", main_);

    builder.SetInsertPoint(BB0);

    //   %1 = alloca [1024 x i32], align 16
    AllocaInst * const v1 = builder.CreateAlloca(ArrayType::get(builder.getInt32Ty(), 1024));
    v1->setAlignment(Align::Constant<16>());

    //   %2 = alloca [1024 x i32], align 16
    AllocaInst * const v2 = builder.CreateAlloca(ArrayType::get(builder.getInt32Ty(), 1024));
    v2->setAlignment(Align::Constant<16>());

    //   call void @llvm.lifetime.start.p0(i64 4096, ptr nonnull %1) #4
    auto * lts_v1 = builder.CreateCall(llvm_lifetime_start_p0, { builder.getInt64(4096), v1 });
    lts_v1->addParamAttr(1, Attribute::AttrKind::NonNull);

    //   call void @llvm.lifetime.start.p0(i64 4096, ptr nonnull %2) #4
    auto * lts_v2 = builder.CreateCall(llvm_lifetime_start_p0, { builder.getInt64(4096), v2 });
    lts_v2->addParamAttr(1, Attribute::AttrKind::NonNull);

    //   br label %3
    builder.CreateBr(BB3);

    // 3:                                                ; preds = %0, %27
    builder.SetInsertPoint(BB3);

    //   %4 = phi i32 [ 0, %0 ], [ %28, %27 ]
    auto * const v4 = builder.CreatePHI(builder.getInt32Ty(), 2);

    //   %5 = shl i32 %4, 8
    auto * const v5 = builder.CreateShl(v4, 8);

    //   br label %30
    builder.CreateBr(BB30);

    // 6:                                                ; preds = %27, %9
    builder.SetInsertPoint(BB6);

    //   %7 = phi i32 [ %10, %9 ], [ 0, %27 ]
    auto * const v7 = builder.CreatePHI(builder.getInt32Ty(), 2);

    //   %8 = shl i32 %7, 8
    auto * const v8 = builder.CreateShl(v7, 8);

    //   br label %12
    builder.CreateBr(BB12);

    // 9:                                                ; preds = %12
    builder.SetInsertPoint(BB9);

    //   %10 = add nuw nsw i32 %7, 1
    auto * const v10 = builder.CreateAdd(v7, builder.getInt32(1), "", true, true);

    //   %11 = icmp eq i32 %10, 128
    auto * const v11 = builder.CreateICmp(CmpInst::Predicate::ICMP_EQ, v10, builder.getInt32(128));

    //   br i1 %11, label %54, label %6, !llvm.loop !5
    builder.CreateCondBr(v11, BB54, BB6);

    // 12:                                               ; preds = %12, %6
    builder.SetInsertPoint(BB12);

    //   %13 = phi i32 [ 0, %6 ], [ %25, %12 ]
    auto * const v13 = builder.CreatePHI(builder.getInt32Ty(), 2);

    //   %14 = and i32 %13, 224
    auto * const v14 = builder.CreateAnd(v13, builder.getInt32(224));

    //   %15 = or i32 %14, %8
    auto * const v15 = builder.CreateOr(v14, v8);

    //   %16 = lshr exact i32 %15, 5
    auto * const v16 = builder.CreateLShr(v15, builder.getInt32(5), "", true);

    //   %17 = zext i32 %16 to i64
    auto * const v17 = builder.CreateZExt(v16, builder.getInt64Ty());

    //   %18 = getelementptr inbounds i32, ptr %2, i64 %17
    auto * const v18 = builder.CreateGEP(builder.getInt32Ty(), v2, v17, "", true);

    //   %19 = load i32, ptr %18, align 4, !tbaa !7
    auto * const v19 = builder.CreateLoad(builder.getInt32Ty(), v18);
    v19->setAlignment(Align::Constant<4>());

    //   %20 = and i32 %13, 31
    auto * const v20 = builder.CreateAnd(v13, builder.getInt32(31));

    //   %21 = shl nuw i32 1, %20
    auto * const v21 = builder.CreateShl(builder.getInt32(1), v20, "", true);

    //   %22 = and i32 %21, %19
    auto * const v22 = builder.CreateAnd(v21, v19);

    //   %23 = icmp eq i32 %22, 0
    auto * const v23 = builder.CreateICmp(llvm::CmpInst::ICMP_EQ, v22, builder.getInt32(0));

    //   %24 = select i1 %23, i32 -16777216, i32 -16711936
    auto * const v24 = builder.CreateSelect(v23, builder.getInt32(-16777216), builder.getInt32(-16711936));

    //   tail call void @sim_set_pixel(i32 noundef %13, i32 noundef %7, i32 noundef %24) #4
    auto * const ssp_call1 = builder.CreateCall(sim_set_pixel, {v13, v7, v24 });
    ssp_call1->setTailCall();

    ssp_call1->addParamAttr(0, Attribute::AttrKind::NoUndef);
    ssp_call1->addParamAttr(1, Attribute::AttrKind::NoUndef);
    ssp_call1->addParamAttr(2, Attribute::AttrKind::NoUndef);

    //   %25 = add nuw nsw i32 %13, 1
    auto * const v25 = builder.CreateAdd(v13, builder.getInt32(1), "", true, true);

    //   %26 = icmp eq i32 %25, 256
    auto * const v26 = builder.CreateICmp(CmpInst::Predicate::ICMP_EQ, v25, builder.getInt32(256));

    //   br i1 %26, label %9, label %12, !llvm.loop !11
    builder.CreateCondBr(v26, BB9, BB12);

    // 27:                                               ; preds = %51
    builder.SetInsertPoint(BB27);

    //   %28 = add nuw nsw i32 %4, 1
    auto * const v28 = builder.CreateAdd(v4, builder.getInt32(1), "", true, true);

    //   %29 = icmp eq i32 %28, 128
    auto * const v29 = builder.CreateICmp(CmpInst::Predicate::ICMP_EQ, v28, builder.getInt32(128));

    //   br i1 %29, label %6, label %3, !llvm.loop !12
    builder.CreateCondBr(v29, BB6, BB3);

    // 30:                                               ; preds = %3, %51
    builder.SetInsertPoint(BB30);

    //   %31 = phi i32 [ 0, %3 ], [ %52, %51 ]
    auto * const v31 = builder.CreatePHI(builder.getInt32Ty(), 2);

    //   %32 = tail call i32 @sim_rand() #4
    auto * const v32 = builder.CreateCall(sim_rand);
    v32->setTailCall();

    //   %33 = and i32 %32, 1
    auto * const v33 = builder.CreateAnd(v32, builder.getInt32(1));

    //   %34 = or i32 %31, %5
    auto * const v34 = builder.CreateOr(v31, v5);

    //   %35 = icmp eq i32 %33, 0
    auto * const v35 = builder.CreateICmp(llvm::CmpInst::ICMP_EQ, v33, builder.getInt32(0));

    //   %36 = and i32 %31, 31
    auto * const v36 = builder.CreateAnd(v31, builder.getInt32(31));

    //   %37 = shl nuw i32 1, %36
    auto * const v37 = builder.CreateShl(builder.getInt32(1), v36, "", true);

    //   br i1 %35, label %44, label %38
    builder.CreateCondBr(v35, BB44, BB38);

    // 38:                                               ; preds = %30
    builder.SetInsertPoint(BB38);

    //   %39 = lshr i32 %34, 5
    auto * const v39 = builder.CreateLShr(v34, builder.getInt32(5));

    //   %40 = zext i32 %39 to i64
    auto * const v40 = builder.CreateZExt(v39, builder.getInt64Ty());

    //   %41 = getelementptr inbounds i32, ptr %2, i64 %40
    auto * const v41 = builder.CreateGEP(builder.getInt32Ty(), v2, v40, "", true);

    //   %42 = load i32, ptr %41, align 4, !tbaa !7
    auto * const v42 = builder.CreateLoad(builder.getInt32Ty(), v41);
    v42->setAlignment(Align::Constant<4>());

    //   %43 = or i32 %42, %37
    auto * const v43 = builder.CreateOr(v42, v37);

    //   store i32 %43, ptr %41, align 4, !tbaa !7
    auto * const v43_store = builder.CreateStore(v43, v41);
    v43_store->setAlignment(Align::Constant<4>());

    //   br label %51
    builder.CreateBr(BB51);

    // 44:                                               ; preds = %30
    builder.SetInsertPoint(BB44);

    //   %45 = xor i32 %37, -1
    auto * const v45 = builder.CreateXor(v37, builder.getInt32(-1));

    //   %46 = lshr i32 %34, 5
    auto * const v46 = builder.CreateLShr(v34, builder.getInt32(5));

    //   %47 = zext i32 %46 to i64
    auto * const v47 = builder.CreateZExt(v46, builder.getInt64Ty());

    //   %48 = getelementptr inbounds i32, ptr %2, i64 %47
    auto * const v48 = builder.CreateGEP(builder.getInt32Ty(), v2, v47, "", true);

    //   %49 = load i32, ptr %48, align 4, !tbaa !7
    auto * const v49 = builder.CreateLoad(builder.getInt32Ty(), v48);
    v49->setAlignment(Align::Constant<4>());

    //   %50 = and i32 %49, %45
    auto * const v50 = builder.CreateAnd(v49, v45);

    //   store i32 %50, ptr %48, align 4, !tbaa !7
    auto * const v50_store = builder.CreateStore(v50, v48);
    v50_store->setAlignment(Align::Constant<4>());

    //   br label %51
    builder.CreateBr(BB51);

    // 51:                                               ; preds = %38, %44
    builder.SetInsertPoint(BB51);

    //   %52 = add nuw nsw i32 %31, 1
    auto * const v52 = builder.CreateAdd(v31, builder.getInt32(1), "", true, true);

    //   %53 = icmp eq i32 %52, 256
    auto * const v53 = builder.CreateICmp(llvm::CmpInst::ICMP_EQ, v52, builder.getInt32(256));

    //   br i1 %53, label %27, label %30, !llvm.loop !13
    builder.CreateCondBr(v53, BB27, BB30);

    // 54:                                               ; preds = %9, %200
    builder.SetInsertPoint(BB54);

    //   %55 = phi ptr [ %56, %200 ], [ %1, %9 ]
    auto * const v55 = builder.CreatePHI(builder.getPtrTy(), 2);

    //   %56 = phi ptr [ %55, %200 ], [ %2, %9 ]
    auto * const v56 = builder.CreatePHI(builder.getPtrTy(), 2);

    //   tail call void @sim_flush() #4
    auto * const sf_call = builder.CreateCall(sim_flush);
    sf_call->setTailCall();

    //   br label %57
    builder.CreateBr(BB57);

    // 57:                                               ; preds = %67, %54
    builder.SetInsertPoint(BB57);

    //   %58 = phi i32 [ 0, %54 ], [ %172, %67 ]
    auto * const v58 = builder.CreatePHI(builder.getInt32Ty(), 2);

    //   %59 = phi i32 [ 0, %54 ], [ %173, %67 ]
    auto * const v59 = builder.CreatePHI(builder.getInt32Ty(), 2);

    //   %60 = phi i32 [ 0, %54 ], [ %64, %67 ]
    auto * const v60 = builder.CreatePHI(builder.getInt32Ty(), 2);

    //   %61 = shl i32 %60, 8
    auto * const v61 = builder.CreateShl(v60, 8);

    //   %62 = add nuw nsw i32 %61, 32512
    auto * const v62 = builder.CreateAdd(v61, builder.getInt32(32512), "", true, true);

    //   %63 = and i32 %62, 32512
    auto * const v63 = builder.CreateAnd(v62, builder.getInt32(32512));

    //   %64 = add nuw nsw i32 %60, 1
    auto * const v64 = builder.CreateAdd(v60, builder.getInt32(1), "", true, true);

    //   %65 = shl i32 %64, 8
    auto * const v65 = builder.CreateShl(v64, builder.getInt32(8));

    //   %66 = and i32 %65, 32512
    auto * const v66 = builder.CreateAnd(v65, builder.getInt32(32512));

    //   br label %84
    builder.CreateBr(BB84);

    // 67:                                               ; preds = %82
    builder.SetInsertPoint(BB67);

    //   %68 = icmp eq i32 %64, 128
    auto * v68 = builder.CreateICmp(llvm::CmpInst::ICMP_EQ, v64, builder.getInt32(128));

    //   br i1 %68, label %175, label %57, !llvm.loop !14
    builder.CreateCondBr(v68, BB175, BB57);

    // 84:                                               ; preds = %82, %57
    builder.SetInsertPoint(BB84);

    //   %85 = phi i32 [ %58, %57 ], [ %172, %82 ]
    auto * const v85 = builder.CreatePHI(builder.getInt32Ty(), 2);

    //   %86 = phi i32 [ %59, %57 ], [ %173, %82 ]
    auto * const v86 = builder.CreatePHI(builder.getInt32Ty(), 2);

    //   %87 = phi i32 [ 0, %57 ], [ %108, %82 ]
    auto * const v87 = builder.CreatePHI(builder.getInt32Ty(), 2);

    //   %88 = add nsw i32 %87, -1
    auto * const v88 = builder.CreateAdd(v87, builder.getInt32(-1), "", false, true);

    //   %89 = and i32 %88, 224
    auto * const v89 = builder.CreateAnd(v88, builder.getInt32(224));

    //   %90 = or i32 %89, %63
    auto * const v90 = builder.CreateOr(v89, v63);

    //   %91 = lshr exact i32 %90, 5
    auto * const v91 = builder.CreateLShr(v90, builder.getInt32(5), "", true);

    //   %92 = zext i32 %91 to i64
    auto * const v92 = builder.CreateZExt(v91, builder.getInt64Ty());

    //   %93 = getelementptr inbounds i32, ptr %56, i64 %92
    auto * const v93 = builder.CreateGEP(builder.getInt32Ty(), v56, v92, "", true);

    //   %94 = load i32, ptr %93, align 4, !tbaa !7
    auto * const v94 = builder.CreateLoad(builder.getInt32Ty(), v93);
    v94->setAlignment(Align::Constant<4>());

    //   %95 = and i32 %88, 31
    auto * const v95 = builder.CreateAnd(v88, builder.getInt32(31));

    //   %96 = lshr i32 %94, %95
    auto * const v96 = builder.CreateLShr(v94, v95);

    //   %97 = and i32 %96, 1
    auto * const v97 = builder.CreateAnd(v96, builder.getInt32(1));

    //   %98 = and i32 %87, 224
    auto * const v98 = builder.CreateAnd(v87, builder.getInt32(224));

    //   %99 = or i32 %98, %63
    auto * const v99 = builder.CreateOr(v98, v63);

    //   %100 = lshr exact i32 %99, 5
    auto * const v100 = builder.CreateLShr(v99, builder.getInt32(5), "", true);

    //   %101 = zext i32 %100 to i64
    auto * const v101 = builder.CreateZExt(v100, builder.getInt64Ty());

    //   %102 = getelementptr inbounds i32, ptr %56, i64 %101
    auto * const v102 = builder.CreateGEP(builder.getInt32Ty(), v56, v101, "", true);

    //   %103 = load i32, ptr %102, align 4, !tbaa !7
    auto * const v103 = builder.CreateLoad(builder.getInt32Ty(), v102);
    v103->setAlignment(Align::Constant<4>());

    //   %104 = and i32 %87, 31
    auto * const v104 = builder.CreateAnd(v87, builder.getInt32(31));

    //   %105 = lshr i32 %103, %104
    auto * const v105 = builder.CreateLShr(v103, v104);

    //   %106 = and i32 %105, 1
    auto * const v106 = builder.CreateAnd(v105, builder.getInt32(1));

    //   %107 = add nuw nsw i32 %106, %97
    auto * const v107 = builder.CreateAdd(v106, v97, "", true, true);

    //   %108 = add nuw nsw i32 %87, 1
    auto * const v108 = builder.CreateAdd(v87, builder.getInt32(1), "", true, true);

    //   %109 = and i32 %108, 224
    auto * const v109 = builder.CreateAnd(v108, builder.getInt32(224));

    //   %110 = or i32 %109, %63
    auto * const v110 = builder.CreateOr(v109, v63);

    //   %111 = lshr exact i32 %110, 5
    auto * const v111 = builder.CreateLShr(v110, builder.getInt32(5), "", true);

    //   %112 = zext i32 %111 to i64
    auto * const v112 = builder.CreateZExt(v111, builder.getInt64Ty());

    //   %113 = getelementptr inbounds i32, ptr %56, i64 %112
    auto * const v113 = builder.CreateGEP(builder.getInt32Ty(), v56, v112, "", true);

    //   %114 = load i32, ptr %113, align 4, !tbaa !7
    auto * const v114 = builder.CreateLoad(builder.getInt32Ty(), v113);
    v114->setAlignment(Align::Constant<4>());

    //   %115 = and i32 %108, 31
    auto * const v115 = builder.CreateAnd(v108, builder.getInt32(31));

    //   %116 = lshr i32 %114, %115
    auto * const v116 = builder.CreateLShr(v114, v115);

    //   %117 = and i32 %116, 1
    auto * const v117 = builder.CreateAnd(v116, builder.getInt32(1));

    //   %118 = add nuw nsw i32 %107, %117
    auto * const v118 = builder.CreateAdd(v107, v117, "", true, true);

    //   %119 = or i32 %89, %61
    auto * const v119 = builder.CreateOr(v89, v61);

    //   %120 = lshr exact i32 %119, 5
    auto * const v120 = builder.CreateLShr(v119, builder.getInt32(5), "", true);

    //   %121 = zext i32 %120 to i64
    auto * const v121 = builder.CreateZExt(v120, builder.getInt64Ty());

    //   %122 = getelementptr inbounds i32, ptr %56, i64 %121
    auto * const v122 = builder.CreateGEP(builder.getInt32Ty(), v56, v121, "", true);

    //   %123 = load i32, ptr %122, align 4, !tbaa !7
    auto * const v123 = builder.CreateLoad(builder.getInt32Ty(), v122);
    v123->setAlignment(Align::Constant<4>());

    //   %124 = lshr i32 %123, %95
    auto * const v124 = builder.CreateLShr(v123, v95);

    //   %125 = and i32 %124, 1
    auto * const v125 = builder.CreateAnd(v124, builder.getInt32(1));

    //   %126 = add nuw nsw i32 %118, %125
    auto * const v126 = builder.CreateAdd(v118, v125, "", true, true);

    //   %127 = or i32 %109, %61
    auto * const v127 = builder.CreateOr(v109, v61);

    //   %128 = lshr exact i32 %127, 5
    auto * const v128 = builder.CreateLShr(v127, builder.getInt32(5), "", true);

    //   %129 = zext i32 %128 to i64
    auto * const v129 = builder.CreateZExt(v128, builder.getInt64Ty());

    //   %130 = getelementptr inbounds i32, ptr %56, i64 %129
    auto * const v130 = builder.CreateGEP(builder.getInt32Ty(), v56, v129, "", true);

    //   %131 = load i32, ptr %130, align 4, !tbaa !7
    auto * const v131 = builder.CreateLoad(builder.getInt32Ty(), v130);
    v131->setAlignment(Align::Constant<4>());

    //   %132 = lshr i32 %131, %115
    auto * const v132 = builder.CreateLShr(v131, v115);

    //   %133 = and i32 %132, 1
    auto * const v133 = builder.CreateAnd(v132, builder.getInt32(1));

    //   %134 = add nuw nsw i32 %126, %133
    auto * const v134 = builder.CreateAdd(v126, v133, "", true, true);

    //   %135 = or i32 %89, %66
    auto * const v135 = builder.CreateOr(v89, v66);

    //   %136 = lshr exact i32 %135, 5
    auto * const v136 = builder.CreateLShr(v135, builder.getInt32(5), "", true);

    //   %137 = zext i32 %136 to i64
    auto * const v137 = builder.CreateZExt(v136, builder.getInt64Ty());

    //   %138 = getelementptr inbounds i32, ptr %56, i64 %137
    auto * const v138 = builder.CreateGEP(builder.getInt32Ty(), v56, v137, "", true);

    //   %139 = load i32, ptr %138, align 4, !tbaa !7
    auto * const v139 = builder.CreateLoad(builder.getInt32Ty(), v138);
    v139->setAlignment(Align::Constant<4>());

    //   %140 = lshr i32 %139, %95
    auto * const v140 = builder.CreateLShr(v139, v95);

    //   %141 = and i32 %140, 1
    auto * const v141 = builder.CreateAnd(v140, builder.getInt32(1));

    //   %142 = add nuw nsw i32 %134, %141
    auto * const v142 = builder.CreateAdd(v134, v141, "", true, true);

    //   %143 = or i32 %98, %66
    auto * const v143 = builder.CreateOr(v98, v66);

    //   %144 = lshr exact i32 %143, 5
    auto * const v144 = builder.CreateLShr(v143, builder.getInt32(5), "", true);

    //   %145 = zext i32 %144 to i64
    auto * const v145 = builder.CreateZExt(v144, builder.getInt64Ty());

    //   %146 = getelementptr inbounds i32, ptr %56, i64 %145
    auto * const v146 = builder.CreateGEP(builder.getInt32Ty(), v56, v145, "", true);

    //   %147 = load i32, ptr %146, align 4, !tbaa !7
    auto * const v147 = builder.CreateLoad(builder.getInt32Ty(), v146);
    v147->setAlignment(Align::Constant<4>());

    //   %148 = lshr i32 %147, %104
    auto * const v148 = builder.CreateLShr(v147, v104);

    //   %149 = and i32 %148, 1
    auto * const v149 = builder.CreateAnd(v148, builder.getInt32(1));

    //   %150 = add nuw nsw i32 %142, %149
    auto * const v150 = builder.CreateAdd(v142, v149, "", true, true);

    //   %151 = or i32 %109, %66
    auto * const v151 = builder.CreateOr(v109, v66);

    //   %152 = lshr exact i32 %151, 5
    auto * const v152 = builder.CreateLShr(v151, builder.getInt32(5), "", true);

    //   %153 = zext i32 %152 to i64
    auto * const v153 = builder.CreateZExt(v152, builder.getInt64Ty());

    //   %154 = getelementptr inbounds i32, ptr %56, i64 %153
    auto * const v154 = builder.CreateGEP(builder.getInt32Ty(), v56, v153, "", true);

    //   %155 = load i32, ptr %154, align 4, !tbaa !7
    auto * const v155 = builder.CreateLoad(builder.getInt32Ty(), v154);
    v155->setAlignment(Align::Constant<4>());

    //   %156 = lshr i32 %155, %115
    auto * const v156 = builder.CreateLShr(v155, v115);

    //   %157 = and i32 %156, 1
    auto * const v157 = builder.CreateAnd(v156, builder.getInt32(1));

    //   %158 = add nuw nsw i32 %150, %157
    auto * const v158 = builder.CreateAdd(v150, v157, "", true, true);

    //   %159 = or i32 %98, %61
    auto * const v159 = builder.CreateOr(v98, v61);

    //   %160 = lshr exact i32 %159, 5
    auto * const v160 = builder.CreateLShr(v159, builder.getInt32(5), "", true);

    //   %161 = zext i32 %160 to i64
    auto * const v161 = builder.CreateZExt(v160, builder.getInt64Ty());

    //   %162 = getelementptr inbounds i32, ptr %56, i64 %161
    auto * const v162 = builder.CreateGEP(builder.getInt32Ty(), v56, v161, "", true);

    //   %163 = load i32, ptr %162, align 4, !tbaa !7
    auto * const v163 = builder.CreateLoad(builder.getInt32Ty(), v162);
    v163->setAlignment(Align::Constant<4>());

    //   %164 = shl nuw i32 1, %104
    auto * const v164 = builder.CreateShl(builder.getInt32(1), v104, "", true);

    //   %165 = and i32 %163, %164
    auto * const v165 = builder.CreateAnd(v163, v164);

    //   %166 = icmp eq i32 %165, 0
    auto * const v166 = builder.CreateICmp(llvm::CmpInst::ICMP_EQ, v165, builder.getInt32(0));

    //   %167 = and i32 %158, -2
    auto * const v167 = builder.CreateAnd(v158, builder.getInt32(-2));

    //   %168 = icmp eq i32 %167, 2
    auto * const v168 = builder.CreateICmp(llvm::CmpInst::ICMP_EQ, v167, builder.getInt32(2));

    //   %169 = icmp eq i32 %158, 3
    auto * const v169 = builder.CreateICmp(llvm::CmpInst::ICMP_EQ, v158, builder.getInt32(3));

    //   %170 = select i1 %166, i1 %169, i1 %168
    auto * const v170 = builder.CreateSelect(v166, v169, v168);

    //   %171 = xor i1 %166, %170
    auto * const v171 = builder.CreateXor(v166, v170);

    //   %172 = select i1 %171, i32 %85, i32 1
    auto * const v172 = builder.CreateSelect(v171, v85, builder.getInt32(1));

    //   %173 = select i1 %170, i32 1, i32 %86
    auto * const v173 = builder.CreateSelect(v170, builder.getInt32(1), v86);

    //   %174 = or i32 %87, %61
    auto * const v174 = builder.CreateOr(v87, v61);

    //   br i1 %170, label %69, label %75
    builder.CreateCondBr(v170, BB69, BB75);

    // 69:                                               ; preds = %84
    builder.SetInsertPoint(BB69);

    //   %70 = lshr i32 %174, 5
    auto * const v70 = builder.CreateLShr(v174, builder.getInt32(5));

    //   %71 = zext i32 %70 to i64
    auto * const v71 = builder.CreateZExt(v70, builder.getInt64Ty());

    //   %72 = getelementptr inbounds i32, ptr %55, i64 %71
    auto * const v72 = builder.CreateGEP(builder.getInt32Ty(), v55, v71, "", true);

    //   %73 = load i32, ptr %72, align 4, !tbaa !7
    auto * const v73 = builder.CreateLoad(builder.getInt32Ty(), v72);
    v73->setAlignment(Align::Constant<4>());

    //   %74 = or i32 %73, %164
    auto * const v74 = builder.CreateOr(v73, v164);

    //   store i32 %74, ptr %72, align 4, !tbaa !7
    auto * const v74_store = builder.CreateStore(v74, v72);
    v74_store->setAlignment(Align::Constant<4>());

    //   br label %82
    builder.CreateBr(BB82);

    // 75:                                               ; preds = %84
    builder.SetInsertPoint(BB75);

    //   %76 = xor i32 %164, -1
    auto * const v76 = builder.CreateXor(v164, builder.getInt32(-1));

    //   %77 = lshr i32 %174, 5
    auto * const v77 = builder.CreateLShr(v174, builder.getInt32(5));

    //   %78 = zext i32 %77 to i64
    auto * const v78 = builder.CreateZExt(v77, builder.getInt64Ty());

    //   %79 = getelementptr inbounds i32, ptr %55, i64 %78
    auto * const v79 = builder.CreateGEP(builder.getInt32Ty(), v55, v78, "", true);

    //   %80 = load i32, ptr %79, align 4, !tbaa !7
    auto * const v80 = builder.CreateLoad(builder.getInt32Ty(), v79);
    v80->setAlignment(Align::Constant<4>());

    //   %81 = and i32 %80, %76
    auto * const v81 = builder.CreateAnd(v80, v76);

    //   store i32 %81, ptr %79, align 4, !tbaa !7
    auto * const v81_store = builder.CreateStore(v81, v79);
    v81_store->setAlignment(Align::Constant<4>());

    //   br label %82
    builder.CreateBr(BB82);

    // 82:                                               ; preds = %75, %69
    builder.SetInsertPoint(BB82);

    //   %83 = icmp eq i32 %108, 256
    auto * const v83 = builder.CreateICmp(llvm::CmpInst::ICMP_EQ, v108, builder.getInt32(256));

    //   br i1 %83, label %67, label %84, !llvm.loop !15
    builder.CreateCondBr(v83, BB67, BB84);

    // 175:                                              ; preds = %67
    builder.SetInsertPoint(BB175);

    //   %176 = icmp eq i32 %172, 0
    auto * const v176 = builder.CreateICmp(llvm::CmpInst::ICMP_EQ, v172, builder.getInt32(0));

    //   %177 = icmp eq i32 %173, 0
    auto * const v177 = builder.CreateICmp(llvm::CmpInst::ICMP_EQ, v173, builder.getInt32(0));

    //   %178 = select i1 %176, i1 %177, i1 false
    auto * const v178 = builder.CreateSelect(v176, v177, builder.getInt1(false));

    //   br i1 %178, label %201, label %179
    builder.CreateCondBr(v178, BB201, BB179);

    // 179:                                              ; preds = %175, %182
    builder.SetInsertPoint(BB179);

    //   %180 = phi i32 [ %183, %182 ], [ 0, %175 ]
    auto * const v180 = builder.CreatePHI(builder.getInt32Ty(), 2);

    //   %181 = shl i32 %180, 8
    auto * const v181 = builder.CreateShl(v180, 8);

    //   br label %185
    builder.CreateBr(BB185);

    // 182:                                              ; preds = %185
    builder.SetInsertPoint(BB182);

    //   %183 = add nuw nsw i32 %180, 1
    auto * const v183 = builder.CreateAdd(v180, builder.getInt32(1), "", true, true);

    //   %184 = icmp eq i32 %183, 128
    auto * const v184 = builder.CreateICmp(llvm::CmpInst::ICMP_EQ, v183, builder.getInt32(128));

    //   br i1 %184, label %200, label %179, !llvm.loop !5
    builder.CreateCondBr(v184, BB200, BB179);

    // 185:                                              ; preds = %185, %179
    builder.SetInsertPoint(BB185);

    //   %186 = phi i32 [ 0, %179 ], [ %198, %185 ]
    auto * const v186 = builder.CreatePHI(builder.getInt32Ty(), 2);

    //   %187 = and i32 %186, 224
    auto * const v187 = builder.CreateAnd(v186, builder.getInt32(224));

    //   %188 = or i32 %187, %181
    auto * const v188 = builder.CreateOr(v187, v181);

    //   %189 = lshr exact i32 %188, 5
    auto * const v189 = builder.CreateLShr(v188, builder.getInt32(5), "", true);

    //   %190 = zext i32 %189 to i64
    auto * const v190 = builder.CreateZExt(v189, builder.getInt64Ty());

    //   %191 = getelementptr inbounds i32, ptr %55, i64 %190
    auto * const v191 = builder.CreateGEP(builder.getInt32Ty(), v55, v190, "", true);

    //   %192 = load i32, ptr %191, align 4, !tbaa !7
    auto * const v192 = builder.CreateLoad(builder.getInt32Ty(), v191);
    v192->setAlignment(Align::Constant<4>());

    //   %193 = and i32 %186, 31
    auto * const v193 = builder.CreateAnd(v186, builder.getInt32(31));

    //   %194 = shl nuw i32 1, %193
    auto * const v194 = builder.CreateShl(builder.getInt32(1), v193, "", true);

    //   %195 = and i32 %194, %192
    auto * const v195 = builder.CreateAnd(v194, v192);

    //   %196 = icmp eq i32 %195, 0
    auto * const v196 = builder.CreateICmp(llvm::CmpInst::ICMP_EQ, v195, builder.getInt32(0));

    //   %197 = select i1 %196, i32 -16777216, i32 -16711936
    auto * const v197 = builder.CreateSelect(v196, builder.getInt32(-16777216), builder.getInt32(-16711936));

    //   tail call void @sim_set_pixel(i32 noundef %186, i32 noundef %180, i32 noundef %197) #4
    auto * const ssp_call2 = builder.CreateCall(sim_set_pixel, {v186, v180, v197 });
    ssp_call2->setTailCall();

    ssp_call2->addParamAttr(0, Attribute::AttrKind::NoUndef);
    ssp_call2->addParamAttr(1, Attribute::AttrKind::NoUndef);
    ssp_call2->addParamAttr(2, Attribute::AttrKind::NoUndef);

    //   %198 = add nuw nsw i32 %186, 1
    auto * const v198 = builder.CreateAdd(v186, builder.getInt32(1), "", true, true);

    //   %199 = icmp eq i32 %198, 256
    auto * const v199 = builder.CreateICmp(llvm::CmpInst::ICMP_EQ, v198, builder.getInt32(256));

    //   br i1 %199, label %182, label %185, !llvm.loop !11
    builder.CreateCondBr(v199, BB182, BB185);

    // 200:                                              ; preds = %182
    builder.SetInsertPoint(BB200);

    //   br label %54, !llvm.loop !16
    builder.CreateBr(BB54);

    // 201:                                              ; preds = %175
    builder.SetInsertPoint(BB201);

    //   call void @llvm.lifetime.end.p0(i64 4096, ptr nonnull %2) #4
    builder.CreateCall(llvm_lifetime_end_p0, { builder.getInt64(4096), v2 });

    //   call void @llvm.lifetime.end.p0(i64 4096, ptr nonnull %1) #4
    builder.CreateCall(llvm_lifetime_end_p0, { builder.getInt64(4096), v1 });

    //   ret i32 0
    builder.CreateRet(builder.getInt32(0));

    // }

    // %4 = phi i32 [ 0, %0 ], [ %28, %27 ]
    v4->addIncoming(builder.getInt32(0), BB0);
    v4->addIncoming(v28, BB27);

    // %7 = phi i32 [ %10, %9 ], [ 0, %27 ]
    v7->addIncoming(v10, BB9);
    v7->addIncoming(builder.getInt32(0), BB27);

    // %13 = phi i32 [ 0, %6 ], [ %25, %12 ]
    v13->addIncoming(builder.getInt32(0), BB6);
    v13->addIncoming(v25, BB12);

    // %31 = phi i32 [ 0, %3 ], [ %52, %51 ]
    v31->addIncoming(builder.getInt32(0), BB3);
    v31->addIncoming(v52, BB51);

    // %55 = phi ptr [ %56, %200 ], [ %1, %9 ]
    v55->addIncoming(v56, BB200);
    v55->addIncoming(v1, BB9);

    // %56 = phi ptr [ %55, %200 ], [ %2, %9 ]
    v56->addIncoming(v55, BB200);
    v56->addIncoming(v2, BB9);

    // %58 = phi i32 [ 0, %54 ], [ %172, %67 ]
    v58->addIncoming(builder.getInt32(0), BB54);
    v58->addIncoming(v172, BB67);

    // %59 = phi i32 [ 0, %54 ], [ %173, %67 ]
    v59->addIncoming(builder.getInt32(0), BB54);
    v59->addIncoming(v173, BB67);

    // %60 = phi i32 [ 0, %54 ], [ %64, %67 ]
    v60->addIncoming(builder.getInt32(0), BB54);
    v60->addIncoming(v64, BB67);

    // %85 = phi i32 [ %58, %57 ], [ %172, %82 ]
    v85->addIncoming(v58, BB57);
    v85->addIncoming(v172, BB82);

    // %86 = phi i32 [ %59, %57 ], [ %173, %82 ]
    v86->addIncoming(v59, BB57);
    v86->addIncoming(v173, BB82);

    // %87 = phi i32 [ 0, %57 ], [ %108, %82 ]
    v87->addIncoming(builder.getInt32(0), BB57);
    v87->addIncoming(v108, BB82);

    // %180 = phi i32 [ %183, %182 ], [ 0, %175 ]
    v180->addIncoming(v183, BB182);
    v180->addIncoming(builder.getInt32(0), BB175);

    // %186 = phi i32 [ 0, %179 ], [ %198, %185 ]
    v186->addIncoming(builder.getInt32(0), BB179);
    v186->addIncoming(v198, BB185);

    // TODO
    // !llvm.module.flags = !{!0, !1, !2, !3}
    // !llvm.ident = !{!4}

    // !0 = !{i32 1, !"wchar_size", i32 4}
    // !1 = !{i32 8, !"PIC Level", i32 2}
    // !2 = !{i32 7, !"PIE Level", i32 2}
    // !3 = !{i32 7, !"uwtable", i32 2}
    // !4 = !{!"clang version 16.0.6"}
    // !5 = distinct !{!5, !6}
    // !6 = !{!"llvm.loop.mustprogress"}
    // !7 = !{!8, !8, i64 0}
    // !8 = !{!"int", !9, i64 0}
    // !9 = !{!"omnipotent char", !10, i64 0}
    // !10 = !{!"Simple C/C++ TBAA"}
    // !11 = distinct !{!11, !6}
    // !12 = distinct !{!12, !6}
    // !13 = distinct !{!13, !6}
    // !14 = distinct !{!14, !6}
    // !15 = distinct !{!15, !6}
    // !16 = distinct !{!16, !6}

    return main_;
}

int main() {
    LLVMContext context;

    // ; ModuleID = 'src/main.c'
    // source_filename = "src/main.c"
    auto * const module = new Module("src/main.c", context);

    // target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
    module->setDataLayout("e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128");

    // target triple = "x86_64-pc-linux-gnu"
    module->setTargetTriple("x86_64-pc-linux-gnu");

    auto * const main_function = generate_main(module);

    std::error_code err;
    raw_fd_stream fos { "main.gen.ll", err };
    module->print(fos, nullptr);

    LLVMInitializeNativeTarget();
    LLVMInitializeNativeAsmPrinter();

    ExecutionEngine * const ee = EngineBuilder(std::unique_ptr<Module>(module)).create();

    ee->InstallLazyFunctionCreator([&](const std::string & name) -> void * {
        if (name == "sim_set_pixel") {
            return reinterpret_cast<void *>(sim_set_pixel);
        }

        if (name == "sim_flush") {
            return reinterpret_cast<void *>(sim_flush);
        }

        if (name == "sim_rand") {
            return reinterpret_cast<void *>(sim_rand);
        }

        if (name == "__stack_chk_fail") {
            return reinterpret_cast<void *>(stack_chk_fail_mock);
        }

        outs() << "Not found " << name << "\n";
        return nullptr;
    });

    ee->finalizeObject();

    const GenericValue result = ee->runFunction(main_function, {});
}
