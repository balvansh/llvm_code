; ModuleID = 'loop-test-mem2reg.bc'
source_filename = "lopp-test.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@.str = private unnamed_addr constant [3 x i8] c"%d\00", align 1

; Function Attrs: noinline nounwind uwtable
define i32 @main() #0 {
  br label %1

; <label>:1:                                      ; preds = %17, %0
  %.0 = phi i32 [ 0, %0 ], [ %.2, %17 ]
  %2 = icmp ne i32 undef, 10
  br i1 %2, label %3, label %19

; <label>:3:                                      ; preds = %1
  %4 = srem i32 undef, 2
  %5 = icmp eq i32 %4, 0
  br i1 %5, label %6, label %9

; <label>:6:                                      ; preds = %3
  %7 = mul nsw i32 4, undef
  %8 = add nsw i32 %.0, %7
  br label %17

; <label>:9:                                      ; preds = %3
  br label %10

; <label>:10:                                     ; preds = %14, %9
  %.01 = phi i32 [ 0, %9 ], [ %15, %14 ]
  %.1 = phi i32 [ %.0, %9 ], [ %13, %14 ]
  %11 = icmp slt i32 %.01, %.1
  br i1 %11, label %12, label %16

; <label>:12:                                     ; preds = %10
  %13 = add nsw i32 %.1, %.01
  br label %14

; <label>:14:                                     ; preds = %12
  %15 = add nsw i32 %.01, 1
  br label %10

; <label>:16:                                     ; preds = %10
  br label %17

; <label>:17:                                     ; preds = %16, %6
  %.2 = phi i32 [ %8, %6 ], [ %.1, %16 ]
  %18 = call i32 (i8*, i32, ...) bitcast (i32 (...)* @print to i32 (i8*, i32, ...)*)(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str, i32 0, i32 0), i32 %.2)
  br label %1

; <label>:19:                                     ; preds = %1
  ret i32 0
}

declare i32 @print(...) #1

attributes #0 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 6.0.0-1ubuntu2 (tags/RELEASE_600/final)"}
