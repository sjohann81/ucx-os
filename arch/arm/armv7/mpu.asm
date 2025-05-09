;-------------------------------------------------------------------------------
; HL_sys_mpu.asm
;
; Copyright (C) 2009-2018 Texas Instruments Incorporated - www.ti.com
;
;
;  Redistribution and use in source and binary forms, with or without
;  modification, are permitted provided that the following conditions
;  are met:
;
;    Redistributions of source code must retain the above copyright
;    notice, this list of conditions and the following disclaimer.
;
;    Redistributions in binary form must reproduce the above copyright
;    notice, this list of conditions and the following disclaimer in the
;    documentation and/or other materials provided with the
;    distribution.
;
;    Neither the name of Texas Instruments Incorporated nor the names of
;    its contributors may be used to endorse or promote products derived
;    from this software without specific prior written permission.
;
;  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
;  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
;  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
;  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
;  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
;  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
;  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
;  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
;  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
;  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
;  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
;
;
;

;-------------------------------------------------------------------------------
; Initalize Mpu
; SourceId : MPU_SourceId_001
; DesignId : MPU_DesignId_001
; Requirements : HL_CONQ_MPU_SR3

    .def     _mpuInit_
    .asmfunc

_mpuInit_
        ; Disable mpu
        mrc   p15, #0, r0, c1, c0, #0
        bic   r0,  r0, #1
        dsb
        mcr   p15, #0, r0, c1, c0, #0
        isb
        ; Disable background region
        mrc   p15, #0, r0,      c1, c0, #0
        bic   r0,  r0, #0x20000
        mcr   p15, #0, r0,      c1, c0, #0

        bx lr

        .endasmfunc

;-------------------------------------------------------------------------------
; Enable Mpu
; SourceId : MPU_SourceId_002
; DesignId : MPU_DesignId_002
; Requirements : HL_CONQ_MPU_SR4

    .def     _mpuEnable_
    .asmfunc

_mpuEnable_

        mrc   p15, #0, r0, c1, c0, #0
        orr   r0,  r0, #1
        dsb
        mcr   p15, #0, r0, c1, c0, #0
        isb
        bx    lr

    .endasmfunc

;-------------------------------------------------------------------------------
; Returns number of implemented Mpu regions
; SourceId : MPU_SourceId_006
; DesignId : MPU_DesignId_006
; Requirements : HL_CONQ_MPU_SR14

    .def     _mpuGetNumberOfRegions_
    .asmfunc

_mpuGetNumberOfRegions_

        mrc   p15, #0, r0,      c0, c0, #4
        uxtb  r0,  r0, ROR #8
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Set mpu region number
; SourceId : MPU_SourceId_008
; DesignId : MPU_DesignId_008
; Requirements : HL_CONQ_MPU_SR8

    .def     _mpuSetRegion_
    .asmfunc

_mpuSetRegion_

        mcr   p15, #0, r0, c6, c2, #0
        bx    lr

    .endasmfunc

;-------------------------------------------------------------------------------
; Set base address
; SourceId : MPU_SourceId_010
; DesignId : MPU_DesignId_010
; Requirements : HL_CONQ_MPU_SR9

    .def     _mpuSetRegionBaseAddress_
    .asmfunc

_mpuSetRegionBaseAddress_

        mcr   p15, #0, r0, c6, c1, #0
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Set type and permission
; SourceId : MPU_SourceId_012
; DesignId : MPU_DesignId_012
; Requirements : HL_CONQ_MPU_SR11, HL_CONQ_MPU_SR12

    .def     _mpuSetRegionTypeAndPermission_
    .asmfunc

_mpuSetRegionTypeAndPermission_

        orr   r0,  r0, r1
        mcr   p15, #0, r0, c6, c1, #4
        bx    lr

    .endasmfunc

;-------------------------------------------------------------------------------
; Set region size register value
; SourceId : MPU_SourceId_015
; DesignId : MPU_DesignId_015
; Requirements : HL_CONQ_MPU_SR10, HL_CONQ_MPU_SR13

    .def     _mpuSetRegionSizeRegister_
    .asmfunc

_mpuSetRegionSizeRegister_

        mcr   p15, #0, r0, c6, c1, #2
        bx    lr

    .endasmfunc


