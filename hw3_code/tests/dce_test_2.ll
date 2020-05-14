; ModuleID = 'dce_test_2.bc'
source_filename = "tests/dce_test_2.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: noinline nounwind optnone uwtable
define i32 @foo(i32, i32) #0 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 %0, i32* %3, align 4
  store i32 %1, i32* %4, align 4
  %5 = load i32, i32* %3, align 4
  %6 = sub nsw i32 %5, 3
  store i32 %6, i32* %3, align 4
  %7 = load i32, i32* %3, align 4
  %8 = load i32, i32* %4, align 4
  %9 = mul nsw i32 %7, %8
  store i32 %9, i32* %4, align 4
  %10 = load i32, i32* %3, align 4
  %11 = load i32, i32* %3, align 4
  %12 = load i32, i32* %4, align 4
  %13 = add nsw i32 %11, %12
  %14 = sdiv i32 %10, %13
  store i32 %14, i32* %3, align 4
  %15 = load i32, i32* %3, align 4
  %16 = load i32, i32* %4, align 4
  %17 = mul nsw i32 %15, %16
  ret i32 %17
}

; Function Attrs: noinline nounwind optnone uwtable
define i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  store i32 0, i32* %1, align 4
  store i32 1, i32* %2, align 4
  store i32 2, i32* %3, align 4
  store i32 3, i32* %4, align 4
  %8 = load i32, i32* %2, align 4
  %9 = load i32, i32* %4, align 4
  %10 = load i32, i32* %3, align 4
  %11 = sdiv i32 %9, %10
  %12 = add nsw i32 %8, %11
  store i32 %12, i32* %7, align 4
  %13 = load i32, i32* %3, align 4
  %14 = call i32 @foo(i32 1, i32 5)
  %15 = icmp sgt i32 %13, %14
  br i1 %15, label %16, label %24

; <label>:16:                                     ; preds = %0
  store i32 5, i32* %2, align 4
  %17 = load i32, i32* %7, align 4
  %18 = mul nsw i32 %17, 2
  store i32 %18, i32* %7, align 4
  %19 = load i32, i32* %2, align 4
  %20 = load i32, i32* %7, align 4
  %21 = mul nsw i32 %19, %20
  %22 = load i32, i32* %4, align 4
  %23 = add nsw i32 %21, %22
  store i32 %23, i32* %4, align 4
  br label %34

; <label>:24:                                     ; preds = %0
  store i32 6, i32* %2, align 4
  %25 = load i32, i32* %2, align 4
  %26 = load i32, i32* %4, align 4
  %27 = sdiv i32 %25, %26
  store i32 %27, i32* %7, align 4
  %28 = load i32, i32* %7, align 4
  %29 = mul nsw i32 %28, 5
  %30 = load i32, i32* %3, align 4
  %31 = load i32, i32* %7, align 4
  %32 = mul nsw i32 %30, %31
  %33 = add nsw i32 %29, %32
  store i32 %33, i32* %4, align 4
  br label %34

; <label>:34:                                     ; preds = %24, %16
  %35 = load i32, i32* %3, align 4
  %36 = add nsw i32 %35, 1
  store i32 %36, i32* %2, align 4
  %37 = load i32, i32* %3, align 4
  %38 = add nsw i32 %37, 2
  store i32 %38, i32* %3, align 4
  %39 = load i32, i32* %3, align 4
  %40 = add nsw i32 %39, 3
  store i32 %40, i32* %2, align 4
  store i32 20, i32* %6, align 4
  %41 = load i32, i32* %6, align 4
  %42 = load i32, i32* %7, align 4
  %43 = mul nsw i32 %41, %42
  store i32 %43, i32* %7, align 4
  %44 = load i32, i32* %4, align 4
  ret i32 %44
}

attributes #0 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 6.0.0-1ubuntu2 (tags/RELEASE_600/final)"}
