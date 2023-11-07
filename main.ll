; ModuleID = 'hw1/src/main.c'
source_filename = "hw1/src/main.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@0 = private unnamed_addr constant [37 x i8] c"  %1 = alloca [1024 x i32], align 16\00", align 1
@1 = private unnamed_addr constant [37 x i8] c"  %2 = alloca [1024 x i32], align 16\00", align 1
@2 = private unnamed_addr constant [65 x i8] c"  call void @llvm.lifetime.start.p0(i64 4096, ptr nonnull %1) #4\00", align 1
@3 = private unnamed_addr constant [113 x i8] c"  call void @llvm.memset.p0.i64(ptr noundef nonnull align 16 dereferenceable(4096) %1, i8 0, i64 4096, i1 false)\00", align 1
@4 = private unnamed_addr constant [65 x i8] c"  call void @llvm.lifetime.start.p0(i64 4096, ptr nonnull %2) #4\00", align 1
@5 = private unnamed_addr constant [113 x i8] c"  call void @llvm.memset.p0.i64(ptr noundef nonnull align 16 dereferenceable(4096) %2, i8 0, i64 4096, i1 false)\00", align 1
@6 = private unnamed_addr constant [14 x i8] c"  br label %3\00", align 1
@7 = private unnamed_addr constant [21 x i8] c"  %5 = shl i32 %4, 8\00", align 1
@8 = private unnamed_addr constant [15 x i8] c"  br label %30\00", align 1
@9 = private unnamed_addr constant [21 x i8] c"  %8 = shl i32 %7, 8\00", align 1
@10 = private unnamed_addr constant [15 x i8] c"  br label %12\00", align 1
@11 = private unnamed_addr constant [30 x i8] c"  %10 = add nuw nsw i32 %7, 1\00", align 1
@12 = private unnamed_addr constant [29 x i8] c"  %11 = icmp eq i32 %10, 128\00", align 1
@13 = private unnamed_addr constant [48 x i8] c"  br i1 %11, label %54, label %6, !llvm.loop !5\00", align 1
@14 = private unnamed_addr constant [25 x i8] c"  %14 = and i32 %13, 224\00", align 1
@15 = private unnamed_addr constant [23 x i8] c"  %15 = or i32 %14, %8\00", align 1
@16 = private unnamed_addr constant [30 x i8] c"  %16 = lshr exact i32 %15, 5\00", align 1
@17 = private unnamed_addr constant [28 x i8] c"  %17 = zext i32 %16 to i64\00", align 1
@18 = private unnamed_addr constant [52 x i8] c"  %18 = getelementptr inbounds i32, ptr %2, i64 %17\00", align 1
@19 = private unnamed_addr constant [45 x i8] c"  %19 = load i32, ptr %18, align 4, !tbaa !7\00", align 1
@20 = private unnamed_addr constant [24 x i8] c"  %20 = and i32 %13, 31\00", align 1
@21 = private unnamed_addr constant [27 x i8] c"  %21 = shl nuw i32 1, %20\00", align 1
@22 = private unnamed_addr constant [25 x i8] c"  %22 = and i32 %21, %19\00", align 1
@23 = private unnamed_addr constant [27 x i8] c"  %23 = icmp eq i32 %22, 0\00", align 1
@24 = private unnamed_addr constant [52 x i8] c"  %24 = select i1 %23, i32 -16777216, i32 -16711936\00", align 1
@25 = private unnamed_addr constant [85 x i8] c"  tail call void @sim_set_pixel(i32 noundef %13, i32 noundef %7, i32 noundef %24) #4\00", align 1
@26 = private unnamed_addr constant [31 x i8] c"  %25 = add nuw nsw i32 %13, 1\00", align 1
@27 = private unnamed_addr constant [29 x i8] c"  %26 = icmp eq i32 %25, 256\00", align 1
@28 = private unnamed_addr constant [49 x i8] c"  br i1 %26, label %9, label %12, !llvm.loop !11\00", align 1
@29 = private unnamed_addr constant [30 x i8] c"  %28 = add nuw nsw i32 %4, 1\00", align 1
@30 = private unnamed_addr constant [29 x i8] c"  %29 = icmp eq i32 %28, 128\00", align 1
@31 = private unnamed_addr constant [48 x i8] c"  br i1 %29, label %6, label %3, !llvm.loop !12\00", align 1
@32 = private unnamed_addr constant [37 x i8] c"  %32 = tail call i32 @sim_rand() #4\00", align 1
@33 = private unnamed_addr constant [23 x i8] c"  %33 = and i32 %32, 1\00", align 1
@34 = private unnamed_addr constant [23 x i8] c"  %34 = or i32 %31, %5\00", align 1
@35 = private unnamed_addr constant [27 x i8] c"  %35 = icmp eq i32 %33, 0\00", align 1
@36 = private unnamed_addr constant [24 x i8] c"  %36 = and i32 %31, 31\00", align 1
@37 = private unnamed_addr constant [27 x i8] c"  %37 = shl nuw i32 1, %36\00", align 1
@38 = private unnamed_addr constant [34 x i8] c"  br i1 %35, label %44, label %38\00", align 1
@39 = private unnamed_addr constant [24 x i8] c"  %39 = lshr i32 %34, 5\00", align 1
@40 = private unnamed_addr constant [28 x i8] c"  %40 = zext i32 %39 to i64\00", align 1
@41 = private unnamed_addr constant [52 x i8] c"  %41 = getelementptr inbounds i32, ptr %2, i64 %40\00", align 1
@42 = private unnamed_addr constant [45 x i8] c"  %42 = load i32, ptr %41, align 4, !tbaa !7\00", align 1
@43 = private unnamed_addr constant [24 x i8] c"  %43 = or i32 %42, %37\00", align 1
@44 = private unnamed_addr constant [44 x i8] c"  store i32 %43, ptr %41, align 4, !tbaa !7\00", align 1
@45 = private unnamed_addr constant [24 x i8] c"  %45 = xor i32 %37, -1\00", align 1
@46 = private unnamed_addr constant [24 x i8] c"  %46 = lshr i32 %34, 5\00", align 1
@47 = private unnamed_addr constant [28 x i8] c"  %47 = zext i32 %46 to i64\00", align 1
@48 = private unnamed_addr constant [52 x i8] c"  %48 = getelementptr inbounds i32, ptr %2, i64 %47\00", align 1
@49 = private unnamed_addr constant [45 x i8] c"  %49 = load i32, ptr %48, align 4, !tbaa !7\00", align 1
@50 = private unnamed_addr constant [25 x i8] c"  %50 = and i32 %49, %45\00", align 1
@51 = private unnamed_addr constant [44 x i8] c"  store i32 %50, ptr %48, align 4, !tbaa !7\00", align 1
@52 = private unnamed_addr constant [15 x i8] c"  br label %51\00", align 1
@53 = private unnamed_addr constant [31 x i8] c"  %52 = add nuw nsw i32 %31, 1\00", align 1
@54 = private unnamed_addr constant [29 x i8] c"  %53 = icmp eq i32 %52, 256\00", align 1
@55 = private unnamed_addr constant [50 x i8] c"  br i1 %53, label %27, label %30, !llvm.loop !13\00", align 1
@56 = private unnamed_addr constant [33 x i8] c"  tail call void @sim_flush() #4\00", align 1
@57 = private unnamed_addr constant [15 x i8] c"  br label %57\00", align 1
@58 = private unnamed_addr constant [23 x i8] c"  %61 = shl i32 %60, 8\00", align 1
@59 = private unnamed_addr constant [35 x i8] c"  %62 = add nuw nsw i32 %61, 32512\00", align 1
@60 = private unnamed_addr constant [27 x i8] c"  %63 = and i32 %62, 32512\00", align 1
@61 = private unnamed_addr constant [31 x i8] c"  %64 = add nuw nsw i32 %60, 1\00", align 1
@62 = private unnamed_addr constant [23 x i8] c"  %65 = shl i32 %64, 8\00", align 1
@63 = private unnamed_addr constant [27 x i8] c"  %66 = and i32 %65, 32512\00", align 1
@64 = private unnamed_addr constant [15 x i8] c"  br label %84\00", align 1
@65 = private unnamed_addr constant [29 x i8] c"  %68 = icmp eq i32 %64, 128\00", align 1
@66 = private unnamed_addr constant [51 x i8] c"  br i1 %68, label %175, label %57, !llvm.loop !14\00", align 1
@67 = private unnamed_addr constant [25 x i8] c"  %70 = lshr i32 %174, 5\00", align 1
@68 = private unnamed_addr constant [28 x i8] c"  %71 = zext i32 %70 to i64\00", align 1
@69 = private unnamed_addr constant [53 x i8] c"  %72 = getelementptr inbounds i32, ptr %55, i64 %71\00", align 1
@70 = private unnamed_addr constant [45 x i8] c"  %73 = load i32, ptr %72, align 4, !tbaa !7\00", align 1
@71 = private unnamed_addr constant [25 x i8] c"  %74 = or i32 %73, %164\00", align 1
@72 = private unnamed_addr constant [44 x i8] c"  store i32 %74, ptr %72, align 4, !tbaa !7\00", align 1
@73 = private unnamed_addr constant [25 x i8] c"  %76 = xor i32 %164, -1\00", align 1
@74 = private unnamed_addr constant [25 x i8] c"  %77 = lshr i32 %174, 5\00", align 1
@75 = private unnamed_addr constant [28 x i8] c"  %78 = zext i32 %77 to i64\00", align 1
@76 = private unnamed_addr constant [53 x i8] c"  %79 = getelementptr inbounds i32, ptr %55, i64 %78\00", align 1
@77 = private unnamed_addr constant [45 x i8] c"  %80 = load i32, ptr %79, align 4, !tbaa !7\00", align 1
@78 = private unnamed_addr constant [25 x i8] c"  %81 = and i32 %80, %76\00", align 1
@79 = private unnamed_addr constant [44 x i8] c"  store i32 %81, ptr %79, align 4, !tbaa !7\00", align 1
@80 = private unnamed_addr constant [15 x i8] c"  br label %82\00", align 1
@81 = private unnamed_addr constant [30 x i8] c"  %83 = icmp eq i32 %108, 256\00", align 1
@82 = private unnamed_addr constant [50 x i8] c"  br i1 %83, label %67, label %84, !llvm.loop !15\00", align 1
@83 = private unnamed_addr constant [28 x i8] c"  %88 = add nsw i32 %87, -1\00", align 1
@84 = private unnamed_addr constant [25 x i8] c"  %89 = and i32 %88, 224\00", align 1
@85 = private unnamed_addr constant [24 x i8] c"  %90 = or i32 %89, %63\00", align 1
@86 = private unnamed_addr constant [30 x i8] c"  %91 = lshr exact i32 %90, 5\00", align 1
@87 = private unnamed_addr constant [28 x i8] c"  %92 = zext i32 %91 to i64\00", align 1
@88 = private unnamed_addr constant [53 x i8] c"  %93 = getelementptr inbounds i32, ptr %56, i64 %92\00", align 1
@89 = private unnamed_addr constant [45 x i8] c"  %94 = load i32, ptr %93, align 4, !tbaa !7\00", align 1
@90 = private unnamed_addr constant [24 x i8] c"  %95 = and i32 %88, 31\00", align 1
@91 = private unnamed_addr constant [26 x i8] c"  %96 = lshr i32 %94, %95\00", align 1
@92 = private unnamed_addr constant [23 x i8] c"  %97 = and i32 %96, 1\00", align 1
@93 = private unnamed_addr constant [25 x i8] c"  %98 = and i32 %87, 224\00", align 1
@94 = private unnamed_addr constant [24 x i8] c"  %99 = or i32 %98, %63\00", align 1
@95 = private unnamed_addr constant [31 x i8] c"  %100 = lshr exact i32 %99, 5\00", align 1
@96 = private unnamed_addr constant [30 x i8] c"  %101 = zext i32 %100 to i64\00", align 1
@97 = private unnamed_addr constant [55 x i8] c"  %102 = getelementptr inbounds i32, ptr %56, i64 %101\00", align 1
@98 = private unnamed_addr constant [47 x i8] c"  %103 = load i32, ptr %102, align 4, !tbaa !7\00", align 1
@99 = private unnamed_addr constant [25 x i8] c"  %104 = and i32 %87, 31\00", align 1
@100 = private unnamed_addr constant [29 x i8] c"  %105 = lshr i32 %103, %104\00", align 1
@101 = private unnamed_addr constant [25 x i8] c"  %106 = and i32 %105, 1\00", align 1
@102 = private unnamed_addr constant [35 x i8] c"  %107 = add nuw nsw i32 %106, %97\00", align 1
@103 = private unnamed_addr constant [32 x i8] c"  %108 = add nuw nsw i32 %87, 1\00", align 1
@104 = private unnamed_addr constant [27 x i8] c"  %109 = and i32 %108, 224\00", align 1
@105 = private unnamed_addr constant [26 x i8] c"  %110 = or i32 %109, %63\00", align 1
@106 = private unnamed_addr constant [32 x i8] c"  %111 = lshr exact i32 %110, 5\00", align 1
@107 = private unnamed_addr constant [30 x i8] c"  %112 = zext i32 %111 to i64\00", align 1
@108 = private unnamed_addr constant [55 x i8] c"  %113 = getelementptr inbounds i32, ptr %56, i64 %112\00", align 1
@109 = private unnamed_addr constant [47 x i8] c"  %114 = load i32, ptr %113, align 4, !tbaa !7\00", align 1
@110 = private unnamed_addr constant [26 x i8] c"  %115 = and i32 %108, 31\00", align 1
@111 = private unnamed_addr constant [29 x i8] c"  %116 = lshr i32 %114, %115\00", align 1
@112 = private unnamed_addr constant [25 x i8] c"  %117 = and i32 %116, 1\00", align 1
@113 = private unnamed_addr constant [36 x i8] c"  %118 = add nuw nsw i32 %107, %117\00", align 1
@114 = private unnamed_addr constant [25 x i8] c"  %119 = or i32 %89, %61\00", align 1
@115 = private unnamed_addr constant [32 x i8] c"  %120 = lshr exact i32 %119, 5\00", align 1
@116 = private unnamed_addr constant [30 x i8] c"  %121 = zext i32 %120 to i64\00", align 1
@117 = private unnamed_addr constant [55 x i8] c"  %122 = getelementptr inbounds i32, ptr %56, i64 %121\00", align 1
@118 = private unnamed_addr constant [47 x i8] c"  %123 = load i32, ptr %122, align 4, !tbaa !7\00", align 1
@119 = private unnamed_addr constant [28 x i8] c"  %124 = lshr i32 %123, %95\00", align 1
@120 = private unnamed_addr constant [25 x i8] c"  %125 = and i32 %124, 1\00", align 1
@121 = private unnamed_addr constant [36 x i8] c"  %126 = add nuw nsw i32 %118, %125\00", align 1
@122 = private unnamed_addr constant [26 x i8] c"  %127 = or i32 %109, %61\00", align 1
@123 = private unnamed_addr constant [32 x i8] c"  %128 = lshr exact i32 %127, 5\00", align 1
@124 = private unnamed_addr constant [30 x i8] c"  %129 = zext i32 %128 to i64\00", align 1
@125 = private unnamed_addr constant [55 x i8] c"  %130 = getelementptr inbounds i32, ptr %56, i64 %129\00", align 1
@126 = private unnamed_addr constant [47 x i8] c"  %131 = load i32, ptr %130, align 4, !tbaa !7\00", align 1
@127 = private unnamed_addr constant [29 x i8] c"  %132 = lshr i32 %131, %115\00", align 1
@128 = private unnamed_addr constant [25 x i8] c"  %133 = and i32 %132, 1\00", align 1
@129 = private unnamed_addr constant [36 x i8] c"  %134 = add nuw nsw i32 %126, %133\00", align 1
@130 = private unnamed_addr constant [25 x i8] c"  %135 = or i32 %89, %66\00", align 1
@131 = private unnamed_addr constant [32 x i8] c"  %136 = lshr exact i32 %135, 5\00", align 1
@132 = private unnamed_addr constant [30 x i8] c"  %137 = zext i32 %136 to i64\00", align 1
@133 = private unnamed_addr constant [55 x i8] c"  %138 = getelementptr inbounds i32, ptr %56, i64 %137\00", align 1
@134 = private unnamed_addr constant [47 x i8] c"  %139 = load i32, ptr %138, align 4, !tbaa !7\00", align 1
@135 = private unnamed_addr constant [28 x i8] c"  %140 = lshr i32 %139, %95\00", align 1
@136 = private unnamed_addr constant [25 x i8] c"  %141 = and i32 %140, 1\00", align 1
@137 = private unnamed_addr constant [36 x i8] c"  %142 = add nuw nsw i32 %134, %141\00", align 1
@138 = private unnamed_addr constant [25 x i8] c"  %143 = or i32 %98, %66\00", align 1
@139 = private unnamed_addr constant [32 x i8] c"  %144 = lshr exact i32 %143, 5\00", align 1
@140 = private unnamed_addr constant [30 x i8] c"  %145 = zext i32 %144 to i64\00", align 1
@141 = private unnamed_addr constant [55 x i8] c"  %146 = getelementptr inbounds i32, ptr %56, i64 %145\00", align 1
@142 = private unnamed_addr constant [47 x i8] c"  %147 = load i32, ptr %146, align 4, !tbaa !7\00", align 1
@143 = private unnamed_addr constant [29 x i8] c"  %148 = lshr i32 %147, %104\00", align 1
@144 = private unnamed_addr constant [25 x i8] c"  %149 = and i32 %148, 1\00", align 1
@145 = private unnamed_addr constant [36 x i8] c"  %150 = add nuw nsw i32 %142, %149\00", align 1
@146 = private unnamed_addr constant [26 x i8] c"  %151 = or i32 %109, %66\00", align 1
@147 = private unnamed_addr constant [32 x i8] c"  %152 = lshr exact i32 %151, 5\00", align 1
@148 = private unnamed_addr constant [30 x i8] c"  %153 = zext i32 %152 to i64\00", align 1
@149 = private unnamed_addr constant [55 x i8] c"  %154 = getelementptr inbounds i32, ptr %56, i64 %153\00", align 1
@150 = private unnamed_addr constant [47 x i8] c"  %155 = load i32, ptr %154, align 4, !tbaa !7\00", align 1
@151 = private unnamed_addr constant [29 x i8] c"  %156 = lshr i32 %155, %115\00", align 1
@152 = private unnamed_addr constant [25 x i8] c"  %157 = and i32 %156, 1\00", align 1
@153 = private unnamed_addr constant [36 x i8] c"  %158 = add nuw nsw i32 %150, %157\00", align 1
@154 = private unnamed_addr constant [25 x i8] c"  %159 = or i32 %98, %61\00", align 1
@155 = private unnamed_addr constant [32 x i8] c"  %160 = lshr exact i32 %159, 5\00", align 1
@156 = private unnamed_addr constant [30 x i8] c"  %161 = zext i32 %160 to i64\00", align 1
@157 = private unnamed_addr constant [55 x i8] c"  %162 = getelementptr inbounds i32, ptr %56, i64 %161\00", align 1
@158 = private unnamed_addr constant [47 x i8] c"  %163 = load i32, ptr %162, align 4, !tbaa !7\00", align 1
@159 = private unnamed_addr constant [29 x i8] c"  %164 = shl nuw i32 1, %104\00", align 1
@160 = private unnamed_addr constant [28 x i8] c"  %165 = and i32 %163, %164\00", align 1
@161 = private unnamed_addr constant [29 x i8] c"  %166 = icmp eq i32 %165, 0\00", align 1
@162 = private unnamed_addr constant [26 x i8] c"  %167 = and i32 %158, -2\00", align 1
@163 = private unnamed_addr constant [29 x i8] c"  %168 = icmp eq i32 %167, 2\00", align 1
@164 = private unnamed_addr constant [29 x i8] c"  %169 = icmp eq i32 %158, 3\00", align 1
@165 = private unnamed_addr constant [42 x i8] c"  %170 = select i1 %166, i1 %169, i1 %168\00", align 1
@166 = private unnamed_addr constant [27 x i8] c"  %171 = xor i1 %166, %170\00", align 1
@167 = private unnamed_addr constant [40 x i8] c"  %172 = select i1 %171, i32 %85, i32 1\00", align 1
@168 = private unnamed_addr constant [40 x i8] c"  %173 = select i1 %170, i32 1, i32 %86\00", align 1
@169 = private unnamed_addr constant [25 x i8] c"  %174 = or i32 %87, %61\00", align 1
@170 = private unnamed_addr constant [35 x i8] c"  br i1 %170, label %69, label %75\00", align 1
@171 = private unnamed_addr constant [29 x i8] c"  %176 = icmp eq i32 %172, 0\00", align 1
@172 = private unnamed_addr constant [29 x i8] c"  %177 = icmp eq i32 %173, 0\00", align 1
@173 = private unnamed_addr constant [43 x i8] c"  %178 = select i1 %176, i1 %177, i1 false\00", align 1
@174 = private unnamed_addr constant [37 x i8] c"  br i1 %178, label %201, label %179\00", align 1
@175 = private unnamed_addr constant [25 x i8] c"  %181 = shl i32 %180, 8\00", align 1
@176 = private unnamed_addr constant [16 x i8] c"  br label %185\00", align 1
@177 = private unnamed_addr constant [33 x i8] c"  %183 = add nuw nsw i32 %180, 1\00", align 1
@178 = private unnamed_addr constant [31 x i8] c"  %184 = icmp eq i32 %183, 128\00", align 1
@179 = private unnamed_addr constant [52 x i8] c"  br i1 %184, label %200, label %179, !llvm.loop !5\00", align 1
@180 = private unnamed_addr constant [27 x i8] c"  %187 = and i32 %186, 224\00", align 1
@181 = private unnamed_addr constant [27 x i8] c"  %188 = or i32 %187, %181\00", align 1
@182 = private unnamed_addr constant [32 x i8] c"  %189 = lshr exact i32 %188, 5\00", align 1
@183 = private unnamed_addr constant [30 x i8] c"  %190 = zext i32 %189 to i64\00", align 1
@184 = private unnamed_addr constant [55 x i8] c"  %191 = getelementptr inbounds i32, ptr %55, i64 %190\00", align 1
@185 = private unnamed_addr constant [47 x i8] c"  %192 = load i32, ptr %191, align 4, !tbaa !7\00", align 1
@186 = private unnamed_addr constant [26 x i8] c"  %193 = and i32 %186, 31\00", align 1
@187 = private unnamed_addr constant [29 x i8] c"  %194 = shl nuw i32 1, %193\00", align 1
@188 = private unnamed_addr constant [28 x i8] c"  %195 = and i32 %194, %192\00", align 1
@189 = private unnamed_addr constant [29 x i8] c"  %196 = icmp eq i32 %195, 0\00", align 1
@190 = private unnamed_addr constant [54 x i8] c"  %197 = select i1 %196, i32 -16777216, i32 -16711936\00", align 1
@191 = private unnamed_addr constant [89 x i8] c"  tail call void @sim_set_pixel(i32 noundef %186, i32 noundef %180, i32 noundef %197) #4\00", align 1
@192 = private unnamed_addr constant [33 x i8] c"  %198 = add nuw nsw i32 %186, 1\00", align 1
@193 = private unnamed_addr constant [31 x i8] c"  %199 = icmp eq i32 %198, 256\00", align 1
@194 = private unnamed_addr constant [53 x i8] c"  br i1 %199, label %182, label %185, !llvm.loop !11\00", align 1
@195 = private unnamed_addr constant [31 x i8] c"  br label %54, !llvm.loop !16\00", align 1
@196 = private unnamed_addr constant [63 x i8] c"  call void @llvm.lifetime.end.p0(i64 4096, ptr nonnull %2) #4\00", align 1
@197 = private unnamed_addr constant [63 x i8] c"  call void @llvm.lifetime.end.p0(i64 4096, ptr nonnull %1) #4\00", align 1
@198 = private unnamed_addr constant [12 x i8] c"  ret i32 0\00", align 1

