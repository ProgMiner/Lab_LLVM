; ModuleID = 'src/main.c'
source_filename = "src/main.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca [1024 x i32], align 16
  %3 = alloca [1024 x i32], align 16
  %4 = alloca ptr, align 8
  %5 = alloca ptr, align 8
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca ptr, align 8
  store i32 0, ptr %1, align 4
  call void @llvm.memset.p0.i64(ptr align 16 %2, i8 0, i64 4096, i1 false)
  call void @llvm.memset.p0.i64(ptr align 16 %3, i8 0, i64 4096, i1 false)
  %9 = getelementptr inbounds [1024 x i32], ptr %2, i64 0, i64 0
  store ptr %9, ptr %4, align 8
  %10 = getelementptr inbounds [1024 x i32], ptr %3, i64 0, i64 0
  store ptr %10, ptr %5, align 8
  store i32 0, ptr %6, align 4
  br label %11

11:                                               ; preds = %28, %0
  %12 = load i32, ptr %6, align 4
  %13 = icmp ult i32 %12, 128
  br i1 %13, label %14, label %31

14:                                               ; preds = %11
  store i32 0, ptr %7, align 4
  br label %15

15:                                               ; preds = %24, %14
  %16 = load i32, ptr %7, align 4
  %17 = icmp ult i32 %16, 256
  br i1 %17, label %18, label %27

18:                                               ; preds = %15
  %19 = load ptr, ptr %5, align 8
  %20 = load i32, ptr %7, align 4
  %21 = load i32, ptr %6, align 4
  %22 = call i32 @sim_rand()
  %23 = urem i32 %22, 2
  call void @set_gen_value(ptr noundef %19, i32 noundef %20, i32 noundef %21, i32 noundef %23)
  br label %24

24:                                               ; preds = %18
  %25 = load i32, ptr %7, align 4
  %26 = add i32 %25, 1
  store i32 %26, ptr %7, align 4
  br label %15, !llvm.loop !6

27:                                               ; preds = %15
  br label %28

28:                                               ; preds = %27
  %29 = load i32, ptr %6, align 4
  %30 = add i32 %29, 1
  store i32 %30, ptr %6, align 4
  br label %11, !llvm.loop !8

31:                                               ; preds = %11
  %32 = load ptr, ptr %5, align 8
  call void @draw_gen(ptr noundef %32)
  br label %33

33:                                               ; preds = %38, %31
  %34 = load ptr, ptr %4, align 8
  %35 = load ptr, ptr %5, align 8
  %36 = call i32 @calc_gen(ptr noundef %34, ptr noundef %35)
  %37 = icmp ne i32 %36, 0
  br i1 %37, label %38, label %43

38:                                               ; preds = %33
  %39 = load ptr, ptr %4, align 8
  call void @draw_gen(ptr noundef %39)
  %40 = load ptr, ptr %5, align 8
  store ptr %40, ptr %8, align 8
  %41 = load ptr, ptr %4, align 8
  store ptr %41, ptr %5, align 8
  %42 = load ptr, ptr %8, align 8
  store ptr %42, ptr %4, align 8
  br label %33, !llvm.loop !9

43:                                               ; preds = %33
  ret i32 0
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr nocapture writeonly, i8, i64, i1 immarg) #1

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define internal void @set_gen_value(ptr noundef %0, i32 noundef %1, i32 noundef %2, i32 noundef %3) #0 {
  %5 = alloca ptr, align 8
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  store ptr %0, ptr %5, align 8
  store i32 %1, ptr %6, align 4
  store i32 %2, ptr %7, align 4
  store i32 %3, ptr %8, align 4
  %10 = load i32, ptr %6, align 4
  %11 = srem i32 %10, 256
  %12 = add nsw i32 %11, 256
  %13 = srem i32 %12, 256
  store i32 %13, ptr %6, align 4
  %14 = load i32, ptr %7, align 4
  %15 = srem i32 %14, 128
  %16 = add nsw i32 %15, 128
  %17 = srem i32 %16, 128
  store i32 %17, ptr %7, align 4
  %18 = load i32, ptr %7, align 4
  %19 = mul nsw i32 %18, 256
  %20 = load i32, ptr %6, align 4
  %21 = add nsw i32 %19, %20
  store i32 %21, ptr %9, align 4
  %22 = load i32, ptr %8, align 4
  %23 = icmp ne i32 %22, 0
  br i1 %23, label %24, label %35

24:                                               ; preds = %4
  %25 = load i32, ptr %9, align 4
  %26 = urem i32 %25, 32
  %27 = shl i32 1, %26
  %28 = load ptr, ptr %5, align 8
  %29 = load i32, ptr %9, align 4
  %30 = udiv i32 %29, 32
  %31 = zext i32 %30 to i64
  %32 = getelementptr inbounds i32, ptr %28, i64 %31
  %33 = load i32, ptr %32, align 4
  %34 = or i32 %33, %27
  store i32 %34, ptr %32, align 4
  br label %47

