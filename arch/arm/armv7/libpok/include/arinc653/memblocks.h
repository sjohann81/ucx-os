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

#include "config.h"

#ifndef JET_ARINC653_MEMBLOCKS
#define JET_ARINC653_MEMBLOCKS

#include <arinc653/types.h>

typedef NAME_TYPE MEMORY_BLOCK_NAME_TYPE;
typedef enum {
    MB_READ,
    MB_READ_WRITE
} MEMORY_BLOCK_MODE_TYPE;

typedef APEX_INTEGER MEMORY_BLOCK_SIZE_TYPE;

typedef struct {
    MESSAGE_ADDR_TYPE MEMORY_BLOCK_ADDR;
    MEMORY_BLOCK_MODE_TYPE MEMORY_BLOCK_MODE;
    MEMORY_BLOCK_SIZE_TYPE MEMORY_BLOCK_SIZE;
} MEMORY_BLOCK_STATUS_TYPE;


void GET_MEMORY_BLOCK_STATUS (
        /*in */ MEMORY_BLOCK_NAME_TYPE   MEMORY_BLOCK_NAME,
        /*out*/ MEMORY_BLOCK_STATUS_TYPE *MEMORY_BLOCK_STATUS,
        /*out*/ RETURN_CODE_TYPE         *RETURN_CODE);

#endif
