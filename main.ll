; ModuleID = 'src/main.c'
source_filename = "src/main.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca [32768 x i8], align 16
  %3 = alloca [32768 x i8], align 16
  %4 = alloca ptr, align 8
  %5 = alloca ptr, align 8
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca ptr, align 8
  store i32 0, ptr %1, align 4
  call void @llvm.memset.p0.i64(ptr align 16 %2, i8 0, i64 32768, i1 false)
  call void @llvm.memset.p0.i64(ptr align 16 %3, i8 0, i64 32768, i1 false)
  %9 = getelementptr inbounds [32768 x i8], ptr %2, i64 0, i64 0
  store ptr %9, ptr %4, align 8
  %10 = getelementptr inbounds [32768 x i8], ptr %3, i64 0, i64 0
  store ptr %10, ptr %5, align 8
  store i32 0, ptr %6, align 4
  br label %11

11:                                               ; preds = %34, %0
  %12 = load i32, ptr %6, align 4
  %13 = icmp ult i32 %12, 128
  br i1 %13, label %14, label %37

14:                                               ; preds = %11
  store i32 0, ptr %7, align 4
  br label %15

15:                                               ; preds = %30, %14
  %16 = load i32, ptr %7, align 4
  %17 = icmp ult i32 %16, 256
  br i1 %17, label %18, label %33

18:                                               ; preds = %15
  %19 = call i32 @sim_rand()
  %20 = urem i32 %19, 2
  %21 = icmp ne i32 %20, 0
  %22 = load ptr, ptr %5, align 8
  %23 = load i32, ptr %6, align 4
  %24 = mul i32 %23, 256
  %25 = load i32, ptr %7, align 4
  %26 = add i32 %24, %25
  %27 = zext i32 %26 to i64
  %28 = getelementptr inbounds i8, ptr %22, i64 %27
  %29 = zext i1 %21 to i8
  store i8 %29, ptr %28, align 1
  br label %30

30:                                               ; preds = %18
  %31 = load i32, ptr %7, align 4
  %32 = add i32 %31, 1
  store i32 %32, ptr %7, align 4
  br label %15, !llvm.loop !6

33:                                               ; preds = %15
  br label %34

34:                                               ; preds = %33
  %35 = load i32, ptr %6, align 4
  %36 = add i32 %35, 1
  store i32 %36, ptr %6, align 4
  br label %11, !llvm.loop !8

37:                                               ; preds = %11
  %38 = load ptr, ptr %5, align 8
  call void @draw_gen(ptr noundef %38)
  br label %39

39:                                               ; preds = %43, %37
  %40 = load ptr, ptr %4, align 8
  %41 = load ptr, ptr %5, align 8
  %42 = call zeroext i1 @calc_gen(ptr noundef %40, ptr noundef %41)
  br i1 %42, label %43, label %49

43:                                               ; preds = %39
  %44 = call i32 @usleep(i32 noundef 100000)
  %45 = load ptr, ptr %4, align 8
  call void @draw_gen(ptr noundef %45)
  %46 = load ptr, ptr %5, align 8
  store ptr %46, ptr %8, align 8
  %47 = load ptr, ptr %4, align 8
  store ptr %47, ptr %5, align 8
  %48 = load ptr, ptr %8, align 8
  store ptr %48, ptr %4, align 8
  br label %39, !llvm.loop !9

49:                                               ; preds = %39
  ret i32 0
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr nocapture writeonly, i8, i64, i1 immarg) #1

declare i32 @sim_rand() #2

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define internal void @draw_gen(ptr noundef %0) #0 {
  %2 = alloca ptr, align 8
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store ptr %0, ptr %2, align 8
  store i32 0, ptr %3, align 4
  store i32 0, ptr %4, align 4
  store i32 0, ptr %4, align 4
  br label %5

5:                                                ; preds = %31, %1
  %6 = load i32, ptr %4, align 4
  %7 = icmp ult i32 %6, 128
  br i1 %7, label %8, label %34

8:                                                ; preds = %5
  store i32 0, ptr %3, align 4
  br label %9

9:                                                ; preds = %27, %8
  %10 = load i32, ptr %3, align 4
  %11 = icmp ult i32 %10, 256
  br i1 %11, label %12, label %30

12:                                               ; preds = %9
  %13 = load i32, ptr %3, align 4
  %14 = load i32, ptr %4, align 4
  %15 = load ptr, ptr %2, align 8
  %16 = load i32, ptr %4, align 4
  %17 = mul i32 %16, 256
  %18 = load i32, ptr %3, align 4
  %19 = add i32 %17, %18
  %20 = zext i32 %19 to i64
  %21 = getelementptr inbounds i8, ptr %15, i64 %20
  %22 = load i8, ptr %21, align 1
  %23 = trunc i8 %22 to i1
  %24 = zext i1 %23 to i32
  %25 = mul nsw i32 65280, %24
  %26 = add i32 -16777216, %25
  call void @sim_set_pixel(i32 noundef %13, i32 noundef %14, i32 noundef %26)
  br label %27

