; ModuleID = 'src/main.c'
source_filename = "src/main.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: nounwind sspstrong uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %1 = alloca [1024 x i32], align 16
  %2 = alloca [1024 x i32], align 16
  call void @llvm.lifetime.start.p0(i64 4096, ptr nonnull %1) #4
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 16 dereferenceable(4096) %1, i8 0, i64 4096, i1 false)
  call void @llvm.lifetime.start.p0(i64 4096, ptr nonnull %2) #4
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 16 dereferenceable(4096) %2, i8 0, i64 4096, i1 false)
  br label %3

3:                                                ; preds = %0, %27
  %4 = phi i32 [ 0, %0 ], [ %28, %27 ]
  %5 = shl i32 %4, 8
  br label %30

6:                                                ; preds = %27, %9
  %7 = phi i32 [ %10, %9 ], [ 0, %27 ]
  %8 = shl i32 %7, 8
  br label %12

9:                                                ; preds = %12
  %10 = add nuw nsw i32 %7, 1
  %11 = icmp eq i32 %10, 128
  br i1 %11, label %54, label %6, !llvm.loop !5

12:                                               ; preds = %12, %6
  %13 = phi i32 [ 0, %6 ], [ %25, %12 ]
  %14 = and i32 %13, 224
  %15 = or i32 %14, %8
  %16 = lshr exact i32 %15, 5
  %17 = zext i32 %16 to i64
  %18 = getelementptr inbounds i32, ptr %2, i64 %17
  %19 = load i32, ptr %18, align 4, !tbaa !7
  %20 = and i32 %13, 31
  %21 = shl nuw i32 1, %20
  %22 = and i32 %21, %19
  %23 = icmp eq i32 %22, 0
  %24 = select i1 %23, i32 -16777216, i32 -16711936
  tail call void @sim_set_pixel(i32 noundef %13, i32 noundef %7, i32 noundef %24) #4
  %25 = add nuw nsw i32 %13, 1
  %26 = icmp eq i32 %25, 256
  br i1 %26, label %9, label %12, !llvm.loop !11

27:                                               ; preds = %51
  %28 = add nuw nsw i32 %4, 1
  %29 = icmp eq i32 %28, 128
  br i1 %29, label %6, label %3, !llvm.loop !12

30:                                               ; preds = %3, %51
  %31 = phi i32 [ 0, %3 ], [ %52, %51 ]
  %32 = tail call i32 @sim_rand() #4
  %33 = and i32 %32, 1
  %34 = or i32 %31, %5
  %35 = icmp eq i32 %33, 0
  %36 = and i32 %31, 31
  %37 = shl nuw i32 1, %36
  br i1 %35, label %44, label %38

38:                                               ; preds = %30
  %39 = lshr i32 %34, 5
  %40 = zext i32 %39 to i64
  %41 = getelementptr inbounds i32, ptr %2, i64 %40
  %42 = load i32, ptr %41, align 4, !tbaa !7
  %43 = or i32 %42, %37
  store i32 %43, ptr %41, align 4, !tbaa !7
  br label %51

44:                                               ; preds = %30
  %45 = xor i32 %37, -1
  %46 = lshr i32 %34, 5
  %47 = zext i32 %46 to i64
  %48 = getelementptr inbounds i32, ptr %2, i64 %47
  %49 = load i32, ptr %48, align 4, !tbaa !7
  %50 = and i32 %49, %45
  store i32 %50, ptr %48, align 4, !tbaa !7
  br label %51

51:                                               ; preds = %38, %44
  %52 = add nuw nsw i32 %31, 1
  %53 = icmp eq i32 %52, 256
  br i1 %53, label %27, label %30, !llvm.loop !13

54:                                               ; preds = %9, %200
  %55 = phi ptr [ %56, %200 ], [ %1, %9 ]
  %56 = phi ptr [ %55, %200 ], [ %2, %9 ]
  tail call void @sim_flush() #4
  br label %57