; Function Attrs: nounwind sspstrong uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  tail call void @log_llvm_ir(ptr nonnull @0) #4
  %1 = alloca [1024 x i32], align 16
  tail call void @log_llvm_ir(ptr nonnull @1) #4
  %2 = alloca [1024 x i32], align 16
  tail call void @log_llvm_ir(ptr nonnull @2) #4
  call void @llvm.lifetime.start.p0(i64 4096, ptr nonnull %1) #4
  tail call void @log_llvm_ir(ptr nonnull @3) #4
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 16 dereferenceable(4096) %1, i8 0, i64 4096, i1 false)
  tail call void @log_llvm_ir(ptr nonnull @4) #4
  call void @llvm.lifetime.start.p0(i64 4096, ptr nonnull %2) #4
  tail call void @log_llvm_ir(ptr nonnull @5) #4
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 16 dereferenceable(4096) %2, i8 0, i64 4096, i1 false)
  tail call void @log_llvm_ir(ptr nonnull @6) #4
  br label %3

3:                                                ; preds = %0, %27
  %4 = phi i32 [ 0, %0 ], [ %28, %27 ]
  tail call void @log_llvm_ir(ptr nonnull @7) #4
  %5 = shl i32 %4, 8
  tail call void @log_llvm_ir(ptr nonnull @8) #4
  br label %30

