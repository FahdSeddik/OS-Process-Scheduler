CC = gcc

# Group together likely to be used together files
DEP = ./Dependencies/minheap.c ./Dependencies/queue.c ./Dependencies/pcb.c ./Dependencies/list.c
ALGO = ./Algorithms/hpf.c ./Algorithms/rr.c ./Algorithms/srtn.c ./Algorithms/sync.c
PMGR = ./ProcessManagement/message_queue.c ./MemoryManagement/buddy.c ./ProcessManagement/process_manager.c ./ProcessManagement/semaphore.c
LOGG = ./Logging/logger.c -lm

# Any used files that are needed in each of those should be here
scheduler := $(DEP) $(ALGO) $(PMGR) $(LOGG) # This means that when compiling scheduler it will add those c files as pre-compile
generator := $(PMGR) $(LOGG) ./Dependencies/pcb.c

# Repeated variables
build_dir := build

.PHONY: all clean

all: clean build

# Here is where we use the $(scheduler) variable while compiling
build:
	mkdir -p $(build_dir)
	$(CC) process_generator.c $(generator) clk_utils.c -o $(build_dir)/process_generator.out
	$(CC) clk.c clk_utils.c ./ProcessManagement/semaphore.c ./Dependencies/pcb.c -o $(build_dir)/clk.out
	$(CC) scheduler.c $(scheduler) clk_utils.c -o $(build_dir)/scheduler.out
	$(CC) process.c clk_utils.c ./ProcessManagement/semaphore.c -o $(build_dir)/process.out
	$(CC) test_generator.c -o $(build_dir)/test_generator.out

clean:
	rm -rf *.out $(build_dir) processes.txt

run:
	./$(build_dir)/process_generator.out
