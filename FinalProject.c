#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_MOVIES 100
#define MAX_COMMENT_LENGTH 100

typedef struct {
    int id;
    char* name;
    char* genre;
    char studio[31];
    int year;
    struct vote* votes;
    int num_votes;
} movie;

typedef struct {
    int value;
    char* comment;
    char country[16];
} vote;

FILE* safe_open(const char* filename, const char* mode)
{
    FILE* file = fopen(filename, mode);
    if (!file)
    {
        printf("Unable to open file.\n");
        return 0;
    }
    return file;
}

int countLines(char* filename)
{
    FILE* file;
    file = safe_open(filename, "r");    
    int count = 0;  
    char c;
    for (c = getc(file); c != EOF; c = getc(file))
        if (c == '\n')
            count = count + 1;
    fclose(file);
    return count;
}

void freeMovie(movie* m) {
    // Implementation to free memory allocated for a movie
}

void freeMovies(movie* movies, int size) {
    // Implementation to free memory allocated for an array of movies
}

int countGenre(char* genre, movie* movies, int size) {
    // Implementation of countGenre function
}

// Other function prototypes...

int main() {
    // Read data from files and populate arrays of movies and votes
    // Define array of movies
    movie movies[MAX_MOVIES];
    int num_movies = 0;

    // Populate array of movies
    // Use countLines and FromFile2Movies functions to read data from moviesData.txt
    // Use FromFile2Votes function to read data from votingData.txt

    // Display main menu and handle user input
    int choice;
    do {
        // Display menu
        printf("\nMain Menu\n");
        printf("1. Add Movie\n");
        printf("2. Add Vote\n");
        printf("3. Print Votes\n");
        printf("4. Count Genre\n");
        // Add more menu options for other functions as per requirements

        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            // Call addMovie function
            break;
        case 2:
            // Call addVote function
            break;
        case 3:
            // Call printVotes function
            break;
        case 4:
            // Call countGenre function
            break;
            // Add cases for other menu options
        case 0:
            printf("Exiting program...\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);

    // Save updated data back to files
    // Free dynamically allocated memory
    freeMovies(movies, num_movies);

    return 0;
}
