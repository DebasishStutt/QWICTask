# QWICTask

Problem Statement: https://bitbucket.org/qwic/iot-test/src/master/

----------------------------------------------------------------------------------------------------------------------------------------



Solution to the task by QWIC
----------------------------------------------------------------------------------------------------------------------------------------

The solution satisfies the rules stated in the problem statement.
This readme tries to explain parts of the code that might not be obvious at the first glance.

1. struct abortHandler is a exception or error handling struct that can be used to indicate an unexpected or error situation that is then unifiedly handled and the program aborts with proper reason being displayed.

2. The message "Automaton-LG is no more!!" is displayed when $ is reached. 

3. testmaps.txt lists  out some maps that were tested on the code. As the dimensions of the map were not limited so effectively infinite number of maps exists.

4. For the case where Automaton-LG never reaches the $, I have considered 100 steps of Automaton-LG's movement before she/he says "In infinite loop!!! Extract the repeating pattern by observation."

100 steps might be less for a BIG map to actually enumerate a repeating pattern, but again that is open ended problem as the dimensions of the maps are not limited.
