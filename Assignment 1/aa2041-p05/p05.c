
// Ashwin Anand RUID: 192007894 Net Id: aa2041

// Packages needed for program to run
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>

// Function called up
int OccurencesCountFunction(char* PassedFileText, char* FindingSubString);

// main method of program
int main(int argc,char* argv[])
{
    // file pointer created
    FILE* filepointer;

    // file pointer assigned value of argument 1 passed into and is going to be read ("r")
    filepointer = fopen(argv[1],"r");

    // file pointer value check
    if(filepointer == NULL)
    {

        // error message printing if filepointer has nothing in it because nothing has been passed in
        perror("./p05");
        return -1;

    }

    // checking if system call is mentioned in command line instruction
    int systemcallbooleanchecknum = 0 ;


    // for loop to go through arguments to check for "--systemcall"
    for(int a = 0; a < argc ; a++)
    {

        if(strstr(argv[a],"--systemcalls"))

            // assigned value of 1 as system call is specified in command line
            systemcallbooleanchecknum = 1;

    }


         // if loop if the argument count is only 3 variables and not more or less
    if(argc == 3)
    {

        // moves file pointer to end of the file after any data is done with a 0 offset
        fseek(filepointer,0, SEEK_END);

        // file size contains the file pointer position in a file
        int filesize = ftell(filepointer);

        // moves file pointer to end of the file after any data is done with a 0 offset
        fseek(filepointer,0,SEEK_SET);

        // String on file which well help calculate file size which is used in fread function in later lines of code
        char* StringOfFile = (char*)malloc(filesize*sizeof(char) + 1);

        // reads the information from the file from the file pointer
        fread(StringOfFile, 1,filesize,filepointer); // 1

        // setting the filestring to a char of 0
        StringOfFile[filesize] = '\0';

        // Occurence count will keep on getting updated and printing out value till end of data
        int OccurenceCountReturned = OccurencesCountFunction(StringOfFile,argv[2]);

        // printing the occurence count
        printf("%d\n", OccurenceCountReturned);

        return 0;

    }

    // if loop that will go through if there are more than 4 arguments and system call is not there
    if(argc >= 4 && strcmp(argv[2],"--systemcalls")!=0 )
    {
        // moves file pointer to end of the file after any data is done with a 0 offset
        fseek(filepointer,0, SEEK_END);

        // file size contains the file pointer position in a file
        int filesize = ftell(filepointer);

         // moves file pointer to end of the file after any data is done with a 0 offset
        fseek(filepointer,0,SEEK_SET);

        // String on file which well help calculate file size which is used in fread function in later lines of code
        char* StringOfFile = (char*)malloc(filesize*sizeof(char) + 1);

        // reads the information from the file from the file pointer
        fread(StringOfFile, 1,filesize,filepointer); // 1

        // setting the filestring to a char of 0
        StringOfFile[filesize] = '\0';

        // argumentcount number that should be accessed
        int argumentcount = 2;

        // loop will keep on going from argv[2] till the value is null at the end of data
        while(argv[argumentcount] != NULL)
        {

            // Occurence count will keep on getting updated and printing out value till end of data
            int OccurenceCountReturned = OccurencesCountFunction(StringOfFile,argv[argumentcount]);

            // printing the occurence count
            printf("%d\n", OccurenceCountReturned);

            // argument counter increment +1 for every loop
            argumentcount++;
        }

        return 0;

    }

    // if loop that will go through if there are more than 4 arguments and system call is there
    if(argc >= 4 && strcmp(argv[2],"--systemcalls")==0)
    {

        // text file that will be opened and be read only
        int filedirectory = open(argv[1],O_RDONLY);

        // buffer size f
        int buffersize = 1000;

        // String on file which well help calculate file size which is used in read function in later lines of code
        char* StringOfFile = (char*)malloc(buffersize*sizeof(char));

        // reads the information from the file
        read(filedirectory,StringOfFile,1000);

        // print statement to print string of file
        printf("%s\n",StringOfFile);

        // argumentcount number that should be accessed
        int argumentcount = 3;

         // loop will keep on going from argv[3] till the value is null at the end of data
        while(argv[argumentcount] != NULL){

            // Occurence count will keep on getting updated and printing out value till end of data
            int OccurenceCountReturned = OccurencesCountFunction(StringOfFile,argv[argumentcount]);

            // printing the occurence count
            printf("%d\n", OccurenceCountReturned);

            // argument counter increment +1 for every loop
            argumentcount++;
        }

        // free function will delete the memory of File text
        free(StringOfFile);

        // close's text file
        close(filedirectory);

        return 0;

    }



}

/* OccurenceCountFunction will count the occurences of the findingsubstring through the passedfiletext and returns an int of occurences found */
int OccurencesCountFunction(char* PassedFileText, char* FindingSubString)
{

    // Occurence Count value
    int OccurencesCounted = 0;

    // length of FindingSubString
    int FindingSubstringLength =  strlen(FindingSubString);
    //printf("%d",FindingSubstringLength);

    // length of PassedFileText
    int PassedFileTextLength = strlen(PassedFileText);
    // printf("%d",PassedFileTextLength);

    // char of the length of PassedFileText
    char UpperCasePassedFile[PassedFileTextLength];

    // char of the length of FindingSubString
    char UpperCaseFindingSubstring[FindingSubstringLength];

    // for loop to make all the the letters in findingsubstring uppercase to not have case sensitive issues
    for(int a = 0 ;FindingSubString[a]; a++)
    {

        UpperCaseFindingSubstring[a] = toupper(FindingSubString[a]);

    }


    // for loop to make all the the letters in passedfiletext uppercase to not have case sensitive issues
    for(int b = 0; PassedFileText[b]; b++)
    {

        UpperCasePassedFile[b] = toupper(PassedFileText[b]);

    }

    // for loop to compare the chars between pasedfile and substringfile and increase occurencescounted counter
    for(int startingpoint = 0; startingpoint <= PassedFileTextLength-FindingSubstringLength;startingpoint++)
    {

        // lettercount set to 0
        int lettercount = 0;

        // for loop that will go through the full substring length
        for(lettercount = 0; lettercount < FindingSubstringLength;lettercount++)
        {

            // checking if the substring letter is matching the passedfile letter
            if(UpperCaseFindingSubstring[lettercount] == UpperCasePassedFile[startingpoint+lettercount])
            {
               continue;
            }

            else
            {
                break;
            }

        }
           // occurencematched incremented if the lettersmatched equal findingsubstring
           if(lettercount == FindingSubstringLength)
           {
                OccurencesCounted++;
           }
    }
       // occurencecounted returned value
       return OccurencesCounted;
}
