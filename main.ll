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

11:                                               ; preds = %33, %0
  %12 = load i32, ptr %6, align 4
  %13 = icmp ult i32 %12, 128
  br i1 %13, label %14, label %36

14:                                               ; preds = %11
  store i32 0, ptr %7, align 4
  br label %15

15:                                               ; preds = %29, %14
  %16 = load i32, ptr %7, align 4
  %17 = icmp ult i32 %16, 256
  br i1 %17, label %18, label %32

18:                                               ; preds = %15
  %19 = call i32 @sim_rand()
  %20 = urem i32 %19, 2
  %21 = trunc i32 %20 to i8
  %22 = load ptr, ptr %5, align 8
  %23 = load i32, ptr %6, align 4
  %24 = mul i32 %23, 256
  %25 = load i32, ptr %7, align 4
  %26 = add i32 %24, %25
  %27 = zext i32 %26 to i64
  %28 = getelementptr inbounds i8, ptr %22, i64 %27
  store i8 %21, ptr %28, align 1
  br label %29

29:                                               ; preds = %18
  %30 = load i32, ptr %7, align 4
  %31 = add i32 %30, 1
  store i32 %31, ptr %7, align 4
  br label %15, !llvm.loop !6

32:                                               ; preds = %15
  br label %33

33:                                               ; preds = %32
  %34 = load i32, ptr %6, align 4
  %35 = add i32 %34, 1
  store i32 %35, ptr %6, align 4
  br label %11, !llvm.loop !8

36:                                               ; preds = %11
  %37 = load ptr, ptr %5, align 8
  call void @draw_gen(ptr noundef %37)
  br label %38

38:                                               ; preds = %43, %36
  %39 = load ptr, ptr %4, align 8
  %40 = load ptr, ptr %5, align 8
  %41 = call zeroext i8 @calc_gen(ptr noundef %39, ptr noundef %40)
  %42 = icmp ne i8 %41, 0
  br i1 %42, label %43, label %48

43:                                               ; preds = %38
  %44 = load ptr, ptr %4, align 8
  call void @draw_gen(ptr noundef %44)
  %45 = load ptr, ptr %5, align 8
  store ptr %45, ptr %8, align 8
  %46 = load ptr, ptr %4, align 8
  store ptr %46, ptr %5, align 8
  %47 = load ptr, ptr %8, align 8
  store ptr %47, ptr %4, align 8
  br label %38, !llvm.loop !9

48:                                               ; preds = %38
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
  br label %5

5:                                                ; preds = %30, %1
  %6 = load i32, ptr %3, align 4
  %7 = icmp ult i32 %6, 128
  br i1 %7, label %8, label %33

8:                                                ; preds = %5
  store i32 0, ptr %4, align 4
  br label %9

9:                                                ; preds = %26, %8
  %10 = load i32, ptr %4, align 4
  %11 = icmp ult i32 %10, 256
  br i1 %11, label %12, label %29

12:                                               ; preds = %9
  %13 = load i32, ptr %4, align 4
  %14 = load i32, ptr %3, align 4
  %15 = load ptr, ptr %2, align 8
  %16 = load i32, ptr %3, align 4
  %17 = mul i32 %16, 256
  %18 = load i32, ptr %4, align 4
  %19 = add i32 %17, %18
  %20 = zext i32 %19 to i64
  %21 = getelementptr inbounds i8, ptr %15, i64 %20
  %22 = load i8, ptr %21, align 1
  %23 = zext i8 %22 to i32
  %24 = mul nsw i32 65280, %23
  %25 = add i32 -16777216, %24
  call void @sim_set_pixel(i32 noundef %13, i32 noundef %14, i32 noundef %25)
  br label %26

26:                                               ; preds = %12
  %27 = load i32, ptr %4, align 4
  %28 = add i32 %27, 1
  store i32 %28, ptr %4, align 4
  br label %9, !llvm.loop !10

29:                                               ; preds = %9
  br label %30

30:                                               ; preds = %29
  %31 = load i32, ptr %3, align 4
  %32 = add i32 %31, 1
  store i32 %32, ptr %3, align 4
  br label %5, !llvm.loop !11

33:                                               ; preds = %5
  call void @sim_flush()
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define internal zeroext i8 @calc_gen(ptr noundef %0, ptr noundef %1) #0 {
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

