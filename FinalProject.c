#define _CRT_SECURE_NO_WARNINGS
#define MAX_MOVIES 100
#define MAX_TITLE_LENGTH 50
#define MAX_COMMENT_LENGTH 100
#define MAX_VOTES 100
#define MOVIES "moviesData.txt"
#define VOTES "votingData.txt"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct vote {
    int value;
    char* p2comment;
    char country[16];
} vote;

typedef struct movie {
    int id;
    char* p2name;
    char* p2genre;
    char studio[31];
    int year;
    vote* votes;
    int num_votes;
} movie;

void allocateMovieStrings(movie* m)
{
    m->p2name = (char*)malloc(MAX_COMMENT_LENGTH * sizeof(char)); // allocate mem for name
    m->p2genre = (char*)malloc(MAX_COMMENT_LENGTH * sizeof(char)); // allocate mem for genre

    if (m->p2name == NULL || m->p2genre == NULL) { // allocation check
        printf("Memory allocation failed.\n");
        exit(1);
    }
}

FILE* safe_open(const char* filename, const char* mode)
{
    FILE* file;
    file = fopen(filename, mode);
    if (!file)
    {
        printf("Unable to open file.\n");
        return 1;
    }
    return file;
}

int countLines(const char* filename)
{
    FILE* file;
    file = safe_open(filename, "rt");
    int count = 0;
    char c;
    fscanf(file, "%*[^\n]\n"); // skip first line
    for (c = getc(file); c != EOF; c = getc(file)) // read line 
        if (c == '\n')
            count = count + 1; // update count 
    fclose(file);
    return count + 1; // add 1 for last line 
}

void FromFile2Movie(const char* filename, movie* array, int size)
{
    FILE* movies_file;
    movies_file = safe_open(filename, "rt");
    fscanf(movies_file, "%*[^\n]\n"); //skip format line
    int read = 0;
    int records = 0;
    while (records < size)
    {
        allocateMovieStrings(&array[records]); //allocate mem for name and genre
        read = fscanf(movies_file, "%d,%[^,],%[^,],%30[^,],%d\n", &array[records].id, array[records].p2name, array[records].p2genre, array[records].studio, &array[records].year);
        if (read == 5) records++; // all fields read
        else if (read != 5 && !feof(movies_file)) //handle failed read from file
        {
            printf("File format is incorrect.\n");
            break;
        }
        else if (ferror(movies_file))
        {
            printf("Error reading from file.\n");
            break;
        }
    }

    fclose(movies_file);
}
int FromFile2Votes(const char *filename, movie *movies, int numMovies) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: Cannot open file %s\n", filename);
        return 0; // Indicate failure
    }

    char buffer[256]; 
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        char *token = strtok(buffer, ":");

        
        int movieId = atoi(token);

        
        int movieIndex = -1; 
        for (int i = 0; i < numMovies; i++) {
            if (movies[i].id == movieId) {
                movieIndex = i;
                break;
            }
        }

        if (movieIndex == -1) {
            fprintf(stderr, "Error: Vote for unknown movie (ID: %d)\n", movieId);
            continue; 
        }

       
        vote *newVote = malloc(sizeof(vote)); 
        if (newVote == NULL) {
            fprintf(stderr, "Memory allocation error!\n");
            fclose(fp);
            return 0; 
        }

        
        token = strtok(NULL, ":"); 
        newVote->value = atoi(token);

        token = strtok(NULL, ":");
        strcpy(newVote->country, token); 

        token = strtok(NULL, ":"); 
        if (token != NULL && strcmp(token, "-\n") != 0) { // Handle comment
            newVote->p2comment = malloc(strlen(token) + 1);
            if (newVote->p2comment == NULL) {
                fprintf(stderr, "Memory allocation error!\n");
                free(newVote);
                fclose(fp);
                return 0; 
            }
            strcpy(newVote->p2comment, token); 
        } else {
            newVote->p2comment = NULL; // No comment
        }

        // 5. Add the vote to the movie's list (you'll need to implement dynamic resizing) 
        // ...

    }

    fclose(fp);
    return 1; 
}


int addMovie(movie movies[], int *size) {
    char title[MAX_TITLE_LENGTH];
    int year;
    int rating;

    // Input movie details from the user
    printf("Enter the title of the movie: ");
    scanf("%s", title);
    printf("Enter the year of the movie: ");
    scanf("%d", &year);
    printf("Enter the rating of the movie: ");
    scanf("%f", &rating);

    // Check if the movie already exists in the array
    for (int i = 0; i < *size; i++) {
        if (strcmp(movies[i].p2name, title) == 0 && movies[i].year == year) {
            printf("Movie already exists in the list.\n");
            return 0; // Movie already exists
        }
    }

    // Add the new movie to the array
    strcpy(movies[*size].p2name, title);
    movies[*size].year = year;
    movies[*size].votes = rating;
    movies[*size].num_votes = 0;
    (*size)++; // Update the size of the movie array

    return 1; // Movie added successfully
}

