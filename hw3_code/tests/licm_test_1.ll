; ModuleID = 'licm_test_1.bc'
source_filename = "tests/licm_test_1.c"
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
  store i32 0, i32* %1, align 4
  store i32 5, i32* %2, align 4
  store i32 3, i32* %7, align 4
  %8 = load i32, i32* %7, align 4
  %9 = icmp slt i32 %8, 2
  br i1 %9, label %c, label %b

b:                                     ; preds = %0
  %10 = load i32, i32* %2, align 4
  %11 = icmp sle i32 %10, 0
  br i1 %11, label %c, label %dd

c:                                     ; preds = %b, %0
  store i32 -1, i32* %1, align 4
  br label %r

dd:                                     ; preds = %b
  store i32 0, i32* %2, align 4
  br label %e

e:                                     ; preds = %p, %dd
  %12 = load i32, i32* %2, align 4
  %13 = icmp slt i32 %12, 10
  br i1 %13, label %ff, label %q

ff:                                     ; preds = %e
  %14 = load i32, i32* %2, align 4
  %15 = add nsw i32 %14, 1
  store i32 %15, i32* %3, align 4
  %16 = load i32, i32* %2, align 4
  %17 = mul nsw i32 %16, 5
  store i32 %17, i32* %4, align 4
  %18 = load i32, i32* %3, align 4
  %19 = sdiv i32 %18, 2
  store i32 %19, i32* %4, align 4
  store i32 1, i32* %3, align 4
  br label %g

g:                                     ; preds = %n, %ff
  %20 = load i32, i32* %3, align 4
  %21 = icmp slt i32 %20, 10
  br i1 %21, label %s, label %o

s:                                     ; preds = %g
  br label %h

h:                                     ; preds = %i, %s
  %22 = load i32, i32* %4, align 4
  %23 = icmp ugt i32 %22, 0
  br i1 %23, label %i, label %j

i:                                     ; preds = %h
  %24 = load i32, i32* %4, align 4
  %25 = add i32 %24, -1
  store i32 %25, i32* %4, align 4
  br label %h

j:                                     ; preds = %h
  %26 = load i32, i32* %4, align 4
  %27 = load i32, i32* %2, align 4
  %28 = icmp eq i32 %26, %27
  br i1 %28, label %k, label %l

k:                                     ; preds = %j
  %29 = load i32, i32* %2, align 4
  %30 = srem i32 %29, 3
  store i32 %30, i32* %2, align 4
  %31 = load i32, i32* %2, align 4
  %32 = mul nsw i32 %31, 2
  store i32 %32, i32* %4, align 4
  br label %m

l:                                     ; preds = %j
  store i32 30, i32* %4, align 4
  br label %m

m:                                     ; preds = %l, %k
  br label %n

n:                                     ; preds = %m
  %33 = load i32, i32* %3, align 4
  %34 = add nsw i32 %33, 1
  store i32 %34, i32* %3, align 4
  br label %g

o:                                     ; preds = %g
  %35 = load i32, i32* %2, align 4
  %36 = mul nsw i32 %35, 2
  store i32 %36, i32* %4, align 4
  %37 = load i32, i32* %3, align 4
  %38 = sdiv i32 %37, 5
  store i32 %38, i32* %5, align 4
  store i32 30, i32* %6, align 4
  br label %p

p:                                     ; preds = %o
  %39 = load i32, i32* %2, align 4
  %40 = add nsw i32 %39, 1
  store i32 %40, i32* %2, align 4
  br label %e

q:                                     ; preds = %e
  %41 = load i32, i32* %7, align 4
  store i32 %41, i32* %1, align 4
  br label %r

r:                                     ; preds = %q, %c
  %42 = load i32, i32* %1, align 4
  ret i32 %42
}

attributes #0 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 6.0.0-1ubuntu2 (tags/RELEASE_600/final)"}
