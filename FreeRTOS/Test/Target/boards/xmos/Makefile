
TARGET = XCORE-AI-EXPLORER

APP_NAME = SMP-on-target-test

BUILD_DIR = build
OUT_DIR = bin

TEST_ROOT = ./
KERNEL_ROOT = ../../../../Source
PORTABLE_ROOT = $(KERNEL_ROOT)/portable
MEMMANG_ROOT = $(PORTABLE_ROOT)/MemMang
XCORE_PORT_ROOT = $(PORTABLE_ROOT)/ThirdParty/xClang/XCOREAI
RTOS_SUPPORT_ROOT = ./lib_rtos_support
UNITY_ROOT = ../../../CMock/CMock/vendor/unity
TEST_CASE_ROOT = $(TEST_ROOT)/../../tests/smp/multiple_tasks_running
TEST_CASE_BOARD = $(TEST_ROOT)/tests/smp/multiple_tasks_running

$(info $$TEST_ROOT is [${TEST_ROOT}])
$(info $$KERNEL_ROOT is [${KERNEL_ROOT}])
$(info $$PORTABLE_ROOT is [${PORTABLE_ROOT}])
$(info $$MEMMANG_ROOT is [${MEMMANG_ROOT}])
$(info $$XCORE_PORT_ROOT is [${XCORE_PORT_ROOT}])
$(info $$RTOS_SUPPORT_ROOT is [${RTOS_SUPPORT_ROOT}])
$(info $$UNITY_ROOT is [${UNITY_ROOT}])
$(info $$TEST_CASE_ROOT is [${TEST_CASE_ROOT}])
$(info $$TEST_CASE_BOARD is [${TEST_CASE_BOARD}])

INCLUDE_DIRS = $(UNITY_ROOT)/src $(TEST_CASE_ROOT) $(TEST_ROOT) $(UNITY_SOURCES/src)\
               $(KERNEL_ROOT)/include $(XCORE_PORT_ROOT) \
               $(RTOS_SUPPORT_ROOT)/api $(RTOS_SUPPORT_ROOT)/src

TEST_BOARD_SOURCES = $(TEST_ROOT)/main.xc \
                     $(TEST_ROOT)/main.c \
                     $(TEST_CASE_BOARD)/multiple_task_running_test_runner.c

TEST_CASE_SOURCES = $(TEST_ROOT)/multiple_tasks_running.c

UNITY_SOURCES = $(UNITY_ROOT)/src/unity.c
$(info $$UNITY_SOURCES is [${UNITY_SOURCES}])

RTOS_SUPPORT_SOURCES = $(RTOS_SUPPORT_ROOT)/src/rtos_cores.c \
                       $(RTOS_SUPPORT_ROOT)/src/rtos_interrupt.c \
                       $(RTOS_SUPPORT_ROOT)/src/rtos_irq.c \
                       $(RTOS_SUPPORT_ROOT)/src/rtos_locks.c \
                       $(RTOS_SUPPORT_ROOT)/src/rtos_printf.c \
                       $(RTOS_SUPPORT_ROOT)/src/rtos_time.c

KERNEL_SOURCES = $(KERNEL_ROOT)/event_groups.c \
                 $(KERNEL_ROOT)/list.c \
                 $(KERNEL_ROOT)/queue.c \
                 $(KERNEL_ROOT)/stream_buffer.c \
                 $(KERNEL_ROOT)/tasks.c \
                 $(KERNEL_ROOT)/timers.c \
                 $(MEMMANG_ROOT)/heap_4.c \
                 $(XCORE_PORT_ROOT)/port.c \
                 $(XCORE_PORT_ROOT)/port.xc \
                 $(XCORE_PORT_ROOT)/portasm.S

SOURCES = $(TEST_BOARD_SOURCES) $(TEST_CASE_SOURCES) $(KERNEL_SOURCES) $(UNITY_SOURCES) $(RTOS_SUPPORT_SOURCES)

OBJS = $(addprefix $(BUILD_DIR)/,$(notdir $(addsuffix .o,$(SOURCES))))
$(info $$OBJS is [${OBJS}])

ROOT_DIRS = $(TEST_ROOT) $(KERNEL_ROOT) $(MEMMANG_ROOT) \
            $(XCORE_PORT_ROOT) $(RTOS_SUPPORT_ROOT)/src \
            $(TEST_CASE_ROOT) $(TEST_CASE_BOARD) $(UNITY_ROOT)/src

vpath %.c $(ROOT_DIRS)
vpath %.xc $(ROOT_DIRS)
vpath %.S $(ROOT_DIRS)

FLAGS = -Wall -O2 -g -report -fxscope \
        $(TEST_ROOT)/$(TARGET).xn \
        $(TEST_ROOT)/config.xscope \
        $(addprefix -I,$(INCLUDE_DIRS))

XCLANG = xcc

.PHONY: all clean run

all: $(OUT_DIR)/$(APP_NAME).xe

# Include the header file dependencies for all the sources
-include $(patsubst %.o,%.d,$(OBJS))

$(BUILD_DIR)/%.o: %
	@"mkdir" -p $(@D)
	@echo "$(XCLANG) -c -MT"$@" -MMD -MP -MF"$(patsubst %.o,%.d,$@)" -MT"$(patsubst %.o,%.d,$@)" -o $@ $< $(FLAGS)"
	$(XCLANG) -c -MT"$@" -MMD -MP -MF"$(patsubst %.o,%.d,$@)" -MT"$(patsubst %.o,%.d,$@)" -o $@ $< $(FLAGS)

$(OUT_DIR)/$(APP_NAME).xe: $(OBJS)
	@"mkdir" -p $(@D)
	$(XCLANG) -o $@ $^ $(FLAGS)
	
clean:
	$(RM) -r $(OUT_DIR) $(BUILD_DIR)

run: $(OUT_DIR)/$(APP_NAME).xe
	xrun --xscope $(OUT_DIR)/$(APP_NAME).xe

