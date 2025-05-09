/*
 * COPIED! DO NOT MODIFY!
 *
 * Instead of modifying this file, modify original one (kernel/include/uapi/partition_arinc_types.h).
 */
/*
 * Institute for System Programming of the Russian Academy of Sciences
 * Copyright (C) 2016 ISPRAS
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, Version 3.
 *
 * This program is distributed in the hope # that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License version 3 for more details.
 */

#ifndef __JET_UAPI_PARTITION_ARINC_H__
#define __JET_UAPI_PARTITION_ARINC_H__

#include <uapi/partition_types.h>

/**
 * \enum pok_partition_mode_t
 * \brief The different modes of a partition
 */
typedef enum
{ 
   /*
    * In init mode, only main thread (process) is run.
    * This's the only mode where one can create various resources.
    *
    * There's really no difference between cold and warm init.
    *   
    * When partition is initially started, it's in cold init.
    *
    * HM table and set_partition_mode function may restart 
    * partition into either cold or warm init mode.
    *
    * The exact type of init can be introspected by an application,
    * but otherwise, it makes no difference.
    */
   POK_PARTITION_MODE_INIT_COLD = 1, 
   POK_PARTITION_MODE_INIT_WARM = 2,

   /*
    * In normal mode, all threads except main can be run.
    *
    * No resources can be allocated.
    */
   POK_PARTITION_MODE_NORMAL    = 3, 

   /*
    * Partition is stopped.
    */
   POK_PARTITION_MODE_IDLE      = 4,
}pok_partition_mode_t;

typedef struct {
   pok_partition_id_t id;
   pok_time_t period;
   pok_time_t duration;
   int32_t lock_level;
   pok_partition_mode_t mode;
   pok_start_condition_t  start_condition;
} pok_partition_status_t;

#endif /* __JET_UAPI_PARTITION_ARINC_H__ */