int main() {
    movie movies[MAX_MOVIES];
    int size = 0;
    int result;

    result = addMovie(movies, &size);
    if (result == 1) {
        printf("Movie added successfully!\n");
    } else {
        printf("Failed to add movie.\n");
    }

    return 0;
}

int addVote(int movieId,  movie movies[], int size) {
    // Search for the movie with the given ID
    int index = -1;
    for (int i = 0; i < size; i++) {
        if (movies[i].id == movieId) {
            index = i;
            break;
        }
    }
    // If movie not found, return 0 (failure)
    if (index == -1) {
        printf("Movie with ID %d not found.\n", movieId);
        return 0;
    }

    // Get vote data from the user
    int newVote;
    printf("Enter the vote for movie %s (%d): ", movies[index].p2name, movieId);
    scanf("%d", &newVote);

    // Check if the vote already exists in the movie's vote list
    for (int i = 0; i < movies[index].num_votes; i++) {
        if (movies[index].votes[i].value == newVote) {
            printf("Vote %d already exists for movie %s (%d).\n", newVote, movies[index].p2name, movieId);
            return 0; // Vote already exists, return 0 (failure)
        }
    }

    // Add the new vote to the movie's vote list
    if (movies[index].num_votes < MAX_VOTES) {
        movies[index].votes = newVote;
        movies[index].num_votes++ ;
        printf("Vote %d added successfully for movie %s (%d).\n", newVote, movies[index].p2name, movieId);
        return 1; // Successfully added vote, return 1 (success)
    } else {
        printf("Maximum number of votes reached for movie %s (%d).\n", movies[index].p2name, movieId);
        return 0; // Maximum number of votes reached, return 0 (failure)
    }
}

int printVotes(const char* movieName, const movie* movies, int size)
{
    int found = 0;
    for (int i = 0; i < size; i++) {
        if (strcmp(movies[i].p2name,movieName)==0)
        {
            found = 1;
            if (movies[i].num_votes == 0) {
                printf("There are no vote for the movie '%s'.\n", movieName);
                return 0; 
            }
            else
            {
                printf("Comments for '%s' :\n", movieName);
                for (int j = 0; j < movies[i].num_votes; j++) {
                    if (movies[i].votes[j].p2comment != NULL)
                        printf("- %s\n", movies[i].votes[j].p2comment);
                }
                return 1;
            }
        }
    }
    if (!found) {
        printf("Movie '%s' was not found.\n", movieName);
        return -1;
    }
    return 0;
}

void countGenre(char* genre, const movie* movies, int size) {
    int found = 0;
    for (int i = 0; i < size; i++) {
        if (strcmp(movies[i].p2genre, genre) == 0) 
        {
            found = 1;
            printf("%s\n", movies[i].p2name);
        }
    }
    if (!found)
        printf("No movies found with the '%s' genre.\n", genre);
}

void printValue(int V, const char* C, const movie* movies, int size) 
{
    int found = 0; 
    for (int i = 0; i < size; i++) // find movie 
    {
        if (movies[i].num_votes > 0) //check if current movie has votes
        {
            for (int j = 0; j < movies[i].num_votes; j++) // go through vote array for movie
            {
                if (movies[i].votes[j].value == V && strcmp(movies[i].votes[j].country, C) == 0) // check if movie has V and C
                {
                    found = 1;
                    printf("%s\n", movies[i].p2name); //print name of movie
                    break;
                }
            }
        }
    }
    if (!found)
        printf("No movies found with votes %d from country '%s'.\n", V, C); // Movie not found 
}

void countCountry(int Y, movie* movies, int size) 
{

}


int main() {
    // Example usage
    struct movie movies[10] = {
        {1, "The Matrix", 1999, 8.7, 0, {}}, // Example movie data
        {2, "Inception", 2010, 8.8, 0, {}}   // Example movie data
        // Add more movie data as needed...
    };
    int numMovies = 2; // Update this with the actual number of movies

    // Example usage of addVote function
    int movieId;
    printf("Enter the ID of the movie to add a vote: ");
    scanf("%d", &movieId);
    addVote(movieId, movies, numMovies); // Call addVote function

    return 0;
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
    int num_movies = countLines(MOVIES);

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
