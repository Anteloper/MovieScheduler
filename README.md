# MovieSchduler

###Usage
In a unix environment:<br \> <br \> 

```
g++ main.cpp
./a.out input.txt 720 1380
```
The second and third two arguments are optional: they give the user the option to overwrite the default<br \> 
the open and close time that are otherwise calculated based on weekday.<br \> <br \> 


###Philosophy
In general in my code when forced to make a tradeoff, I chose simplicity and readability of code over efficiency.<br \>
I contemplated using global variables to minimize expensive arguments such as vectors being returned by value by some functions<br \>
but decided against it, for the decreased maintainability that global variables create, and the fact that passing by value for vectors<br \>
is heavily optimized for low overhead in C++11. I passed by reference where I could, but in the recursive case, it created too much of a mess.<br \><br \>

In attacking the core problem of scheduling, I prioritized maximum number of showings in a day above all else. <br \>
I assumed A movie may end at the same minute that the theater closes. <br \>
The next priority was scheduling as late in the day as possible, and the final priority was creating readable showtimes<br \> <br \> 

I solved this problem recursively, using the following rules<br \>
 1. Check the maximum number of showtimes possible by starting at the latest possible time to the close<br \>
 2. Move the last showtime to the earliest easy-to-read time<br \>
 3. Check if the maximum number of shows in the day changed as a result of the move<br \>
 4. If it did, use the original showtime, not the easy-to-read one.<br \>
 5. Repeat the above rules on the next latest showtime, using this one as the new "closing time"<br \><br \>

 In choosing how to store the data in an abstract data type, I was faced with choice of classes or structs.<br \>
 In some ways, this problem lended itself to structs, as the member variables had no real need to be protected from the programmer using them <br \>
 and the movie objects were essentially just a bag of data, with no crucial member functions. I opted for classes instead however, largely because 
 the Movie data type needed to std::strings which aren't primitive types and minimize the advantages of structs.<br \>