35:                                               ; preds = %4
  %36 = load i32, ptr %9, align 4
  %37 = urem i32 %36, 32
  %38 = shl i32 1, %37
  %39 = xor i32 %38, -1
  %40 = load ptr, ptr %5, align 8
  %41 = load i32, ptr %9, align 4
  %42 = udiv i32 %41, 32
  %43 = zext i32 %42 to i64
  %44 = getelementptr inbounds i32, ptr %40, i64 %43
  %45 = load i32, ptr %44, align 4
  %46 = and i32 %45, %39
  store i32 %46, ptr %44, align 4
  br label %47

47:                                               ; preds = %35, %24
  ret void
}

declare i32 @sim_rand() #2

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define internal void @draw_gen(ptr noundef %0) #0 {
  %2 = alloca ptr, align 8
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store ptr %0, ptr %2, align 8
  store i32 0, ptr %3, align 4
  br label %5

5:                                                ; preds = %25, %1
  %6 = load i32, ptr %3, align 4
  %7 = icmp ult i32 %6, 128
  br i1 %7, label %8, label %28

8:                                                ; preds = %5
  store i32 0, ptr %4, align 4
  br label %9

9:                                                ; preds = %21, %8
  %10 = load i32, ptr %4, align 4
  %11 = icmp ult i32 %10, 256
  br i1 %11, label %12, label %24

12:                                               ; preds = %9
  %13 = load i32, ptr %4, align 4
  %14 = load i32, ptr %3, align 4
  %15 = load ptr, ptr %2, align 8
  %16 = load i32, ptr %4, align 4
  %17 = load i32, ptr %3, align 4
  %18 = call i32 @get_gen_value(ptr noundef %15, i32 noundef %16, i32 noundef %17)
  %19 = mul i32 65280, %18
  %20 = add i32 -16777216, %19
  call void @sim_set_pixel(i32 noundef %13, i32 noundef %14, i32 noundef %20)
  br label %21

21:                                               ; preds = %12
  %22 = load i32, ptr %4, align 4
  %23 = add i32 %22, 1
  store i32 %23, ptr %4, align 4
  br label %9, !llvm.loop !10

24:                                               ; preds = %9
  br label %25

25:                                               ; preds = %24
  %26 = load i32, ptr %3, align 4
  %27 = add i32 %26, 1
  store i32 %27, ptr %3, align 4
  br label %5, !llvm.loop !11

28:                                               ; preds = %5
  call void @sim_flush()
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define internal i32 @calc_gen(ptr noundef %0, ptr noundef %1) #0 {
  %3 = alloca ptr, align 8
  %4 = alloca ptr, align 8
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  %10 = alloca i32, align 4
  %11 = alloca i32, align 4
  %12 = alloca i32, align 4
  %13 = alloca i32, align 4
  store ptr %0, ptr %3, align 8
  store ptr %1, ptr %4, align 8
  store i32 0, ptr %5, align 4
  store i32 0, ptr %6, align 4
  store i32 0, ptr %7, align 4
  br label %14

14:                                               ; preds = %93, %2
  %15 = load i32, ptr %7, align 4
  %16 = icmp slt i32 %15, 128
  br i1 %16, label %17, label %96

17:                                               ; preds = %14
  store i32 0, ptr %8, align 4
  br label %18

18:                                               ; preds = %89, %17
  %19 = load i32, ptr %8, align 4
  %20 = icmp slt i32 %19, 256
  br i1 %20, label %21, label %92

21:                                               ; preds = %18
  store i32 0, ptr %9, align 4
  store i32 -1, ptr %10, align 4
  br label %22

22:                                               ; preds = %51, %21
  %23 = load i32, ptr %10, align 4
  %24 = icmp sle i32 %23, 1
  br i1 %24, label %25, label %54

25:                                               ; preds = %22
  store i32 -1, ptr %11, align 4
  br label %26

26:                                               ; preds = %47, %25
  %27 = load i32, ptr %11, align 4
  %28 = icmp sle i32 %27, 1
  br i1 %28, label %29, label %50

29:                                               ; preds = %26
  %30 = load i32, ptr %11, align 4
  %31 = icmp eq i32 %30, 0
  br i1 %31, label %32, label %36

32:                                               ; preds = %29
  %33 = load i32, ptr %10, align 4
  %34 = icmp eq i32 %33, 0
  br i1 %34, label %35, label %36

35:                                               ; preds = %32
  br label %47

36:                                               ; preds = %32, %29
  %37 = load ptr, ptr %4, align 8
  %38 = load i32, ptr %8, align 4
  %39 = load i32, ptr %11, align 4
  %40 = add nsw i32 %38, %39
  %41 = load i32, ptr %7, align 4
  %42 = load i32, ptr %10, align 4
  %43 = add nsw i32 %41, %42
  %44 = call i32 @get_gen_value(ptr noundef %37, i32 noundef %40, i32 noundef %43)
  %45 = load i32, ptr %9, align 4
  %46 = add i32 %45, %44
  store i32 %46, ptr %9, align 4
  br label %47

47:                                               ; preds = %36, %35
  %48 = load i32, ptr %11, align 4
  %49 = add nsw i32 %48, 1
  store i32 %49, ptr %11, align 4
  br label %26, !llvm.loop !12

