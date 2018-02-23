/* PROGRESS
Part 1 - Validation
Goal: To validate string input
Checklist:
+ If 1 argument (only program name), display usage
+ If 2 arguments and is --usage, display usage
+ If 2 arguments and is --version, display version
+ If 3 arguments and is --hexdump [FILE], run hexdump
+ If 4 arguments and is --hexdump < [FILE], run hexdump with argument
+ Else, display error message accordingly

String: char {name}[] = "{filename};"
*/

#include <stdio.h>
#include <string.h> // dea;s with strings stored in character's arrays. eg: strlen, strcmp

//declaring functions
void usage(); //instructions
void runHexDump(); //hex values
void runHexStdin(); //hex with stdin

double valVersion = 1; //sets version value; at current version, set to 1
//changes version if necessary

//-----------------------------------------------

int main(int argc, char *argv[])
{
    int argCount = argc; //argc is the number of input arguments
    
    if (argCount == 2)
	{ // if there are two arguments - but in fact ./pndump.exe "other argument comes here"
        // ./pndump.exe counts as an argument
        if ((strcmp(argv[1], "--usage")) == 0)  //checks whether the argument is "--usage"
			usage(argv[0], stdout); //runs usage
		else if ((strcmp(argv[1], "--version")) == 0) //checks whether the argument is "--version"
			fprintf(stdout, "%.01f\n", valVersion); //runs version
		else if ((strcmp(argv[1], "--hexdump")) == 0) //checks whether the argument is "--hexdump"
			runHexStdin();
    }
	//no need of curly brakets in this case as there was only one line inside each if/else if statement
    
	//checks if there are 3 arguments and if the 2nd argument is equals --hexdump (by using strcmp)
    else if ((argCount == 3) && (strcmp(argv[1], "--hexdump") == 0))
	{ // "==" is for comparison
		FILE* fileInput = fopen(argv[2], "rb"); /*checks if file is readable, or exists, or otherwise
		if file doesn't exist or cannot be read, return an error */
		if(fileInput != NULL)
		{
			// null has zero value. so it is checking whether the file is empty or not
			runHexDump(argv[2]);//If file exists and is readable, run normal hexdump
		}			
		else
		{
			fprintf(stderr, "No such file: \"%s\"\n", argv[2]); //returns error formatted to specs
			return 1; //exits with code 1
		}
	}
	else
	{
		fprintf(stderr, "pnmdump: bad arguments\n"); //returns error message
		usage(argv[0], stderr);
		return 1;
	}
	return 0; // end of main
}



void usage(char programName[], FILE *channel) //Only prints instructions, therefore there is no need to return any value
//therefore we use VOID instead of any other data type.
{
	fprintf(channel, "Usage:\n");
	//If location not needed: printf("./pnmdump.exe --[command]\n");
	//NB: Arguments' array-pointers do not include dividing space
	fprintf(channel, "%s --version\n", programName);
	fprintf(channel, "%s --usage\n", programName);
	fprintf(channel, "%s --hexdump [FILE]\n", programName);
}



void runHexDump(char* dirFileIn) //takes from called original dump
{
    FILE* fileRead = fopen(dirFileIn, "rb"); //reads as binary file
	//No need for validation here - it was already done in main
	/*IDEA: In a file, process every 8 bytes, including remainer [=8n+R]*/
	
	int bytesRead; //indicates the size of blocks to read
	unsigned char chunk[8]; //allocates size of blocks to read, in chunks of 8 bytes
	int totalSize = 0; //Counts total size of file
	
    for(;;){ // infinite loop
		bytesRead = fread(chunk, 1, 8, fileRead); //Finds out if length is big enough
		
		if ((bytesRead < 8) && (bytesRead <= 0))
			break; //loop is immediately terminated
        
		fprintf(stdout, "%07x", totalSize); //prints current size
		totalSize = totalSize + bytesRead; //increments to remainder if incomplete			
		//bytesRead is either = 8 or < 8
        
		for(int count=0; count < bytesRead; count++) //checks if character in chunk is valid
		{
			fprintf(stdout, "  ");
			fprintf(stdout, "%02X ", chunk[count]); //prints hex value of char
			if (((int)chunk[count] >= 32) && ((int)chunk[count] <= 126)) //invalid character replaced with '.'
				putc(chunk[count], stdout); //prints char
			else
				putc('.', stdout);
		}
		fprintf(stdout, "\n");
		if (bytesRead < 8)
			break;
	}
	fprintf(stdout, "%07x\n", totalSize);
	fclose(fileRead); //close IO reader
}



void runHexStdin(){// same as runHexDump function, but works for stdin (no parameter-passing is available)
	int bytesRead; //indicates the size of blocks to read
	unsigned char chunk[8]; //allocates size of blocks to read, in chunks of 8 bytes
	int totalSize = 0; //Counts total size of file
	if (stdin == NULL)
		fprintf(stderr, "Enter valid input\n"); //returns error message
	else{
		for(;;)
		{ //infinite loop
			bytesRead = fread(chunk, 1, 8, stdin); // chunks of 8 bytes
			
			if ((bytesRead < 8) && (bytesRead <= 0)) //if chunk read at end has less than 8 bytes, terminate
                break;
			fprintf(stdout, "%07x", totalSize); //prints current size
			totalSize = totalSize + bytesRead; //increments to remainder if incomplete			
			//bytesRead is either = 8 or < 8
			for(int count=0; count < bytesRead; count++) //checks if character in chunk is valid
			{
				fprintf(stdout, "  ");
				fprintf(stdout, "%02X ", chunk[count]); //prints hex value of char
				if (((int)chunk[count] >= 32) && ((int)chunk[count] <= 126)) //invalid character replaced with '.'
					putc(chunk[count], stdout); //prints char
				else
					putc('.', stdout);
			}
			fprintf(stdout, "\n");
			if (bytesRead < 8)
                break;
		}
	fprintf(stdout, "%07x\n", totalSize);
	fclose(stdin); //close IO reader
	}
}