27:                                               ; preds = %12
  %28 = load i32, ptr %3, align 4
  %29 = add i32 %28, 1
  store i32 %29, ptr %3, align 4
  br label %9, !llvm.loop !10

30:                                               ; preds = %9
  br label %31

31:                                               ; preds = %30
  %32 = load i32, ptr %4, align 4
  %33 = add i32 %32, 1
  store i32 %33, ptr %4, align 4
  br label %5, !llvm.loop !11

34:                                               ; preds = %5
  call void @sim_flush()
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define internal zeroext i1 @calc_gen(ptr noundef %0, ptr noundef %1) #0 {
  %3 = alloca ptr, align 8
  %4 = alloca ptr, align 8
  %5 = alloca i8, align 1
  %6 = alloca i8, align 1
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  %10 = alloca i8, align 1
  %11 = alloca i8, align 1
  %12 = alloca i8, align 1
  %13 = alloca i8, align 1
  store ptr %0, ptr %3, align 8
  store ptr %1, ptr %4, align 8
  store i8 0, ptr %5, align 1
  store i8 0, ptr %6, align 1
  store i32 0, ptr %7, align 4
  br label %14

14:                                               ; preds = %118, %2
  %15 = load i32, ptr %7, align 4
  %16 = icmp slt i32 %15, 128
  br i1 %16, label %17, label %121

17:                                               ; preds = %14
  store i32 0, ptr %8, align 4
  br label %18

18:                                               ; preds = %114, %17
  %19 = load i32, ptr %8, align 4
  %20 = icmp slt i32 %19, 256
  br i1 %20, label %21, label %117

21:                                               ; preds = %18
  store i32 0, ptr %9, align 4
  store i8 -1, ptr %10, align 1
  br label %22

22:                                               ; preds = %58, %21
  %23 = load i8, ptr %10, align 1
  %24 = sext i8 %23 to i32
  %25 = icmp sle i32 %24, 1
  br i1 %25, label %26, label %61

26:                                               ; preds = %22
  store i8 -1, ptr %11, align 1
  br label %27

27:                                               ; preds = %54, %26
  %28 = load i8, ptr %11, align 1
  %29 = sext i8 %28 to i32
  %30 = icmp sle i32 %29, 1
  br i1 %30, label %31, label %57

31:                                               ; preds = %27
  %32 = load i8, ptr %11, align 1
  %33 = sext i8 %32 to i32
  %34 = icmp eq i32 %33, 0
  br i1 %34, label %35, label %40

35:                                               ; preds = %31
  %36 = load i8, ptr %10, align 1
  %37 = sext i8 %36 to i32
  %38 = icmp eq i32 %37, 0
  br i1 %38, label %39, label %40

39:                                               ; preds = %35
  br label %54

40:                                               ; preds = %35, %31
  %41 = load ptr, ptr %4, align 8
  %42 = load i32, ptr %8, align 4
  %43 = load i8, ptr %11, align 1
  %44 = sext i8 %43 to i32
  %45 = add nsw i32 %42, %44
  %46 = load i32, ptr %7, align 4
  %47 = load i8, ptr %10, align 1
  %48 = sext i8 %47 to i32
  %49 = add nsw i32 %46, %48
  %50 = call zeroext i1 @get_gen_value(ptr noundef %41, i32 noundef %45, i32 noundef %49)
  %51 = zext i1 %50 to i32
  %52 = load i32, ptr %9, align 4
  %53 = add i32 %52, %51
  store i32 %53, ptr %9, align 4
  br label %54

54:                                               ; preds = %40, %39
  %55 = load i8, ptr %11, align 1
  %56 = add i8 %55, 1
  store i8 %56, ptr %11, align 1
  br label %27, !llvm.loop !12

57:                                               ; preds = %27
  br label %58

58:                                               ; preds = %57
  %59 = load i8, ptr %10, align 1
  %60 = add i8 %59, 1
  store i8 %60, ptr %10, align 1
  br label %22, !llvm.loop !13

61:                                               ; preds = %22
  %62 = load ptr, ptr %4, align 8
  %63 = load i32, ptr %7, align 4
  %64 = mul nsw i32 %63, 256
  %65 = load i32, ptr %8, align 4
  %66 = add nsw i32 %64, %65
  %67 = sext i32 %66 to i64
  %68 = getelementptr inbounds i8, ptr %62, i64 %67
  %69 = load i8, ptr %68, align 1
  %70 = trunc i8 %69 to i1
  %71 = zext i1 %70 to i8
  store i8 %71, ptr %12, align 1
  %72 = load i8, ptr %12, align 1
  %73 = trunc i8 %72 to i1
  br i1 %73, label %74, label %83

74:                                               ; preds = %61
  %75 = load i32, ptr %9, align 4
  %76 = icmp eq i32 %75, 2
  br i1 %76, label %80, label %77

