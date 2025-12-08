; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"

@STR0 = private unnamed_addr constant [6 x i8] c"CHARS\00", align 1

declare ptr @fopen(ptr, ptr)

declare ptr @freopen(ptr, ptr, ptr)

declare i32 @fclose(ptr)

declare i32 @fflush(ptr)

declare i32 @fgetc(ptr)

declare i32 @fputc(i32, ptr)

declare i32 @getc(ptr)

declare i32 @putc(i32, ptr)

declare i32 @getchar()

declare i32 @putchar(i8)

declare i32 @ungetc(i32, ptr)

declare ptr @fgets(ptr, i32, ptr)

declare i32 @fputs(ptr, ptr)

declare i32 @puts(ptr)

declare ptr @gets(ptr)

declare i32 @fread(ptr, i32, i32, ptr)

declare i32 @fwrite(ptr, i32, i32, ptr)

declare i32 @fseek(ptr, i64, i32)

declare i64 @ftell(ptr)

declare void @rewind(ptr)

declare i32 @fgetpos(ptr, ptr)

declare i32 @fsetpos(ptr, ptr)

declare i32 @feof(ptr)

declare i32 @ferror(ptr)

declare void @clearerr(ptr)

declare void @perror(ptr)

declare i32 @setvbuf(ptr, ptr, i32, i32)

declare void @setbuf(ptr, ptr)

declare i32 @remove(ptr)

declare i32 @rename(ptr, ptr)

declare ptr @tmpfile()

declare ptr @tmpnam(ptr)

declare i32 @fileno(ptr)

declare ptr @fdopen(i32, ptr)

declare i32 @printf(ptr)

declare i32 @fprintf(ptr, ptr)

declare i32 @sprintf(ptr, ptr)

declare i32 @snprintf(ptr, i32, ptr)

declare i32 @dprintf(i32, ptr)

declare i32 @vprintf(ptr, ptr)

declare i32 @vfprintf(ptr, ptr, ptr)

declare i32 @vsprintf(ptr, ptr, ptr)

declare i32 @vsnprintf(ptr, i32, ptr, ptr)

declare i32 @vdprintf(i32, ptr, ptr)

declare i32 @scanf(ptr)

declare i32 @fscanf(ptr, ptr)

declare i32 @sscanf(ptr, ptr)

declare i32 @vscanf(ptr, ptr)

declare i32 @vfscanf(ptr, ptr, ptr)

declare i32 @vsscanf(ptr, ptr, ptr)

define i32 @main() {
entry:
  %str = alloca ptr, align 8
  store ptr @STR0, ptr %str, align 8
  %str1 = load ptr, ptr %str, align 8
  %puts = call i32 @puts(ptr %str1)
  ret i32 0
}
