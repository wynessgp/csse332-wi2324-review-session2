# CSSE332 Practice Exam 2, Winter 23-24
## How to download and edit the files here:
I'd suggest you start by creating a new directory on your WSL. You can do this by running the following command:
```
mkdir <dir name>
```
Once you are done with that, go ahead and cd into that directory. You'll then want to clone this repository into that new directory by running:
```
git clone git@github.com:wynessgp/CSSE332-Review-Session-2-Material.git
```
That should create a copy of all of the repository's files on your WSL, inside a new folder. Like last session, I would recommend leaving the GitHub page up for the purposes of navigating the README - I've included links so you can quickly jump around.

## *Administrative* Notes:
This github will ONLY contain the practice exam, the solutions will NOT be posted here. I've been explicitly asked not to post them here as it will make my sample solutions public. Please, reach out to me individually if you really find having the solutions would help you that much - but remember, it'll take away from the learning process if you can't get them on your own, so you'll REALLY have to pry them out of my hands. I'll give you hints, though.<br><br>
Your second exam will have a similar format to exam 1 in terms of what days the questions are on, and the relative difficulty, but this exam will not **exclusively** be coding. Instead, your professors would like for you to think about these problems more conceptually. As far as I'm aware, the structure is as follows:
- Day 1: Paper portion. It will likely only contain a few problems; with less of an emphasis on **exactly** correct syntax and more for understanding. You might write some psuedocode here.
- Day 2: Coding portion. Here you will be tested on your syntax and will likely be asked to solve a more difficult problem, likely to take up the whole time. 

The content will be focused on creating threads, locks, conditional variables, and your ability to safely use shared memory spaces.

Again, you will be connecting to a remote server for the *coding* problems on this exam. That means no notes (unless if your professor allows you to clone over your OS repo), except for a small cheat sheet. Make sure you review your syntax for calls like pthread_create, pthread_join, pthread_mutex_lock, etc. 

Also, a gentle reminder that man pages are your best friend - they show you all of the info you'd need for functions. You can access the man page for pthread_create by simply typing:
```
man pthread_create
```

## *Content* Notes
Threads, unlike processes, have **SHARED** global memory... more specifically, they share a code section, data section, and some OS resources (opened files, signals, etc). However, threads do happen to have unique stacks, so local variables are largely unaffected.
### On Global Variables
This is why we have to be so cautious about how we utilize global variables. If there's a chance that another thread may need to access what your current thread is looking at, you'll need to use a lock to prevent an inconsistent state between the two separate threads. You don't want to have a thread actively updating a variable's value while another is doing a comparison with it - one should be resolved before another, otherwise you'll get incorrect behavior. So unless if you can guarantee that your threads will not overlap in **ANY** way, make sure you **LOCK!** <br><br>
Most of the time, you will need locks. The only situation I can possibly think of where you won't need one is if you can guarantee that threads access only their own specified index within an array. The original thread can then come back and clean up the data after the child threads are done.
### On Critical Sections...
In order to have threads really run anything in parallel, it is imperative that you **UNLOCK** as soon as you are done with any thread overhead code - that way it gives other threads an opportunity to actually run the relevant code in the **CRITICAL SECTION**. If you do not unlock, your threads will run in a serial manner as opposed to a parallel one, which is WAY slower. I have written a small example to show this off, not unlike what you guys did for max in conditional variable basics: <br><br>
Properly unlocking example:
```
$ ./with_unlocks.bin
timing threads starting now!
Thread 1 printing!
Thread 4 printing!
Thread 5 printing!
Thread 2 printing!
Thread 3 printing!
Thread 7 printing!
Thread 8 printing!
Thread 6 printing!
Thread 11 printing!
Thread 9 printing!
Thread 13 printing!
Thread 10 printing!
Thread 15 printing!
Thread 12 printing!
Thread 14 printing!
Thread 16 printing!
Thread 17 printing!
Thread 18 printing!
Thread 19 printing!
Thread 20 printing!
Program took 3178 usecs
```
Improperly unlocking example:
```
$ ./without_unlocks.bin
timing threads starting now!
Thread 1 printing!
Thread 2 printing!
Thread 3 printing!
Thread 5 printing!
Thread 4 printing!
Thread 6 printing!
Thread 7 printing!
Thread 8 printing!
Thread 9 printing!
Thread 10 printing!
Thread 12 printing!
Thread 13 printing!
Thread 11 printing!
Thread 14 printing!
Thread 16 printing!
Thread 15 printing!
Thread 17 printing!
Thread 19 printing!
Thread 18 printing!
Thread 20 printing!
Program took 12454 usecs
```
As you can see, that's an almost 4x boost to performance by properly unlocking. If you want to tinker with this a bit, I have included my benchmark programs in the ```benchmarks``` folder in the GitHub. There is a makefile provided so you can change the ```#define``` statements at the top as you may wish. Note that there is only a 2 line difference between these files!

