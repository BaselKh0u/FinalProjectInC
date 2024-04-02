#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_MOVIES 100
#define MAX_GENRE_LENGTH 20 
#define MAX_TITLE_LENGTH 50 
#define MAX_COMMENT_LENGTH 100 
#define MAX_COUNTRY_LENGTH 50
#define MAX_STUDIO_LENGTH 50
#define MAX_VOTES 100
#define MOVIES "moviesData.txt"
#define VOTES "votingData.txt"

typedef struct
{
	int value; // the grade of the reviewer 
	char* p2comment;   // comment 
	char country[MAX_COUNTRY_LENGTH]; // origin country 

} vote;

typedef struct
{
	int id; // the id of the reviewer 
	char* p2name;   // the name of the movie (title)
	char* p2genre;   // the type of the movie genre 
	char studio[MAX_STUDIO_LENGTH];    // studio name
	int year;
	vote* p2list;
	int Nvotes;

} movie;

// Function to count the Lines in a text file
int countLines(const char* filename)
{
	int count = 0;
	char buffer[MAX_TITLE_LENGTH + MAX_GENRE_LENGTH + MAX_COUNTRY_LENGTH + MAX_STUDIO_LENGTH + MAX_COMMENT_LENGTH];
	FILE* file_ptr;
	if (fopen_s(&file_ptr, filename, "r") == 0)
	{
		while (fgets(buffer, sizeof(buffer), file_ptr) != NULL)
		{
			count++;
		}
		fclose(file_ptr);
	}
	else {
		printf("%s: File opening failed!\n", filename);
		exit(EXIT_FAILURE);
	}

	return count;
}

// Function receives the name of the movie file, a pointer to the movie array
// and its size. The function will reads from the file all the data of the movies and fill the array.
int FromFile2Movies(const char* filename, movie** movies, const int size)
{
	*movies = (movie*)malloc(sizeof(movie) * size);
	if (movies == NULL)
	{
		printf("Failed to allocate memory.\n");
		exit(EXIT_FAILURE);
	}
	char* field;
	char* next_field;
	FILE* movie_file;
	char buffer[MAX_TITLE_LENGTH + MAX_GENRE_LENGTH + MAX_COUNTRY_LENGTH + MAX_STUDIO_LENGTH + MAX_COMMENT_LENGTH];
	int counter = 0;

	if (fopen_s(&movie_file, filename, "r") == 0)
	{
		fgets(buffer, sizeof(buffer), movie_file);
		// printf("IGNORE: %s\n", buffer);
		while (fgets(buffer, sizeof(buffer), movie_file) && counter < size)
		{
			(*movies)[counter].Nvotes = 0;
			(*movies)[counter].p2list = NULL;

			field = strtok_s(buffer, ",", &next_field);
			(*movies)[counter].id = atoi(field);

			field = strtok_s(NULL, ",", &next_field);
			(*movies)[counter].p2name = (char*)malloc(MAX_TITLE_LENGTH);
			strcpy_s((*movies)[counter].p2name, MAX_TITLE_LENGTH, field);

			field = strtok_s(NULL, ",", &next_field);
			(*movies)[counter].p2genre = (char*)malloc(MAX_GENRE_LENGTH);
			strcpy_s((*movies)[counter].p2genre, MAX_GENRE_LENGTH, field);

			field = strtok_s(NULL, ",", &next_field);
			strcpy_s((*movies)[counter].studio, MAX_STUDIO_LENGTH, field);

			field = strtok_s(NULL, "\n", &next_field);
			(*movies)[counter].year = atoi(field);

			counter++;
		}
		fclose(movie_file);
	}
	else
	{
		printf("%s: File opening failed!\n", filename);
		exit(EXIT_FAILURE);
	}

	return counter;
}

