/**************************************************************************************

Date: 6th March, 2021

Authors: ME20B178, EE20B075, EE20B080 (Group 4C)

Purpose: To find the mean arrival time and its std dev, between droplets and the same
         for droplet width for column 2 data

Inputs: None
        
Outputs: Mean droplet width,standard deviation ,mean arrival time and its std dev
***************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int rec = 0; //No of records in the file (In the file given, this was 9800)
float arr[2][10000]; //Stores the content of the file .2D array was initally proposed for column 1 and column 2
float *fir = arr[0]; //Pointer for first row of array data
float *sec = arr[1]; //Pointer for second row of array data
char s1[10], s2[10]; //Temporary strings retrieved from text file
float mavg[2][10000]; //Moving average array
float stdev[2][10000]; //Standard deviation array
float mu, dev; //Mean and Standard Deviation
int wd = 51; // Setting width
float P = 1; //
int N= 9800; // That is the total number of values in each column;
float T2[1000],Tau2[1000] ; // Holds Droplet width and arrival time value 
float upper_limit[2][10000] ,lower_limit[2][10000] ;//For Bollinger Bands



float calc(int start,int end,int count,float *arr,int mean,int stdev) //Calculates and returns mean and standard deviation for all arrays that are assigned this function
{
    float ms, sume = 0, sumr = 0;
    int i;
    for(i = start; i < end; i++) //loop to calculate sum of data as well as sum of squares of data
    {
        sume += arr[i];
        sumr += arr[i]*arr[i];
    }
    mu = sume / count; //Average is calculated
    ms = sumr / count; //Mean square is calculated
    dev = sqrt(ms - mu*mu); //Standard deviation is calculated

    if(mean == 1){

       return mu;  //Will return mean

    }

    else if(stdev == 1){

       return dev; //Will return Standard Deviation

   }

   else{

         return 0; // Just in case something goes wrong (in this program no problem as of now)

   }
}


void calc_mavg_stdev(float* p, short int c) //Stores moving average and stdev in 2 arrays
{   //calculation of moving average and stdev for data/terms at the border {first 25 and last 25} this is because our width is 51 
    int i,j,numofterms;                              // i, j = variables for iteration , numofterms= total no of terms in the summation for every iteration
    float suml ,sumsql ,sumb ,sumsqb ,varl ,varb;    // suml/sumsql is sum/sumofsquare for last 25 terms, sumb/sumsqb is sum/sumofsquare for beginning 25 terms ,varb and varl are the variance
    
    for(i=0;i<(wd- 1)/2 ; i++)        // loop of 25 iterations 
    {   
        sumb =0 ,sumsqb=0 ,suml = 0,sumsql = 0;  //initially 0
        numofterms = (wd- 1)/2 + i + 1 ; // in every sum no of terms depends on the position(index) of that data in array
        for(j=0;j< numofterms ; j++)           // calculation of sums and sums of squares  
        {
             sumb   += p[j] ;
             sumsqb += p[j] * p [j] ;
             suml   += p[rec - numofterms +j] ;
             sumsql += p[rec - numofterms +j] * p [rec - numofterms +j] ;
        }
        
        varb = (sumsqb - sumb *sumb /numofterms )/numofterms ; // variance is calculated
        varl = (sumsql - suml*suml /numofterms )/numofterms ;

        mavg[c][i] = sumb/numofterms;             //moving average is calculated for beginning terms
        stdev[c][i] = sqrt(varb) ;               //standerd deviation is calculated for beginning terms
        mavg[c][rec-i - 1] = suml/numofterms ;    //moving average is calculated for end terms
        stdev[c][rec-i - 1] = sqrt(varl ) ;      //standerd deviation is calculated for end terms
    
    }
    for(i = (wd-1)/2; i < rec-(wd-1)/2; i++)      //calculation of moving average for all the remaining terms
    {
        mavg[c][i] = calc(i-(wd-1)/2,i+(wd+1)/2,wd,arr[c],1,0);
        stdev[c][i] = calc(i-(wd-1)/2,i+(wd+1)/2,wd,arr[c],0,1);
    }

}

// searches and returns droplet position for a range till the data values get back in the threshold
int search_dip(int i , int *resume) {             //*resume is the pointer to ct
    double min = 15; 
    int min_pos ;
    while( arr[1][i] < lower_limit[1][i]){
        if (arr[1][i] <min) {  // loop to find minima (droplets)
            min = arr[1][i] ;
            min_pos = i ;    
        }
        i++ ;
    }
    *resume = i ;
    return min_pos ;
}



int main()
{
    
    int k,j,i,ct,tempos,dips_position[2][10000] ;  // this array stores the position of all the dips we encounter (in order) 

    FILE* fp;
    fp = fopen("data.txt", "r");  //Opening the txt file

    if (fp==NULL) //In case the file is not found
    {  
        fprintf(stderr, "Can't open file\n");
        return 1;
    }   

    while(fscanf(fp , "%s %s" , s1 , s2)==2) // Loop to store the file data in the structure array
    {     
        arr[0][rec] = atof(s1);
        arr[1][rec] = atof(s2);
        rec++; 
    }

    calc_mavg_stdev(fir, 0); //Moving average and standard deviation is stored for first row
    calc_mavg_stdev(sec, 1); //Moving average and standard deviation is stored for second row
    
    fclose(fp);  //Closing file pointer


    for(j = 0 ; j < rec ;j ++){
       upper_limit[1][j] = mavg[1][j] + P *stdev[1][j] ;               // array of upper limit for data 2
       lower_limit[1][j] = mavg[1][j] - P *stdev[1][j] ;               // arry of lower limit for data 2
       upper_limit[0][j] = mavg[0][j] + P *stdev[0][j] ;               // array of upper limit for data 1
       lower_limit[0][j] = mavg[0][j] - P *stdev[0][j] ;               // arry of lower limit for data 1
    }

    i=0 ,k= 0 ;
    
    while (i<rec){  //Loop to get all the droplet positions

        if ( arr[1][i]< lower_limit[1][i]){

            tempos = search_dip (i,&ct) ;

            if( (ct-i) >3 ){

                 dips_position[1][k] = tempos ;
                 k++ ;
            }

            i= ct - 1 ;     //position from which to continue our search = ct
            
        }

        i++ ;
    }   
    
    int index = 0; // for storing the arrival time and droplet width
    for(int z = 1;z<k-2;z = z+2){ //Loop to calculate droplet width

         T2[index] = dips_position[1][z+1] - dips_position[1][z];
         index++;

    }
   
    printf("Mean Of T2 is : %0.3f\n",calc(0,index,index,T2,1,0));  //Prints the mean of all droplet widths
    printf("Standard Deviation Of T2 is : %0.3f\n",calc(0,index,index,T2,0,1)); //Prints the standard deviation of droplet widths

    index = 0; //using same variable to do the same for arrival time 
   
    for(int z1 = 0;z1<k - 3;  z1 = z1 + 3){// Loop to store all the arrival times

          Tau2[index] = dips_position[1][z1+2] - dips_position[1][z1];
          index = index + 1;
     
    }
    
    printf("Mean Of Tau2 is : %0.3f\n",calc(0,index,index,Tau2,1,0));      //Displays mean arrival time 
    printf("Standard Deviation  Of Tau2 is : %0.3f\n",calc(0,index,index,Tau2,0,1));  //Displays standard deviation for the same
    return 0;
}