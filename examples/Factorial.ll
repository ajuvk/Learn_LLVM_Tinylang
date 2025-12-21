; ModuleID = 'Factorial.mod'
source_filename = "Factorial.mod"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i64 @_t4Fact4FACT(i64 %N) {
entry:
  %0 = icmp sle i64 %N, 1
  br i1 %0, label %if.body, label %after.if

if.body:                                          ; preds = %entry
  ret i64 1

after.if:                                         ; preds = %entry
  br label %while.cond

while.cond:                                       ; preds = %while.body, %after.if
  %1 = phi i64 [ %4, %while.body ], [ 1, %after.if ]
  %2 = phi i64 [ %5, %while.body ], [ 1, %after.if ]
  %3 = icmp sle i64 %2, %N
  br i1 %3, label %while.body, label %after.while

while.body:                                       ; preds = %while.cond
  %4 = mul nsw i64 %1, %2
  %5 = add nsw i64 %2, 1
  br label %while.cond

after.while:                                      ; preds = %while.cond
  ret i64 %1
}
