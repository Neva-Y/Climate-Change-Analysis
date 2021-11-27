/* Solution to comp20005 Assignment 2, 2019 semester 1.

   Authorship Declaration:

   (1) I certify that the program contained in this submission is completely
   my own individual work, except where explicitly noted by comments that
   provide details otherwise.  I understand that work that has been developed
   by another student, or by me in collaboration with other students,
   or by non-students as a result of request, solicitation, or payment,
   may not be submitted for assessment in this subject.  I understand that
   submitting for assessment work developed by or in collaboration with
   other students or non-students constitutes Academic Misconduct, and
   may be penalized by mark deductions, or by other penalties determined
   via the University of Melbourne Academic Honesty Policy, as described
   at https://academicintegrity.unimelb.edu.au.

   (2) I also certify that I have not provided a copy of this work in either
   softcopy or hardcopy or any other form to any other student, and nor will
   I do so until after the marks are released. I understand that providing
   my work to other students, regardless of my intention or any undertakings
   made to me by that other student, is also Academic Misconduct.

   (3) I further understand that providing a copy of the assignment
   specification to any form of code authoring or assignment tutoring
   service, or drawing the attention of others to such services and code
   that may have been made available via such a service, may be regarded
   as Student General Misconduct (interfering with the teaching activities
   of the University and/or inciting others to commit Academic Misconduct).
   I understand that an allegation of Student General Misconduct may arise
   regardless of whether or not I personally make use of such solutions
   or sought benefit from such actions.

   Signed by: Goh Xing Yang 1001969
   Dated: 14/5/2019

*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/************************************************************************/

/* hash-defines for data control */
#define MONTHSINYEAR 12						/* number on months in a year */
#define MAXYEARS 250						/* maximum years for input data */
#define MAXLINES (MAXYEARS * MONTHSINYEAR)  /* maximum lines of input data */
#define FIRSTLINE 0							/* first line of data in arrays */
#define MISSINGVAL -2						/* represents an invalid data */
#define DATA

/* hash-defines for graphing control */
#define GRAPHSCALE 24					 /* constant graph scale (y-axis) */
#define GRAPHPLOT 100					 /* determines what number to plot */
#define GRAPHSPACE "   "				 /* controls the graph spacing */
#define GRAPHLINE "0 +-----+"			 /* beginning x-axis format on graph*/
#define GRAPHMEAN " ****"				 /* represents a mean value on graph */

/* hash-defines for stage sequence control */
#define STAGE1 1
#define STAGE2 2
#define STAGE3 3
#define STAGE4 4

/* typedef for struct to store input data */
typedef struct{
	int    station;    /* station number*/
	int    year;	   /* year of data */
	int    month;	   /* month of data */
	double mmrain;	   /* monthly precipitation total (mm) */
	char   validate;   /* validation status of data */
} dataline_t;

/* typedefs for 1-d and 2-d arrays */
typedef double array2d_t[MAXLINES][MONTHSINYEAR];
typedef double month_vector_t[MONTHSINYEAR];
typedef int    year_vector_t[MAXLINES];

/* typedef for struct to store rain data */
typedef struct{
	array2d_t      rain_array;   /* store rain (mm) values in n by 12 matrix */
	month_vector_t month_avg;    /* store average rain (mm) values */
	year_vector_t  year_values;  /* store the year of the rain values */
} raindata_t;
	
/* global variable declared to print out the strings of month names */
char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
				  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
					  
/* function prototypes */
void   initial_data_processing();
void   invalid_data_error(char *msg, int line);
int    read_data(dataline_t *alldata);
void   input_line_years(int print_years[], int argc, char *argv[]);
void   check_data(dataline_t *alldata, int nlines);
void   new_line();
int    read_and_check(dataline_t *alldata, int print_years[], int argc,
					  char *argv[]);
void   print_S1_months(dataline_t *alldata, int indexcounter, int currentyear,
					   raindata_t *raindata, int raincounter);
void   print_S1_lines(dataline_t *alldata, raindata_t *raindata, int nlines,
				      int stage);
double calculate_rainfall(raindata_t *raindata, int indexcount, int nlines);
int    num_years(dataline_t *alldata, raindata_t *raindata, int nlines);
int    num_rainvals(raindata_t *raindata, int indexcount, int nyears);
int    first_rainyear(dataline_t *alldata, raindata_t *raindata, int nyears,
					  int index);	
int    last_rainyear(dataline_t *alldata, raindata_t *raindata, int nyears,
				     int index);
void   print_year_range(int nyears, int startyear, int endyear, int stage,
					    int nvals);
int    print_year_vals(dataline_t *alldata, raindata_t *raindata, int nlines,
					   int nyears, int index, int stage);
void   print_avg_rainfall(dataline_t *alldata, raindata_t *raindata,
						  int nyears, int index, int vals);
void   print_S2_lines(dataline_t *alldata, raindata_t *raindata, int nlines,
					  int nyears, int stage);
double kendall_tau(raindata_t *raindata, int index, int nyears, int nvals);
void   print_S3_lines(dataline_t *alldata, raindata_t *raindata, int nlines,
					  int nyears, int stage);
void   S4_parameters(raindata_t *raindata, int index, int stage);
int    round_up(int rainvalue, int scale);
void   rain_graph_formatting(raindata_t *raindata, int scale, int year,
							 int index, double maxval);
void   print_graph_value(raindata_t *raindata, int currentline, int scale,
						 int plotval, int index);
void   print_S4_lines(raindata_t *raindata, int print_years[], int argc,
					  int nyears, int stage);
void   ta_daa();

/************************************************************************/

/* main binding program
*/
int
main(int argc, char *argv[]){
	// creating an array of structs to store all input data from the file
	dataline_t alldata[MAXLINES];
	// creating a struct to store rain data
	raindata_t raindata;
	// declaring buddy variables for number of data lines and years
	int nlines, nyears;
	// declaring an array of size argc to store the years to print in stage 4
	int print_years[argc];
	
	// obtain the buddy variables
	nlines=read_and_check(alldata, print_years, argc, argv);
	nyears=num_years(alldata, &raindata, nlines);
	
	// execute and output the 4 stages
	print_S1_lines(alldata, &raindata, nlines, STAGE1);
	print_S2_lines(alldata, &raindata, nlines, nyears, STAGE2);
	print_S3_lines(alldata, &raindata, nlines, nyears, STAGE3);
	print_S4_lines(&raindata, print_years, argc, nyears, STAGE4);
	
	ta_daa();
	return 0;
}

/************************************************************************/

/* checking initial file format and removing the header line
*/
void
initial_data_processing(){
	char ch;
	double val;
	// checking if the data file begins with a number, terminating if a number
	// is found 
	if (scanf("%lf", &val)==1){
		invalid_data_error("Data formatting error", __LINE__);
	}
	// taking the header line and doing nothing to it (throwing it away) 
	while ((ch=getchar())!= '\n'){
		// terminating the program if it reaches the end of the file 
		if(ch==EOF){
			invalid_data_error("Data ended abruptly", __LINE__);
		}
	}
}

/************************************************************************/

/* function to print an error message and exit the program 
*/
void
invalid_data_error(char *msg, int line){
	printf("Error on line %3d: %s\n", line, msg);
	printf("The program will now terminate\n");
	exit(EXIT_FAILURE);
	return;
}

/************************************************************************/

/* reading the data into the structs
*/
int
read_data(dataline_t *alldata){
	int nread=0, mm, yy, stanum;
	double precip;
	char valdata;
	// reading in one line of data at a time, IDCJAC0001 is a fixed string
	// identifier for the data type
	while(scanf("IDCJAC0001,%d,%d,%d,%lf,%c\n", &stanum, &yy, &mm, 
				&precip, &valdata)==5){
		if (nread==MAXLINES){
			invalid_data_error("Data lines exceed limit", __LINE__);
		}
		alldata[nread].station=stanum;
		alldata[nread].year=yy;
		alldata[nread].month=mm;
		alldata[nread].mmrain=precip;
		alldata[nread].validate=valdata;
		nread++;
	}
	return nread;
}

/************************************************************************/

/* checking if the data is valid, terminating if there are any errors
*/
void
check_data(dataline_t *alldata, int nlines){
	char check_c;
	double check_d;
	int i;
	// terminating if there are no data lines 
	if (nlines==0){
		invalid_data_error("No data lines", __LINE__);
	}
	// checking for extra characters or numbers left in the data file, 
	// terminating if any are found 
	if ((scanf("%lf", &check_d)==1) || (scanf("%c", &check_c)==1)){
		invalid_data_error("Data format does not meet expectations", __LINE__);
	} 
	// general data checks, terminating the program if any invalid input data 
	// is found
	for (i=0;i<nlines;i++){
		// checking for any negative rainfall mm
		if ((alldata[i].mmrain)<0){
			invalid_data_error("Negative rainfall (mm)", __LINE__);
		}
		// checking for any months out of the 1-12 range
		if ((alldata[i].month)<1 || (alldata[i].month)>12){
			invalid_data_error("Invalid month", __LINE__);
		}
		// checking for any negative years
		if ((alldata[i].year)<0){
			invalid_data_error("Invalid year", __LINE__);
		}
		// checking if all station values are the same
		if ((alldata[FIRSTLINE].station)!=(alldata[i].station)){
			invalid_data_error("Stations do not match", __LINE__);
		}
	}
}

/************************************************************************/

/* obtain values for years from input line to print in stage 4, use argc and
argv to access command line inputs */
void
input_line_years(int print_years[], int argc, char *argv[]){
	int i;
	// start at 1 to ignore executable file name
	for (i=1;i<argc;i++){
		// converting character string to an integer and storing it in an array
		print_years[i]=atoi(argv[i]);
	}
}

/************************************************************************/

/* printing a new line
*/
void
new_line(){
	printf("\n");
}

/************************************************************************/

/* combined function to check and read data (from file and input line)
*/
int
read_and_check(dataline_t *alldata, int print_years[], int argc, char *argv[]){
	int nlines;
	initial_data_processing();
	nlines=read_data(alldata);
	check_data(alldata, nlines);
	input_line_years(print_years, argc, argv);
	return nlines;
}

/************************************************************************/

/* calculates the number of years of data in the input data file and assigning
new year values into an array */
int
num_years(dataline_t *alldata, raindata_t *raindata, int nlines){
	// initially assigning the current year to -2 so it enters the loop on the
	// first data line
	int yearscount=0, currentyear=MISSINGVAL, i;
	for(i=0;i<nlines;i++){
		if (currentyear!=alldata[i].year){
			// assigning the new year value into a vector
			raindata->year_values[yearscount]=alldata[i].year;
			yearscount++;
			// changing the current year to the new year value
			currentyear=alldata[i].year;
		}
	}
	return yearscount;
}

/************************************************************************/

/* printing the months for S1, subsequently assigning rain mm values into 
an array (marking as -2 when the month's data is not recorded) */
void
print_S1_months(dataline_t *alldata, int indexcounter, int currentyear,
				raindata_t *raindata, int raincounter){
	// creating a month values array 
	int month_values[MONTHSINYEAR] = {1,2,3,4,5,6,7,8,9,10,11,12}; 
	int j=0, numprint=0;
	// making sure that the year is constant and that we have printed 12 vals
	while (currentyear==(alldata[indexcounter].year)||
		  numprint<MONTHSINYEAR){
	// checking if there is data for the j'th month
		if (alldata[indexcounter].month==month_values[j]){
			// filling the rain_array with the rain mm data if there 
			// is a month present
			raindata->rain_array[raincounter][j]=alldata[indexcounter].mmrain;
			if (alldata[indexcounter].validate=='N'){
				printf(" %s*", months[j]);
			}else{
				printf(" %s ", months[j]);
			}
			//moving to the next data line in the struct
			indexcounter++;
		}else{
			// filling the rain_array with -2 if there is no month reading
			raindata->rain_array[raincounter][j]=MISSINGVAL;
			printf(" ... ");
		}
		// adding 1 to the number of printed vals and adding 1 to the month 
		// counter
		j++;
		numprint++;
	}
}

/************************************************************************/

/* combined function to execute stage 1
*/
void
print_S1_lines(dataline_t *alldata, raindata_t *raindata, int nlines,
			   int stage){
	// current year initialised as -2 so it always enters the loop
	// on the first data line
	int i, currentyear=MISSINGVAL, raincounter=0;
	printf("S%d, site number %06d, %d datalines in input\n", stage,
		   alldata[FIRSTLINE].station, nlines);
	for (i=0;i<nlines;i++){
		// only executing the printing for new years
		if ((alldata[i].year)!=currentyear){
			currentyear=alldata[i].year;
			printf("S%d, %d:", stage, currentyear);
			print_S1_months(alldata, i, currentyear, raindata, raincounter);
			new_line();
			// raincounter used to control the index of the rain_array, adding
			// by one after every iteration
			raincounter++;
		}
	}
	new_line();
}

/************************************************************************/

/* calculates the cumulative rainfall for a certain month throughout the years
*/
double
calculate_rainfall(raindata_t *raindata, int indexcount, int nyears){
	int i;
	double totrain=0;
	for(i=0;i<nyears;i++){
		// only adding if there is rainfall data for the given year and month
		if (raindata->rain_array[i][indexcount]!=MISSINGVAL){
			totrain+=raindata->rain_array[i][indexcount];
		}
	}
	return totrain;
}

/************************************************************************/

/* calculates the number of data values for a certain month throughout 
the years */
int
num_rainvals(raindata_t *raindata, int indexcount, int nyears){
	int i, numvals=0;
	for(i=0;i<nyears;i++){
		// increments by 1 only if there is rainfall data for the given 
		// year and month
		if (raindata->rain_array[i][indexcount]!=MISSINGVAL){
			numvals++;
		}
	}
	return numvals;
}

/************************************************************************/

/* finds the first year that rain data is provided for a given month
*/
int 
first_rainyear(dataline_t *alldata, raindata_t *raindata, int nyears,
			   int index){
	int i, currentyear;
	// initalising currentyear as the first year in the data file
	currentyear=raindata->year_values[FIRSTLINE];
	for (i=0;i<nyears;i++){
		// if a value is provided for the given month and year, return the year
		if (raindata->rain_array[i][index]!=MISSINGVAL){
			return currentyear;
		}
		// adding 1 to the year if there is no value provided for the given 
		// month and year
		currentyear++;
	}
	// returning -2 if there is no year (for the given month) where the 
	// rain data has values
	return MISSINGVAL;
}

/************************************************************************/

/* finds the last year that rain data is provided for a given month
*/
int 
last_rainyear(dataline_t *alldata, raindata_t *raindata, int nyears,
			  int index){
	int i, currentyear;
	// initalising currentyear as the final year in the data file
	currentyear=raindata->year_values[nyears-1];
	// if a value is provided for the given month and year, return the year
	for (i=(nyears-1);i>=0;i--){
		if (raindata->rain_array[i][index]!=MISSINGVAL){
			return currentyear;
		}
		// subtracting 1 to the year if there is no value provided for the
		// given month and year
		currentyear--;
	}
	// returning -2 if there is no year (for the given month) where the 
	// rain data has values
	return MISSINGVAL;
}

/************************************************************************/

/* prints the beginning lines for stage 2 and 3 
*/
int
print_year_vals(dataline_t *alldata, raindata_t *raindata, int nlines,
				int nyears, int index, int stage){
	int startyear, endyear, vals;
	startyear=first_rainyear(alldata, raindata, nyears, index);
	endyear=last_rainyear(alldata, raindata, nyears, index);
	vals=num_rainvals(raindata, index, nyears);
	printf("S%d, %s, ", stage, months[index]);
	print_year_range(nyears, startyear, endyear, stage, vals);
	return vals;
}

/************************************************************************/

/* prints the number of rain values for a month (across all years), then 
prints the range of years where there are rain values */
void
print_year_range(int nyears, int startyear, int endyear, int stage, int nvals){
	// stage 2
	if (stage==STAGE2){
		if (nyears!=0 && startyear!=MISSINGVAL){
			printf("%2d values, %d-%d, ", nvals, startyear, endyear);
		}
		else{
			printf(" 0 values");
		}
	}
	// stage 3
	if (stage==STAGE3){
		// extra stage 3 condition to print the year range
		if (nyears!=0 && startyear!=MISSINGVAL && nvals>1){
			printf("%2d values, %d-%d, ", nvals, startyear, endyear);
		}
		else{
			if (nvals==1){
				printf(" %d values", nvals);
			}
			else{
				printf(" 0 values");
			}
		}
	}
}

/************************************************************************/
			
/* prints the average rainfall for a given month through the years, storing
the values in raindata's (struct) vector
*/		
void
print_avg_rainfall(dataline_t *alldata, raindata_t *raindata, int nyears, 
				   int index, int vals){
	double tot_rain;
	tot_rain=calculate_rainfall(raindata, index, nyears);
	if (nyears!=0 && 
		first_rainyear(alldata, raindata, nyears, index)!=MISSINGVAL){
		raindata->month_avg[index]=tot_rain/vals;
		printf("mean of %5.1fmm", raindata->month_avg[index]);
	}
	else{
		raindata->month_avg[index]=MISSINGVAL;
	}
}

/************************************************************************/

/* combined function to print stage 2 lines
*/
void
print_S2_lines(dataline_t *alldata, raindata_t *raindata, int nlines,
				int nyears, int stage){
	int i, vals;
	for (i=0;i<MONTHSINYEAR;i++){
		vals=print_year_vals(alldata, raindata, nlines, nyears, i, STAGE2);
		print_avg_rainfall(alldata, raindata, nyears, i, vals);
		new_line();
	}
	new_line();
}

/************************************************************************/

/* calculates the kendall's tau for a given month
*/
double
kendall_tau(raindata_t *raindata, int index, int nyears, int nvals){
	int i, j, runningsum=0;
	// returns -2 if there are 1 or less values (cannot compute kendalls' tau)
	if (nvals<=1){
		return MISSINGVAL;
	}
	for (i=0;i<nyears;i++){
		if (raindata->rain_array[i][index]==MISSINGVAL){
			// if there is no rain data for that year, skip to the 
			// next iteration
			continue;
		}
		for (j=i+1;j<nyears;j++){
			if (raindata->rain_array[j][index]==MISSINGVAL){
				// condition to continue next iteration here too
				continue;
			}
			// comparing the values to see which rainfall value is larger,
			// adding 1 or subtracting 1 accordingly
			if (raindata->rain_array[i][index]>raindata->rain_array[j][index]){
				runningsum-=1;
			}
			else if (raindata->rain_array[i][index]<
					 raindata->rain_array[j][index]){
				runningsum+=1;
			}
		}
	}
	return (double) 1/((nvals*(nvals-1))/2)*runningsum;
}

/************************************************************************/

/* combined function to print stage 3 lines
*/
void 
print_S3_lines(dataline_t *alldata, raindata_t *raindata, int nlines,
				int nyears, int stage){
	int i, nvals;
	double tau;
	for (i=0;i<MONTHSINYEAR;i++){
		nvals=print_year_vals(alldata, raindata, nlines, nyears, i, stage);
		tau=kendall_tau(raindata, i, nyears, nvals);
		// only print the value if there is a valid tau
		if (tau!=MISSINGVAL){
			printf("tau of %5.2f", tau);
		}
		new_line();
	}
	new_line();
}

/************************************************************************/

/* prints the maximum rainfall in a year (taking monthly averages into account)
and the scale of the graph */
void
S4_parameters(raindata_t *raindata, int index, int stage){
	double maxval=MISSINGVAL;
	int i, j, year, scale, maxscale;
	year=raindata->year_values[index];
	//find highest rainfall out of the monthly averages
	for (j=0;j<MONTHSINYEAR;j++){
		if (raindata->month_avg[j]>maxval){
			maxval=raindata->month_avg[j];
		}
	}
	// find highest rainfall out of individual rainfall data in the given year
	for (i=0;i<MONTHSINYEAR;i++){
		if ((raindata->rain_array[index][i])>maxval){
			maxval=raindata->rain_array[index][i];
		}
	}
	// rounding the maximum value up to the nearest integer
	maxscale=ceil(maxval);
	// incrementing the value until it is a factor of 24
	while (maxscale%GRAPHSCALE!=0){
		maxscale++;
	}
	scale=maxscale/GRAPHSCALE;
	printf("S%d, %d max is %5.1f, scale is %d\n", stage, year, maxval, scale);
	rain_graph_formatting(raindata, scale, year, index, maxval);
}

/************************************************************************/

/* rounds an integer up to the nearest factor of the given scale
*/
int
round_up(int rainvalue, int scale){
	// incrementing by 1 when the remainder in nonzero
	while (rainvalue%scale!=0){
		rainvalue++;
	}
	return rainvalue;
}

/************************************************************************/

/* formatting the graph layout
*/
void
rain_graph_formatting(raindata_t *raindata, int scale, int year, int index,
					  double maxval){
	int i, j, k, plotval;
	// modulus the year with 100 to obtain the value to plot on the graph
	plotval=year%GRAPHPLOT;
	for (i=GRAPHSCALE;i>0;i--){
		// ensures that there is at least one line of data on the top line in
		// the graph (does not have to be 24 points every time in the y-axis)
		if (round_up(ceil(maxval),scale)>=i*scale){
			printf("%4d |", scale*i);
			// passing to another function to print the values in the line
			print_graph_value(raindata, i, scale, plotval, index);
		}
	}
	// printing the x-axis format at the bottom of the graph
	printf("%s%s", GRAPHSPACE, GRAPHLINE);
	for (j=1;j<MONTHSINYEAR;j++){
		printf("----+");
	}
	new_line();
	printf("%s%s", GRAPHSPACE, GRAPHSPACE);
	for (k=0;k<MONTHSINYEAR;k++){
		printf(" %s ", months[k]);
	}
	new_line();
	new_line();
}

/************************************************************************/

/* printing a line of the rainfall data
*/
void
print_graph_value(raindata_t *raindata, int currentline, int scale,
				  int plotval, int index){
	int i, rainvalue, meanrainval;
	// iterating for each month (Jan-Dec)
	for (i=0;i<MONTHSINYEAR;i++){
		// rounding the rain values up to the nearest integer
		rainvalue=ceil(raindata->rain_array[index][i]);
		meanrainval=ceil(raindata->month_avg[i]);
		// printing scenarios when there is a rain value present
		if (rainvalue!=MISSINGVAL){
			// if both the mean value and the rain value intersect
			if (round_up(rainvalue, scale)>=currentline*scale &&
				round_up(meanrainval, scale)==currentline*scale){
				printf(" *%02d*", plotval);
			}
			// printing only the rain value
			else if (round_up(rainvalue, scale)>=currentline*scale){
				printf("  %02d ", plotval);
			}
			// printing only the mean rain value
			else if (round_up(meanrainval, scale)==currentline*scale){
				printf("%s", GRAPHMEAN);
			}
			// neither condition is met, print 5 spaces to allign graph
			else{
				printf(" %s ", GRAPHSPACE);
			}
		}
		// printing scenarios when there is only a mean rain value present
		else if (rainvalue==MISSINGVAL && meanrainval!=MISSINGVAL &&
				 round_up(meanrainval, scale)==currentline*scale){
			printf("%s", GRAPHMEAN);
		}
		else{
			printf(" %s ", GRAPHSPACE);
		}
		
	}
	new_line();
}

/************************************************************************/
				
/* combined function to print stage 4 lines
*/
void
print_S4_lines(raindata_t *raindata, int print_years[], int argc, int nyears,
			   int stage){
	int i, j;
	for (i=1;i<argc;i++){
		for (j=0;j<nyears;j++){
			// print the rainfall graphs only if data from the command line 
			// years correspond to data from the file
			if (print_years[i]==raindata->year_values[j]){
				S4_parameters(raindata, j, stage);
			}
		}
	}
}

/************************************************************************/

/* prints Ta daa!
*/
void
ta_daa(){
	printf("Ta daa!\n");
}

/************************************************************************/

/* programming is fun! */