14:                                               ; preds = %112, %2
  %15 = load i32, ptr %7, align 4
  %16 = icmp slt i32 %15, 128
  br i1 %16, label %17, label %115

17:                                               ; preds = %14
  store i32 0, ptr %8, align 4
  br label %18

18:                                               ; preds = %108, %17
  %19 = load i32, ptr %8, align 4
  %20 = icmp slt i32 %19, 256
  br i1 %20, label %21, label %111

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
  %50 = call zeroext i8 @get_gen_value(ptr noundef %41, i32 noundef %45, i32 noundef %49)
  %51 = zext i8 %50 to i32
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
  store i8 %69, ptr %12, align 1
  %70 = load i8, ptr %12, align 1
  %71 = zext i8 %70 to i32
  %72 = icmp ne i32 %71, 0
  br i1 %72, label %73, label %82

73:                                               ; preds = %61
  %74 = load i32, ptr %9, align 4
  %75 = icmp eq i32 %74, 2
  br i1 %75, label %79, label %76

76:                                               ; preds = %73
  %77 = load i32, ptr %9, align 4
  %78 = icmp eq i32 %77, 3
  br label %79

79:                                               ; preds = %76, %73
  %80 = phi i1 [ true, %73 ], [ %78, %76 ]
  %81 = zext i1 %80 to i32
  br label %86

82:                                               ; preds = %61
  %83 = load i32, ptr %9, align 4
  %84 = icmp eq i32 %83, 3
  %85 = zext i1 %84 to i32
  br label %86

86:                                               ; preds = %82, %79
  %87 = phi i32 [ %81, %79 ], [ %85, %82 ]
  %88 = trunc i32 %87 to i8
  store i8 %88, ptr %13, align 1
  %89 = load i8, ptr %12, align 1
  %90 = zext i8 %89 to i32
  %91 = load i8, ptr %13, align 1
  %92 = zext i8 %91 to i32
  %93 = icmp ne i32 %90, %92
  br i1 %93, label %94, label %95

94:                                               ; preds = %86
  store i8 1, ptr %5, align 1
  br label %95

95:                                               ; preds = %94, %86
  %96 = load i8, ptr %13, align 1
  %97 = icmp ne i8 %96, 0
  br i1 %97, label %98, label %99

98:                                               ; preds = %95
  store i8 1, ptr %6, align 1
  br label %99

99:                                               ; preds = %98, %95
  %100 = load i8, ptr %13, align 1
  %101 = load ptr, ptr %3, align 8
  %102 = load i32, ptr %7, align 4
  %103 = mul nsw i32 %102, 256
  %104 = load i32, ptr %8, align 4
  %105 = add nsw i32 %103, %104
  %106 = sext i32 %105 to i64
  %107 = getelementptr inbounds i8, ptr %101, i64 %106
  store i8 %100, ptr %107, align 1
  br label %108

108:                                              ; preds = %99
  %109 = load i32, ptr %8, align 4
  %110 = add nsw i32 %109, 1
  store i32 %110, ptr %8, align 4
  br label %18, !llvm.loop !14

111:                                              ; preds = %18
  br label %112

112:                                              ; preds = %111
  %113 = load i32, ptr %7, align 4
  %114 = add nsw i32 %113, 1
  store i32 %114, ptr %7, align 4
  br label %14, !llvm.loop !15

115:                                              ; preds = %14
  %116 = load i8, ptr %5, align 1
  %117 = zext i8 %116 to i32
  %118 = icmp ne i32 %117, 0
  br i1 %118, label %123, label %119

119:                                              ; preds = %115
  %120 = load i8, ptr %6, align 1
  %121 = zext i8 %120 to i32
  %122 = icmp ne i32 %121, 0
  br label %123

123:                                              ; preds = %119, %115
  %124 = phi i1 [ true, %115 ], [ %122, %119 ]
  %125 = zext i1 %124 to i32
  %126 = trunc i32 %125 to i8
  ret i8 %126
}

declare void @sim_set_pixel(i32 noundef, i32 noundef, i32 noundef) #2

declare void @sim_flush() #2

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define internal zeroext i8 @get_gen_value(ptr noundef %0, i32 noundef %1, i32 noundef %2) #0 {
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
  ret i8 %22
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
