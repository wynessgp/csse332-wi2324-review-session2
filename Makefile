all: grading.bin classroom.bin

# no dependencies here

grading.bin: grading.c
	gcc -pthread -o grading.bin grading.c

classroom.bin: classroom.c
	gcc -pthread -o classroom.bin classroom.c

# no need to run this, but it'll clear out any .bin files
clean:
	rm -rf *.bin
