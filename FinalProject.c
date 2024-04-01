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
#define MAX_COUNTRIES 196
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
    // Prompt the user to enter movie details
    char name[MAX_TITLE_LENGTH];
    char studio[31];
    char genre[MAX_GENRE_LENGTH];
    int year;

    printf("Enter the name of the movie: ");
    scanf(" %[^\n]", name);

    printf("\nEnter the studio of the movie: ");
    scanf(" %[^\n]", studio);

    printf("\nEnter the year of the movie: ");
    while (scanf("%d", &year) != 1)
    {
        printf("\nInvalid input. Please enter a valid year: ");
        while (getchar() != '\n');
    }

    printf("\nEnter the genre of the movie: ");
    scanf(" %[^\n]", genre);


    // Reallocate memory
    *movies = realloc(*movies, (*size + 1) * sizeof(Movie));
    if (*movies == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    // Allocate memory for the name and genre of the new movie
    (*movies)[*size].p2name = malloc((strlen(name) + 1) * sizeof(char));
    if ((*movies)[*size].p2name == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    (*movies)[*size].p2genre = malloc((strlen(genre) + 1) * sizeof(char));
    if ((*movies)[*size].p2genre == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    // Set the movie details
    (*movies)[*size].id = *size + 1; // IDs start from 1 and increment by 1
    strcpy((*movies)[*size].p2name, name);
    strcpy((*movies)[*size].studio, studio);
    strcpy((*movies)[*size].p2genre, genre);
    (*movies)[*size].year = year;
    (*movies)[*size].numVotes = 0;
    (*movies)[*size].votes = NULL;

    // Increment the size of the movies array
    (*size)++;
}



int addVote(int movieId, Movie* movies, int size)
{
    int index = -1; // Initialize index
    for (int i = 0; i < size; i++) // Iterate through array
    {
        if (movies[i].id == movieId) // Match index to movie's location in the array
        {
            index = i;
            break;
        }
    }
    if (index == -1)  // If movie doesn't exist in the array
    {
        printf("Movie with ID %d not found.\n", movieId);
        return 0;
    }

    int newVote;
    do {
        printf("Enter the vote for movie %s (%d) (between 1-10): ", movies[index].p2name, movieId);
        scanf("%d", &newVote);
        if (newVote < 1 || newVote>10)
            printf("Please enter a valid vote.\n");
    } while (newVote < 1 || newVote>10); // Input check

    for (int i = 0; i < movies[index].numVotes; i++)
    {
        if (movies[index].votes[i].value == newVote) // Check if vote already exists
        {
            printf("Vote %d already exists for movie %s (%d).\n", newVote, movies[index].p2name, movieId);
            return 0;
        }
    }

    if (movies[index].numVotes < MAX_VOTES)
    {
        // Allocate memory for the votes array if it hasn't been allocated yet
        if (movies[index].votes == NULL)
        {
            movies[index].votes = (Vote*)malloc(MAX_VOTES * sizeof(Vote));
            if (movies[index].votes == NULL)
            {
                printf("Memory allocation failed.\n");
                return 0;
            }
        }

        // Allocate memory for the new vote's comment
        movies[index].votes[movies[index].numVotes].p2comment = (char*)malloc(MAX_COMMENT_LENGTH * sizeof(char));
        if (movies[index].votes[movies[index].numVotes].p2comment == NULL)
        {
            printf("Memory allocation failed.\n");
            return 0;
        }

        // Set the value of the new vote
        movies[index].votes[movies[index].numVotes].value = newVote;

        // Prompt the user to enter comment for the new vote
        printf("Enter comment for the vote: ");
        getchar(); // Consume newline left in the input buffer
        fgets(movies[index].votes[movies[index].numVotes].p2comment, MAX_COMMENT_LENGTH, stdin);
        movies[index].votes[movies[index].numVotes].p2comment[strcspn(movies[index].votes[movies[index].numVotes].p2comment, "\n")] = '\0'; // Remove newline character

        // Prompt the user to enter country for the new vote
        printf("Enter country for the vote: ");
        fgets(movies[index].votes[movies[index].numVotes].country, MAX_COUNTRY_LENGTH, stdin);
        movies[index].votes[movies[index].numVotes].country[strcspn(movies[index].votes[movies[index].numVotes].country, "\n")] = '\0'; // Remove newline character

        // Increment the number of votes for the movie
        movies[index].numVotes++;

        printf("Vote %d added successfully for movie %s (%d).\n", newVote, movies[index].p2name, movieId);
        return 1;
    }
    else
    {
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
    printf("5. Print value\n");
    printf("6. Count countries\n");
    printf("7. Max by country\n");
    printf("8. Recommendations\n");
    printf("9. Delete worst\n");
    printf("0. Exit\n");
}

void saveMoviesToFile(const char* filename, Movie* movies, int size)
{
    FILE* file = safe_open(filename, "wt");
    fprintf(file, "format:m_id,movie_name,Genre,Lead Studio,Year\n");
    for (int i = 0; i < size; i++)
    {
        fprintf(file, "%d,%s,%s,%d\n", movies[i].id, movies[i].p2name, movies[i].studio, movies[i].year);
    }
    fclose(file);
}

void saveVotesToFile(const char* filename, Movie* movies, int size)
{
    FILE* file = safe_open(filename, "wt");
    fprintf(file, "m_id:vote:country:comment\n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < movies[i].numVotes; j++)
        {
            // Check for empty comment
            const char* comment = strcmp(movies[i].votes[j].p2comment, "-") == 0 ? "" : movies[i].votes[j].p2comment;
            fprintf(file, "%d:%d:%s:%s\n", movies[i].id, movies[i].votes[j].value, movies[i].votes[j].country, comment);
        }
    }
    fclose(file);
}


int printVotes(const char* movieName, Movie* movies, int size)
{
    for (int i = 0; i < size; i++) //iterate through array
    {
        if (strcmp(movies[i].p2name, movieName) == 0)  //Compare names
        {
            if (movies[i].numVotes == 0) // If movie has no votes
            {
                printf("No votes for movie %s.\n", movieName);
                return 0;
            }
            printf("Comments for movie %s:\n", movieName);
            for (int j = 0; j < movies[i].numVotes; j++)  //Movie has votes
            {
                printf("Comment: %s\n", movies[i].votes[j].p2comment);
                printf("Country: %s\n", movies[i].votes[j].country);
            }
            return 1;
        }
    }
    // Movie not found in the array
    return -1;
}

int countGenre(const char* genre, Movie* movies, int size)
{
    int found = 0;
    printf("Movies of genre %s:\n", genre);
    for (int i = 0; i < size; i++) //Iterate through the array
    {
        if (strcmp(movies[i].p2genre, genre) == 0) // Find movies with the same genre
        {
            printf("- %s\n", movies[i].p2name);
            found = 1;
        }
    }
    if (!found) // No movies with the genre entered exist in the array
    {
        printf("No movies found for %s genre.\n", genre);
        return -1;
    }
    return found;
}

void printValue(int value, const char* country, Movie* movies, int size) {
    int found = 0;
    printf("Movies with vote value %d from country %s:\n", value, country);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < movies[i].numVotes; j++)
        {
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

int countCountry(int year, Movie* movies, int size)
{
    char countries[MAX_COUNTRIES][MAX_COUNTRY_LENGTH]; // Array to store unique countries
    int numUniqueCountries = 0; // Counter for unique countries

    // Iterate through movies to find votes for the given year
    for (int i = 0; i < size; i++)
    {
        if (movies[i].year == year)
        {
            // Iterate through the votes of the current movie
            for (int j = 0; j < movies[i].numVotes; j++)
            {
                // Check if the country has already been counted
                int isNewCountry = 1;
                for (int k = 0; k < numUniqueCountries; k++)
                {
                    if (strcmp(movies[i].votes[j].country, countries[k]) == 0)
                    {
                        isNewCountry = 0;
                        break;
                    }
                }
                // If it's a new country, add it to the list of unique countries
                if (isNewCountry)
                {
                    strcpy(countries[numUniqueCountries], movies[i].votes[j].country);
                    numUniqueCountries++;
                }
            }
        }
    }


    // Print the number of unique countries
    if (numUniqueCountries == 0)
    {
        printf("No votes for movies from the year %d.\n", year);
    }
    else
    {
        printf("Number of unique countries that voted for movies from the year %d, is %d.\n", year, numUniqueCountries);
    }

    return numUniqueCountries;
}

void maxByCountry(Movie* movies, int size)
{
    // Array to keep track of the number of comments by country
    int commentCount[MAX_COUNTRIES] = { 0 };
    int maxCommentCount = 0; // Maximum comment count
    char countries[MAX_COUNTRIES][MAX_COUNTRY_LENGTH]; // Array to store country names

    // Initialize countries array
    for (int i = 0; i < MAX_COUNTRIES; i++)
    {
        countries[i][0] = '\0'; // Set all country names to empty string
    }

    // Count comments by country
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < movies[i].numVotes; j++)
        {
            if (strcmp(movies[i].votes[j].p2comment, "-") != 0)
            {
                // Find or add the country to the list
                int countryIndex = -1;
                for (int k = 0; k < MAX_COUNTRIES; k++)
                {
                    if (strcmp(countries[k], movies[i].votes[j].country) == 0)
                    {
                        countryIndex = k;
                        break;
                    }
                    else if (countries[k][0] == '\0')
                    {
                        strcpy(countries[k], movies[i].votes[j].country);
                        countryIndex = k;
                        break;
                    }
                }
                // Increment comment count for the country
                if (countryIndex != -1)
                {
                    commentCount[countryIndex]++;
                    // Update maximum comment count
                    if (commentCount[countryIndex] > maxCommentCount)
                    {
                        maxCommentCount = commentCount[countryIndex];
                    }
                }
            }
        }
    }

    // Print countries with maximum comment count
    printf("Countries with the most comments:\n");
    int found = 0;
    for (int i = 0; i < MAX_COUNTRIES; i++)
    {
        if (commentCount[i] == maxCommentCount && countries[i][0] != '\0')
        {
            printf("- %s\n", countries[i]);
            found = 1;
        }
    }

    if (!found)
    {
        printf("No comments found.\n");
    }
}

void RecommendMovie(int X, Movie* movies, int size) {
    FILE* recommendationFile = fopen("Recommendation.txt", "w"); //Open new file for recommendations
    if (recommendationFile == NULL) {
        printf("Error opening file.\n");
        return;
    }
    for (int i = 0; i < size; i++)
    {
        // Initialize for each loop
        int totalVotes = 0;
        int totalScore = 0;
        float average = 0;

        if (movies[i].numVotes == 0) continue; //Skip movies that have no votes

        for (int j = 0; j < movies[i].numVotes; j++)
        {
            totalVotes++;
            totalScore += movies[i].votes[j].value;
        }
        //Calculating average vote rating for each movie
        average = (float)totalScore / totalVotes;

        if (average >= X)
        {
            // write movies with average greater than X to recommendations file
            fprintf(recommendationFile, "%s, %s\n", movies[i].p2name, movies[i].p2genre);
        }
    }

    fclose(recommendationFile);
}

void deleteWorst(const char* genre, Movie* movies, int* size)
{
    int minVote = 10; // Initialize minVote to a high value
    int found = 0; // Flag to check if any movie of the given genre is found

    // Find the minimum vote for movies of the given genre
    for (int i = 0; i < *size; i++) {
        if (strcmp(movies[i].p2genre, genre) == 0)
        {
            found = 1; // At least one movie of the given genre is found
            for (int j = 0; j < movies[i].numVotes; j++)
            {
                if (movies[i].votes[j].value < minVote)
                {
                    minVote = movies[i].votes[j].value;
                }
            }
        }
    }

    if (!found)
    {
        printf("No movies of genre %s found.\n", genre);
        return;
    }

    // Delete all votes with the minimum value for movies of the given genre
    for (int i = 0; i < *size; i++) {
        if (strcmp(movies[i].p2genre, genre) == 0)
        {
            int k = 0; // Index for copying votes without the worst vote
            for (int j = 0; j < movies[i].numVotes; j++)
            {
                if (movies[i].votes[j].value != minVote)
                {
                    movies[i].votes[k++] = movies[i].votes[j];
                }
            }
            movies[i].numVotes = k; // Update the number of votes for the movie
        }
    }

    printf("Votes with the lowest value for genre %s have been deleted.\n", genre);
}

void freeMemory(Movie* movies, int size)
{
    // Free memory for each movie's votes
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < movies[i].numVotes; j++)
        {
            free(movies[i].votes[j].p2comment);
        }
        free(movies[i].votes);
        free(movies[i].p2name);
        free(movies[i].p2genre);
    }

    // Free memory for the array of movies
    free(movies);
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
        while (getchar() != '\n'); // Clear input buffer
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
            fgets(movieName, MAX_TITLE_LENGTH, stdin);
            movieName[strcspn(movieName, "\n")] = '\0';
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

        case 5:
            printf("please enter a vote value (between 1-10): \n");
            char C[MAX_COUNTRY_LENGTH];
            int V;
            do {
                scanf("%d", &V);
                if (V < 1 || V > 10)
                    printf("Please enter a valid vote value: \n");
            } while (V < 1 || V>10);
            while (getchar() != '\n'); // Clear input buffer
            printf("Please enter a country: \n");
            fgets(C, MAX_COUNTRY_LENGTH, stdin);
            C[strcspn(C, "\n")] = '\0';
            printValue(V, C, movies, size);
            break;

        case 6:
            printf("Please enter a year: ");
            int Y;
            do
            {
                scanf("%d", &Y);
                if (Y < 1900 || Y>2100)
                    printf("Please enter a valid year: ");
            } while (Y < 1900 || Y>2100);
            countCountry(Y, movies, size);
            break;

        case 7:
            maxByCountry(movies, size);
            break;

        case 8:
            printf("Please enter a minimum vote value: ");
            int X;
            do {
                scanf("%d", &X);
                if (X < 1 || X>10)
                    printf("Please enter a vaild vote value: ");
            } while (X < 1 || X>10);
            RecommendMovie(X, movies, size);
            break;

        case 9:
            printf("Please enter a genre: ");
            char G[MAX_GENRE_LENGTH];
            scanf("%s", G);
            deleteWorst(G, movies, size);
            break;

        case 0:
            printf("Exiting program...\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);

    saveMoviesToFile(MOVIES, movies, size);
    saveVotesToFile(VOTES, movies, size);
    freeMemory(movies, size);
    return 0;
}
