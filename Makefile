all: grading.bin classroom.bin

# no dependencies here

grading.bin: grading.c
	gcc grading.c -o grading.bin

classroom.bin: classroom.c
	gcc classroom.c -o classroom.bin

# no need to run this, but it'll clear out any .bin files
clean:
	rm -rf *.bin