57:                                               ; preds = %67, %54
  %58 = phi i32 [ 0, %54 ], [ %172, %67 ]
  %59 = phi i32 [ 0, %54 ], [ %173, %67 ]
  %60 = phi i32 [ 0, %54 ], [ %64, %67 ]
  %61 = shl i32 %60, 8
  %62 = add nuw nsw i32 %61, 32512
  %63 = and i32 %62, 32512
  %64 = add nuw nsw i32 %60, 1
  %65 = shl i32 %64, 8
  %66 = and i32 %65, 32512
  br label %84

67:                                               ; preds = %82
  %68 = icmp eq i32 %64, 128
  br i1 %68, label %175, label %57, !llvm.loop !14

69:                                               ; preds = %84
  %70 = lshr i32 %174, 5
  %71 = zext i32 %70 to i64
  %72 = getelementptr inbounds i32, ptr %55, i64 %71
  %73 = load i32, ptr %72, align 4, !tbaa !7
  %74 = or i32 %73, %164
  store i32 %74, ptr %72, align 4, !tbaa !7
  br label %82

75:                                               ; preds = %84
  %76 = xor i32 %164, -1
  %77 = lshr i32 %174, 5
  %78 = zext i32 %77 to i64
  %79 = getelementptr inbounds i32, ptr %55, i64 %78
  %80 = load i32, ptr %79, align 4, !tbaa !7
  %81 = and i32 %80, %76
  store i32 %81, ptr %79, align 4, !tbaa !7
  br label %82

82:                                               ; preds = %75, %69
  %83 = icmp eq i32 %108, 256
  br i1 %83, label %67, label %84, !llvm.loop !15

84:                                               ; preds = %82, %57
  %85 = phi i32 [ %58, %57 ], [ %172, %82 ]
  %86 = phi i32 [ %59, %57 ], [ %173, %82 ]
  %87 = phi i32 [ 0, %57 ], [ %108, %82 ]
  %88 = add nsw i32 %87, -1
  %89 = and i32 %88, 224
  %90 = or i32 %89, %63
  %91 = lshr exact i32 %90, 5
  %92 = zext i32 %91 to i64
  %93 = getelementptr inbounds i32, ptr %56, i64 %92
  %94 = load i32, ptr %93, align 4, !tbaa !7
  %95 = and i32 %88, 31
  %96 = lshr i32 %94, %95
  %97 = and i32 %96, 1
  %98 = and i32 %87, 224
  %99 = or i32 %98, %63
  %100 = lshr exact i32 %99, 5
  %101 = zext i32 %100 to i64
  %102 = getelementptr inbounds i32, ptr %56, i64 %101
  %103 = load i32, ptr %102, align 4, !tbaa !7
  %104 = and i32 %87, 31
  %105 = lshr i32 %103, %104
  %106 = and i32 %105, 1
  %107 = add nuw nsw i32 %106, %97
  %108 = add nuw nsw i32 %87, 1
  %109 = and i32 %108, 224
  %110 = or i32 %109, %63
  %111 = lshr exact i32 %110, 5
  %112 = zext i32 %111 to i64
  %113 = getelementptr inbounds i32, ptr %56, i64 %112
  %114 = load i32, ptr %113, align 4, !tbaa !7
  %115 = and i32 %108, 31
  %116 = lshr i32 %114, %115
  %117 = and i32 %116, 1
  %118 = add nuw nsw i32 %107, %117
  %119 = or i32 %89, %61
  %120 = lshr exact i32 %119, 5
  %121 = zext i32 %120 to i64
  %122 = getelementptr inbounds i32, ptr %56, i64 %121
  %123 = load i32, ptr %122, align 4, !tbaa !7
  %124 = lshr i32 %123, %95
  %125 = and i32 %124, 1
  %126 = add nuw nsw i32 %118, %125
  %127 = or i32 %109, %61
  %128 = lshr exact i32 %127, 5
  %129 = zext i32 %128 to i64
  %130 = getelementptr inbounds i32, ptr %56, i64 %129
  %131 = load i32, ptr %130, align 4, !tbaa !7
  %132 = lshr i32 %131, %115
  %133 = and i32 %132, 1
  %134 = add nuw nsw i32 %126, %133
  %135 = or i32 %89, %66
  %136 = lshr exact i32 %135, 5
  %137 = zext i32 %136 to i64
  %138 = getelementptr inbounds i32, ptr %56, i64 %137
  %139 = load i32, ptr %138, align 4, !tbaa !7
  %140 = lshr i32 %139, %95
  %141 = and i32 %140, 1
  %142 = add nuw nsw i32 %134, %141
  %143 = or i32 %98, %66
  %144 = lshr exact i32 %143, 5
  %145 = zext i32 %144 to i64
  %146 = getelementptr inbounds i32, ptr %56, i64 %145
  %147 = load i32, ptr %146, align 4, !tbaa !7
  %148 = lshr i32 %147, %104
  %149 = and i32 %148, 1
  %150 = add nuw nsw i32 %142, %149
  %151 = or i32 %109, %66
  %152 = lshr exact i32 %151, 5
  %153 = zext i32 %152 to i64
  %154 = getelementptr inbounds i32, ptr %56, i64 %153
  %155 = load i32, ptr %154, align 4, !tbaa !7
  %156 = lshr i32 %155, %115
  %157 = and i32 %156, 1
  %158 = add nuw nsw i32 %150, %157
  %159 = or i32 %98, %61
  %160 = lshr exact i32 %159, 5
  %161 = zext i32 %160 to i64
  %162 = getelementptr inbounds i32, ptr %56, i64 %161
  %163 = load i32, ptr %162, align 4, !tbaa !7
  %164 = shl nuw i32 1, %104
  %165 = and i32 %163, %164
  %166 = icmp eq i32 %165, 0
  %167 = and i32 %158, -2
  %168 = icmp eq i32 %167, 2
  %169 = icmp eq i32 %158, 3
  %170 = select i1 %166, i1 %169, i1 %168
  %171 = xor i1 %166, %170
  %172 = select i1 %171, i32 %85, i32 1
  %173 = select i1 %170, i32 1, i32 %86
  %174 = or i32 %87, %61
  br i1 %170, label %69, label %75