// Function receives the name of the viewer votes file, The array of films
// and its size.The function reads from the file and fill in the voting lists for each film
int FromFile2Votes(const char* filename, movie** movies, const int size)
{
	char* field;
	char* next_field;
	char buffer[MAX_GENRE_LENGTH + MAX_COUNTRY_LENGTH + MAX_COMMENT_LENGTH];
	int counter = 0;
	FILE* votes_file;
	if (fopen_s(&votes_file, filename, "r") == 0)
	{
		fgets(buffer, sizeof(buffer), votes_file);
		// printf("IGNORE: %s\n", buffer);
		while (fgets(buffer, sizeof(buffer), votes_file))
		{
			int mid = -1;
			vote new_vote;
			field = strtok_s(buffer, ":", &next_field);
			mid = atoi(field);

			field = strtok_s(NULL, ":", &next_field);
			new_vote.value = atoi(field);

			field = strtok_s(NULL, ":", &next_field);
			strcpy_s(new_vote.country, 15, field);

			field = strtok_s(NULL, "\n", &next_field);
			new_vote.p2comment = (char*)malloc(MAX_COMMENT_LENGTH);
			strcpy_s(new_vote.p2comment, MAX_COMMENT_LENGTH, field);

			for (size_t i = 0; i < size; i++)
			{
				if ((*movies)[i].id == mid)
				{
					int nv = (*movies)[i].Nvotes;
					vote* new_votes = realloc((*movies)[i].p2list, (nv + 1) * sizeof(vote));
					new_votes[nv].value = new_vote.value;
					new_votes[nv].p2comment = malloc(MAX_COMMENT_LENGTH);
					strcpy_s(new_votes[nv].p2comment, MAX_COMMENT_LENGTH, new_vote.p2comment);
					strcpy_s(new_votes[nv].country, MAX_COUNTRY_LENGTH, new_vote.country);
					(*movies)[i].p2list = new_votes;
					(*movies)[i].Nvotes++;
				}
			}
			counter++;
		}
		fclose(votes_file);
	}
	else {
		printf("%s: File opening failed!\n", filename);
		exit(EXIT_FAILURE);
	}

	return counter;
}

// Function receives the array of movies and its size. The function take the movie data
// from the user without the list of viewer votes and add the The film for the array.
int addMovie(movie** movies, const int size)
{

	char name[MAX_TITLE_LENGTH], genre[MAX_GENRE_LENGTH], studio[MAX_STUDIO_LENGTH];
	int year = 0;

	while (getchar() != '\n');
	printf("Enter movie name: ");
	scanf_s("%99[^\n]", name, (unsigned)_countof(name));

	while (getchar() != '\n');
	printf("Enter movie genre: ");
	scanf_s("%99[^\n]", genre, (unsigned)_countof(genre));

	while (getchar() != '\n');
	printf("Enter studio name: ");
	scanf_s("%99[^\n]", studio, (unsigned)_countof(studio));

	int valid = 0;
	do {
		printf("Enter year: ");
		if (scanf_s("%d", &year) == 1)
			valid = 1;
		else {
			while (getchar() != '\n');
			printf("Invalid input. Please enter a valid number.\n");
		}
	} while (!valid);
	valid = 0;

	for (size_t i = 0; i < size; i++)
	{
		if (strcmp((*movies)[i].p2name, name) == 0)
		{
			return 0;
		}
	}

	movie* new_movies = realloc(*movies, (size + 1) * sizeof(movie));
	if (new_movies == NULL)
	{
		printf("Failed to allocate memory.\n");
		exit(EXIT_FAILURE);
	}
	new_movies[size].p2name = (char*)malloc(MAX_TITLE_LENGTH);
	new_movies[size].p2genre = (char*)malloc(MAX_GENRE_LENGTH);
	if (new_movies[size].p2genre == NULL || new_movies[size].p2name == NULL)
	{
		printf("Failed to allocate memory.\n");
		exit(EXIT_FAILURE);
	}

	new_movies[size].id = size + 1;
	strcpy_s(new_movies[size].p2name, MAX_TITLE_LENGTH, name);
	strcpy_s(new_movies[size].p2genre, MAX_GENRE_LENGTH, genre);
	strcpy_s(new_movies[size].studio, MAX_STUDIO_LENGTH, studio);
	new_movies[size].year = year;
	new_movies[size].Nvotes = 0;
	new_movies[size].p2list = NULL;

	*movies = new_movies;
	return 1;
}

