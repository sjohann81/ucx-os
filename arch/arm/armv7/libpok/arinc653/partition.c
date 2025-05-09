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

#ifdef POK_NEEDS_ARINC653_PARTITION

#include <arinc653/types.h>
#include <arinc653/partition.h>

#include <core/partition.h>

#include <errno.h>
#include <utils.h>

#define MAP_ERROR(from, to) case (from): *return_code = (to); break
#define MAP_ERROR_DEFAULT(to) default: *return_code = (to); break

void GET_PARTITION_STATUS (PARTITION_STATUS_TYPE *partition_status,
                           RETURN_CODE_TYPE      *return_code)
{
  pok_partition_status_t core_status;

  pok_current_partition_get_status(&core_status);

  partition_status->IDENTIFIER = core_status.id;
  partition_status->PERIOD = core_status.period;
  partition_status->DURATION = core_status.duration;
  partition_status->LOCK_LEVEL = core_status.lock_level;

#define MAP(from, to) case (from): partition_status->OPERATING_MODE = (to); break
  switch (core_status.mode) {
    MAP(POK_PARTITION_MODE_IDLE, IDLE);
    MAP(POK_PARTITION_MODE_NORMAL, NORMAL);
    MAP(POK_PARTITION_MODE_INIT_COLD, COLD_START);
    MAP(POK_PARTITION_MODE_INIT_WARM, WARM_START);
  }
#undef MAP

  partition_status->START_CONDITION = core_status.start_condition; // TODO proper conversion

  *return_code = NO_ERROR;
}

void SET_PARTITION_MODE (OPERATING_MODE_TYPE operating_mode,
                         RETURN_CODE_TYPE *return_code)
{
  pok_partition_mode_t core_mode;
  pok_ret_t            core_ret;

   switch (operating_mode)
   {
      case IDLE:
         core_mode = POK_PARTITION_MODE_IDLE;
         break;

      case NORMAL:
         core_mode = POK_PARTITION_MODE_NORMAL;
         break;

      case COLD_START:
         core_mode = POK_PARTITION_MODE_INIT_COLD;
         break;

      case WARM_START:
         core_mode = POK_PARTITION_MODE_INIT_WARM;
         break;

      default:
         *return_code = INVALID_PARAM;
         return;
   }

   core_ret = pok_partition_set_mode (core_mode);

   switch (core_ret) {
      MAP_ERROR(POK_ERRNO_OK, NO_ERROR);
      MAP_ERROR(POK_ERRNO_UNAVAILABLE, NO_ACTION);
      MAP_ERROR(POK_ERRNO_PARTITION_MODE, INVALID_MODE);
      MAP_ERROR_DEFAULT(INVALID_PARAM);
   }
}

#endif
