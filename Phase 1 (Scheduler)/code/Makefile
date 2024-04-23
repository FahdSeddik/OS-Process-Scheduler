CC = gcc
DEP = ./Dependencies/minheap.c ./Dependencies/queue.c ./Dependencies/pcb.c ./Dependencies/process_table.c
ALGO = ./Algorithms/hpf.c ./Algorithms/rr.c ./Algorithms/srtn.c
PMGR = ./ProcessManagement/message_queue.c ./ProcessManagement/process_manager.c
LOGG = ./Logging/logger.c

build_dir := build
out_dir := $(build_dir)/out

.PHONY: all clean

all: clean build

build:
	mkdir -p $(out_dir)
	$(CC) process_generator.c -o $(out_dir)/process_generator.out
	$(CC) clk.c -o $(out_dir)/clk.out
	$(CC) scheduler.c $(DEP) $(ALGO) $(PMGR) $(LOGG) -o $(out_dir)/scheduler.out
	$(CC) process.c -o $(out_dir)/process.out
	$(CC) test_generator.c -o $(out_dir)/test_generator.out

clean:
	rm -rf *.out $(build_dir) processes.txt

run:
	./$(out_dir)/process_generator.out
