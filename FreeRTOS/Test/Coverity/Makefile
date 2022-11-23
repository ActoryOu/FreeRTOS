# indent with spaces
.RECIPEPREFIX := $(.RECIPEPREFIX) $(.RECIPEPREFIX)

# Change to match installed location
export CC ?= /usr/local/bin/gcc
export LD ?= /usr/local/bin/ld

COVERITY_ROOT_DIR      :=  $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
KERNEL_ROOT_DIR        :=  $(COVERITY_ROOT_DIR)/../../Source
OUTPUT_DIR             :=  $(COVERITY_ROOT_DIR)/build
INCLUDE_DIR            :=  -I$(COVERITY_ROOT_DIR)/SingleCoreConfig -I$(KERNEL_ROOT_DIR)/include

PROJ_SRCS := $(wildcard *.c) $(wildcard $(KERNEL_ROOT_DIR)/*.c)
PROJ_OBJS := $(notdir $(patsubst %.c,%.o,$(PROJ_SRCS) ))
OUT_OBJS  := $(addprefix $(OUTPUT_DIR)/, $(PROJ_OBJS))
OUT_BIN   := $(addprefix $(OUTPUT_DIR)/, coverity.bin)

all: coverity

coverity: $(OUT_BIN)

$(OUT_BIN): directories
    $(CC) -o $@ $(INCLUDE_DIR) $(PROJ_SRCS)

    # cov-build --emit-complementary-info --dir cov-out make
    # cd cov-out/
    # cov-analyze --dir . --coding-standard-config $(COVERITY_ROOT_DIR)/coverity_misra.config
    # cov-format-errors --dir . --html-output coverity_output


directories:
    -mkdir -p $(OUTPUT_DIR)

clean :
    rm -rf $(OUTPUT_DIR)
    rm -rf $(COVERITY_ROOT_DIR)/cov-out