6:                                                ; preds = %27, %9
  %7 = phi i32 [ %10, %9 ], [ 0, %27 ]
  tail call void @log_llvm_ir(ptr nonnull @9) #4
  %8 = shl i32 %7, 8
  tail call void @log_llvm_ir(ptr nonnull @10) #4
  br label %12

9:                                                ; preds = %12
  tail call void @log_llvm_ir(ptr nonnull @11) #4
  %10 = add nuw nsw i32 %7, 1
  tail call void @log_llvm_ir(ptr nonnull @12) #4
  %11 = icmp eq i32 %10, 128
  tail call void @log_llvm_ir(ptr nonnull @13) #4
  br i1 %11, label %55, label %6, !llvm.loop !5

12:                                               ; preds = %12, %6
  %13 = phi i32 [ 0, %6 ], [ %25, %12 ]
  tail call void @log_llvm_ir(ptr nonnull @14) #4
  %14 = and i32 %13, 224
  tail call void @log_llvm_ir(ptr nonnull @15) #4
  %15 = or i32 %14, %8
  tail call void @log_llvm_ir(ptr nonnull @16) #4
  %16 = lshr exact i32 %15, 5
  tail call void @log_llvm_ir(ptr nonnull @17) #4
  %17 = zext i32 %16 to i64
  tail call void @log_llvm_ir(ptr nonnull @18) #4
  %18 = getelementptr inbounds i32, ptr %2, i64 %17
  tail call void @log_llvm_ir(ptr nonnull @19) #4
  %19 = load i32, ptr %18, align 4, !tbaa !7
  tail call void @log_llvm_ir(ptr nonnull @20) #4
  %20 = and i32 %13, 31
  tail call void @log_llvm_ir(ptr nonnull @21) #4
  %21 = shl nuw i32 1, %20
  tail call void @log_llvm_ir(ptr nonnull @22) #4
  %22 = and i32 %21, %19
  tail call void @log_llvm_ir(ptr nonnull @23) #4
  %23 = icmp eq i32 %22, 0
  tail call void @log_llvm_ir(ptr nonnull @24) #4
  %24 = select i1 %23, i32 -16777216, i32 -16711936
  tail call void @log_llvm_ir(ptr nonnull @25) #4
  tail call void @sim_set_pixel(i32 noundef %13, i32 noundef %7, i32 noundef %24) #4
  tail call void @log_llvm_ir(ptr nonnull @26) #4
  %25 = add nuw nsw i32 %13, 1
  tail call void @log_llvm_ir(ptr nonnull @27) #4
  %26 = icmp eq i32 %25, 256
  tail call void @log_llvm_ir(ptr nonnull @28) #4
  br i1 %26, label %9, label %12, !llvm.loop !11

