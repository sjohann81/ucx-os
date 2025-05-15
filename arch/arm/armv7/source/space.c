/*
 * space.c
 *
 *  Created on: 14 mai 2019
 *      Author: Alexy
 */

#include <arch/deployment.h>
#include <stdint.h>
#include "include/types.h"
#include <common.h>
#include <libc.h>
#include <asp/arch.h>
#include <asp/space.h>
#include <mpu.h>
#include <space.h>

#include "../../boards/armv7/include/bsp/serial.h"
#define mpuSUBREGION_DISABLE (mpuSUBREGION0_DISABLE | mpuSUBREGION1_DISABLE | mpuSUBREGION2_DISABLE |\
                              mpuSUBREGION3_DISABLE | mpuSUBREGION4_DISABLE | mpuSUBREGION5_DISABLE |\
                              mpuSUBREGION6_DISABLE | mpuSUBREGION7_DISABLE)

#define USER_START_MPU_REGION 5

#if 0
static uint32_t mpu_get_size(uint32_t size)
{
    if(size < 33)
            return    MPU_32_BYTES;
    if(size < 65)
            return    MPU_64_BYTES;
    if(size < 129)
            return    MPU_128_BYTES;
    if(size < 257)
            return    MPU_256_BYTES;
    if(size < 513)
            return    MPU_512_BYTES;
    if(size < 1025)
            return    MPU_1_KB;
    if(size < 2049)
            return    MPU_2_KB;
    if(size < 4097)
            return    MPU_4_KB;
    if(size < 8193)
            return    MPU_8_KB;
    if(size < 16385)
            return    MPU_16_KB;
    if(size < 32769)
            return    MPU_32_KB;
    if(size < 65537)
            return    MPU_64_KB;
    if(size < 131073)
            return    MPU_128_KB;
    if(size < 262145)
            return    MPU_256_KB;
    if(size < 524289)
            return    MPU_512_KB;
    if(size < 1048577)
            return    MPU_1_MB;
    if(size < 2097153)
            return    MPU_2_MB;
    if(size < 4194305)
            return    MPU_4_MB;
    if(size < 8388609)
            return    MPU_8_MB;
    if(size < 16777217)
            return    MPU_16_MB;
    if(size < 33554433)
            return    MPU_32_MB;
    if(size < 67108865)
            return    MPU_64_MB;
    if(size < 134217729)
            return    MPU_128_MB;
    if(size < 268435457)
            return    MPU_256_MB;
    if(size < 536870913)
            return    MPU_512_MB;
    if(size < 1073741825)
            return    MPU_1_GB;
    if(size < 2147483649)
            return    MPU_2_GB;
    return    MPU_4_GB;
}

static uint32_t get_alignement(uint32_t size)
{
    if(size < 33)
            return    32;
    if(size < 65)
            return    64;
    if(size < 129)
            return    128;
    if(size < 257)
            return    256;
    if(size < 513)
            return    512;
    if(size < 1025)
            return    1024;
    if(size < 2049)
            return    2048;
    if(size < 4097)
            return    4096;
    if(size < 8193)
            return    8192;
    if(size < 16385)
            return    16384;
    if(size < 32769)
            return    32768;
    if(size < 65537)
            return    65536;
    if(size < 131073)
            return    131072;
    if(size < 262145)
            return    262144;
    if(size < 524289)
            return    524288;
    if(size < 1048577)
            return    1048576;
    if(size < 2097153)
            return    2097152;
    if(size < 4194305)
            return    4194304;
    if(size < 8388609)
            return    8388608;
    if(size < 16777217)
            return    16777216;
    if(size < 33554433)
            return    33554432;
    if(size < 67108865)
            return    67108864;
    if(size < 134217729)
            return    134217728;
    if(size < 268435457)
            return    268435456;
    if(size < 536870913)
            return    536870912;
    if(size < 1073741825)
            return    1073741824;
    if(size < 2147483649)
            return    2147483648;
    return    0;
}
#endif