50:                                               ; preds = %26
  br label %51

51:                                               ; preds = %50
  %52 = load i32, ptr %10, align 4
  %53 = add nsw i32 %52, 1
  store i32 %53, ptr %10, align 4
  br label %22, !llvm.loop !13

54:                                               ; preds = %22
  %55 = load ptr, ptr %4, align 8
  %56 = load i32, ptr %8, align 4
  %57 = load i32, ptr %7, align 4
  %58 = call i32 @get_gen_value(ptr noundef %55, i32 noundef %56, i32 noundef %57)
  store i32 %58, ptr %12, align 4
  %59 = load i32, ptr %12, align 4
  %60 = icmp ne i32 %59, 0
  br i1 %60, label %61, label %70

61:                                               ; preds = %54
  %62 = load i32, ptr %9, align 4
  %63 = icmp eq i32 %62, 2
  br i1 %63, label %67, label %64

64:                                               ; preds = %61
  %65 = load i32, ptr %9, align 4
  %66 = icmp eq i32 %65, 3
  br label %67

67:                                               ; preds = %64, %61
  %68 = phi i1 [ true, %61 ], [ %66, %64 ]
  %69 = zext i1 %68 to i32
  br label %74

70:                                               ; preds = %54
  %71 = load i32, ptr %9, align 4
  %72 = icmp eq i32 %71, 3
  %73 = zext i1 %72 to i32
  br label %74

74:                                               ; preds = %70, %67
  %75 = phi i32 [ %69, %67 ], [ %73, %70 ]
  store i32 %75, ptr %13, align 4
  %76 = load i32, ptr %12, align 4
  %77 = load i32, ptr %13, align 4
  %78 = icmp ne i32 %76, %77
  br i1 %78, label %79, label %80

79:                                               ; preds = %74
  store i32 1, ptr %5, align 4
  br label %80

80:                                               ; preds = %79, %74
  %81 = load i32, ptr %13, align 4
  %82 = icmp ne i32 %81, 0
  br i1 %82, label %83, label %84

83:                                               ; preds = %80
  store i32 1, ptr %6, align 4
  br label %84

84:                                               ; preds = %83, %80
  %85 = load ptr, ptr %3, align 8
  %86 = load i32, ptr %8, align 4
  %87 = load i32, ptr %7, align 4
  %88 = load i32, ptr %13, align 4
  call void @set_gen_value(ptr noundef %85, i32 noundef %86, i32 noundef %87, i32 noundef %88)
  br label %89

89:                                               ; preds = %84
  %90 = load i32, ptr %8, align 4
  %91 = add nsw i32 %90, 1
  store i32 %91, ptr %8, align 4
  br label %18, !llvm.loop !14

92:                                               ; preds = %18
  br label %93

93:                                               ; preds = %92
  %94 = load i32, ptr %7, align 4
  %95 = add nsw i32 %94, 1
  store i32 %95, ptr %7, align 4
  br label %14, !llvm.loop !15

96:                                               ; preds = %14
  %97 = load i32, ptr %5, align 4
  %98 = icmp ne i32 %97, 0
  br i1 %98, label %102, label %99

99:                                               ; preds = %96
  %100 = load i32, ptr %6, align 4
  %101 = icmp ne i32 %100, 0
  br label %102

102:                                              ; preds = %99, %96
  %103 = phi i1 [ true, %96 ], [ %101, %99 ]
  %104 = zext i1 %103 to i32
  ret i32 %104
}

declare void @sim_set_pixel(i32 noundef, i32 noundef, i32 noundef) #2

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define internal i32 @get_gen_value(ptr noundef %0, i32 noundef %1, i32 noundef %2) #0 {
  %4 = alloca ptr, align 8
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  store ptr %0, ptr %4, align 8
  store i32 %1, ptr %5, align 4
  store i32 %2, ptr %6, align 4
  %8 = load i32, ptr %5, align 4
  %9 = srem i32 %8, 256
  %10 = add nsw i32 %9, 256
  %11 = srem i32 %10, 256
  store i32 %11, ptr %5, align 4
  %12 = load i32, ptr %6, align 4
  %13 = srem i32 %12, 128
  %14 = add nsw i32 %13, 128
  %15 = srem i32 %14, 128
  store i32 %15, ptr %6, align 4
  %16 = load i32, ptr %6, align 4
  %17 = mul nsw i32 %16, 256
  %18 = load i32, ptr %5, align 4
  %19 = add nsw i32 %17, %18
  store i32 %19, ptr %7, align 4
  %20 = load ptr, ptr %4, align 8
  %21 = load i32, ptr %7, align 4
  %22 = udiv i32 %21, 32
  %23 = zext i32 %22 to i64
  %24 = getelementptr inbounds i32, ptr %20, i64 %23
  %25 = load i32, ptr %24, align 4
  %26 = load i32, ptr %7, align 4
  %27 = urem i32 %26, 32
  %28 = lshr i32 %25, %27
  %29 = and i32 %28, 1
  ret i32 %29
}

declare void @sim_flush() #2

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
