#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MOVIES 100
#define MAX_GENRE_LENGTH 20
#define MAX_TITLE_LENGTH 50
#define MAX_COMMENT_LENGTH 100
#define MAX_COUNTRY_LENGTH 16
#define MAX_VOTES 100
#define MOVIES "moviesData.txt"
#define VOTES "votingData.txt"

typedef struct {
    int value;
    char* p2comment;
    char country[MAX_COUNTRY_LENGTH];
} Vote;

typedef struct {
    int id;
    char* p2name;
    char studio[31];
    int year;
    int numVotes;
    char* p2genre;
    Vote* votes;
} Movie;

FILE* safe_open(const char* filename, const char* mode) {
    FILE* file = fopen(filename, mode);
    if (!file) {
        printf("Unable to open file.\n");
        exit(1);
    }
    return file;
}

int countLines(const char* filename) {
    FILE* file = safe_open(filename, "rt");
    int count = 0;
    char c;
    while ((c = getc(file)) != EOF) {
        if (c == '\n') {
            count++;
        }
    }
    fclose(file);
    return count;
}

void allocateMovieStrings(Movie* m, int size)
{
    for (int i = 0; i < size; i++)
    {
        m[i].p2name = (char*)malloc(MAX_TITLE_LENGTH * sizeof(char)); // allocate mem for movie title
        m[i].p2genre = (char*)malloc(MAX_GENRE_LENGTH * sizeof(char)); //allocate mem for movie genre

        if (m->p2name == NULL || m->p2genre == NULL)
        {
            printf("Memory allocation failed.\n");
            exit(1); // encountered error
        }
    }
}

void allocateMovievotes(Movie* m, int size)
{
    for (int i = 0; i < size; i++)
    {
        m[i].votes = malloc(MAX_VOTES * sizeof(Vote)); // allocate mem for votes array in the movie array
        if (m[i].votes == NULL)
        {
            printf(" Votes memory allocation failed.\n");
            exit(1);
        }


    }
}

void allocateVoteComments(Movie* m, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < m[i].numVotes; j++)
        {
            if (i != size)
            {
                m[i].votes[j].p2comment = malloc(MAX_COMMENT_LENGTH * sizeof(char));
                if (m[i].votes[j].p2comment == NULL) {
                    printf("Memory allocation for comments failed.\n");
                    exit(1);
                }
            }
        }

    }
}

void allocateMem4Movies(Movie* movies, int size)
{
    allocateMovieStrings(movies, size);
    allocateMovievotes(movies, size);

}