27:                                               ; preds = %51
  tail call void @log_llvm_ir(ptr nonnull @29) #4
  %28 = add nuw nsw i32 %4, 1
  tail call void @log_llvm_ir(ptr nonnull @30) #4
  %29 = icmp eq i32 %28, 128
  tail call void @log_llvm_ir(ptr nonnull @31) #4
  br i1 %29, label %6, label %3, !llvm.loop !12

30:                                               ; preds = %3, %51
  %31 = phi i32 [ 0, %3 ], [ %53, %51 ]
  tail call void @log_llvm_ir(ptr nonnull @32) #4
  %32 = tail call i32 @sim_rand() #4
  tail call void @log_llvm_ir(ptr nonnull @33) #4
  %33 = and i32 %32, 1
  tail call void @log_llvm_ir(ptr nonnull @34) #4
  %34 = or i32 %31, %5
  tail call void @log_llvm_ir(ptr nonnull @35) #4
  %35 = icmp eq i32 %33, 0
  tail call void @log_llvm_ir(ptr nonnull @36) #4
  %36 = and i32 %31, 31
  tail call void @log_llvm_ir(ptr nonnull @37) #4
  %37 = shl nuw i32 1, %36
  tail call void @log_llvm_ir(ptr nonnull @38) #4
  br i1 %35, label %44, label %38

