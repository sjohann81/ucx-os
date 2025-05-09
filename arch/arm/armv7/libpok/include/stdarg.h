/*****************************************************************************/
/* stdarg.h                                                                  */
/*                                                                           */
/* Copyright (c) 1996 Texas Instruments Incorporated                         */
/* http://www.ti.com/                                                        */
/*                                                                           */
/*  Redistribution and  use in source  and binary forms, with  or without    */
/*  modification,  are permitted provided  that the  following conditions    */
/*  are met:                                                                 */
/*                                                                           */
/*     Redistributions  of source  code must  retain the  above copyright    */
/*     notice, this list of conditions and the following disclaimer.         */
/*                                                                           */
/*     Redistributions in binary form  must reproduce the above copyright    */
/*     notice, this  list of conditions  and the following  disclaimer in    */
/*     the  documentation  and/or   other  materials  provided  with  the    */
/*     distribution.                                                         */
/*                                                                           */
/*     Neither the  name of Texas Instruments Incorporated  nor the names    */
/*     of its  contributors may  be used to  endorse or  promote products    */
/*     derived  from   this  software  without   specific  prior  written    */
/*     permission.                                                           */
/*                                                                           */
/*  THIS SOFTWARE  IS PROVIDED BY THE COPYRIGHT  HOLDERS AND CONTRIBUTORS    */
/*  "AS IS"  AND ANY  EXPRESS OR IMPLIED  WARRANTIES, INCLUDING,  BUT NOT    */
/*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR    */
/*  A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT    */
/*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,    */
/*  SPECIAL,  EXEMPLARY,  OR CONSEQUENTIAL  DAMAGES  (INCLUDING, BUT  NOT    */
/*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,    */
/*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY    */
/*  THEORY OF  LIABILITY, WHETHER IN CONTRACT, STRICT  LIABILITY, OR TORT    */
/*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE    */
/*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.     */
/*                                                                           */
/*****************************************************************************/

#ifndef _STDARG
#define _STDARG

#pragma diag_push
#pragma CHECK_MISRA("-19.7") /* need function-like macros */
#pragma CHECK_MISRA("-19.10") /* need types as macro arguments */
#pragma CHECK_MISRA("-20.1") /* standard headers must define standard names */
#pragma CHECK_MISRA("-20.2") /* standard headers must define standard names */


/*
 * Unusual type definitions.
 */
#ifdef __TI_COMPILER_VERSION__
typedef struct __va_list_t {
    void * __ap;
} __va_list;
#elif defined(__GNUCLIKE_BUILTIN_VARARGS)
typedef __builtin_va_list	__va_list;	/* internally known to gcc */
#else
typedef	char *			__va_list;
#endif /* __GNUCLIKE_BUILTIN_VARARGS */
#if defined(__GNUCLIKE_BUILTIN_VAALIST) && !defined(__GNUC_VA_LIST) \
    && !defined(__NO_GNUC_VA_LIST)
#define __GNUC_VA_LIST
typedef __va_list		__gnuc_va_list;	/* compatibility w/GNU headers*/
#endif

#ifndef _VA_LIST_DECLARED
#define _VA_LIST_DECLARED
typedef __va_list va_list;
#endif

#define _VA_RNDUP(sz) 	(((sz) + 3) & ~3)
#define _VA_MASK(tp)     ((__ALIGNOF__(tp) == 8) ? 7 : 3)
#define _VA_ALN(ap,tp)  (((int)(ap) + _VA_MASK(tp)) & ~(_VA_MASK(tp)))

#define va_start(ap, parmN)						      \
   ((ap).__ap = ((void *)((((int)__va_parmadr(parmN))&~3) +                   \
		                              _VA_RNDUP(sizeof(parmN)))))

#ifdef __big_endian__
#define va_arg(_ap, _type)					 	      \
    (((_ap).__ap = (void*)(_VA_ALN((_ap).__ap, _type) +                       \
			                         _VA_RNDUP(sizeof(_type)))),  \
     (*(_type *)((int)(_ap).__ap -                                            \
		   (__va_argcsu(_type) ? _VA_RNDUP(sizeof(_type))	      \
		                       : (sizeof(_type))))))
#else
#define va_arg(_ap, _type) 						      \
   (((_ap).__ap = (void *)(_VA_ALN((_ap).__ap,_type) +                        \
			   _VA_RNDUP(sizeof(_type)))),                        \
	     (*(_type *) ((int)(_ap).__ap - _VA_RNDUP(sizeof(_type)))))
#endif

#define va_end(ap) ((void)0)
#define va_copy(dst, src) ((dst)=(src))


#pragma diag_pop

#endif /* _STDARG */