// Function receives the id of the movie, the array of movies and its size.
// The function receives the vote data for the movie from the user and add it to the vote list for this movie.
int addVote(int mid, movie** movies, const int size)
{
	char country[15], comment[100];
	int value = 0, valid = 0;

	do {
		printf("Enter Vote Value: ");
		if (scanf_s("%d", &value) == 1)
			valid = 1;
		else {
			while (getchar() != '\n');
			printf("Invalid input. Please enter a valid number.\n");
		}
	} while (!valid);
	valid = 0;

	printf("Enter country: ");
	scanf_s("%s", country, (unsigned)_countof(country));

	while (getchar() != '\n');
	printf("Enter comment: ");
	scanf_s("%99[^\n]", comment, (unsigned)_countof(comment));

	int idx = -1;
	for (size_t i = 0; i < size; i++)
	{
		if ((*movies)[i].id == mid)
		{
			idx = i;
			for (size_t j = 0; j < (*movies)[i].Nvotes; j++)
			{
				if ((*movies)[i].p2list[j].value == value && (strcmp((*movies)[i].p2list[j].country, country) == 0) && (strcmp((*movies)[i].p2list[j].p2comment, comment) == 0))
				{
					return 0;
				}
			}
			break;
		}
	}

	if (idx == -1)
	{
		return 0;
	}

	int nv = (*movies)[idx].Nvotes;
	vote* new_votes = realloc((*movies)[idx].p2list, (nv + 1) * sizeof(vote));
	if (new_votes == NULL)
	{
		printf("Failed to allocate memory.\n");
		exit(EXIT_FAILURE);
	}
	new_votes[nv].value = value;
	new_votes[nv].p2comment = malloc(MAX_COMMENT_LENGTH);
	strcpy_s(new_votes[nv].p2comment, MAX_COMMENT_LENGTH, comment);
	strcpy_s(new_votes[nv].country, MAX_COUNTRY_LENGTH, country);
	(*movies)[idx].p2list = new_votes;
	(*movies)[idx].Nvotes++;
	return 1;
}

// Function gets the name of the movie, the array of movies and its size, and prints all the comments
// and the countries of the film.
// return 0 if the list of viewer votes is empty, return -1 if the movie does not exist. return 1 if votes found.
int printVotes(const char* name, movie** movies, const int size)
{
	for (int i = 0; i < size; i++)
	{
		if (strcmp((*movies)[i].p2name, name) == 0)
		{
			if ((*movies)[i].Nvotes == 0)
			{
				return 0;
			}
			for (int j = 0; j < (*movies)[i].Nvotes; j++)
			{
				printf("%d, %s, %s\n", (*movies)[i].p2list[j].value, (*movies)[i].p2list[j].country, (*movies)[i].p2list[j].p2comment);
			}
			return 1;
		}
	}
	return -1;
}

// Function gets the movie of a given genre, the movie array, and its size.
// The function prints the names of the movies of the given genre. If there are no movies of genre in the movie array, 
int countGenre(const char* genre, movie** movies, const int size)
{
	int count = 0;
	for (int i = 0; i < size; i++)
	{
		if (strcmp((*movies)[i].p2genre, genre) == 0)
		{
			printf("%s\n", (*movies)[i].p2name);
			count++;
		}
	}
	return count;
}

// Function receives the value of the voting score, country, the movie array and its size.
// The function will print the names of all movies that have received a vote from any viewer in country for the movie.
int printValue(const int score, const char* country, movie** movies, const int size)
{
	int count = 0;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < (*movies)[i].Nvotes; j++)
		{
			if (strcmp((*movies)[i].p2list[j].country, country) == 0 && (*movies)[i].p2list[j].value == score)
			{
				printf("%s\n", (*movies)[i].p2name);
				count++;
			}

		}
	}
	return count;
}

