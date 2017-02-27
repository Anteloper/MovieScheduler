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

int openTime;
int closeTime;


class Movie{
private:
    std::string title;
    int yearReleased;
    std::string rating;
    int runTimeMinutes;
    
public:
    
    //Constructs a Movie object from an input file line
    Movie(std::string line){
        
        //Find locations of commas
        size_t titlePointer = line.find(',');
        size_t yearPointer = line.find(',', titlePointer+1);
        size_t ratingPointer = line.find(',', yearPointer+1);
        size_t hoursPointer = line.find(':', ratingPointer+1);
        
        //Split and format string by commas found, store as appropriate data type
        this->title = line.substr(0, titlePointer);
        this->yearReleased = std::stoi(line.substr(titlePointer+2, yearPointer-titlePointer-2));
        this->rating = line.substr(yearPointer+2, ratingPointer-yearPointer-2);
        int hours = std::stoi(line.substr(ratingPointer+2, hoursPointer-ratingPointer-2));
        int minutes = std::stoi(line.substr(hoursPointer+1));
        this->runTimeMinutes = hours*60 + minutes;
    }
    
    
    Movie(std::string title, int yearReleased, std::string rating, int runTimeMinutes){
        this->title = title;
        this->yearReleased = yearReleased;
        this->rating = rating;
        this->runTimeMinutes = runTimeMinutes;
    }
    //Accessors
    std::string getTitle() const{ return title; }
    int getYearReleased() const { return yearReleased; }
    std::string getRating() const { return rating; }
    int getRunTimeMinutes() const { return runTimeMinutes; }
    std::pair<int, int> getRunTimeMinutesHours() const {
        return std::pair<int, int>(runTimeMinutes/60, runTimeMinutes%60);
    }
    
    //Mutators
    void setTitle(std::string title){this->title = title;}
    void setYearReleased(int yearReleased){ this->yearReleased = yearReleased; }
    void setRating(int rating){ this->rating = rating; }
    void setRunTime(int runTimeMinutes){ this->runTimeMinutes = runTimeMinutes;}
    void setRunTime(int hours, int minutes){ runTimeMinutes = hours*60 + minutes;}
};




void readFile(const char * fileName){
    std::ifstream inputFile;
    std::string line;
    
    std::vector<Movie> movies;
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
        std::cout << "Could not open the file inputted, the program will exit now.\n";

}



int main(int argc, const char * argv[]) {
    readFile(argv[1]);
    return 0;
}