175:                                              ; preds = %67
  %176 = icmp eq i32 %172, 0
  %177 = icmp eq i32 %173, 0
  %178 = select i1 %176, i1 %177, i1 false
  br i1 %178, label %201, label %179

179:                                              ; preds = %175, %182
  %180 = phi i32 [ %183, %182 ], [ 0, %175 ]
  %181 = shl i32 %180, 8
  br label %185

182:                                              ; preds = %185
  %183 = add nuw nsw i32 %180, 1
  %184 = icmp eq i32 %183, 128
  br i1 %184, label %200, label %179, !llvm.loop !5

185:                                              ; preds = %185, %179
  %186 = phi i32 [ 0, %179 ], [ %198, %185 ]
  %187 = and i32 %186, 224
  %188 = or i32 %187, %181
  %189 = lshr exact i32 %188, 5
  %190 = zext i32 %189 to i64
  %191 = getelementptr inbounds i32, ptr %55, i64 %190
  %192 = load i32, ptr %191, align 4, !tbaa !7
  %193 = and i32 %186, 31
  %194 = shl nuw i32 1, %193
  %195 = and i32 %194, %192
  %196 = icmp eq i32 %195, 0
  %197 = select i1 %196, i32 -16777216, i32 -16711936
  tail call void @sim_set_pixel(i32 noundef %186, i32 noundef %180, i32 noundef %197) #4
  %198 = add nuw nsw i32 %186, 1
  %199 = icmp eq i32 %198, 256
  br i1 %199, label %182, label %185, !llvm.loop !11

200:                                              ; preds = %182
  br label %54, !llvm.loop !16

201:                                              ; preds = %175
  call void @llvm.lifetime.end.p0(i64 4096, ptr nonnull %2) #4
  call void @llvm.lifetime.end.p0(i64 4096, ptr nonnull %1) #4
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