// Function receives year, the array of movies and its size, 
// and prints the number of different countries that voted for some movie from year.
// A country should only be counted once.
int countCountry(const int year, movie** movies, const int size)
{
	int count = 0;
	char countries[500][15];
	for (int i = 0; i < size; i++)
	{
		if ((*movies)[i].year == year)
		{
			for (int j = 0; j < (*movies)[i].Nvotes; j++)
			{
				int found = 0;
				for (int k = 0; k < count; k++)
				{
					if (strcmp((*movies)[i].p2list[j].country, countries[k]) == 0)
					{
						found = 1;
						break;
					}
				}
				if (found == 0)
				{
					strcpy_s(countries[count], 15, (*movies)[i].p2list[j].country);
					count++;
				}
			}
		}
	}
	return count;
}

// Function gets the movies array and its size.
// The function will print the name of the country whose viewers wrote the most comments (an empty comment is not counted).
// Incase there are several such countries, the names of all countries are printed.
int maxByCountry(movie** movies, const int size)
{
	int count = 0;
	char countries[500][MAX_COUNTRY_LENGTH];
	int counter[500];
	for (int i = 0; i < 500; i++)
		counter[i] = 0;

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < (*movies)[i].Nvotes; j++)
		{
			if (strcmp((*movies)[i].p2list[j].p2comment, "-") != 0)
			{
				int found = -1;
				for (int k = 0; k < count; k++)
				{
					if (strcmp((*movies)[i].p2list[j].country, countries[k]) == 0)
					{
						found = k;
						break;
					}
				}
				if (found == -1)
				{
					counter[count]++;
					strcpy_s(countries[count], 15, (*movies)[i].p2list[j].country);
					count++;
				}
				else
				{
					counter[found]++;
				}
			}
		}
	}

	int max = 0;
	for (int i = 0; i < 500; i++)
	{
		if (counter[i] > max)
		{
			max = counter[i];
		}
	}

	for (int i = 0; i < 500; i++)
	{
		if (counter[i] == max)
		{
			printf("%s\n", countries[i]);
		}
	}
	return max;
}

// Function gets a vote value, the array of movies and its size. 
// The function will create a file called Recommendation.txt into which the names of 
// the movies and its genre will be written of which the average vote is large than average.
void RecommendMovie(const int vote, movie** movies, const int size)
{
	FILE* file_recommend;
	int res = fopen_s(&file_recommend, "Recommendation.txt", "w");
	if (res == 0)
	{
		fprintf(file_recommend, "%s\n", "movie_name,Genre");
		float average = 0;
		for (int i = 0; i < size; i++)
		{
			average = 0;
			for (int j = 0; j < (*movies)[i].Nvotes; j++)
			{
				average += (*movies)[i].p2list[j].value;
			}
			average /= (*movies)[i].Nvotes;
			if (average > vote)
			{
				fprintf(file_recommend, "%s, %s\n", (*movies)[i].p2name, (*movies)[i].p2genre);
			}
		}
		fclose(file_recommend);
	}
	else
	{
		printf("%s: File opening failed!\n", "Recommendation.txt");
		exit(EXIT_FAILURE);
	}
}

