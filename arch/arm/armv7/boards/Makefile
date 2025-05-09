TI_ENV = C:/ti/ccs2011/ccs/tools/compiler/ti-cgt-arm_18.12.2.LTS

SRC_DIR    = armv7
BUILD_DIR  = build
INC_DIR    = armv7/includes
BIN_DIR    = ./

SRC_DIRS = $(SRC_DIR)
OBJ_DIRS = $(BUILD_DIR)

C_SRCS = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
A_SRCS = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.asm))
C_OBJS = $(C_SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.obj)
A_OBJS = $(A_SRCS:$(SRC_DIR)/%.asm=$(BUILD_DIR)/%.obj)

LINKER_FILE = ../config/linker.cmd

AS = $(TI_ENV)/bin/armcl.exe
LD = $(TI_ENV)/bin/armlnk.exe
CC = $(TI_ENV)/bin/armcl.exe


CFLAGS = -mv7R5 --code_state=32 --float_support=VFPv3D16 -Ooff -c      \
         --opt_for_speed=0 -g --c99 --diag_warning=225 --diag_wrap=off \
         --display_error_number --enum_type=packed --abi=eabi          \
         --include_path="armv7/include"                                \
         --include_path="../kernel/arch/armv7/include"                 \
         --include_path="../kernel/include"
         
ASFLAGS = $(CFLAGS)
        
LDFLAGS = -mv7R5 --code_state=32 --float_support=VFPv3D16 -Ooff         \
          --opt_for_speed=0 -g --c99 --diag_warning=225 --diag_wrap=off \
           --display_error_number --enum_type=packed --abi=eabi -z      \
           -m"JetOS.map"        \
           --reread_libs --diag_wrap=off --display_error_number         \
           --warn_sections --xml_link_info="JetOS_linkInfo.xml"         \
           --rom_model --be32 -o "JetOS.out" $(LINKER_FILE)             
           
.PHONY: all
all: init boards.a

init:
	mkdir -p $(OBJ_DIRS)
	mkdir -p $(BIN_DIR)

# kernel generation
boards.a: compile_asm compile_cc
	@echo -e  "\e[32mCreating Archive file \e[22m\e[39m-> \e[1m\e[94m$@\e[22m\e[39m"
	$(LD) -r $(C_OBJS) $(A_OBJS) -o $(BIN_DIR)/$@
	@echo -e "\e[1m\e[94m============ Archive finished ============\e[22m\e[39m"
	@echo -e

compile_asm: $(A_OBJS)
	@echo -e "\e[1m\e[94m============ Compiled ASM sources ============\e[22m\e[39m"
	@echo -e

compile_cc: $(C_OBJS)
	@echo -e "\e[1m\e[94m============ Compiled C sources ============\e[22m\e[39m"
	@echo -e

$(BUILD_DIR)/%.obj: $(SRC_DIR)/%.c
	@echo -e  "\e[32m$< \e[22m\e[39m-> \e[1m\e[94m$@\e[22m\e[39m"
	$(CC) $(CFLAGS) $< --obj_directory="$(BUILD_DIR)"

$(BUILD_DIR)/%.obj: $(SRC_DIR)/%.asm
	@echo -e  "\e[32m$< \e[22m\e[39m-> \e[1m\e[94m$@\e[22m\e[39m"
	$(AS) $(ASFLAGS) $< --obj_directory="$(BUILD_DIR)"

clean:
	$(RM) -rf $(BIN_DIR)/boards.a $(BUILD_DIR)

	@echo -e "\e[1m\e[94m============ Cleaning Object files and Binaries ============\e[22m\e[39m"
	@echo -e


# Check header files modifications
-include $(C_OBJS:.obj=.d)
-include $(A_OBJS:.obj=.d)