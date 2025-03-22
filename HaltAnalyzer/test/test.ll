
define dso_local i32 @foo(i32 %x, i32 %y) #0 {
entry:
  %retval = alloca i32, align 4
  %x.addr = alloca i32, align 4
  %y.addr = alloca i32, align 4
  store i32 %x, i32* %x.addr, align 4
  store i32 %y, i32* %y.addr, align 4
  %0 = load i32, i32* %x.addr, align 4
  %cmp = icmp slt i32 %0, 43
  br i1 %cmp, label %if.then, label %if.else4

if.then:                                          ; preds = %entry
  %call = call i32 (...) @my_halt()
  %1 = load i32, i32* %y.addr, align 4
  %cmp1 = icmp sgt i32 %1, 45
  br i1 %cmp1, label %if.then2, label %if.else

if.then2:                                         ; preds = %if.then
  %2 = load i32, i32* %x.addr, align 4
  %add = add nsw i32 %2, 1
  store i32 %add, i32* %retval, align 4
  br label %return

if.else:                                          ; preds = %if.then
  %call3 = call i32 (...) @bar()
  %3 = load i32, i32* %x.addr, align 4
  store i32 %3, i32* %retval, align 4
  br label %return

if.else4:                                         ; preds = %entry
  %4 = load i32, i32* %y.addr, align 4
  store i32 %4, i32* %retval, align 4
  br label %return

return:                                           ; preds = %if.else4, %if.else, %if.then2
  %5 = load i32, i32* %retval, align 4
  ret i32 %5
}

declare dso_local i32 @my_halt(...) #1

declare dso_local i32 @bar(...) #1

