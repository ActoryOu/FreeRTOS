
TARGET = XCORE-AI-EXPLORER

PROJ_MULTIPLE_TASKS_RUNNING = multiple_tasks_running
PROJ_SCHEDULE_HIGHEST_PRIORITY = schedule_highest_priority

BUILD_DIR = build
OUT_DIR = bin

TEST_ROOT = ./
KERNEL_ROOT = ../../../../Source
PORTABLE_ROOT = $(KERNEL_ROOT)/portable
MEMMANG_ROOT = $(PORTABLE_ROOT)/MemMang
XCORE_PORT_ROOT = $(PORTABLE_ROOT)/ThirdParty/xClang/XCOREAI
RTOS_SUPPORT_ROOT = ./lib_rtos_support
UNITY_ROOT = ../../../CMock/CMock/vendor/unity

$(info $$TEST_ROOT is [${TEST_ROOT}])
$(info $$KERNEL_ROOT is [${KERNEL_ROOT}])
$(info $$PORTABLE_ROOT is [${PORTABLE_ROOT}])
$(info $$MEMMANG_ROOT is [${MEMMANG_ROOT}])
$(info $$XCORE_PORT_ROOT is [${XCORE_PORT_ROOT}])
$(info $$RTOS_SUPPORT_ROOT is [${RTOS_SUPPORT_ROOT}])
$(info $$UNITY_ROOT is [${UNITY_ROOT}])

INLUCDE_DIRS = $(UNITY_ROOT)/src $(TEST_ROOT) $(UNITY_SOURCES/src)\
               $(KERNEL_ROOT)/include $(XCORE_PORT_ROOT) \
               $(RTOS_SUPPORT_ROOT)/api $(RTOS_SUPPORT_ROOT)/src

TEST_BOARD_SOURCES = $(TEST_ROOT)/main.xc \
                     $(TEST_ROOT)/main.c

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

COMMON_SOURCES = $(TEST_BOARD_SOURCES) $(KERNEL_SOURCES) $(UNITY_SOURCES) $(RTOS_SUPPORT_SOURCES)

ROOT_DIRS_COMMON = $(TEST_ROOT) $(KERNEL_ROOT) $(MEMMANG_ROOT) \
                   $(XCORE_PORT_ROOT) $(RTOS_SUPPORT_ROOT)/src \
                   $(UNITY_ROOT)/src

FLAGS_COMMON = -Wall -O2 -g -report -fxscope \
               $(TEST_ROOT)/$(TARGET).xn \
               $(TEST_ROOT)/config.xscope

# Test case "Multiple Tasks Running".
TEST_ROOT_MULTIPLE_TASKS_RUNNING = $(TEST_ROOT)/../../tests/smp/multiple_tasks_running
TEST_BOARD_MULTIPLE_TASKS_RUNNING = $(TEST_ROOT)/tests/smp/multiple_tasks_running

INCLUDE_DIRS_MULTIPLE_TASKS_RUNNING = $(TEST_ROOT_MULTIPLE_TASKS_RUNNING) $(INLUCDE_DIRS)

MULTIPLE_TASKS_RUNNING_SOURCES = $(TEST_ROOT)/multiple_tasks_running.c \
                                 $(TEST_BOARD_MULTIPLE_TASKS_RUNNING)/multiple_task_running_test_runner.c

ROOT_DIRS_MULTIPLE_TASKS_RUNNING = $(TEST_ROOT_MULTIPLE_TASKS_RUNNING) $(TEST_BOARD_MULTIPLE_TASKS_RUNNING)
SOURCES_MULTIPLE_TASKS_RUNNING = $(MULTIPLE_TASKS_RUNNING_SOURCES) $(COMMON_SOURCES)

OBJS_MULTIPLE_TASKS_RUNNING = $(addprefix $(BUILD_DIR)/$(PROJ_MULTIPLE_TASKS_RUNNING)/,$(notdir $(addsuffix .o,$(SOURCES_MULTIPLE_TASKS_RUNNING))))

FLAGS_MULTIPLE_TASKS_RUNNING = $(addprefix -I,$(INCLUDE_DIRS_MULTIPLE_TASKS_RUNNING))

# Test case "Schedule Highest Priority".
TEST_ROOT_SCHEDULE_HIGHEST_PRIORITY = $(TEST_ROOT)/../../tests/smp/schedule_highest_priority
TEST_BOARD_SCHEDULE_HIGHEST_PRIORITY = $(TEST_ROOT)/tests/smp/schedule_highest_priority

INCLUDE_DIRS_SCHEDULE_HIGHEST_PRIORITY = $(TEST_ROOT_SCHEDULE_HIGHEST_PRIORITY) $(INLUCDE_DIRS)

