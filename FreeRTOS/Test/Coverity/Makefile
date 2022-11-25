# indent with spaces
.RECIPEPREFIX := $(.RECIPEPREFIX) $(.RECIPEPREFIX)

# Change to match installed location
export CC ?= /usr/local/bin/gcc
export LD ?= /usr/local/bin/ld

COVERITY_ROOT_DIR      :=  $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
KERNEL_ROOT_DIR        :=  $(COVERITY_ROOT_DIR)/../../Source
OUTPUT_DIR             :=  $(COVERITY_ROOT_DIR)/build
INCLUDE_DIR            :=  -I$(KERNEL_ROOT_DIR)/include

PROJ_SRCS := $(wildcard *.c) $(wildcard $(KERNEL_ROOT_DIR)/*.c)
PROJ_OBJS := $(notdir $(patsubst %.c,%.o,$(PROJ_SRCS) ))
OUT_OBJS  := $(addprefix $(OUTPUT_DIR)/, $(PROJ_OBJS))

# Setting configuration combinations
INCLDUE_SINGLE_CORE    :=  -I$(COVERITY_ROOT_DIR)/SingleCoreConfig
INCLDUE_MULTI_CORE1    :=  -I$(COVERITY_ROOT_DIR)/MultiCore1Config
INCLDUE_MULTI_CORE2    :=  -I$(COVERITY_ROOT_DIR)/MultiCore2Config
OUT_BIN_SINGLE_CORE    := $(addprefix $(OUTPUT_DIR)/, coveritySingleCore.bin)
OUT_BIN_MULTI_CORE1    := $(addprefix $(OUTPUT_DIR)/, coverityMultiCore1.bin)
OUT_BIN_MULTI_CORE2    := $(addprefix $(OUTPUT_DIR)/, coverityMultiCore2.bin)

singlecore: $(OUT_BIN_SINGLE_CORE)

multicore1: $(OUT_BIN_MULTI_CORE1)

multicore2: $(OUT_BIN_MULTI_CORE2)

$(OUT_BIN_SINGLE_CORE): directories
    $(CC) -o $@ $(INCLDUE_SINGLE_CORE) $(INCLUDE_DIR) $(PROJ_SRCS)

$(OUT_BIN_MULTI_CORE1): directories
    $(CC) -o $@ $(INCLDUE_MULTI_CORE1) $(INCLUDE_DIR) $(PROJ_SRCS)

$(OUT_BIN_MULTI_CORE2): directories
    $(CC) -o $@ $(INCLDUE_MULTI_CORE2) $(INCLUDE_DIR) $(PROJ_SRCS)

directories:
    -mkdir -p $(OUTPUT_DIR)

clean :
    rm -rf $(OUTPUT_DIR)