### On Signal vs. Broadcast
Sometimes we will ask you to implement things that expect multiple threads to be running a specific way all at once. Think tunnel, three jobs, and the like. If you cannot guarantee exactly how many threads will be waiting in a queue, and you'd like for **MULTIPLE** of the waiting ones to start running at the same time, it is in your best interest to use *broadcast*. Even if you have multiple threads currently running, if there's any chance you might have *MORE* after this one, you should *broadcast*.<br> <br>
In the case of ```tunnel```, think about an ambulance interrupting normal traffic flow - cars might start piling up at the EW entrance, so when that ambulance leaves, it should *broadcast* to those EW cars. (This is a bit of an oversimplifiction, but you get the point) If it alerts only one car with signal, then suddenly the EW side of the tunnel shrinks to one lane! <br><br>
As far as signal goes, this is for a *targeted* number of threads. *Signal* itself only wakes up one thread in a queue - so if you have multiple threads running a *signal* call, you'll end up waking up however many corresponding threads. You can also have just one thread call it multiple times. <br><br>
It is important to note that these can both generate viable solutions to a problem depending on how you make threads wait, but switching between the two can sometimes vastly reduce the amount of code you have for a problem. <br><br>
An important thing to note is that these will **NOT** error if there are no threads waiting in a queue, it just simply won't wake anyone up. <br><br> 
Additionally, you should aim to use both of these calls while you still have a lock. Why? Most of the time a program will still have correct behavior regardless if you signal while locked or unlocked, but if you want to 100% guarantee correctness, it is **safer** to use broadcast and signal while you are holding a lock, that way you know which thread is actually doing the signal or broadcast. I'm fairly certain this is what Mohammad and Hays expect from you, so you might as well do it!

## On Waiting
To make a long paragraph short: use while loops whenever you anticipate having a wait. It prevents any cases where the thread gets woken up and suddenly goes rogue, ruining every other part of your code. <br><br>
The more elaborate reason why we use while loops - whenever that thread gets woken up from that wait statement, it'll have to re-evaluate the condition of the while loop. So if the world still isn't a better place, the thread goes back to waiting in the queue, no harm done. It provides us with a lot more flexibility when it comes to signal and broadcast, since any threads that we still don't want to run still won't run. If you don't use while loops, we will find you. 

## Practice Problems
No funny business this time with a Christmas theme, I promise. You get something worse...
### **Problem 1: Grading** 
Length: Medium <br>
Objective: More familiarity with threads! <br>
Files to modify: ```grading.c``` <br>
### Description:
Suppose that you are a grader for your favorite CSSE class. (Operating Systems, obviously!) You are working as a member of a team in order to get graded assignments out in a timely manner to students. One day, you are approached by one of the instructors you work for. Suddenly, they say: <br><br>
"Hey, why don't we start giving our graders a designated group of students that they grade - that should make things go a lot faster, right?" <br><br>
This begs the question -- why weren't you doing that before? <br><br>
Anyways, you're now a couple days into the system, and assignments are going way faster than before.One downside you've noticed - the website you grade on seems to get very unstable if there are too many people on it... <br><br>
So, let's attempt to model something like this using threads in C. In ```grading.c```, I have provided you with the code to create the student and grader threads. Here are some strict requirements on what your solution must do to model this grading process: <br>
<ol>
    <li>All of the students associated with a grader must make their submission before they can begin grading.</li>
    <li>Once a grader has finished their allocated assignments, they must indicate it using the printout provided.</li>
    <li>Only WEBSITE_MAX people (graders, students) may be on the website at any one point in time. Everyone else must wait their turn. </li>
    <li>In the case that there are more graders than the WEBSITE_MAX allows, they should alternate who's on the website. (i.e, everyone should get a turn)</li>
</ol>
Something important to note: you need to be careful to avoid starvation here - you are not guaranteed that all of the graders arrive before all of the students make their submissions, nor are you guaranteed that all of the students will have made a submission before all of the graders arrive.

### **Problem 2: The Classroom** 
Length: Long <br>
Objective: More practice! <br>
Files to modify: ```classroom.c``` <br>
### Description:
Suppose that in a weird alternate Rose-Hulman, students can only enter a classroom once a professor has already entered. Additionally, suppose that each class is always taught by two professors, one ME, and one CS. (What a weird combo!) <br><br>
Diverting even further from reality, let's suppose that these classes the professors are teaching have no set roster. So you could have 18 students one day, and a completely unique set of 24 students the next day. Additionally, suppose that all of the classes taught in this classroom are the exact same (so no constraints on what students can enter or not). <br><br>
Some additional quirks: you are not guaranteed that every CS and ME professor shows up on every given day, so some days, a couple of CS and ME professors have to leave, having not taught a class. If it's time for their class to start and there isn't the other type of professor available, they leave. <br><br>
Finally, there is still a max student capacity on the classroom. Students will file in until they reach the max capacity, and once it is reached, everyone else must wait until the next instance of the class is offered. Once the class starts, no more students should be able to enter. If all of the professors for the day have arrived and some students still haven't been able to attend a class, they will leave, unhappy. <br><br>
Also, if professors arrive and another group is already teaching, they must wait until the current group is done, and the students leave. They will only check to see if a professor of the other type is *available* once they get the opportunity to *enter* the classroom. <br><br>
As such, there's quite a few requirements here on your solution in order to get things working properly: <br>
<ol>
    <li>All print statements about professors entering a classroom must have an accompanying one of the other professor's type.</li>
    <li>Students should only enter the classroom if a professor does.</li>
    <li>If a professor cannot find an accompanying professor by the time they go to teach, they need to leave.</li>
    <li>No professor should leave the classroom until all of the students in the classroom do.</li>
    <li>If all of the professors have taught for the day and there are students left over, they should leave.</li>
    <li>There doesn't need to be a full class's worth of students waiting to start the class.</li>
</ol>
A small hint to help with what I deem to be the hardest requirement - the last one: <br> <br>
It is sufficient to have whoever is alerting the other professor also alert all of the waiting students that "hey, this class is starting". If there is more than the max capacity waiting, that's OK, but if there's less, that's also OK. <br><br>
This leads to a weird case where there might be 0 students waiting, but given all of the other constraints I've given you, I'd say that's OK. <br><br>

Also, your professor thread specific code might look very different from the traditional style here. Don't worry about it too much, as long as it's not "hacky". 

### **Problem 3: Office Hours** 
Length: Short <br>
Objective: Be familiar in a situation which doesn't require locks! <br>
Files to modify: ```office_hours.c``` <br>
### Description:








