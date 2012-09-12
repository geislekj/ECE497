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


void exportgpio(int port){
	//Using sysfs we need to write the 3 digit gpio number to /sys/class/gpio/export
	//This will create the folder /sys/class/gpio/gpio37
	if ((fp = fopen(SYSFS_GPIO_DIR "/export", "ab")) == NULL)

		{
			printf("Cannot open export file.\n");
			exit(1);
		}
	//Set pointer to begining of the file
		rewind(fp);
		//Write our value of "37" to the file
		char set_value[5];
		sprintf(&set_value, "%d\0",port);
		fwrite(&set_value, sizeof(char), 3, fp);
		fclose(fp);
	
	printf("...export file accessed, new pin now accessible\n");
	
}

void setdirectionout(int port, int readIn){
	char filestringval[40];
	sprintf(&filestringval, "%s%s%d%s\0", SYSFS_GPIO_DIR, "/gpio", port, "/direction");
	if ((fp = fopen(filestringval, "rb+")) == NULL)
	{
		printf("Cannot open direction file.\n");
		exit(1);
	}
	//Set pointer to begining of the file
	rewind(fp);
	//Write our value of "out" to the file
	char set_value[5];
	if(readIn == 1){
		strcpy(set_value,"in");
	}else{
		strcpy(set_value,"out");
	}
	fwrite(&set_value, sizeof(char), 3, fp);
	fclose(fp);
	printf("...direction set to output\n");		
}

void gioWrite(int port, int trigger){
	char* filestringval[40];
	sprintf(&filestringval, "%s%s%d%s\0", SYSFS_GPIO_DIR, "/gpio", port, "/value");
	if ((fp = fopen(filestringval, "rb+")) == NULL)
	{
		printf("Cannot open value file.\n");
		exit(1);
	}
	
	if(trigger == 1){
			//Set pointer to begining of the file
			rewind(fp);
			//Write our value of "1" to the file 
			char set_value[5];
			strcpy(set_value,"1");
			fwrite(&set_value, sizeof(char), 1, fp);
			fflush(fp);
//			printf("...value set to 1...\n");
	}else{
			//Set pointer to begining of the file
			rewind(fp);
			//Write our value of "0" to the file 
			char set_value[5];
			strcpy(set_value,"0");
			fwrite(&set_value, sizeof(char), 1, fp);
			fflush(fp);
//			printf("...value set to 0...\n");	
	}	
	fclose(fp);
}

int gioRead(int port){
	char* filestringval[40];
	sprintf(&filestringval, "%s%s%d%s\0", SYSFS_GPIO_DIR, "/gpio", port, "/value");
	if ((fp = fopen(filestringval, "rb+")) == NULL)
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
	return atoi(&inString);
}

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

int readi2cTemp(){
	char *end;
	int res, i2cbus, address, size, file;
	int daddress;
	char filename[20];

	i2cbus = 3;
	address  = 72;
	daddress = 0;
	size = I2C_SMBUS_BYTE;

	sprintf(filename, "/dev/i2c-%d", i2cbus);
	file = open(filename, O_RDWR);
	if (file<0) {
		if (errno == ENOENT) {
			fprintf(stderr, "Error: Could not open file "
				"/dev/i2c-%d: %s\n", i2cbus, strerror(ENOENT));
		} else {
			fprintf(stderr, "Error: Could not open file "
				"`%s': %s\n", filename, strerror(errno));
			if (errno == EACCES)
				fprintf(stderr, "Run as root?\n");
		}
		exit(1);
	}

	if (ioctl(file, I2C_SLAVE, address) < 0) {
		fprintf(stderr,
			"Error: Could not set address to 0x%02x: %s\n",
			address, strerror(errno));
		return -errno;
	}

	res = i2c_smbus_write_byte(file, daddress);
	if (res < 0) {
		fprintf(stderr, "Warning - write failed, filename=%s, daddress=%d\n",
			filename, daddress);
	}
	res = i2c_smbus_read_byte_data(file, daddress);
	close(file);

	if (res < 0) {
		fprintf(stderr, "Error: Read failed, res=%d\n", res);
		exit(2);
	}

	printf("temp:%dC\n", res);	
}

int main(int argc, char** argv)
{

	//Light pos
	int lightpos = 0;	

	// LED VALUES
	int LED1 = 10;
	int bLED1 = 2;
	int LED2 = 8;
	int bLED2 = 2;
	int LED3 = 9;
	int bLED3 = 2;
	int LED4 = 6;
	int bLED4 = 2;
	int LED5 = 7;
	int bLED5 = 2;
	int Switch = 28;
	int bSwitch = 1;

	LED1 =  (bLED1*32)+LED1;
	LED2 =  (bLED2*32)+LED2;
	LED3 =  (bLED3*32)+LED3;
	LED4 =  (bLED4*32)+LED4;
	LED5 =  (bLED5*32)+LED5;
	Switch = (bSwitch*32)+Switch;

	if (argc < 1) {
		printf("Error");
		exit(-1);
	}

	printf("\n*********************************\n"
		"*  Welcome to Mini Project 1 *\n"
		"*  brought to you by Kevin Geisler *\n"
		"**********************************\n");

	exportgpio(LED1);
	exportgpio(LED2);
	exportgpio(LED3);
	exportgpio(LED4);
	exportgpio(LED5);
	exportgpio(Switch);

	//SET DIRECTION
	//Open the LED's sysfs file in binary for reading and writing, store file pointer in fp
	setdirectionout(LED1,0);	
	setdirectionout(LED2,0);
	setdirectionout(LED3,0);
	setdirectionout(LED4,0);
	setdirectionout(LED5,0);
	setdirectionout(Switch,1);

	//Run an infinite loop - will require Ctrl-C to exit this program
	while(1)
	{
	if(lightpos == 0 || lightpos == 9){
		gioWrite(LED1,1);
		gioWrite(LED2,0);
	} else if (lightpos == 1 || lightpos == 8){
		gioWrite(LED2,1);
		gioWrite(LED1,0);
		gioWrite(LED3,0);
	} else if (lightpos == 2 || lightpos == 7){
		gioWrite(LED3,1);
		gioWrite(LED2,0);
		gioWrite(LED4,0);
	} else if (lightpos == 3 || lightpos == 6){
		gioWrite(LED4,1);
		gioWrite(LED3,0);
		gioWrite(LED5,0);
	} else if (lightpos == 4 || lightpos == 5){
		gioWrite(LED5,1);
		gioWrite(LED4,0);
	}
	
	if(gioRead(Switch) != 1){
		readi2cTemp();
	}

	usleep(readanalog() * 1000);

	lightpos++;
	if(lightpos == 10){
		lightpos = 0;
	}
	}
	return 0;
}