// Function gets a movie genre, the movie array and its size. 
// The function deletes the vote with the lowest value for movie genre.
// If there are several of them, all of them are deleted from the database.
int deleteWorst(const char* genre, movie** movies, const int size)
{
	int found = 0;
	int worst = 9999;
	for (int i = 0; i < size; i++)
	{
		if (strcmp((*movies)[i].p2genre, genre) == 0)
		{
			for (int j = 0; j < (*movies)[i].Nvotes; j++)
			{
				if ((*movies)[i].p2list[j].value < worst)
				{
					worst = (*movies)[i].p2list[j].value;
				}
			}
		}
	}

	for (int i = 0; i < size; i++)
	{
		if (strcmp((*movies)[i].p2genre, genre) == 0)
		{
			for (int j = 0; j < (*movies)[i].Nvotes; j++)
			{
				if ((*movies)[i].p2list[j].value == worst)
				{
					found = 1;
					//printf("Deleted WORST %d, %s\n", worst, (*movies)[i].p2list[j].p2comment);
					for (int k = j; k < (*movies)[i].Nvotes - 1; k++)
					{
						(*movies)[i].p2list[k].value = (*movies)[i].p2list[k + 1].value;
						strcpy((*movies)[i].p2list[k].p2comment, (*movies)[i].p2list[k + 1].p2comment);
						strcpy((*movies)[i].p2list[k].country, (*movies)[i].p2list[k + 1].country);

					}
					(*movies)[i].Nvotes--;
					j--;
				}
			}
		}
	}

	return found;
}

// Function gets a filename for movies database, filename for voting database, the movie array and its size. 
// Function write the updated the values from the array to the respective files
void updateMoviesNVotes(const char* filemovies, const char* filevotes, movie** movies, const int size)
{
	FILE* file_movies;
	FILE* file_votes;
	int res1 = fopen_s(&file_movies, filemovies, "w");
	if (res1 != 0)
	{
		printf("%s: File opening failed!\n", filemovies);
		exit(EXIT_FAILURE);
	}

	int res2 = fopen_s(&file_votes, filevotes, "w");
	if (res2 != 0)
	{
		printf("%s: File opening failed!\n", filevotes);
		exit(EXIT_FAILURE);
	}

	fprintf(file_movies, "%s\n", "format:m_id,movie_name,Genre,Lead Studio,Year");
	fprintf(file_votes, "%s\n", "m_id:vote:coutry:comment //- means an empty comment");

	for (int i = 0; i < size; i++)
	{
		fprintf(file_movies, "%d,%s,%s,%s,%d\n", (*movies)[i].id, (*movies)[i].p2name, (*movies)[i].p2genre, (*movies)[i].studio, (*movies)[i].year);
		for (int j = 0; j < (*movies)[i].Nvotes; j++)
		{
			fprintf(file_votes, "%d:%d:%s:%s\n", (*movies)[i].id, (*movies)[i].p2list[j].value, (*movies)[i].p2list[j].country, (*movies)[i].p2list[j].p2comment);
		}
	}
	fclose(file_movies);
	fclose(file_votes);
}

