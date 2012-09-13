// An awesome led display show
//
//Created by Dingo_aus, 7 January 2009
//email: dingo_aus [at] internode <dot> on /dot/ net
// From http://www.avrfreaks.net/wiki/index.php/Documentation:Linux/GPIO#gpio_framework
//
//Created in AVR32 Studio (version 2.0.2) running on Ubuntu 8.04
// Modified by Mark A. Yoder, 21-July-2011

//Exstensively rewitten by Kevin Geisler, 8-Sept-2012

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev-user.h>
#include <fcntl.h>
#include <errno.h>

#define SYSFS_GPIO_DIR "/sys/class/gpio"
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

