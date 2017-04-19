# desknet
A program to network USB keyboards in a school classroom.

There is a fundamental mismatch between what a teacher does and what a computer can do. The
computer can be a fairly decent tutor, helping one student at a time. A good manager might
get two kids working at one machine, but if he tries for three, he'll likely wind up with
one kid working and two socializing. Teachers, by contrast, work with groups of students,
thus the mismatch. A major challenge to computer aided instruction in the real-world classroom
has been to deal with groups when you don't have a computer for every student.

Desknet is not a general solution to this problem, but it is useful in lower grade classrooms
that work on basic skills. It involves a huge tradeoff, in that each student only gets a single
line on a display, rather than a whole screen, but what the teacher gets in return is
simultaneous access to the program by all students, instantly, whenever she wants it, without
moving students from their desks or to another room. And it turns out that we can do more with
a single screen line than one might think.

The present program includes 4 types of exercises:
    1) Math facts practice for all four operations, presented as real-time competition that 
       includes the entire class. The range of the problems presented is controlled by the 
       teacher, and the scoring is handicapped, so that the top students do not completely 
       dominate the race.
    2) Keyboarding practice. There are one hundred sixty lines of practice, beginning with 
       the home row letters 'f' and 'j', and gradually extending to include all alphabetic 
       reaches.
    3) Word processing. Students can enter compositions of a few hundred words, which can be 
       printed out, either for one individual or an entire class set.
    4) Guided practice. Keyed to whatever math text is in use, this allows students to check 
       their written work on a problem-by-problem basis, as they do it. The program does not 
       tell them the right answer to a problem, only whether their proposed answer is right 
       or wrong. Properly used, this program flags errors the first time they occur and keeps 
       students from practicing a misunderstood algorithm.

You will need to be running gnu/linux as your platform, with the gcc compiler recommended.
Also install SDL2, SDL2-devel, SDL2_ttf, SDL2_ttf-devel, and freetype to run. Run as root.