38:                                               ; preds = %30
  tail call void @log_llvm_ir(ptr nonnull @39) #4
  %39 = lshr i32 %34, 5
  tail call void @log_llvm_ir(ptr nonnull @40) #4
  %40 = zext i32 %39 to i64
  tail call void @log_llvm_ir(ptr nonnull @41) #4
  %41 = getelementptr inbounds i32, ptr %2, i64 %40
  tail call void @log_llvm_ir(ptr nonnull @42) #4
  %42 = load i32, ptr %41, align 4, !tbaa !7
  tail call void @log_llvm_ir(ptr nonnull @43) #4
  %43 = or i32 %42, %37
  tail call void @log_llvm_ir(ptr nonnull @44) #4
  store i32 %43, ptr %41, align 4, !tbaa !7
  br label %51

44:                                               ; preds = %30
  tail call void @log_llvm_ir(ptr nonnull @45) #4
  %45 = xor i32 %37, -1
  tail call void @log_llvm_ir(ptr nonnull @46) #4
  %46 = lshr i32 %34, 5
  tail call void @log_llvm_ir(ptr nonnull @47) #4
  %47 = zext i32 %46 to i64
  tail call void @log_llvm_ir(ptr nonnull @48) #4
  %48 = getelementptr inbounds i32, ptr %2, i64 %47
  tail call void @log_llvm_ir(ptr nonnull @49) #4
  %49 = load i32, ptr %48, align 4, !tbaa !7
  tail call void @log_llvm_ir(ptr nonnull @50) #4
  %50 = and i32 %49, %45
  tail call void @log_llvm_ir(ptr nonnull @51) #4
  store i32 %50, ptr %48, align 4, !tbaa !7
  br label %51

51:                                               ; preds = %38, %44
  %52 = phi ptr [ @52, %38 ], [ @52, %44 ]
  tail call void @log_llvm_ir(ptr nonnull %52) #4
  tail call void @log_llvm_ir(ptr nonnull @53) #4
  %53 = add nuw nsw i32 %31, 1
  tail call void @log_llvm_ir(ptr nonnull @54) #4
  %54 = icmp eq i32 %53, 256
  tail call void @log_llvm_ir(ptr nonnull @55) #4
  br i1 %54, label %27, label %30, !llvm.loop !13

55:                                               ; preds = %9, %202
  %56 = phi ptr [ %57, %202 ], [ %1, %9 ]
  %57 = phi ptr [ %56, %202 ], [ %2, %9 ]
  tail call void @log_llvm_ir(ptr nonnull @56) #4
  tail call void @sim_flush() #4
  tail call void @log_llvm_ir(ptr nonnull @57) #4
  br label %58

58:                                               ; preds = %68, %55
  %59 = phi i32 [ 0, %55 ], [ %174, %68 ]
  %60 = phi i32 [ 0, %55 ], [ %175, %68 ]
  %61 = phi i32 [ 0, %55 ], [ %65, %68 ]
  tail call void @log_llvm_ir(ptr nonnull @58) #4
  %62 = shl i32 %61, 8
  tail call void @log_llvm_ir(ptr nonnull @59) #4
  %63 = add nuw nsw i32 %62, 32512
  tail call void @log_llvm_ir(ptr nonnull @60) #4
  %64 = and i32 %63, 32512
  tail call void @log_llvm_ir(ptr nonnull @61) #4
  %65 = add nuw nsw i32 %61, 1
  tail call void @log_llvm_ir(ptr nonnull @62) #4
  %66 = shl i32 %65, 8
  tail call void @log_llvm_ir(ptr nonnull @63) #4
  %67 = and i32 %66, 32512
  tail call void @log_llvm_ir(ptr nonnull @64) #4
  br label %86

68:                                               ; preds = %83
  tail call void @log_llvm_ir(ptr nonnull @65) #4
  %69 = icmp eq i32 %65, 128
  tail call void @log_llvm_ir(ptr nonnull @66) #4
  br i1 %69, label %177, label %58, !llvm.loop !14

70:                                               ; preds = %86
  tail call void @log_llvm_ir(ptr nonnull @67) #4
  %71 = lshr i32 %176, 5
  tail call void @log_llvm_ir(ptr nonnull @68) #4
  %72 = zext i32 %71 to i64
  tail call void @log_llvm_ir(ptr nonnull @69) #4
  %73 = getelementptr inbounds i32, ptr %56, i64 %72
  tail call void @log_llvm_ir(ptr nonnull @70) #4
  %74 = load i32, ptr %73, align 4, !tbaa !7
  tail call void @log_llvm_ir(ptr nonnull @71) #4
  %75 = or i32 %74, %166
  tail call void @log_llvm_ir(ptr nonnull @72) #4
  store i32 %75, ptr %73, align 4, !tbaa !7
  br label %83

76:                                               ; preds = %86
  tail call void @log_llvm_ir(ptr nonnull @73) #4
  %77 = xor i32 %166, -1
  tail call void @log_llvm_ir(ptr nonnull @74) #4
  %78 = lshr i32 %176, 5
  tail call void @log_llvm_ir(ptr nonnull @75) #4
  %79 = zext i32 %78 to i64
  tail call void @log_llvm_ir(ptr nonnull @76) #4
  %80 = getelementptr inbounds i32, ptr %56, i64 %79
  tail call void @log_llvm_ir(ptr nonnull @77) #4
  %81 = load i32, ptr %80, align 4, !tbaa !7
  tail call void @log_llvm_ir(ptr nonnull @78) #4
  %82 = and i32 %81, %77
  tail call void @log_llvm_ir(ptr nonnull @79) #4
  store i32 %82, ptr %80, align 4, !tbaa !7
  br label %83

83:                                               ; preds = %76, %70
  %84 = phi ptr [ @80, %76 ], [ @80, %70 ]
  tail call void @log_llvm_ir(ptr nonnull %84) #4
  tail call void @log_llvm_ir(ptr nonnull @81) #4
  %85 = icmp eq i32 %110, 256
  tail call void @log_llvm_ir(ptr nonnull @82) #4
  br i1 %85, label %68, label %86, !llvm.loop !15

