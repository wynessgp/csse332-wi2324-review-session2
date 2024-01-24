#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

/* If you are going to modify these,
please ensure that NUM_STUDENTS is some multiple of
NUM_GRADERS. I've set it up this way by default; any modifications
are on you. 
*/
#define NUM_GRADERS 5
#define NUM_STUDENTS 30
#define WEBSITE_MAX 3

// DO NOT TOUCH THE CODE SETTING THIS UP!
int assignments_per_grader = 0;

// you WILL need this to determine what section the grader grades for,
// and what section a student belongs to!
struct thread_args {
    int section_num;
    int thread_num;
};

void* student(void* args) {
    // TODO: you WILL need to use the section_num at some point!
    struct thread_args* student_info = (struct thread_args*) args;

    printf("Student (%d) has arrived!\n", student_info->thread_num);

    // CRITICAL SECTION
    printf("Student (%d) is making a submission!\n", student_info->thread_num);
    // make it take a little bit so it takes some time to have all of the students submit
    sleep(1);
    printf("Student (%d) made a submission!\n", student_info->thread_num);

    return 0;
}

void* grader(void* args) {
    // TODO: you WILL need to use the section_num at some point!
    struct thread_args* grader_info = (struct thread_args*) args;

    printf("Grader (%d) for section %d has arrived!\n", grader_info->thread_num, 
                                                        grader_info->section_num);

    // CRITICAL SECTION
    printf("Grader (%d) is grading an assignment!\n", grader_info->thread_num);
    // sleep for a random amount of time between 1-4 seconds to ensure your solution is correct
    sleep((rand() % 4) + 1);
    printf("Grader (%d) is done grading their current submission!\n", grader_info->thread_num);


    printf("Grader (%d) is done with their assignments!\n", grader_info->thread_num);
    return 0;
}

int main(int argc, char** argv) {
    pthread_t threads[NUM_STUDENTS + NUM_GRADERS];
    struct thread_args t_args[NUM_STUDENTS + NUM_GRADERS];
    srand(time(NULL));

    printf("Grading program started!\n");
    // should be as simple as this.
    assignments_per_grader = NUM_STUDENTS / NUM_GRADERS;
    // if you modify how many students & graders you have, you are 
    // responsible for modifying the above line as well!

    // create all of the students
    for (int i = 0; i < NUM_STUDENTS; i++) {
        struct thread_args cur_args;
        cur_args.section_num = (i % NUM_GRADERS) + 1;
        cur_args.thread_num = i + 1;
        t_args[i] = cur_args;
        pthread_create(&threads[i], 0, student, &t_args[i]);
    }

    // create all of the graders. Some duplicate code here is OK
    for (int i = NUM_STUDENTS; i < NUM_STUDENTS + NUM_GRADERS; i++) {
        struct thread_args cur_args;
        cur_args.section_num = (i % NUM_GRADERS) + 1;
        cur_args.thread_num = i + 1;
        t_args[i] = cur_args;
        pthread_create(&threads[i], 0, grader, &t_args[i]);
    }

    for (int i = 0; i < NUM_STUDENTS + NUM_GRADERS; i++) {
        pthread_join(threads[i], 0);
    }


    printf("Everything finished!\n");
}