void mpu_set_region(uint32_t region_id, uint32_t base_address,
                    uint32_t size, uint32_t type, uint32_t permissions)
{
    _mpuSetRegion_(region_id);
    _mpuSetRegionBaseAddress_(base_address);
    _mpuSetRegionTypeAndPermission_(type, permissions);
    _mpuSetRegionSizeRegister_(size);
}

void ja_space_init(void)
{
    uint32_t phys_start = POK_PARTITION_MEMORY_PHYS_START;
    uint32_t i;
    uint32_t j;
    uint32_t sub_reg_count;
    uint32_t reg_count;

    /* Init MPU */
    _mpuInit_();


    reg_count = _mpuGetNumberOfRegions_();
    /* Disable all regions */
    for(i = 0; i < reg_count; ++i)
    {
        mpu_set_region(i, 0x00000000, MPU_4_GB | mpuREGION_DISABLE | mpuSUBREGION_DISABLE,
                     MPU_NORMAL_OINC_NONSHARED, MPU_PRIV_NA_USER_NA_NOEXEC);
    }

    /* Set the kernel flash region */
    mpu_set_region(mpuREGION2, 0x00000000, MPU_4_MB | mpuREGION_ENABLE,
                     MPU_NORMAL_OIWTNOWA_SHARED, MPU_PRIV_RO_USER_NA_EXEC);
    /* Set the kernel ram region */
    mpu_set_region(mpuREGION3, 0x08000000, MPU_512_KB | mpuREGION_ENABLE,
                     MPU_NORMAL_OIWBWA_NONSHARED, MPU_PRIV_RW_USER_NA_NOEXEC);
    /* Set the kernel peripheral region */
    mpu_set_region(mpuREGION4, 0xF8000000, MPU_128_MB |
                 mpuREGION_ENABLE | mpuSUBREGION0_DISABLE |
                 mpuSUBREGION1_DISABLE | mpuSUBREGION2_DISABLE,
                 MPU_DEVICE_NONSHAREABLE, MPU_PRIV_RW_USER_NA_NOEXEC);
    /* Set the shared section */
    mpu_set_region(mpuREGION5, 0x00100000, MPU_1_MB | mpuREGION_ENABLE,
                   MPU_NORMAL_OIWBWA_NONSHARED, MPU_PRIV_RO_USER_RO_EXEC);

    /* Init all spaces */
    for(i = 0; i < ja_spaces_n; i++)
    {
        struct ja_armv7_space* space = &ja_spaces[i];

        size_t size_total = space->size_normal;

        if(space->size_heap > 0)
        {
            /* Heap should be aligned on 4 */
            size_total = ALIGN_VAL(size_total, 4) + space->size_heap;
        }

        // Store intermediate result.
        space->heap_end =  size_total;

        /* Heap should be aligned on 4 */
        size_total = ALIGN_VAL(size_total, 4) + space->size_stack;

        /* We need 32K alignement */
        size_total = ALIGN_VAL(size_total, 0x8000);
        printf("Partition size 0x%x\n\r", size_total);

        /* Get the number of MPU regions */
        reg_count = size_total / 0x8000;
        if(size_total % 0x8000 != 0)
        {
            ++reg_count;
        }
        /* Get the number of sub region we need */
        sub_reg_count = size_total / 4096;
        printf("Partition regions: %d | Sub regions:  %d\n\r", reg_count, sub_reg_count);
        if(reg_count > 11)
        {
            printf("Not enough memory to create partition space %d\n\r", i);
            while(1);
        }

        space->size_total = size_total;

        /* Such a way, next space will have alignment suitable for code and data. */
        space->phys_base = ALIGN_VAL(phys_start, 8192);

        uint32_t phys_base_end = space->phys_base + space->size_total;
        if(phys_start < phys_base_end)
            phys_start = phys_base_end;

        /** Add MPU entry context */
        for(j = 0; j < reg_count; ++j)
        {
            space->mpu_size[j] = MPU_32_KB | mpuREGION_ENABLE;
            space->mpu_base[j] = 1024 * 32 * j + space->phys_base;
            if(sub_reg_count >= 8)
            {
                sub_reg_count -= 8;
            }
            else
            {
                /* Disable sub regions that are not needed */
                space->mpu_size[j] |= (~(0xFF00 << sub_reg_count)) & 0xFF00;
            }
        }
        space->mpu_reg  = reg_count;
        space->mpu_type = MPU_NORMAL_OIWBWA_NONSHARED;
        space->mpu_perm = MPU_PRIV_RW_USER_RW_EXEC;

        printf("Create partition space: 0x%x -> 0x%x\n\r", space->phys_base, phys_base_end);
    }

    /* Enable MPU */
    _mpuEnable_();

    serial_write("MPU Initialized\r\n", 17);
}

