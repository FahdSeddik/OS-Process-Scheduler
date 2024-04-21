build:
	mkdir build
	gcc process_generator.c -o ./build/process_generator.out
	gcc clk.c -o ./build/clk.out
	gcc scheduler.c ./Dependencies/minheap.c ./Dependencies/queue.c ./Dependencies/pcb.c ./Dependencies/process_table.c -o ./build/scheduler.out
	gcc process.c -o ./build/process.out
	gcc test_generator.c -o ./build/test_generator.out

clean:
	rm -rf *.out ./build/ processes.txt

all: clean build

run:
	./build/process_generator.out
