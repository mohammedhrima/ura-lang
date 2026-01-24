; ModuleID = 'module'
source_filename = "module"
target triple = "arm64-apple-darwin25.2.0"

@_ZTIi = external global ptr

define i32 @func() {
entry:
  %exception = call ptr @__cxa_allocate_exception(i64 4)
  store i32 12, ptr %exception, align 4
  call void @__cxa_throw(ptr %exception, ptr @_ZTIi, ptr null)
  unreachable

after.throw.0:                                    ; No predecessors!
  ret i32 0
}

declare ptr @__cxa_allocate_exception(i64)

; Function Attrs: noreturn
declare void @__cxa_throw(ptr, ptr, ptr) #0

define i32 @main() personality ptr @__gxx_personality_v0 {
entry:
  %exception.storage = alloca i32, align 4
  br label %try.1

try.1:                                            ; preds = %entry
  %func = invoke i32 @func()
          to label %invoke.cont.5 unwind label %lpad.2

lpad.2:                                           ; preds = %try.1
  %lpad = landingpad { ptr, i32 }
          catch ptr @_ZTIi
  %exc.ptr = extractvalue { ptr, i32 } %lpad, 0
  %caught = call ptr @__cxa_begin_catch(ptr %exc.ptr)
  %exception.value = load i32, ptr %caught, align 4
  store i32 %exception.value, ptr %exception.storage, align 4
  br label %catch.3

catch.3:                                          ; preds = %lpad.2
  %error = load i32, ptr %exception.storage, align 4
  call void @__cxa_end_catch()
  ret i32 %error

try.end.4:                                        ; preds = %if.end.6
  ret i32 0

invoke.cont.5:                                    ; preds = %try.1
  %x = alloca i32, align 4
  store i32 10, ptr %x, align 4
  %y = alloca i32, align 4
  store i32 0, ptr %y, align 4
  %y1 = load i32, ptr %y, align 4
  %equal = icmp eq i32 %y1, 0
  br i1 %equal, label %if.then.7, label %if.else.8

if.end.6:                                         ; preds = %if.else.8, %after.throw.9
  br label %try.end.4

if.then.7:                                        ; preds = %invoke.cont.5
  %exception = call ptr @__cxa_allocate_exception(i64 4)
  store i32 1, ptr %exception, align 4
  call void @__cxa_throw(ptr %exception, ptr @_ZTIi, ptr null)
  unreachable

if.else.8:                                        ; preds = %invoke.cont.5
  br label %if.end.6

after.throw.9:                                    ; No predecessors!
  %result = alloca i32, align 4
  %x2 = load i32, ptr %x, align 4
  %y3 = load i32, ptr %y, align 4
  %div = sdiv i32 %x2, %y3
  store i32 %div, ptr %result, align 4
  br label %if.end.6
}

declare i32 @__gxx_personality_v0(...)

declare ptr @__cxa_begin_catch(ptr)

declare void @__cxa_end_catch()

attributes #0 = { noreturn }
