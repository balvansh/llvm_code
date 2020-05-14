; ModuleID = 'licm_test_3.bc'
source_filename = "tests/licm_test_3.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: noinline nounwind optnone uwtable
define i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  %10 = alloca i32, align 4
  %11 = alloca i32, align 4
  store i32 0, i32* %1, align 4
  store i32 0, i32* %6, align 4
  store i32 0, i32* %7, align 4
  store i32 0, i32* %8, align 4
  br label %12

; <label>:12:                                     ; preds = %46, %0
  %13 = load i32, i32* %8, align 4
  %14 = icmp slt i32 %13, 100
  br i1 %14, label %15, label %49

; <label>:15:                                     ; preds = %12
  store i32 5, i32* %2, align 4
  %16 = load i32, i32* %8, align 4
  %17 = load i32, i32* %2, align 4
  %18 = add nsw i32 %16, %17
  store i32 %18, i32* %9, align 4
  %19 = load i32, i32* %9, align 4
  %20 = load i32, i32* %6, align 4
  %21 = add nsw i32 %20, %19
  store i32 %21, i32* %6, align 4
  %22 = load i32, i32* %6, align 4
  %23 = icmp ne i32 %22, 0
  br i1 %23, label %24, label %25

; <label>:24:                                     ; preds = %15
  store i32 6, i32* %4, align 4
  br label %26

; <label>:25:                                     ; preds = %15
  store i32 7, i32* %5, align 4
  br label %26

; <label>:26:                                     ; preds = %25, %24
  %27 = load i32, i32* %2, align 4
  %28 = mul nsw i32 %27, 5
  store i32 %28, i32* %7, align 4
  store i32 0, i32* %10, align 4
  br label %29

; <label>:29:                                     ; preds = %42, %26
  %30 = load i32, i32* %10, align 4
  %31 = icmp slt i32 %30, 10
  br i1 %31, label %32, label %45

; <label>:32:                                     ; preds = %29
  store i32 10, i32* %2, align 4
  store i32 5, i32* %3, align 4
  %33 = load i32, i32* %10, align 4
  %34 = load i32, i32* %3, align 4
  %35 = add nsw i32 %33, %34
  store i32 %35, i32* %11, align 4
  %36 = load i32, i32* %11, align 4
  %37 = load i32, i32* %6, align 4
  %38 = add nsw i32 %37, %36
  store i32 %38, i32* %6, align 4
  %39 = load i32, i32* %7, align 4
  %40 = load i32, i32* %6, align 4
  %41 = add nsw i32 %40, %39
  store i32 %41, i32* %6, align 4
  br label %42

; <label>:42:                                     ; preds = %32
  %43 = load i32, i32* %10, align 4
  %44 = add nsw i32 %43, 1
  store i32 %44, i32* %10, align 4
  br label %29

; <label>:45:                                     ; preds = %29
  br label %46

; <label>:46:                                     ; preds = %45
  %47 = load i32, i32* %8, align 4
  %48 = add nsw i32 %47, 1
  store i32 %48, i32* %8, align 4
  br label %12

; <label>:49:                                     ; preds = %12
  %50 = load i32, i32* %4, align 4
  %51 = add nsw i32 %50, 2
  store i32 %51, i32* %3, align 4
  %52 = load i32, i32* %2, align 4
  %53 = add nsw i32 %52, 4
  store i32 %53, i32* %5, align 4
  %54 = load i32, i32* %6, align 4
  ret i32 %54
}

attributes #0 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 6.0.0-1ubuntu2 (tags/RELEASE_600/final)"}
