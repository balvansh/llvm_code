; ModuleID = 'tests/licm_test_2.bc'
source_filename = "tests/licm_test_2.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: noinline nounwind optnone uwtable
define i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  store i32 0, i32* %1, align 4
  store i32 1, i32* %3, align 4
  br label %4

; <label>:4:                                      ; preds = %8, %0
  %5 = load i32, i32* %3, align 4
  %6 = add nsw i32 %5, 1
  store i32 %6, i32* %3, align 4
  %7 = icmp slt i32 %5, 100
  br i1 %7, label %8, label %17

; <label>:8:                                      ; preds = %4
  store i32 0, i32* %2, align 4
  %9 = load i32, i32* %2, align 4
  %10 = add nsw i32 %9, 2
  store i32 %10, i32* %2, align 4
  %11 = load i32, i32* %2, align 4
  %12 = add nsw i32 %11, 3
  store i32 %12, i32* %2, align 4
  %13 = load i32, i32* %2, align 4
  %14 = mul nsw i32 %13, 4
  store i32 %14, i32* %2, align 4
  %15 = load i32, i32* %2, align 4
  %16 = sdiv i32 %15, 2
  store i32 %16, i32* %2, align 4
  br label %4

; <label>:17:                                     ; preds = %4
  %18 = load i32, i32* %2, align 4
  ret i32 %18
}

attributes #0 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 6.0.0-1ubuntu2 (tags/RELEASE_600/final)"}
