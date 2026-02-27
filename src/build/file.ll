; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"
target triple = "arm64-apple-darwin25.3.0"

@STR0 = private unnamed_addr constant [13 x i8] c"Hello world\0A\00", align 1
@STR1 = private unnamed_addr constant [3 x i8] c"%s\00", align 1

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

declare i32 @write(i32, ptr, i32)

declare i32 @read(i32, ptr, i32)

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

declare i32 @printf(ptr, i32, ...)

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

declare i32 @strlen(ptr)

declare i32 @strnlen(ptr, i32)

declare i32 @strcmp(ptr, ptr)

declare i32 @strncmp(ptr, ptr, i32)

declare i32 @strcasecmp(ptr, ptr)

declare i32 @strncasecmp(ptr, ptr, i32)

declare i32 @strcoll(ptr, ptr)

declare i32 @strxfrm(ptr, ptr, i32)

declare ptr @strcpy(ptr, ptr)

declare ptr @strncpy(ptr, ptr, i32)

declare ptr @strdup(ptr)

declare ptr @strndup(ptr, i32)

declare ptr @strcat(ptr, ptr)

declare ptr @strncat(ptr, ptr, i32)

declare ptr @strchr(ptr, i32)

declare ptr @strrchr(ptr, i32)

declare ptr @strstr(ptr, ptr)

declare ptr @strcasestr(ptr, ptr)

declare i32 @strspn(ptr, ptr)

declare i32 @strcspn(ptr, ptr)

declare ptr @strpbrk(ptr, ptr)

declare ptr @strtok(ptr, ptr)

declare ptr @strtok_r(ptr, ptr, ptr)

declare ptr @strerror(i32)

declare i32 @strerror_r(i32, ptr, i32)

declare ptr @memchr(ptr, i32, i32)

declare ptr @memrchr(ptr, i32, i32)

declare i32 @memcmp(ptr, ptr, i32)

declare ptr @memcpy(ptr, ptr, i32)

declare ptr @memmove(ptr, ptr, i32)

declare ptr @memset(ptr, i32, i32)

declare ptr @memccpy(ptr, ptr, i32, i32)

declare i32 @strlcpy(ptr, ptr, i32)

declare i32 @strlcat(ptr, ptr, i32)

declare ptr @strsignal(i32)

declare ptr @memset_s(ptr, i32, i32, i32)

define i32 @main() {
entry:
  %str = alloca ptr, align 8
  store ptr null, ptr %str, align 8
  %stack = alloca [13 x i8], align 1
  %stack1 = getelementptr [13 x i8], ptr %stack, i32 0, i32 0
  store ptr %stack1, ptr %str, align 8
  %str2 = load ptr, ptr %str, align 8
  %strcpy = call ptr @strcpy(ptr %str2, ptr @STR0)
  %str3 = load ptr, ptr %str, align 8
  %printf = call i32 (ptr, i32, ...) @printf(ptr @STR1, i32 1, ptr %str3)
  ret i32 0
}
