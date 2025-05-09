/*
 *                               POK header
 * 
 * The following file is a part of the POK project. Any modification should
 * made according to the POK licence. You CANNOT use this file or a part of
 * this file is this part of a file for your own project
 *
 * For more information on the POK licence, please see our LICENCE FILE
 *
 * Please follow the coding guidelines described in doc/CODING_GUIDELINES
 *
 *                                      Copyright (c) 2007-2009 POK team 
 *
 * Created by julien on Thu Jan 15 23:34:13 2009 
 */

#include <config.h>

#ifdef POK_NEEDS_ARINC653_ERROR
#include "include/types.h"
#include <errno.h>
#include <arinc653/types.h>
#include <arinc653/error.h>
#include <core/error.h>
#include <core/syscall.h>
#include <string.h>
#include <stdio.h>

#define MAP_ERROR(from, to) case (from): *RETURN_CODE = (to); break
#define MAP_ERROR_DEFAULT(to) default: *RETURN_CODE = (to); break

static ERROR_CODE_TYPE error_pok_to_arinc(int pok_error)
{
    #define MAP(from, to) case (from): return (to);
    switch (pok_error) {
        MAP(POK_ERROR_KIND_DEADLINE_MISSED, DEADLINE_MISSED);
        MAP(POK_ERROR_KIND_APPLICATION_ERROR, APPLICATION_ERROR);
        MAP(POK_ERROR_KIND_NUMERIC_ERROR, NUMERIC_ERROR);
        MAP(POK_ERROR_KIND_ILLEGAL_REQUEST, ILLEGAL_REQUEST);
        MAP(POK_ERROR_KIND_STACK_OVERFLOW, STACK_OVERFLOW);
        MAP(POK_ERROR_KIND_MEMORY_VIOLATION, MEMORY_VIOLATION);
        MAP(POK_ERROR_KIND_HARDWARE_FAULT, HARDWARE_FAULT);
        MAP(POK_ERROR_KIND_POWER_FAIL, POWER_FAIL);
        default: return 0;
    }
    #undef MAP
}

/**
 * At this time, it is implemented to have the same behavior as 
 * RAISE_APPLICATION_ERROR. Should change that in the future
 *
 * XXX If I understand this correctly, it's essentially 
 * a logging function, and shouldn't raise any errors.
 */
void REPORT_APPLICATION_MESSAGE (MESSAGE_ADDR_TYPE    MESSAGE,
                                 MESSAGE_SIZE_TYPE    LENGTH,
                                 RETURN_CODE_TYPE     *RETURN_CODE )
{
   if (LENGTH < 0 || LENGTH > MAX_ERROR_MESSAGE_SIZE) {
       *RETURN_CODE = INVALID_PARAM;
       return;
   }

   // TODO LENGTH is ignored (shouldn't be)
   printf("%s\n", MESSAGE);

   *RETURN_CODE = NO_ERROR;
}

void CREATE_ERROR_HANDLER (SYSTEM_ADDRESS_TYPE  ENTRY_POINT,
                           STACK_SIZE_TYPE      STACK_SIZE,
                           RETURN_CODE_TYPE     *RETURN_CODE)
{
    pok_ret_t core_ret = pok_error_thread_create(STACK_SIZE, (void*)ENTRY_POINT);
    //core_ret = pok_syscall2 (POK_SYSCALL_ERROR_HANDLER_CREATE, (uint32_t)STACK_SIZE, (uint32_t)ENTRY_POINT);

    switch (core_ret) {
        MAP_ERROR(POK_ERRNO_OK, NO_ERROR);
        MAP_ERROR(POK_ERRNO_EXISTS, NO_ACTION);
        MAP_ERROR(POK_ERRNO_PARTITION_MODE, INVALID_MODE);
        MAP_ERROR(POK_ERRNO_EINVAL, INVALID_CONFIG);
        MAP_ERROR_DEFAULT(INVALID_CONFIG);
    }
}

void GET_ERROR_STATUS (ERROR_STATUS_TYPE  *ERROR_STATUS,
                       RETURN_CODE_TYPE   *RETURN_CODE )
{
    pok_error_status_t   core_status;
    pok_ret_t            core_ret;

    core_ret = pok_error_get (&core_status, ERROR_STATUS->MESSAGE);

    if (core_ret == POK_ERRNO_OK) {
        ERROR_STATUS->ERROR_CODE = error_pok_to_arinc(core_status.error_kind);
        ERROR_STATUS->LENGTH = core_status.msg_size;
        ERROR_STATUS->FAILED_PROCESS_ID = core_status.failed_thread + 1; // ARINC process IDs are one higher
        ERROR_STATUS->FAILED_ADDRESS = (SYSTEM_ADDRESS_TYPE)core_status.failed_addr;
    }

    switch (core_ret) {
        MAP_ERROR(POK_ERRNO_OK, NO_ERROR);
        MAP_ERROR(POK_ERRNO_THREAD, INVALID_CONFIG);
        MAP_ERROR(POK_ERRNO_UNAVAILABLE, NO_ACTION);
        MAP_ERROR_DEFAULT(NOT_AVAILABLE);
    }

}

void RAISE_APPLICATION_ERROR (ERROR_CODE_TYPE            ERROR_CODE,
                              MESSAGE_ADDR_TYPE          MESSAGE,
                              ERROR_MESSAGE_SIZE_TYPE    LENGTH,
                              RETURN_CODE_TYPE           *RETURN_CODE)
{
    if (LENGTH < 0 || LENGTH > MAX_ERROR_MESSAGE_SIZE) {
        *RETURN_CODE = INVALID_PARAM;
        return;
    }
    
    if ((ERROR_CODE != APPLICATION_ERROR)) {
        *RETURN_CODE = INVALID_PARAM;
        return;
    }
    
    pok_error_raise_application_error ((char*) MESSAGE, LENGTH);
    
    *RETURN_CODE = NO_ERROR;
}

#endif
