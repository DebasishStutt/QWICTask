# QWICTask

Problem Statement: https://bitbucket.org/qwic/iot-test/src/master/

----------------------------------------------------------------------------------------------------------------------------------------



Solution to the task by QWIC
----------------------------------------------------------------------------------------------------------------------------------------

The solution satisfies the rules stated in the problem statement.
This readme tries to explain parts of the code that might not be obvious at the first glance.

**Dev Env: windows 10, codeblocks, gnu for windows**

1. struct abortHandler is a exception or error handling struct that can be used to indicate an unexpected or error situation that is then unifiedly handled and the program aborts with proper reason being displayed.

2. The message "Automaton-LG is no more!!" is displayed when $ is reached. 

3. testmaps.txt lists  out some maps that were tested on the code.

4. For the case where Automaton-LG never reaches the $, I have considered 100 steps of Automaton-LG's movement before it says "LOOP".

5. A linked list stores the steps taken. If $ is reached within a number of 99 steps then the sequence is displyed or else LOOP is displayed.