void FromFile2Movies(const char* filename, Movie* array, int size)
{
    FILE* movies_file = safe_open(filename, "rt");
    fscanf(movies_file, "%*[^\n]\n"); // first line skip
    int records = 0;
    while (records < size)
    {
        int read = fscanf(movies_file, "%d,%49[^,],%30[^,],%19[^,],%d\n", &array[records].id, array[records].p2name, array[records].p2genre, array[records].studio, &array[records].year);
        if (read == 5)
        {
            array[records].numVotes = 0;
            records++;
        }
        else if (read != 5 && !feof(movies_file))
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

void FromFile2Votes(const char* filename, Movie* movies, int size) {
    FILE* votes_file = safe_open(filename, "rt");
    fscanf(votes_file, "%*[^\n]\n"); // Skip first line
    int records = 0;
    while (records < size) {
        int movieId, voteValue;
        char comment[MAX_COMMENT_LENGTH], country[MAX_COUNTRY_LENGTH];
        int read = fscanf(votes_file, "%d:%d:%16[^:]:%99[^\n]\n", &movieId, &voteValue, country, comment);
        if (read == 4) {
            int movieIndex = -1;
            for (int i = 0; i < size; i++) {
                if (movies[i].id == movieId) {
                    movieIndex = i;
                    break;
                }
            }
            if (movieIndex != -1) {
                if (movies[movieIndex].numVotes < MAX_VOTES)
                {
                    int voteIndex = movies[movieIndex].numVotes;
                    movies[movieIndex].votes = realloc(movies[movieIndex].votes, (voteIndex + 1) * sizeof(Vote));
                    if (movies[movieIndex].votes == NULL)
                    {
                        printf("Memory allocation for votes failed.\n");
                        exit(1);
                    }
                    // Allocate memory for the comment
                    movies[movieIndex].votes[voteIndex].p2comment = malloc((strlen(comment) + 1) * sizeof(char));
                    if (movies[movieIndex].votes[voteIndex].p2comment == NULL)
                    {
                        printf("Memory allocation for comment failed.\n");
                        exit(1);
                    }
                    // Copy comment and other vote information
                    movies[movieIndex].votes[voteIndex].value = voteValue;
                    strcpy(movies[movieIndex].votes[voteIndex].p2comment, comment);
                    strcpy(movies[movieIndex].votes[voteIndex].country, country);
                    // Increment numVotes for the movie
                    movies[movieIndex].numVotes++;
                }
            }
            else if (read != 4 && !feof(votes_file)) {
                printf("File format is incorrect.\n");
            }
            else if (ferror(votes_file)) {
                printf("Error while reading from file.\n");
                break; // End of file reached
            }
        }
        records++; // Increment records regardless of read result
    }
    fclose(votes_file);
}

void addMovie(Movie** movies, int* size)
{

    char name[MAX_TITLE_LENGTH];
    char studio[31];
    int year;
    // Prompt the user to enter movie details
    printf("Enter the name of the movie: ");
    scanf(" %[^\n]", name);

    printf("\nEnter the studio of the movie: ");
    scanf(" %[^\n]", studio);

    printf("\nEnter the year of the movie: ");
    while (scanf("%d", &year) != 1)
    {
        printf("Invalid iput. Please enter a valid year: ");
        while (getchar() != '\n');
    }

    // Allocate memory for the new movie
    *movies = realloc(*movies, (*size + 1) * sizeof(Movie));
    if (*movies == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    // Allocate memory for the name of the new movie
    (*movies)[*size].p2name = malloc((strlen(name) + 1) * sizeof(char));
    if ((*movies)[*size].p2name == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    // Set the movie details
    (*movies)[*size].id = *size + 1; // Assuming IDs start from 1 and increment by 1
    strcpy((*movies)[*size].p2name, name);
    strcpy((*movies)[*size].studio, studio);
    (*movies)[*size].year = year;

    // Increment the size of the movies array
    (*size)++;
}

int addVote(int movieId, Movie movies[], int size) {
    int index = -1;
    for (int i = 0; i < size; i++) {
        if (movies[i].id == movieId) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        printf("Movie with ID %d not found.\n", movieId);
        return 0;
    }

    int newVote;
    printf("Enter the vote for movie %s (%d): ", movies[index].p2name, movieId);
    scanf("%d", &newVote);

    for (int i = 0; i < movies[index].numVotes; i++) {
        if (movies[index].votes[i].value == newVote) {
            printf("Vote %d already exists for movie %s (%d).\n", newVote, movies[index].p2name, movieId);
            return 0;
        }
    }

    if (movies[index].numVotes < MAX_VOTES) {
        movies[index].votes[movies[index].numVotes].value = newVote;
        printf("Enter comment for the vote: ");
        scanf("%s", movies[index].votes[movies[index].numVotes].p2comment);
        printf("Enter country for the vote: ");
        scanf("%s", movies[index].votes[movies[index].numVotes].country);
        movies[index].numVotes++;
        printf("Vote %d added successfully for movie %s (%d).\n", newVote, movies[index].p2name, movieId);
        return 1;
    }
    else {
        printf("Maximum number of votes reached for movie %s (%d).\n", movies[index].p2name, movieId);
        return 0;
    }
}

void printMenu() {
    printf("\nMain Menu\n");
    printf("1. Add Movie\n");
    printf("2. Add Vote\n");
    printf("3. Print Votes\n");
    printf("4. Count Genre\n");
    printf("0. Exit\n");
}

void saveMoviesToFile(const char* filename, Movie* movies, int size) {
    FILE* file = safe_open(filename, "wt");
    fprintf(file, "ID,Title,Studio,Year\n");
    for (int i = 0; i < size; i++) {
        fprintf(file, "%d,%s,%s,%d\n", movies[i].id, movies[i].p2name, movies[i].studio, movies[i].year);
    }
    fclose(file);
}

int printVotes(const char* movieName, Movie* movies, int size) {
    int found = 0;
    for (int i = 0; i < size; i++) {
        if (strcmp(movies[i].p2name, movieName) == 0) {
            found = 1;
            if (movies[i].numVotes == 0) {
                printf("No votes for movie %s.\n", movieName);
                return 0;
            }
            printf("Comments and Countries for movie %s:\n", movieName);
            for (int j = 0; j < movies[i].numVotes; j++) {
                printf("Comment: %s\n", movies[i].votes[j].p2comment);
                printf("Country: %s\n", movies[i].votes[j].country);
            }
            return 1;
        }
    }
    if (!found) {
        printf("Movie %s does not exist.\n", movieName);
        return -1;
    }
}

int countGenre(const char* genre, Movie* movies, int size) {
    int found = 0;
    printf("Movies of genre %s:\n", genre);
    for (int i = 0; i < size; i++) {
        if (strcmp(movies[i].p2genre, genre) == 0) {
            printf("- %s\n", movies[i].p2name);
            found = 1;
        }
    }
    return found;
}

void printValue(int value, const char* country, Movie* movies, int size) {
    int found = 0;
    printf("Movies with vote value %d from country %s:\n", value, country);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < movies[i].numVotes; j++) {
            if (movies[i].votes[j].value == value && strcmp(movies[i].votes[j].country, country) == 0) {
                printf("- %s\n", movies[i].p2name);
                found = 1;
                break;
            }
        }
    }
    if (!found) {
        printf("No movies found with vote value %d from country %s.\n", value, country);
    }
}

//int countCountry(int year, Movie* movies, int size) {
//    int countries[MAX_MOVIES] = { 0 }; // Array to keep track of countries
//    int uniqueCountries = 0; // Counter for unique countries
//
//    for (int i = 0; i < size; i++) {
//        if (movies[i].year == year) {
//            // Check if the country has been counted already
//            int isNewCountry = 1;
//            for (int j = 0; j < uniqueCountries; j++) {
//                if (strcmp(movies[i].country, movies[countries[j]].country) == 0) {
//                    isNewCountry = 0;
//                    break;
//                }
//            }
//
//            // If it's a new country, add it to the list of countries
//            if (isNewCountry) {
//                countries[uniqueCountries] = i;
//                uniqueCountries++;
//            }
//        }
//    }
//
//    if (uniqueCountries == 0) {
//        printf("No movies from year %d have votes from any country.\n", year);
//    }
//    else {
//        printf("Number of different countries that voted for some movie from year %d: %d\n", year, uniqueCountries);
//    }
//
//    return uniqueCountries;
//}

void maxByCountry(Movie* movies, int size) {
    // Array to keep track of the number of comments by country
    int commentCount[MAX_MOVIES] = { 0 };
    int maxCommentCount = 0; // Maximum comment count
    char countries[MAX_MOVIES][MAX_COMMENT_LENGTH]; // Array to store country names

    // Count comments by country
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < movies[i].numVotes; j++) {
            if (movies[i].votes[j].p2comment[0] != '\0') {
                for (int k = 0; k < size; k++) {
                    if (strcmp(countries[k], movies[i].votes[j].country) == 0) {
                        commentCount[k]++;
                        break;
                    }
                    else if (strcmp(countries[k], "") == 0) {
                        strcpy(countries[k], movies[i].votes[j].country);
                        commentCount[k]++;
                        break;
                    }
                }
            }
        }
    }

    // Find maximum comment count
    for (int i = 0; i < size; i++) {
        if (commentCount[i] > maxCommentCount) {
            maxCommentCount = commentCount[i];
        }
    }

    // Print countries with maximum comment count
    printf("Countries with the most comments:\n");
    int found = 0;
    for (int i = 0; i < size; i++) {
        if (commentCount[i] == maxCommentCount) {
            printf("- %s\n", countries[i]);
            found = 1;
        }
    }

    if (!found) {
        printf("No comments found.\n");
    }
}