void ja_space_layout_get(jet_space_id space_id,
    struct jet_space_layout* space_layout)
{
    if(space_id != 0 && space_id <= ja_spaces_n)
    {
        space_layout->kernel_addr = (char*)ja_spaces[space_id - 1].phys_base;
        space_layout->user_addr = space_layout->kernel_addr;
        space_layout->size = ja_spaces[space_id - 1].size_normal;
    }
    else
    {
        while(1);
    }
}

struct jet_kernel_shared_data* __kuser ja_space_shared_data(jet_space_id space_id)
{
    struct ja_armv7_space* space = &ja_spaces[space_id - 1];
    return (struct jet_kernel_shared_data* __kuser)space->phys_base;
}

void __user* ja_space_get_heap(jet_space_id space_id)
{
   struct ja_armv7_space* space = &ja_spaces[space_id - 1];

   return (void __user*)(space->phys_base + space->heap_end - space->size_heap);
}

static jet_space_id current_space_id = 0;



void ja_space_switch (jet_space_id space_id)
{
    uint32_t i;
    if(current_space_id != 0) {
        for(i = 0; i < ja_spaces[current_space_id - 1].mpu_reg; ++i)
        {
            _mpuSetRegion_(USER_START_MPU_REGION + i);
            _mpuSetRegionSizeRegister_(mpuREGION_DISABLE);
            _mpuSetRegionBaseAddress_(0);
            _mpuSetRegionTypeAndPermission_(MPU_NORMAL_OINC_NONSHARED, MPU_PRIV_NA_USER_NA_NOEXEC);
        }
    }
    if(space_id != 0) {
        for(i = 0; i < ja_spaces[space_id - 1].mpu_reg; ++i)
        {
            _mpuSetRegion_(USER_START_MPU_REGION + i);
            _mpuSetRegionBaseAddress_(ja_spaces[space_id - 1].mpu_base[i]);
            _mpuSetRegionTypeAndPermission_(ja_spaces[space_id - 1].mpu_type, ja_spaces[space_id - 1].mpu_perm);
            _mpuSetRegionSizeRegister_(ja_spaces[space_id - 1].mpu_size[i]);
        }
    }

    current_space_id = space_id;
}

void ja_ustack_init (jet_space_id space_id)
{
    if(space_id != 0)
    {
        struct ja_armv7_space* space = &ja_spaces[space_id - 1];

        space->size_stack_used = 0;
    }
}

jet_ustack_t ja_ustack_alloc (jet_space_id space_id, size_t stack_size)
{
    if(space_id != 0)
    {

        struct ja_armv7_space* space = &ja_spaces[space_id - 1];

        size_t size_stack_new = space->size_stack_used + ALIGN_VAL(stack_size, 16);

        if(size_stack_new > space->size_stack) return 0;

        jet_ustack_t result = space->phys_base + space->size_total - space->size_stack_used;

        space->size_stack_used = size_stack_new;

        return result - 32;
    }
    return NULL;
}


uintptr_t pok_virt_to_phys(uintptr_t virt)
{
    return virt;
}

uintptr_t pok_phys_to_virt(uintptr_t phys)
{
    return phys;
}
