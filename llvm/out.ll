; ModuleID = 'logical_ops'
source_filename = "logical_ops"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 1, ptr %a, align 4
  %b = alloca i32, align 4
  store i32 2, ptr %b, align 4
  %a_val = load i32, ptr %a, align 4
  %b_val = load i32, ptr %b, align 4
  %a_gt_1 = icmp sgt i32 %a_val, 1
  %b_gt_1 = icmp sgt i32 %b_val, 1
  %logical_and = and i1 %a_gt_1, %b_gt_1
  %c = alloca i1, align 1
  store i1 %logical_and, ptr %c, align 1
  %logical_or = or i1 %a_gt_1, %b_gt_1
  %d = alloca i1, align 1
  store i1 %logical_or, ptr %d, align 1
  ret i32 0
}