// Function that will display a menu containing all the functions that are defined for operations
// The function also gives option to Type 0 to end the program.
void printMenu(movie** movies, int* size)
{
	int res = 0, opt = 0, valid = 0;
	do
	{
		printf("1 - Add Movies\n");
		printf("2 - Add Vote\n");
		printf("3 - Print Votes\n");
		printf("4 - Count Genre\n");
		printf("5 - Print Value\n");
		printf("6 - Count Country\n");
		printf("7 - Max by Country\n");
		printf("8 - Recommended Movie\n");
		printf("9 - Delete Worst\n");
		printf("0 - EXIT\n");

		do {
			printf("Enter choice: ");
			if (scanf_s("%d", &opt) == 1)
				valid = 1;
			else {
				while (getchar() != '\n');
				printf("Invalid input. Please enter a valid number.\n");
			}
		} while (!valid);
		valid = 0;

		if (opt == 1)
		{
			res = addMovie(movies, *size);
			if (res == 1)
			{
				(*size)++;
			}
			else if (res == 0)
			{
				printf("Movie already exists!\n");
			}
		}
		else if (opt == 2)
		{
			int mid;
			do {
				printf("Enter Movie ID: ");
				if (scanf_s("%d", &mid) == 1)
					valid = 1;
				else {
					while (getchar() != '\n');
					printf("Invalid input. Please enter a valid number.\n");
				}
			} while (!valid);
			valid = 0;

			addVote(mid, movies, *size);
		}
		else if (opt == 3)
		{
			char name[MAX_TITLE_LENGTH];
			while (getchar() != '\n');
			printf("Enter Movie Name: ");
			scanf_s("%99[^\n]", name, (unsigned)_countof(name));
			res = printVotes(name, movies, *size);
			if (res == 0)
			{
				printf("No votes are found for movie %s\n", name);
			}
			else if (res == -1)
			{
				printf("Movie %s not found!\n", name);
			}
		}
		else if (opt == 4)
		{
			char genre[MAX_GENRE_LENGTH];
			while (getchar() != '\n');
			printf("Enter Movie Genre: ");
			scanf_s("%99[^\n]", genre, (unsigned)_countof(genre));
			if (countGenre(genre, movies, *size) == 0)
			{
				printf("No movie for genre %s found!\n", genre);
			}
		}
		else if (opt == 5)
		{
			char country[MAX_COUNTRY_LENGTH];
			int value;
			do {
				printf("Enter Vote Value: ");
				if (scanf_s("%d", &value) == 1)
					valid = 1;
				else {
					while (getchar() != '\n');
					printf("Invalid input. Please enter a valid number.\n");
				}
			} while (!valid);
			valid = 0;
			printf("Enter Country: ");
			scanf_s("%s", country, (unsigned)_countof(country));
			if (printValue(value, country, movies, *size) == 0)
			{
				printf("No movie have %d votes from country %s!\n", value, country);
			}
		}
		else if (opt == 6)
		{
			int year;
			do {
				printf("Enter Year: ");
				if (scanf_s("%d", &year) == 1)
					valid = 1;
				else {
					while (getchar() != '\n');
					printf("Invalid input. Please enter a valid number.\n");
				}
			} while (!valid);
			valid = 0;
			res = countCountry(year, movies, *size);
			if (res == 0)
			{
				printf("No movies have been votes by any country in year %d!\n", year);
			}
			else
			{
				printf("%d countries have voted movies in year %d\n", res, year);
			}
		}
		else if (opt == 7)
		{
			res = maxByCountry(movies, *size);
			printf("Max votes by country are %d\n", res);
		}
		else if (opt == 8)
		{
			int value;
			do {
				printf("Enter Vote Value: ");
				if (scanf_s("%d", &value) == 1)
					valid = 1;
				else {
					while (getchar() != '\n');
					printf("Invalid input. Please enter a valid number.\n");
				}
			} while (!valid);
			valid = 0;
			RecommendMovie(value, movies, *size);
		}
		else if (opt == 9)
		{
			char genre[MAX_GENRE_LENGTH];
			while (getchar() != '\n');
			printf("Enter Movie Genre: ");
			scanf_s("%99[^\n]", genre, (unsigned)_countof(genre));
			if (deleteWorst(genre, movies, *size) == 0)
			{
				printf("No movie for genre %s found!\n", genre);
			}
		}
		printf("\n");
	} while (opt != 0);
}

// Main Driver
// Load Movies, Votings and Print Menus
// After the operatons write updated values to files
// Free any allocated memory
int main()
{
	movie* movies = NULL;
	int Nmovies = FromFile2Movies(MOVIES, &movies, countLines(MOVIES));
	int Nvotes = 0;

	if (movies != NULL)
	{
		Nvotes = FromFile2Votes(VOTES, &movies, Nmovies);
	}

	printMenu(&movies, &Nmovies);

	updateMoviesNVotes(MOVIES, VOTES, &movies, Nmovies);

	for (int i = 0; i < Nmovies; i++)
	{
		for (int j = 0; j < movies[i].Nvotes; j++)
		{
			free(movies[i].p2list[j].p2comment);
		}
		free(movies[i].p2name);
		free(movies[i].p2genre);
		free(movies[i].p2list);
	}
	free(movies);
	return 0;
}
