; NOTE: Assertions have been autogenerated by utils/update_test_checks.py
; RUN: opt < %s -sccp -S | FileCheck %s

@A = constant i32 10

define i712 @test1() {
; CHECK-LABEL: @test1(
; CHECK-NEXT:    [[BB:%.*]] = and i64 ptrtoint (i32* @A to i64), undef
; CHECK-NEXT:    [[C:%.*]] = icmp sge i64 [[BB]], 0
; CHECK-NEXT:    [[X:%.*]] = select i1 [[C]], i712 0, i712 1
; CHECK-NEXT:    ret i712 [[X]]
;
  %P = getelementptr i32, i32* @A, i32 0
  %B = ptrtoint i32* %P to i64
  %BB = and i64 %B, undef
  %C = icmp sge i64 %BB, 0
  %X = select i1 %C, i712 0, i712 1
  ret i712 %X
}



define i712 @test2(i1 %C) {
; CHECK-LABEL: @test2(
; CHECK-NEXT:    ret i712 0
;
  %X = select i1 %C, i712 0, i712 undef
  ret i712 %X
}
