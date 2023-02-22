/*
 * coroutine.h is copyright 1995, 2000 Simon Tatham.
 * Modernized/improved 2023 Tyge Løvset.
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL SIMON TATHAM BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
/*
 * Coroutine mechanics, implemented on top of standard ANSI C. See
 * https://www.chiark.greenend.org.uk/~sgtatham/coroutines.html for
 * a full discussion of the theory behind this.
 * 
 * To use these macros to define a coroutine, you need to write a
 * function that looks something like this:
 * 
 * [Re-entrant version using an explicit context structure]
 * 
 * int ascending(ccontext* ctx) {
 *    cco_context(ctx,
 *      int idx;
 *    );
 *
 *    cco_routine(c,
 *      for (c->idx = 0; c->idx < 10; c->idx += 1)
 *        cco_yield(c->idx);
 *
 *      cco_finish:; // add this to support cco_stop!
 *    );
 *    return -1;
 * }
 * 
 * In the re-entrant version, you need to declare your persistent
 * variables by the `cco_context' macro. This macro takes the base
 * ccontext as first parameter.
 *
 * Note that the context variable is set back to zero when the
 * coroutine terminates (by cco_stop, or by control reaching
 * finish. This can make the re-entrant coroutines more useful
 * than the static ones, because you can tell when they have
 * finished.
 * 
 * This mechanism could have been better implemented using GNU C
 * and its ability to store pointers to labels, but sadly this is
 * not part of the ANSI C standard and so the mechanism is done by
 * case statements instead. That's why you can't put a cco_yield()
 * inside a switch-statement.
 * 
 * The re-entrant macros will malloc() the state structure on first
 * call, and free() it when end is reached. If you want to
 * abort in the middle, you can use, the caller should call the
 * coroutine with `cco_stop(&ctx)' as parameter instead of &ctx alone.
 *
 * Ground rules:
 *  - never put `c_yield' within an explicit `switch'.
 *  - never put two `c_yield' statements on the same source line.
 *  - add `cco_finish:' label at the end of the coroutine, before
 *    any cleanup code. Required to support cco_stop(ctx) argument.
 * 
 * The caller of a re-entrant coroutine must provide a context
 * variable:
 * 
 * void main(void) {
 *    ccontext z = 0;
 *    for (;;) {
 *       int x = ascending(&z);
 *       if (!z) break;
 *       printf("got number %d\n", x);
 *
 *       // stop if x == 5:
 *       if (x == 5) ascending(cco_stop(&z));
 *    }
 * }
 */

#ifndef COROUTINE_H
#define COROUTINE_H

#include <stdlib.h>
#include <assert.h>
/*
 * `c_' macros for re-entrant coroutines.
 */
typedef struct { 
    int _ccoline;
} *ccontext;

#define cco_context(cref, ...) \
    struct ccoContext { \
        int _ccoline; \
        __VA_ARGS__ \
    } **_ccoparam = (struct ccoContext **)cref

#define cco_routine(ctx, ...) \
    do { \
        if (!*_ccoparam) { \
            *_ccoparam = malloc(sizeof **_ccoparam); \
            (*_ccoparam)->_ccoline = 0; \
        } \
        struct ccoContext *ctx = *_ccoparam; \
        switch (ctx->_ccoline) { \
            case 0: __VA_ARGS__ break; \
            default: assert(!"cco_finish: missing"); \
        } \
        free(ctx), *_ccoparam = 0; \
    } while (0)

#define cco_yield(ret) \
    do { \
        (*_ccoparam)->_ccoline = __LINE__; return ret; \
        case __LINE__:; \
    } while (0)

#define cco_finish case -1

#define cco_stop(ctx) \
    ((*(ctx))->_ccoline = -1, (ctx))

#endif /* COROUTINE_H */