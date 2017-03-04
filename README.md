# MovieSchduler

###Usage
In a unix environment:<br \> 

```
g++ main.cpp
./a.out input.txt 720 1380
```
The second and third two arguments are optional: they give the user the option to overwrite the default<br \> 
the open and close time that are otherwise calculated based on weekday.<br \> <br \> 
The example above opens the theater at noon and closes it at at 11pm.


###Philosophy
In general in my code when forced to make a tradeoff, I chose simplicity and readability of code over efficiency.<br \>
I contemplated using global variables to minimize redundantly passing parameters such as opening time, closing time, and vectors containing 
movie and showtime data but decided against it, for the decreased maintainability that global variables create. When passing vectors or Movie
objects to and from functions, I passed by reference to minimize the overhead of copying by value <br \>

In attacking the core problem of scheduling, I prioritized maximum number of showings in a day above all else. <br \>
I assumed A movie may end at the same minute that the theater closes. <br \>
The next priority was scheduling as late in the day as possible, and the final priority was creating readable showtimes<br \> <br \> 



I solved this problem recursively, using the following rules.<br \>
1. Check the maximum number of showtimes possible by starting at the latest possible time to the close<br \>
2. Compute the amount of minutes of "slack" exist in the shedule that shows the maximum shows<br \>

For each show in the maximum-showtime schedule, do the following:<br \>
 
 3. Move the last showtime to the earliest easy-to-read time <br \>
 4. If this change is smaller than the remainng amount of slack, make the change and update the remaining slack <br \>
 5. Otherwise, use the original showtime, not the easy-to-read one.<br \>
 6. Repeat the 3-6 rules on the next latest showtime, using this one as the new "closing time"<br \><br \>

In choosing how to store the data in an abstract data type, I was faced with choice of classes or structs. 
In some ways, this problem lended itself to structs, as the member variables had no real need to be protected from the programmer using them
and the movie objects were essentially just a bag of data, with no crucial member functions. I opted for classes instead however,
largely because the Movie data type needed to std::strings which aren't primitive types and minimize the advantages of structs.