77:                                               ; preds = %74
  %78 = load i32, ptr %9, align 4
  %79 = icmp eq i32 %78, 3
  br label %80

80:                                               ; preds = %77, %74
  %81 = phi i1 [ true, %74 ], [ %79, %77 ]
  %82 = zext i1 %81 to i32
  br label %87

83:                                               ; preds = %61
  %84 = load i32, ptr %9, align 4
  %85 = icmp eq i32 %84, 3
  %86 = zext i1 %85 to i32
  br label %87

87:                                               ; preds = %83, %80
  %88 = phi i32 [ %82, %80 ], [ %86, %83 ]
  %89 = icmp ne i32 %88, 0
  %90 = zext i1 %89 to i8
  store i8 %90, ptr %13, align 1
  %91 = load i8, ptr %12, align 1
  %92 = trunc i8 %91 to i1
  %93 = zext i1 %92 to i32
  %94 = load i8, ptr %13, align 1
  %95 = trunc i8 %94 to i1
  %96 = zext i1 %95 to i32
  %97 = icmp ne i32 %93, %96
  br i1 %97, label %98, label %99

98:                                               ; preds = %87
  store i8 1, ptr %5, align 1
  br label %99

99:                                               ; preds = %98, %87
  %100 = load i8, ptr %13, align 1
  %101 = trunc i8 %100 to i1
  br i1 %101, label %102, label %103

102:                                              ; preds = %99
  store i8 1, ptr %6, align 1
  br label %103

103:                                              ; preds = %102, %99
  %104 = load i8, ptr %13, align 1
  %105 = trunc i8 %104 to i1
  %106 = load ptr, ptr %3, align 8
  %107 = load i32, ptr %7, align 4
  %108 = mul nsw i32 %107, 256
  %109 = load i32, ptr %8, align 4
  %110 = add nsw i32 %108, %109
  %111 = sext i32 %110 to i64
  %112 = getelementptr inbounds i8, ptr %106, i64 %111
  %113 = zext i1 %105 to i8
  store i8 %113, ptr %112, align 1
  br label %114

114:                                              ; preds = %103
  %115 = load i32, ptr %8, align 4
  %116 = add nsw i32 %115, 1
  store i32 %116, ptr %8, align 4
  br label %18, !llvm.loop !14

117:                                              ; preds = %18
  br label %118

118:                                              ; preds = %117
  %119 = load i32, ptr %7, align 4
  %120 = add nsw i32 %119, 1
  store i32 %120, ptr %7, align 4
  br label %14, !llvm.loop !15

121:                                              ; preds = %14
  %122 = load i8, ptr %5, align 1
  %123 = trunc i8 %122 to i1
  br i1 %123, label %127, label %124

124:                                              ; preds = %121
  %125 = load i8, ptr %6, align 1
  %126 = trunc i8 %125 to i1
  br label %127

127:                                              ; preds = %124, %121
  %128 = phi i1 [ true, %121 ], [ %126, %124 ]
  ret i1 %128
}

declare i32 @usleep(i32 noundef) #2

declare void @sim_set_pixel(i32 noundef, i32 noundef, i32 noundef) #2

declare void @sim_flush() #2

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define internal zeroext i1 @get_gen_value(ptr noundef %0, i32 noundef %1, i32 noundef %2) #0 {
  %4 = alloca ptr, align 8
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  store ptr %0, ptr %4, align 8
  store i32 %1, ptr %5, align 4
  store i32 %2, ptr %6, align 4
  %7 = load i32, ptr %5, align 4
  %8 = srem i32 %7, 256
  %9 = add nsw i32 %8, 256
  %10 = srem i32 %9, 256
  store i32 %10, ptr %5, align 4
  %11 = load i32, ptr %6, align 4
  %12 = srem i32 %11, 128
  %13 = add nsw i32 %12, 128
  %14 = srem i32 %13, 128
  store i32 %14, ptr %6, align 4
  %15 = load ptr, ptr %4, align 8
  %16 = load i32, ptr %6, align 4
  %17 = mul nsw i32 %16, 256
  %18 = load i32, ptr %5, align 4
  %19 = add nsw i32 %17, %18
  %20 = sext i32 %19 to i64
  %21 = getelementptr inbounds i8, ptr %15, i64 %20
  %22 = load i8, ptr %21, align 1
  %23 = trunc i8 %22 to i1
  ret i1 %23
}

attributes #0 = { noinline nounwind optnone sspstrong uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "probe-stack"="inline-asm" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nocallback nofree nounwind willreturn memory(argmem: write) }
attributes #2 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 16.0.6"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
!8 = distinct !{!8, !7}
!9 = distinct !{!9, !7}
!10 = distinct !{!10, !7}
!11 = distinct !{!11, !7}
!12 = distinct !{!12, !7}
!13 = distinct !{!13, !7}
!14 = distinct !{!14, !7}
!15 = distinct !{!15, !7}
