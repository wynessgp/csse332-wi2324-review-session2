#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

// professor related logic
#define GUARANTEED_CS_PROFS 3
#define GUARANTEED_ME_PROFS 3
#define TOTAL_PROFS 13

// student cap for the classroom
// these numbers are kind of arbitrary but should work
#define STUDENT_CAPACITY 6
#define NUM_STUDENTS 27

#define CS_INDEX 0
#define ME_INDEX 1
// Note that you can always determine a professor's type by doing: 
// tid % 2.
char* professor_types[2] = {"CS", "ME"};
// DO NOT CHANGE WHAT THESE ARE INITIALIZED TO.
int professors_waiting[2] = {1, 1};
// I've set them to 1 so you can at least guarantee the rest of the ball starts rolling.
// I encourage you to use this array as a state of the world variable.
int num_students_left = 0;
int num_professors_left = 0;

void* student(void* tid) {
    int student_num = *(int*) tid;
    printf("Student (%d) has arrived!\n", student_num);

    // CRITICAL SECTION
    printf("Student (%d) is actively learning!\n", student_num);
    sleep(2);
    printf("Student (%d) is done learning!\n", student_num);

    // You'll need to print this in the case a student leaves:
    // printf("Student (%d) left unhappy!\n", student_num);

    return 0;
}

void* professor(void* tid) {
    int prof_num = *(int*) tid;
    printf("Professor (%d) of type %s has arrived!\n", prof_num,
                                  professor_types[prof_num % 2]);

    // CRITICAL SECTION
    printf("Professor (%d) of type %s is entering the classroom...\n", prof_num, 
                                                  professor_types[prof_num % 2]);
    sleep(2);
    printf("Professor (%d) of type %s is leaving the classroom...\n", prof_num,
                                                  professor_types[prof_num % 2]);

    // You'll need to print this in the case a professor leaves:
    // printf("Professor (%d) left because they couldn't find an accomplice!\n", prof_num);

    return 0;
}

int main(int argc, char** argv) {
    pthread_t threads[TOTAL_PROFS + NUM_STUDENTS];
    int tids[TOTAL_PROFS + NUM_STUDENTS];
    srand(time(NULL));

    // create the professors

    // random professor generation
    // I'll do this by randomly shuffling an array of 40 digits, and we can
    // guarantee that anybody else doesn't use said digits for tid.

    int temp_arr[40];
    for (int i = 0; i < 40; i++) {
        // add 7 to each digit so it doesn't overlap
        temp_arr[i] = i + 7;
    }
    // shuffle the array
    for (int i = 0; i < 40; i++) {
        int temp = temp_arr[i];
        int random_index = rand() % 40;
        temp_arr[i] = temp_arr[random_index];
        temp_arr[random_index] = temp;
    }

    for (int i = 0; i < GUARANTEED_CS_PROFS + GUARANTEED_ME_PROFS; i++) {
        // nice behavior here should generate at least 3 of each.
        tids[i] = i + 1;
        pthread_create(&threads[i], 0, professor, &tids[i]);
    }

    // create the students. note that we have to start 47 minimum for tid
    // as we used up to 40 in random.
    for (int i = TOTAL_PROFS; i < NUM_STUDENTS + TOTAL_PROFS; i++) {
        tids[i] = i + (47 - TOTAL_PROFS);
        pthread_create(&threads[i], 0, student, &tids[i]);
    }

    // create the random prof threads
    for (int i = GUARANTEED_CS_PROFS + GUARANTEED_ME_PROFS; i < TOTAL_PROFS; i++) {
        // pull from the first couple of spots in the randomized array.
        tids[i] = temp_arr[i - (GUARANTEED_CS_PROFS + GUARANTEED_ME_PROFS)];
        pthread_create(&threads[i], 0, professor, &tids[i]);
    }

    for (int i = 0; i < NUM_STUDENTS + TOTAL_PROFS; i++) {
        pthread_join(threads[i], 0);
    }

    printf("Everything finished...\n");

}





