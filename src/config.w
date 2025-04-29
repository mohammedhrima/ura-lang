// allocation
func proto pointer malloc(long size):
func proto pointer calloc(long size, long count):
func proto pointer realloc(pointer ptr, long size):

// String Operations
func proto int strlen(chars str):
func proto chars strcpy(chars dest, chars src):
func proto chars strncpy(chars dest, chars src, long n):
func proto chars strcat(chars dest, chars src):
func proto chars strncat(chars dest, chars src, long n):
func proto int strcmp(chars str1, chars str2):
func proto int strncmp(chars str1, chars str2, long n):
func proto chars strdup(chars str):
func proto chars strchr(chars str, int c):
func proto chars strstr(chars str1, chars str2):

// I/O Operations
func proto int read(int fd, chars buf, int count):
func proto int write(int fd, chars buf, int count):
func proto int putstr(chars str):
func proto int putnbr(int n):
func proto int puts(chars str):
func proto int putchar(int c):
func proto int getchar():
func proto int socket(int domain, int type, int protocol):

// Math Functions
func proto int abs(int n):
func proto long labs(long n):

// System/Process
func proto void exit(int status):