86:                                               ; preds = %83, %58
  %87 = phi i32 [ %59, %58 ], [ %174, %83 ]
  %88 = phi i32 [ %60, %58 ], [ %175, %83 ]
  %89 = phi i32 [ 0, %58 ], [ %110, %83 ]
  tail call void @log_llvm_ir(ptr nonnull @83) #4
  %90 = add nsw i32 %89, -1
  tail call void @log_llvm_ir(ptr nonnull @84) #4
  %91 = and i32 %90, 224
  tail call void @log_llvm_ir(ptr nonnull @85) #4
  %92 = or i32 %91, %64
  tail call void @log_llvm_ir(ptr nonnull @86) #4
  %93 = lshr exact i32 %92, 5
  tail call void @log_llvm_ir(ptr nonnull @87) #4
  %94 = zext i32 %93 to i64
  tail call void @log_llvm_ir(ptr nonnull @88) #4
  %95 = getelementptr inbounds i32, ptr %57, i64 %94
  tail call void @log_llvm_ir(ptr nonnull @89) #4
  %96 = load i32, ptr %95, align 4, !tbaa !7
  tail call void @log_llvm_ir(ptr nonnull @90) #4
  %97 = and i32 %90, 31
  tail call void @log_llvm_ir(ptr nonnull @91) #4
  %98 = lshr i32 %96, %97
  tail call void @log_llvm_ir(ptr nonnull @92) #4
  %99 = and i32 %98, 1
  tail call void @log_llvm_ir(ptr nonnull @93) #4
  %100 = and i32 %89, 224
  tail call void @log_llvm_ir(ptr nonnull @94) #4
  %101 = or i32 %100, %64
  tail call void @log_llvm_ir(ptr nonnull @95) #4
  %102 = lshr exact i32 %101, 5
  tail call void @log_llvm_ir(ptr nonnull @96) #4
  %103 = zext i32 %102 to i64
  tail call void @log_llvm_ir(ptr nonnull @97) #4
  %104 = getelementptr inbounds i32, ptr %57, i64 %103
  tail call void @log_llvm_ir(ptr nonnull @98) #4
  %105 = load i32, ptr %104, align 4, !tbaa !7
  tail call void @log_llvm_ir(ptr nonnull @99) #4
  %106 = and i32 %89, 31
  tail call void @log_llvm_ir(ptr nonnull @100) #4
  %107 = lshr i32 %105, %106
  tail call void @log_llvm_ir(ptr nonnull @101) #4
  %108 = and i32 %107, 1
  tail call void @log_llvm_ir(ptr nonnull @102) #4
  %109 = add nuw nsw i32 %108, %99
  tail call void @log_llvm_ir(ptr nonnull @103) #4
  %110 = add nuw nsw i32 %89, 1
  tail call void @log_llvm_ir(ptr nonnull @104) #4
  %111 = and i32 %110, 224
  tail call void @log_llvm_ir(ptr nonnull @105) #4
  %112 = or i32 %111, %64
  tail call void @log_llvm_ir(ptr nonnull @106) #4
  %113 = lshr exact i32 %112, 5
  tail call void @log_llvm_ir(ptr nonnull @107) #4
  %114 = zext i32 %113 to i64
  tail call void @log_llvm_ir(ptr nonnull @108) #4
  %115 = getelementptr inbounds i32, ptr %57, i64 %114
  tail call void @log_llvm_ir(ptr nonnull @109) #4
  %116 = load i32, ptr %115, align 4, !tbaa !7
  tail call void @log_llvm_ir(ptr nonnull @110) #4
  %117 = and i32 %110, 31
  tail call void @log_llvm_ir(ptr nonnull @111) #4
  %118 = lshr i32 %116, %117
  tail call void @log_llvm_ir(ptr nonnull @112) #4
  %119 = and i32 %118, 1
  tail call void @log_llvm_ir(ptr nonnull @113) #4
  %120 = add nuw nsw i32 %109, %119
  tail call void @log_llvm_ir(ptr nonnull @114) #4
  %121 = or i32 %91, %62
  tail call void @log_llvm_ir(ptr nonnull @115) #4
  %122 = lshr exact i32 %121, 5
  tail call void @log_llvm_ir(ptr nonnull @116) #4
  %123 = zext i32 %122 to i64
  tail call void @log_llvm_ir(ptr nonnull @117) #4
  %124 = getelementptr inbounds i32, ptr %57, i64 %123
  tail call void @log_llvm_ir(ptr nonnull @118) #4
  %125 = load i32, ptr %124, align 4, !tbaa !7
  tail call void @log_llvm_ir(ptr nonnull @119) #4
  %126 = lshr i32 %125, %97
  tail call void @log_llvm_ir(ptr nonnull @120) #4
  %127 = and i32 %126, 1
  tail call void @log_llvm_ir(ptr nonnull @121) #4
  %128 = add nuw nsw i32 %120, %127
  tail call void @log_llvm_ir(ptr nonnull @122) #4
  %129 = or i32 %111, %62
  tail call void @log_llvm_ir(ptr nonnull @123) #4
  %130 = lshr exact i32 %129, 5
  tail call void @log_llvm_ir(ptr nonnull @124) #4
  %131 = zext i32 %130 to i64
  tail call void @log_llvm_ir(ptr nonnull @125) #4
  %132 = getelementptr inbounds i32, ptr %57, i64 %131
  tail call void @log_llvm_ir(ptr nonnull @126) #4
  %133 = load i32, ptr %132, align 4, !tbaa !7
  tail call void @log_llvm_ir(ptr nonnull @127) #4
  %134 = lshr i32 %133, %117
  tail call void @log_llvm_ir(ptr nonnull @128) #4
  %135 = and i32 %134, 1
  tail call void @log_llvm_ir(ptr nonnull @129) #4
  %136 = add nuw nsw i32 %128, %135
  tail call void @log_llvm_ir(ptr nonnull @130) #4
  %137 = or i32 %91, %67
  tail call void @log_llvm_ir(ptr nonnull @131) #4
  %138 = lshr exact i32 %137, 5
  tail call void @log_llvm_ir(ptr nonnull @132) #4
  %139 = zext i32 %138 to i64
  tail call void @log_llvm_ir(ptr nonnull @133) #4
  %140 = getelementptr inbounds i32, ptr %57, i64 %139
  tail call void @log_llvm_ir(ptr nonnull @134) #4
  %141 = load i32, ptr %140, align 4, !tbaa !7
  tail call void @log_llvm_ir(ptr nonnull @135) #4
  %142 = lshr i32 %141, %97
  tail call void @log_llvm_ir(ptr nonnull @136) #4
  %143 = and i32 %142, 1
  tail call void @log_llvm_ir(ptr nonnull @137) #4
  %144 = add nuw nsw i32 %136, %143
  tail call void @log_llvm_ir(ptr nonnull @138) #4
  %145 = or i32 %100, %67
  tail call void @log_llvm_ir(ptr nonnull @139) #4
  %146 = lshr exact i32 %145, 5
  tail call void @log_llvm_ir(ptr nonnull @140) #4
  %147 = zext i32 %146 to i64
  tail call void @log_llvm_ir(ptr nonnull @141) #4
  %148 = getelementptr inbounds i32, ptr %57, i64 %147
  tail call void @log_llvm_ir(ptr nonnull @142) #4
  %149 = load i32, ptr %148, align 4, !tbaa !7
  tail call void @log_llvm_ir(ptr nonnull @143) #4
  %150 = lshr i32 %149, %106
  tail call void @log_llvm_ir(ptr nonnull @144) #4
  %151 = and i32 %150, 1
  tail call void @log_llvm_ir(ptr nonnull @145) #4
  %152 = add nuw nsw i32 %144, %151
  tail call void @log_llvm_ir(ptr nonnull @146) #4
  %153 = or i32 %111, %67
  tail call void @log_llvm_ir(ptr nonnull @147) #4
  %154 = lshr exact i32 %153, 5
  tail call void @log_llvm_ir(ptr nonnull @148) #4
  %155 = zext i32 %154 to i64
  tail call void @log_llvm_ir(ptr nonnull @149) #4
  %156 = getelementptr inbounds i32, ptr %57, i64 %155
  tail call void @log_llvm_ir(ptr nonnull @150) #4
  %157 = load i32, ptr %156, align 4, !tbaa !7
  tail call void @log_llvm_ir(ptr nonnull @151) #4
  %158 = lshr i32 %157, %117
  tail call void @log_llvm_ir(ptr nonnull @152) #4
  %159 = and i32 %158, 1
  tail call void @log_llvm_ir(ptr nonnull @153) #4
  %160 = add nuw nsw i32 %152, %159
  tail call void @log_llvm_ir(ptr nonnull @154) #4
  %161 = or i32 %100, %62
  tail call void @log_llvm_ir(ptr nonnull @155) #4
  %162 = lshr exact i32 %161, 5
  tail call void @log_llvm_ir(ptr nonnull @156) #4
  %163 = zext i32 %162 to i64
  tail call void @log_llvm_ir(ptr nonnull @157) #4
  %164 = getelementptr inbounds i32, ptr %57, i64 %163
  tail call void @log_llvm_ir(ptr nonnull @158) #4
  %165 = load i32, ptr %164, align 4, !tbaa !7
  tail call void @log_llvm_ir(ptr nonnull @159) #4
  %166 = shl nuw i32 1, %106
  tail call void @log_llvm_ir(ptr nonnull @160) #4
  %167 = and i32 %165, %166
  tail call void @log_llvm_ir(ptr nonnull @161) #4
  %168 = icmp eq i32 %167, 0
  tail call void @log_llvm_ir(ptr nonnull @162) #4
  %169 = and i32 %160, -2
  tail call void @log_llvm_ir(ptr nonnull @163) #4
  %170 = icmp eq i32 %169, 2
  tail call void @log_llvm_ir(ptr nonnull @164) #4
  %171 = icmp eq i32 %160, 3
  tail call void @log_llvm_ir(ptr nonnull @165) #4
  %172 = select i1 %168, i1 %171, i1 %170
  tail call void @log_llvm_ir(ptr nonnull @166) #4
  %173 = xor i1 %168, %172
  tail call void @log_llvm_ir(ptr nonnull @167) #4
  %174 = select i1 %173, i32 %87, i32 1
  tail call void @log_llvm_ir(ptr nonnull @168) #4
  %175 = select i1 %172, i32 1, i32 %88
  tail call void @log_llvm_ir(ptr nonnull @169) #4
  %176 = or i32 %89, %62
  tail call void @log_llvm_ir(ptr nonnull @170) #4
  br i1 %172, label %70, label %76

