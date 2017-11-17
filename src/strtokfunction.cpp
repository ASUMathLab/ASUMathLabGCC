#include <iostream>
#include <cmath>
#include <bitset>
#include <string.h>
#include <malloc.h>
#include <stdio.h>
using namespace std;


char * my_strtok(char * str, char *comp)
{
	static int pos;
	static char *s;	
	int i =0, start = pos;

	// Copying the string for further calls of strtok
	if(str!=NULL)
		s = str;
	
	i = 0;
	int j = 0;
	//While not end of string
	while(s[pos] != '\0')
	{
		j = 0;	
		//Comparing of one of the delimiter matches the character in the string
		while(comp[j] != '\0')
		{		
			//Pos point to the next location in the string that we have to read
			if(s[pos] == comp[j])
			{
				//Replace the delimter by \0 to break the string
				s[pos] = '\0';
				pos = pos+1;				
				//Checking for the case where there is no relevant string before the delimeter.
				//start specifies the location from where we have to start reading the next character
				if(s[start] != '\0')
					return (&s[start]);
				else
				{
					// Move to the next string after the delimiter
					start = pos;
					// Decrementing as it will be incremented at the end of the while loop
					pos--;
					break;
				}
			}
			j++;
		}
		pos++;		
	}//End of Outer while
	s[pos] = '\0';
	if(s[start] == '\0')
		return NULL;
	else
		return &s[start];
}


int main() {



char s[] = "Welcome to Computer World";
char delim[] = " ";

char* p = my_strtok(s , delim);



while(p != NULL) {
    cout << p << endl;
    p = my_strtok(NULL, delim);
}





    return 0;
}