void RecommendMovie(int X, Movie* movies, int size) {
    FILE* recommendationFile = fopen("Recommendation.txt", "w");
    if (recommendationFile == NULL) {
        printf("Error opening file.\n");
        return;
    }

    float maxAverage = 0;
    for (int i = 0; i < size; i++) {
        int totalVotes = 0;
        int totalScore = 0;
        float average = 0;

        if (movies[i].numVotes == 0) continue;

        for (int j = 0; j < movies[i].numVotes; j++) {
            totalVotes++;
            totalScore += movies[i].votes[j].value;
        }

        average = (float)totalScore / totalVotes;

        if (average >= X) {
            fprintf(recommendationFile, "%s, %s\n", movies[i].p2name, movies[i].p2genre);
        }
    }

    fclose(recommendationFile);
}

void deleteWorst(const char* genre, Movie* movies, int* size) {
    int minVote = 10; // Initialize minVote to a high value
    int found = 0; // Flag to check if any movie of the given genre is found

    // Find the minimum vote for movies of the given genre
    for (int i = 0; i < *size; i++) {
        if (strcmp(movies[i].p2genre, genre) == 0) {
            found = 1; // At least one movie of the given genre is found
            for (int j = 0; j < movies[i].numVotes; j++) {
                if (movies[i].votes[j].value < minVote) {
                    minVote = movies[i].votes[j].value;
                }
            }
        }
    }

    if (!found) {
        printf("No movies of genre %s found.\n", genre);
        return;
    }

    // Delete all votes with the minimum value for movies of the given genre
    for (int i = 0; i < *size; i++) {
        if (strcmp(movies[i].p2genre, genre) == 0) {
            int k = 0; // Index for copying votes without the worst vote
            for (int j = 0; j < movies[i].numVotes; j++) {
                if (movies[i].votes[j].value != minVote) {
                    movies[i].votes[k++] = movies[i].votes[j];
                }
            }
            movies[i].numVotes = k; // Update the number of votes for the movie
        }
    }

    printf("Votes with the lowest value for genre %s have been deleted.\n", genre);
}

