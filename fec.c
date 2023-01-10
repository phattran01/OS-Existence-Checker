#include <stdio.h> // Include the standard I/O library
#include <stdlib.h> // Include the standard library
#include <string.h> // Include the string library
#include <sys/stat.h> // Include the system-specific library for file attributes
#include <sys/mman.h> // Include the system-specific library for memory mapping files
#include <fcntl.h> // Include the library for manipulating file descriptors
#include <pthread.h> // Include the library for creating and managing threads

void loadFiles(int argc,char** argv); // Declare the loadFiles() function
void driver(); // Declare the driver() function
void threadSearch(void* args); // Declare the threadSearch() function

const int combinedSize = 3000; // Declare a constant integer variable for the combined file size

int currentThread = 0; // Declare a variable to keep track of the current thread number
char* key = "11111111"; // Declare a variable for the search key
int flag = 0; // Declare a variable to indicate whether the key was found

void loadFiles(int argc, char** argv) // Define the loadFiles() function
{
	int size; // Declare a variable for the file size
	char* buff = malloc(combinedSize); // Allocate memory for the buffer
	int numberOfFiles = argc-1; // Calculate the number of files passed as arguments to the program
	char* files[argc]; // Declare an array to hold the memory-mapped files
	char* file; // Declare a variable to hold a single mapped file
	FILE* fdCombined = fopen("combined.txt", "a"); // Open the "combined.txt" file for writing

	// Iterate over the files passed as arguments to the program
	for(int i = 0; i < numberOfFiles; i++)
	{
		struct stat bufferStat; // Declare a stat object to hold information about the file
		char* file_name = argv[i+1]; // Get the name of the current file
		int fd = open(argv[i+1], O_RDONLY); // Open the current file for reading

		// Get the size of the current file
		int status = fstat(fd, &bufferStat);
		size = bufferStat.st_size;

		// Map the current file in memory
		file = (char*) mmap(0, size, PROT_READ, MAP_PRIVATE, fd, 0);
		files[i] = file; // Add the mapped file to the array

		// Write the mapped file to the "combined.txt" file
		fputs(file, fdCombined);
	}
	
	// Close the "combined.txt" file
	fclose(fdCombined);

	// Re-open the "combined.txt" file for reading
	fdCombined = fopen("combined.txt", "r");

	// Check for file errors
	if(ferror(fdCombined) != 0)
	{
		printf("File error");
	}

	// Close the "combined.txt" file
	fclose(fdCombined);
}

void driver() // Define the driver() function
{
	// Get the number of available processors
	int numberOfAvailableProcessors = get_nprocs();
	// Create an array of threads
	pthread_t threads[numberOfAvailableProcessors];
	
	for(int i = 0; i < numberOfAvailableProcessors; i++)
	{
		// Create a new thread
		pthread_create(&threads[i], NULL, threadSearch, (void*) NULL);
	}
	
	// Iterate over the threads
	for(int j = 0; j < numberOfAvailableProcessors; j++)
	{
		// Wait for the thread to finish execution
		pthread_join(threads[j], NULL);
	}

	// Check if the key was found
	if(flag==1)
	{
		printf("found\n"); // Print "found" if the key was found
	}
	else
	{
		printf("not found\n"); // Print "not found" if the key was not found
	}

	// Remove the "combined.txt" file
	remove("combined.txt");
}

void threadSearch(void* args) // Define the threadSearch() function
{
	// Open the "combined.txt" file for reading
	FILE* file = fopen("combined.txt", "r");
	// Declare variables for the file size and the buffer
	int fileSize = 1000;
	char* numbers[fileSize];
	char buffer[fileSize];

	// Declare a variable for the current token
	char* token;
	int i = 0;

	// Read lines from the file until the end is reached
	while(fgets(buffer, (sizeof(char)*fileSize)+1, file) != NULL)
	{
		// Extract the tokens from the current line
		token = strtok(buffer, ",");
		while(token!=NULL)
		{
			// Add the token to the array
			numbers[i]=token;

			// Get the next token
			token = strtok(NULL, ",");
			i++;
		}
	}

	// Increment the current thread number
	int n = currentThread++;

	// Iterate over the numbers in the current thread's range
	for(int j = n * (fileSize / 4); j < ((n + 1) * (fileSize / 4)); j++)
	{
		// Check if the key has already been found
		if (flag == 0)
		{
			// Check if the current number is the key
			if(strcmp(numbers[j],key) == 0)
			{
				// Set the flag if the key was found
				flag = 1;
			}
		}
	}
	
	// Close the "combined.txt" file
	fclose(file);
}
	
int main(int argc, char** argv)
{
	loadFiles(argc, argv);
	driver();
	
	if(flag == 1) 
	{
		
		//printf(flag);	
	}
	
	return 0;
}
	
	
	
	

	
