CC = gcc

# Group together likely to be used together files
DEP = ./Dependencies/minheap.c ./Dependencies/queue.c ./Dependencies/pcb.c ./Dependencies/process_table.c
ALGO = ./Algorithms/hpf.c ./Algorithms/rr.c ./Algorithms/srtn.c
PMGR = ./ProcessManagement/message_queue.c ./ProcessManagement/process_manager.c ./ProcessManagement/semaphore.c
LOGG = ./Logging/logger.c

# Any used files that are needed in each of those should be here
scheduler := $(DEP) $(ALGO) $(PMGR) $(LOGG) # This means that when compiling scheduler it will add those c files as pre-compile
generator := $(PMGR)

# Repeated variables
build_dir := build

.PHONY: all clean

all: clean build

# Here is where we use the $(scheduler) variable while compiling
build:
	mkdir -p $(build_dir)
	$(CC) process_generator.c $(generator) -o $(build_dir)/process_generator.out
	$(CC) clk.c -o $(build_dir)/clk.out
	$(CC) scheduler.c $(scheduler) -o $(build_dir)/scheduler.out 
	$(CC) process.c -o $(build_dir)/process.out
	$(CC) test_generator.c -o $(build_dir)/test_generator.out

clean:
	rm -rf *.out $(build_dir) processes.txt

run:
	./$(build_dir)/process_generator.out