int main()
{
    int size = countLines(MOVIES);
    Movie* movies = (Movie*)malloc(size * sizeof(Movie));
    if (movies == NULL)
    {
        printf("Memory allocation failed.\n");
        return 1;
    }
    allocateMem4Movies(movies, size);
    FromFile2Movies(MOVIES, movies, size);
    allocateVoteComments(movies, size);
    FromFile2Votes(VOTES, movies, size);

    int choice;
    do {
        printMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            addMovie(&movies, &size);
            break;
        case 2:
            printf("Enter the ID of the movie to add a vote: ");
            int movieId;
            scanf("%d", &movieId);
            if (addVote(movieId, movies, size))
                printf("Vote added successfully!\n");
            else
                printf("Failed to add vote.\n");
            break;
        case 3:
            printf("Enter the name of the movie to print votes: ");
            char movieName[MAX_TITLE_LENGTH];
            scanf("%s", movieName);
            int result1 = printVotes(movieName, movies, size);
            if (result1 == 0)
                printf("No votes or movie not found.\n");
            else if (result1 == -1)
                printf("Movie not found.\n");
            break;
        case 4:
            printf("Enter the genre to count: ");
            char genre[MAX_GENRE_LENGTH];
            scanf("%s", genre);
            int result2 = countGenre(genre, movies, size);
            if (!result2)
                printf("No movies found for genre %s.\n", genre);
            break;
        case 0:
            printf("Exiting program...\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);

    saveMoviesToFile(MOVIES, movies, size);
    //saveVotesToFile(VOTES, movies, size); // Uncomment this line once addVote function is implemented

    return 0;
}
