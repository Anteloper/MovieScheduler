//
//  main.cpp
//  MovieScheduler
//
//  Created by Oliver Hill on 2/26/17.
//  Copyright © 2017 Oliver Hill. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>
#include "Movie.cpp"


//All times, including hh:mm are referred to internally in minutes (0-1440)
const int cleanupTime = 35;
const int setupTime = 60;


void readFile(const char * filename, std::vector<Movie> &movies);
std::string getDateString();
int getWeekday();
std::pair<int, int> numberOfShows(int runTime, int openTime, int closeTime);
std::string formattedTime(int m, bool shouldPadHour);
void printScheduleOfMovie(Movie &mov, int openTime, int closeTime);
void populateShowtimes(int runTime, int mustEndBy, int showNum, int freeMinutesRemaining, std::vector<int> &timesSoFar);


int main(int argc, const char * argv[]) {
    int openTime;
    int closeTime;
    std::vector<Movie> movies;
    
    //Monday - Thursday: 8am open, 11pm close
    if(getWeekday() > 0 && getWeekday() <5 ){
        openTime = 480;
        closeTime = 1380;
    }
    //Friday-Sunday: 10:30am open, 11:30pm close
    else{
        openTime = 630;
        closeTime = 1410;
    }
    
    //User can optionally change open/close time from the command line
    //First argument after filename is opening time in minutes since midnight of that morning
    //Second argument after filename is closing time in minnutes since midnight of that morning
    if(argc == 4)
        closeTime = std::stoi(argv[3]);
    if(argc >= 3)
        openTime = std::stoi(argv[2]);
    
    if(argc >=2){
        try{
            //Populate list of Movie objects
            readFile(argv[1], movies);
            
            //Print Schedule Header
            std::cout << getDateString() << std::endl << std::endl;
            
            //Enumerate list, print schedule for each one
            for(int i = 0; i<movies.size(); i++)
                printScheduleOfMovie(movies.at(i), openTime, closeTime);
        }
        catch(std::string exc){
            std::cout << exc;
        }
    }
    else
        std::cout << "Incorrect number of arguments entered, program will terminated \n\n";
    
    return 0;
}


//Reads the input file, creates a movie object for each line, stores it in vector passed in
//May throw an exception if file is not read properly
void readFile(const char * fileName, std::vector<Movie> &movies){
    std::ifstream inputFile;
    std::string line;

    inputFile.open(fileName);
    if(inputFile){
        //Clear the header line
        getline(inputFile, line);
        
        //Create a movie object from each line and store it in the movies vector
        while(getline(inputFile, line))
            movies.push_back(Movie(line));
        
        inputFile.close();
    }
    else
        throw(std::string("Could not open the file inputted, the program will exit now.\n"));
}



//Prints the day's schedule for an individual movie to the console
void printScheduleOfMovie(Movie &mov, int openTime, int closeTime){
    std::string indent = "     ";
    
    //Shows in the day are zero indexed so the first showing is show #0
    std::pair<int, int> scheduleDetails = numberOfShows(mov.getRunTimeMinutes(), openTime, closeTime);
    
    //Populate the showtimes vector prioritizing maximum amount of showings first,
    //Latest possible times second, and readability of times third
    std::vector<int> showtimes;
    populateShowtimes(mov.getRunTimeMinutes(), closeTime, scheduleDetails.first-1, scheduleDetails.second, showtimes);
    
    //Movie Header
    std::cout << mov.getTitle() << " -- Rated " << mov.getRating();
    std::cout << ", " << formattedTime(mov.getRunTimeMinutes(), false) << std::endl;
    
    
    for(int i = showtimes.size()-1; i>=0; i--){
        int startTime = showtimes.at(i);
        std::cout << indent <<  formattedTime(startTime, true) << " - ";
        std::cout << formattedTime(startTime+mov.getRunTimeMinutes(), true) << std::endl;
    }
    
    std::cout << std::endl << std::endl;
    
}


/*Recursively populates a vector full of showtimes with the philosophy that it is never
 worth losing a movie showing for the sake of easy-to-read times. Rules for this are the following:
 1. Check the maximum number of showtimes possible by starting at the latest possible time to the close
 2. Move the last showtime to the earliest easy-to-read time
 3. Check if the maximum number of shows in the day changed as a result of the move
 4. If it did, use the original showtime, not the easy-to-read one.
 5. Repeat the above rules on the next latest showtime, using this one as the new "closing time"*/

void populateShowtimes(int runTime, int mustEndBy, int showNum, int freeMinutesRemaining, std::vector<int> &timesSoFar){
    
    int latestShowStart = mustEndBy-runTime;
    
    //Stopping case
    if(showNum < 0) return;

    //If showtime isn't already readable
    if(latestShowStart%5 != 0){
        int lastDigit = latestShowStart%10;
        int timeChange = lastDigit > 5 ? lastDigit-5 : lastDigit;
        
        //Change to the earliest readable version if the readable version doesnt lose a showing
        if(freeMinutesRemaining >= timeChange){
            latestShowStart = latestShowStart-timeChange;
            freeMinutesRemaining -= timeChange;
        }
    }
    
    timesSoFar.push_back(latestShowStart);
    
    //Recursive step
    populateShowtimes(runTime, latestShowStart-35, showNum-1, freeMinutesRemaining, timesSoFar);
}



//Returns the maximum number of shows possible, assuming showTime is the LAST possible showtime
//And the amount of time remaining at the beginning of the day once all movies are accounted for
std::pair<int, int> numberOfShows(int runTime, int openTime, int closeTime){
    int showTime = closeTime-runTime;
    int num = 0;
    
    while(showTime >= openTime+setupTime){
        num++;
        showTime -= (runTime + cleanupTime);
    }
    showTime += runTime+cleanupTime;
    
    //Maximum number of shows, amount of "slack" in the schedule
    return {num, showTime-openTime};
}




/*DATE AND TIME FORMATTING FUNCTIONS*/


//Returns a readable 24 hour time given the number of minutes that have passed since midnight
std::string formattedTime(int m, bool shouldPadHour){
   
    std::string hours = std::to_string(m/60);
    //Insert 0 into string if necessary and desired
    if(hours.length() == 1 && shouldPadHour)
        hours.insert(hours.begin(), '0');
    
    std::string minutes = std::to_string(m%60);
    //Insert 0 into string if necessary
    if(minutes.length() == 1)
        minutes.insert(minutes.begin(), '0');
    
    return hours + ":" + minutes;
}

int getWeekday(){
    //Get current time
    time_t currentTime = time(0);
    struct tm *now = localtime(&currentTime);
    return now->tm_wday;
}

//These are the moments when I wish I'd used python
std::string getDateString(){
    std::string date = "";
    std::string weekdays[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    
    //Get current time
    time_t currentTime = time(0);
    struct tm *now = localtime(&currentTime);
    std::string weekday = weekdays[now->tm_wday];
    date += weekdays[now->tm_wday] + " ";
    date += std::to_string(now->tm_mon) + "/" + std::to_string(now->tm_mday) + "/" + std::to_string(1900 + now->tm_year);

    return date;
}






