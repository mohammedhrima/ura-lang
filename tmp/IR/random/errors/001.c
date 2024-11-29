#ifndef __CRT_H_
#define __CRT_H_

#include <stdint.h>
#include <string.h>

#define CRT_HAS_INITFINI_ARRAY

void CRTStartup(void);
void __libc_init_array(void);
void __libc_fini_array(void);

// Define this function in your application to control the statup behavior.
// This function should, at a minimum, call CRTStartup() and then call main().
// A default (weakly-linked) implementation is provided in libc that does just that.
// You can override it with a strong definition.
int entry(void);

/** Stack Check Failure Callback
 *
 * This function shall abort the function that called it with a message that a stack overflow
 * has been deteted, and then halt the program via `exit`, `abort`, or a custom panic handler.
 *
 * This function must not return!
 *
 * This function is weakly linked. You can define your own implementation in your program
 * and it will be used instead of the library default.
 *
 * By default, this function prints an error message via `printf` and calls abort().
 *
 * @post The program is terminated.
 */
__attribute__((noreturn)) void __stack_chk_fail(void);

#ifndef DISABLE_STACK_CHK_GUARD_RUNTIME_CONFIG
/** Initialize __stack_chk_guard during boot
 *
 * This function is called during the startup process as a ((constructor)),
 * ensuring it will run before the system is configured.
 *
 * * This function is weakly linked. You can define your own implementation in your program
 * and it will be used instead of the library default.
 *
 * By default, this function sets __stack_chk_guard to the pre-defined canary value.
 * Our recommendation is to override this function on your own system to randomly generate
 * the value used for __stack_chk_guard during boot or to use a unique value tied to
 * the each hardware entity.
 *
 * @returns The value to be used for __stack_chk_guard.
 */
uintptr_t __stack_chk_guard_init(void);
#endif


#endif // __CRT_H_

// #include <crt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef STACK_CHK_GUARD_VALUE
#if UINTPTR_MAX == UINT32_MAX
#define STACK_CHK_GUARD_VALUE 0xa5f3cc8d
#else
#define STACK_CHK_GUARD_VALUE 0xdeadbeefa55a8572
#endif
#endif

#ifdef DISABLE_STACK_CHK_GUARD_RUNTIME_CONFIG
__attribute__((weak)) uintptr_t __stack_chk_guard = STACK_CHK_GUARD_VALUE;
#else
/** Stack check guard variable
 *
 * The value of this variable is used as a stack canary to detect
 * whether an overflow has occurred.
 */
__attribute__((weak)) uintptr_t __stack_chk_guard = 0;

/*
 * Stack protection *must* be disabled for this function. In the case of
 * -fstack-protector-all, this function will fail the check because it
 * changes the value of __stack_chk_guard.
 */
static void __attribute__((constructor, no_stack_protector)) __construct_stk_chk_guard()
{
	if(__stack_chk_guard == 0)
	{
		__stack_chk_guard = __stack_chk_guard_init();
	}
}

__attribute__((weak)) uintptr_t __stack_chk_guard_init(void)
{
	return STACK_CHK_GUARD_VALUE;
}
#endif // ! DISABLE_STACK_CHK_GUARD_RUNTIME_CONFIG

__attribute__((weak, noreturn)) void __stack_chk_fail(void)
{
	printf("Stack overflow detected! exiting the program.\n");
    exit(1);
	// abort();
}

void stack_overflows_here();

const char* buffer_long = "This is a long lontart"
                          "artertertrtertregdfgfd"
                          "tggggggggggggggggggggg"
                          "ggggggggggggg stringff";

void stack_overflows_here()
{
    char buffer_short[20];

    strcpy(buffer_short, buffer_long);
    printf("Overflow case run.");
}

int main(void)
{
    stack_overflows_here();
    printf("Running stack overflow test program:\n");

    return 0;
}

/*
int a
char c
3 byte
long b


*/