+ structures can be defined in llvm ir, check 1:01:48

+ specify pointers types use i32* instead of ptr because it's ambiguous
    + you can store i32 %value, i32*value (to change the value that pointer point to)
    + check also 1:26:59
    + store i32 %x1, i32* %x (works fine)

+ start working string struct

+ use data_type function instead of find(DATA_TYPES, 0)
+ check access for stack and heap
+ methods for structs
+ whenever there is check return syntax_error

+ fix the bug in declaring varibale with same name but inside if statment
+ assembly cod  need to be cheked and well confirmed

+ sizeof("hello world") return sizeof(CHARS): needs to be checked
+ implement output builtin
+ declaring variable with name self is forbiden

this is the link git@github.com:mohammedhrima/ura-vscode-extension.git, where hte extension is saved in github, when pushing to ura-lang github I must like push onlt the ura-lang code wihtout getting errors, and the extension code has it's own repo to push to it,