177:                                              ; preds = %68
  tail call void @log_llvm_ir(ptr nonnull @171) #4
  %178 = icmp eq i32 %174, 0
  tail call void @log_llvm_ir(ptr nonnull @172) #4
  %179 = icmp eq i32 %175, 0
  tail call void @log_llvm_ir(ptr nonnull @173) #4
  %180 = select i1 %178, i1 %179, i1 false
  tail call void @log_llvm_ir(ptr nonnull @174) #4
  br i1 %180, label %203, label %181

181:                                              ; preds = %177, %184
  %182 = phi i32 [ %185, %184 ], [ 0, %177 ]
  tail call void @log_llvm_ir(ptr nonnull @175) #4
  %183 = shl i32 %182, 8
  tail call void @log_llvm_ir(ptr nonnull @176) #4
  br label %187

184:                                              ; preds = %187
  tail call void @log_llvm_ir(ptr nonnull @177) #4
  %185 = add nuw nsw i32 %182, 1
  tail call void @log_llvm_ir(ptr nonnull @178) #4
  %186 = icmp eq i32 %185, 128
  tail call void @log_llvm_ir(ptr nonnull @179) #4
  br i1 %186, label %202, label %181, !llvm.loop !5

187:                                              ; preds = %187, %181
  %188 = phi i32 [ 0, %181 ], [ %200, %187 ]
  tail call void @log_llvm_ir(ptr nonnull @180) #4
  %189 = and i32 %188, 224
  tail call void @log_llvm_ir(ptr nonnull @181) #4
  %190 = or i32 %189, %183
  tail call void @log_llvm_ir(ptr nonnull @182) #4
  %191 = lshr exact i32 %190, 5
  tail call void @log_llvm_ir(ptr nonnull @183) #4
  %192 = zext i32 %191 to i64
  tail call void @log_llvm_ir(ptr nonnull @184) #4
  %193 = getelementptr inbounds i32, ptr %56, i64 %192
  tail call void @log_llvm_ir(ptr nonnull @185) #4
  %194 = load i32, ptr %193, align 4, !tbaa !7
  tail call void @log_llvm_ir(ptr nonnull @186) #4
  %195 = and i32 %188, 31
  tail call void @log_llvm_ir(ptr nonnull @187) #4
  %196 = shl nuw i32 1, %195
  tail call void @log_llvm_ir(ptr nonnull @188) #4
  %197 = and i32 %196, %194
  tail call void @log_llvm_ir(ptr nonnull @189) #4
  %198 = icmp eq i32 %197, 0
  tail call void @log_llvm_ir(ptr nonnull @190) #4
  %199 = select i1 %198, i32 -16777216, i32 -16711936
  tail call void @log_llvm_ir(ptr nonnull @191) #4
  tail call void @sim_set_pixel(i32 noundef %188, i32 noundef %182, i32 noundef %199) #4
  tail call void @log_llvm_ir(ptr nonnull @192) #4
  %200 = add nuw nsw i32 %188, 1
  tail call void @log_llvm_ir(ptr nonnull @193) #4
  %201 = icmp eq i32 %200, 256
  tail call void @log_llvm_ir(ptr nonnull @194) #4
  br i1 %201, label %184, label %187, !llvm.loop !11

202:                                              ; preds = %184
  tail call void @log_llvm_ir(ptr nonnull @195) #4
  br label %55, !llvm.loop !16

203:                                              ; preds = %177
  tail call void @log_llvm_ir(ptr nonnull @196) #4
  call void @llvm.lifetime.end.p0(i64 4096, ptr nonnull %2) #4
  tail call void @log_llvm_ir(ptr nonnull @197) #4
  call void @llvm.lifetime.end.p0(i64 4096, ptr nonnull %1) #4
  tail call void @log_llvm_ir(ptr nonnull @198) #4
  ret i32 0
}

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.start.p0(i64 immarg, ptr nocapture) #1

; Function Attrs: mustprogress nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr nocapture writeonly, i8, i64, i1 immarg) #2

declare i32 @sim_rand() local_unnamed_addr #3

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.end.p0(i64 immarg, ptr nocapture) #1

declare void @sim_set_pixel(i32 noundef, i32 noundef, i32 noundef) local_unnamed_addr #3

declare void @sim_flush() local_unnamed_addr #3

declare void @log_llvm_ir(ptr)

attributes #0 = { nounwind sspstrong uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "probe-stack"="inline-asm" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #2 = { mustprogress nocallback nofree nounwind willreturn memory(argmem: write) }
attributes #3 = { "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{!"clang version 16.0.6"}
!5 = distinct !{!5, !6}
!6 = !{!"llvm.loop.mustprogress"}
!7 = !{!8, !8, i64 0}
!8 = !{!"int", !9, i64 0}
!9 = !{!"omnipotent char", !10, i64 0}
!10 = !{!"Simple C/C++ TBAA"}
!11 = distinct !{!11, !6}
!12 = distinct !{!12, !6}
!13 = distinct !{!13, !6}
!14 = distinct !{!14, !6}
!15 = distinct !{!15, !6}
!16 = distinct !{!16, !6}
