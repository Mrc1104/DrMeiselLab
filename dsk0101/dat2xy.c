/* dat2xy.c - takes 1D daq *.dat file and converts to regular xy format       */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

main(int argc, char *argv[])
{ 	int i,disk,tag,start,stop;
	unsigned long savtim,savdat;
	unsigned idata;
	char *dotpos,*outfilename;
	char *usage = "usage: dat2xy file\n"; 
	FILE *INFILE, *OUTFILE;

	dotpos = (char *)malloc(50);
	outfilename = (char *)malloc(100);
	if (argc != 2) 
	{	printf("%s",usage);
		exit(EXIT_FAILURE);
	}
/*----- make the output file name --------------------------------------------*/
	dotpos=strchr(argv[1],'.');
	strncpy(outfilename,argv[1],dotpos-argv[1]);
	strcat(outfilename,".xy");

/*----- Open input data file ------------------------------------------------*/
        if ((INFILE = fopen(argv[1], "r")) == NULL)
        {       printf("\nCannot open input file %s! \n",argv[1]);
                exit(EXIT_FAILURE);
        }
/*----- Open output data file ------------------------------------------------*/
        if ((OUTFILE = fopen(outfilename, "w")) == NULL)
        {       printf("\nCannot open output file %s! \n",outfilename);
                exit(EXIT_FAILURE);
        }
/*----- read and write data --------------------------------------------------*/
	fscanf(INFILE," %07u %07u %07u %07u %07lu %07lu",&disk,&tag,&start,
		&stop,&savdat,&savtim);
	for (i=start; i<=stop; i++) 
	{	fscanf(INFILE,"%08u",&idata);
		fprintf(OUTFILE,"%d  %d\n",i,idata);
	}
/*----------------------------------------------------------------------------*/
	fclose(INFILE);
	fclose(OUTFILE);
}
