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

declare float @sin(float)

declare float @cos(float)

declare float @tan(float)

declare float @asin(float)

declare float @acos(float)

declare float @atan(float)

declare float @atan2(float, float)

declare float @sinh(float)

declare float @cosh(float)

declare float @tanh(float)

declare float @asinh(float)

declare float @acosh(float)

declare float @atanh(float)

declare float @exp(float)

declare float @exp2(float)

declare float @expm1(float)

declare float @log(float)

declare float @log10(float)

declare float @log2(float)

declare float @log1p(float)

declare float @pow(float, float)

declare float @sqrt(float)

declare float @cbrt(float)

declare float @hypot(float, float)

declare float @ceil(float)

declare float @floor(float)

declare float @trunc(float)

declare float @round(float)

declare i64 @lround(float)

declare float @nearbyint(float)

declare float @rint(float)

declare i64 @lrint(float)

declare float @fmod(float, float)

declare float @remainder(float, float)

declare float @remquo(float, float, ptr)

declare float @copysign(float, float)

declare float @nextafter(float, float)

declare float @nexttoward(float, float)

declare float @ldexp(float, i32)

declare float @frexp(float, ptr)

declare float @modf(float, ptr)

declare float @scalbn(float, i32)

declare float @scalbln(float, i64)

declare i32 @ilogb(float)

declare float @logb(float)

declare float @fabs(float)

declare float @fdim(float, float)

declare float @fmax(float, float)

declare float @fmin(float, float)

declare float @fma(float, float, float)

declare i32 @fpclassify(float)

declare i32 @isfinite(float)

declare i32 @isinf(float)

declare i32 @isnan(float)

declare i32 @isnormal(float)

declare i32 @signbit(float)

declare i32 @isgreater(float, float)

declare i32 @isgreaterequal(float, float)

declare i32 @isless(float, float)

declare i32 @islessequal(float, float)

declare i32 @islessgreater(float, float)

declare i32 @isunordered(float, float)

declare float @erf(float)

declare float @erfc(float)

declare float @tgamma(float)

declare float @lgamma(float)

declare float @j0(float)

declare float @j1(float)

declare float @jn(i32, float)

declare float @y0(float)

declare float @y1(float)

declare float @yn(i32, float)

define i32 @main() {
entry:
  %str = alloca ptr, align 8
  store ptr @STR0, ptr %str, align 8
  %str1 = load ptr, ptr %str, align 8
  %puts = call i32 @puts(ptr %str1)
  ret i32 0
}