SCHEDULE_HIGHEST_PRIORITY_SOURCES = $(TEST_ROOT)/schedule_highest_priority.c \
                                    $(TEST_BOARD_SCHEDULE_HIGHEST_PRIORITY)/schedule_highest_priority_test_runner.c

ROOT_DIRS_SCHEDULE_HIGHEST_PRIORITY = $(TEST_ROOT_SCHEDULE_HIGHEST_PRIORITY) $(TEST_BOARD_SCHEDULE_HIGHEST_PRIORITY)
SOURCES_SCHEDULE_HIGHEST_PRIORITY = $(SCHEDULE_HIGHEST_PRIORITY_SOURCES) $(COMMON_SOURCES)

OBJS_SCHEDULE_HIGHEST_PRIORITY = $(addprefix $(BUILD_DIR)/$(PROJ_SCHEDULE_HIGHEST_PRIORITY)/,$(notdir $(addsuffix .o,$(SOURCES_SCHEDULE_HIGHEST_PRIORITY))))

FLAGS_SCHEDULE_HIGHEST_PRIORITY = $(addprefix -I,$(INCLUDE_DIRS_SCHEDULE_HIGHEST_PRIORITY))

ROOT_DIRS = $(ROOT_DIRS_COMMON) $(ROOT_DIRS_MULTIPLE_TASKS_RUNNING) $(ROOT_DIRS_SCHEDULE_HIGHEST_PRIORITY)

vpath %.c $(ROOT_DIRS)
vpath %.xc $(ROOT_DIRS)
vpath %.S $(ROOT_DIRS)

XCLANG = xcc

.PHONY: all clean run_$(PROJ_MULTIPLE_TASKS_RUNNING) run_$(PROJ_SCHEDULE_HIGHEST_PRIORITY)

all: $(OUT_DIR)/$(PROJ_MULTIPLE_TASKS_RUNNING).xe $(OUT_DIR)/$(PROJ_SCHEDULE_HIGHEST_PRIORITY).xe
	
clean:
	$(RM) -r $(OUT_DIR) $(BUILD_DIR)

$(BUILD_DIR)/$(PROJ_MULTIPLE_TASKS_RUNNING)/%.o: %
	@"mkdir" -p $(@D)
	@echo "$(XCLANG) -c -MT"$@" -MMD -MP -MF"$(patsubst %.o,%.d,$@)" -MT"$(patsubst %.o,%.d,$@)" -o $@ $< $(FLAGS_COMMON)  $(FLAGS_MULTIPLE_TASKS_RUNNING)"
	$(XCLANG) -c -MT"$@" -MMD -MP -MF"$(patsubst %.o,%.d,$@)" -MT"$(patsubst %.o,%.d,$@)" -o $@ $< $(FLAGS_COMMON)  $(FLAGS_MULTIPLE_TASKS_RUNNING)

$(OUT_DIR)/$(PROJ_MULTIPLE_TASKS_RUNNING).xe: $(OBJS_MULTIPLE_TASKS_RUNNING)
	@"mkdir" -p $(@D)
	$(XCLANG) -o $@ $^ $(FLAGS_COMMON) $(FLAGS_MULTIPLE_TASKS_RUNNING)

run_$(PROJ_MULTIPLE_TASKS_RUNNING): $(OUT_DIR)/$(PROJ_MULTIPLE_TASKS_RUNNING).xe
	xrun --xscope $(OUT_DIR)/$(PROJ_MULTIPLE_TASKS_RUNNING).xe

$(BUILD_DIR)/$(PROJ_SCHEDULE_HIGHEST_PRIORITY)/%.o: %
	@"mkdir" -p $(@D)
	@echo "$(XCLANG) -c -MT"$@" -MMD -MP -MF"$(patsubst %.o,%.d,$@)" -MT"$(patsubst %.o,%.d,$@)" -o $@ $< $(FLAGS_COMMON)  $(FLAGS_SCHEDULE_HIGHEST_PRIORITY)"
	$(XCLANG) -c -MT"$@" -MMD -MP -MF"$(patsubst %.o,%.d,$@)" -MT"$(patsubst %.o,%.d,$@)" -o $@ $< $(FLAGS_COMMON)  $(FLAGS_SCHEDULE_HIGHEST_PRIORITY)

$(OUT_DIR)/$(PROJ_SCHEDULE_HIGHEST_PRIORITY).xe: $(OBJS_SCHEDULE_HIGHEST_PRIORITY)
	@"mkdir" -p $(@D)
	$(XCLANG) -o $@ $^ $(FLAGS_COMMON) $(FLAGS_SCHEDULE_HIGHEST_PRIORITY)

run_$(PROJ_SCHEDULE_HIGHEST_PRIORITY): $(OUT_DIR)/$(PROJ_SCHEDULE_HIGHEST_PRIORITY).xe
	xrun --xscope $(OUT_DIR)/$(PROJ_SCHEDULE_HIGHEST_PRIORITY).xe

