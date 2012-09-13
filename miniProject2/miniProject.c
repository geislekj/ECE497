
/**************************************
Example Program for the Large Piezo Vibration Sensor - With Mass

This will display the value of the AIN0 Pin directly to the terminal repeatively.

wrtten by Kevin Geisler, 23-Sept-2012
***************************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define SYSFS_AIN1_DIR "/sys/devices/platform/omap/tsc/ain2"

FILE *fp; 


int readanalog(){
	if ((fp = fopen(SYSFS_AIN1_DIR, "r")) == NULL)
	{
		printf("Cannot open value file.\n");
		exit(1);
	}	

	//Set pointer to begining of the file
	rewind(fp);
	//Write our value of "0" to the file 
	char inString[10];
	fgets(&inString, 10, fp);
	fflush(fp);
//			printf("...value set to 0...\n");	
	fclose(fp);
//	printf(inString);
	return atoi(&inString);
}


int main(int argc, char** argv)
{

	//Run an infinite loop - will require Ctrl-C to exit this program
	while(1)
	{
	printf("%d\n",readanalog());
	usleep(100000);

	}
	return 0;
}

