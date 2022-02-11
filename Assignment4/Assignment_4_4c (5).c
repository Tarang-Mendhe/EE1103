/*********************************************************************************************
Date: 24th February, 2021

Authors: ME20B178, EE20B080, EE20B075 (Group 4C)

Purpose: This program finds threshold voltage(vth) for a given set of data values
         Each set is represented by gate current ,vg(i) where i ∈ [1,11] .The data values in each set are values of drain current (id) 

Inputs:  The program does't require any inputs
       
Outputs: The program gives Threshold voltage(vth) for data sets from vg1 to vg11

In short ,This is how the program works:
1)takes in data values from the sgfet.cvs file and converts it into arrays  (MANOMUKIL pls check this points if it makes sense ir not)
2)Does linear fitting(regression) for all values of vth varying from vd[0] to vd[401] for a particular data set(vg)
3)calculates error for every arbitary vth and searches the vth for which it gets minimum
4)prints the vth for every vg
************************************************************************************************/
//various header files required for the program
#include<stdio.h>
#include<math.h>
#include<stdlib.h>

/*The program by default uses linear fitting(regression) on both sides of any arbitary vth and tries to minimise the rms error from linear fits by searching for better vth [method 2]
this is done by considering all the points on both sides of that arbitary vth, but we can choose to omit points on eiter sides by changing values of a and b */

int a=0;//a and b represents the number of points to be omited on left and right side of vth respectively.This is similar to choosing linear fits to low and high voltage parts of the graph[method 1](visual explanation of this attached in the submission)
int b=0;// eg1: try setting a = 15 , b = 80 to get vth near 1  
         

//functions required for the problem (detailed working of each is commented in function defination)
void linear_solver(double ,double ,double ,double ,double ,double ,double *,double *) ;//this function is used for calculating solution of system of linear equation

void getm1m2(int,double*,double*) ; //this is used for finding m1 and m2 for any vd[i] (the function uses linear_solver for the same)

double error_calc(int,double*,double*) ;// this is used for calculating error value for any vd[i] 

double m1,m2 ;             // m1 and m2 are the slopes of the lines created by using linear fitting on the left and right side of some arbitary vth

struct points{             //Structure to store different values of data from the csv file
        double Vd;
	    double Vg1;
        double Vg2;
        double Vg3; 
        double Vg4;
        double Vg5;
        double Vg6; 
	    double Vg7; 
        double Vg8;
        double Vg9;
        double Vg10; 
        double Vg11;
};

struct points data[405];

int main(){


	FILE* fp; //File pointer to use when reading the file
	struct points data[405]; //variable named data that is an array of user defined structure data type points to store the data temporarily 
	size_t count = 0; //the variable count is to index the data array 
        double arr[12][410];// the 2d array to which the data will be transferred from the array of structures as we found it eay to use pointers with 2d array more comfortably

	fp = fopen("sgfet.csv", "r");  //Opening the csv file


	if (fp==NULL){  //In case the file is not found
 
		fprintf(stderr , "Can't open file\n");
		return 1;

	}	

	fseek(fp,50,SEEK_SET); //Sets the file pointer to the line where the data starts . Please Note that this is done specifically for the file sgdet.csv

        while((fscanf(fp , "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf" , &data[count].Vd , &data[count].Vg1 , &data[count].Vg2 , &data[count].Vg3 , &data[count].Vg4 , &data[count].Vg5 , &data[count].Vg6 , &data[count].Vg7 , &data[count].Vg8 , &data[count].Vg9 , &data[count].Vg10 , &data[count].Vg11)==12)) { // Loop to store the file data in the structure array
               
                count++;
		
	} 
        
        for(size_t j = 0;j<count;j++) {  //Loop to transfer the data from structure to the 2d array
        
             arr[0][j] = data[j].Vd;
             arr[1][j] = data[j].Vg1;
             arr[2][j] = data[j].Vg2;
             arr[3][j] = data[j].Vg3;
             arr[4][j] = data[j].Vg4;
             arr[5][j] = data[j].Vg5;
             arr[6][j] = data[j].Vg6;
             arr[7][j] = data[j].Vg7;
             arr[8][j] = data[j].Vg8;
             arr[9][j] = data[j].Vg9;
             arr[10][j] = data[j].Vg10;
             arr[11][j] = data[j].Vg11;
        }

	fclose(fp); //closing the file


    double min_error , E ;            //vth is found by searching for minimum error and position of vth
    int min_position ;                       
    for(int l=1;l<12;l++){ //l takes different values based on different data sets
    min_error = 99999;
        for (int i = 0; i < 401; i++) {      //minimum error is searched from 0 to 401
            getm1m2 (i , arr[0] , arr[l]) ;        //This assigns values of m1 and m2 for every iteration
            E = error_calc (i , arr[0] , arr[l]) ;    //error (E)  is calculated for every iteration
            if (E < min_error) {                      //checking if E < minimum error, if yes then min error and positon of vth is updated accordingly
                min_error = E ;
                min_position = i ;
            }        
        }
        printf ( "\n vth is %.3f for vg%d \n" , arr[0][min_position] ,l ); // vth and vg is printed
    }
    
    
     return 0;
	 
}



//this function takes in values of vd (), id in the form of arrays and position of any arbitary vth and assigns values of m1 and m2 in the address of global variables m1 and m2 using linear_solver
void getm1m2(int th,double *x,double *y){       //x represents vd  ,y represents id for a particular vg ,th is the position of arbitary vth (between o and 401)
   double vth=x[th] ;                              
   double a1,a2,b1,b2,c1,c2,s1,s2,s3,s4,s5,s6 ;    //a1,b1,c1 and a2,b2,c2 are the coefficients of linear equations , 1) partial derivative of E wrt m1 2) partial derivative of E wrt m2 respectively
   int n=401 ;                                     //total no of data values given =n
   s1=0,s2=0,s3=0,s4=0,s5=0,s6=0;                  // s1,s2,s3,s4,s5,s6 are various summations terms occuring in the coefficients of linear eq
   for(int i=0;i<th-a;i++){                        // s1,s2,s3,s4,s5,s6 are calculated here
       s1+= x[i] *  x[i] ;    
       s2+= x[i] *  y[i] ;
   }
                                                  
   for(int j=th+b;j<n;j++){
       s3+= x[j];
       s4+= y[j];
       s5+= x[j] *  y[j] ;
       s6+= x[j] *  x[j] ;
   }
   a1=  2*s1        +    2*vth*vth*(n-th) ;                 // a1,b1,c1 ,a2,b2,c2 are calculated here
   b1=  2*vth*s3    -    2*vth*vth*(n-th) ; 
   c1= -2*s2        -    2*vth*s4        ;  
   a2=  2*vth*s3    -    2*vth*vth*(n-th) ;
   b2=  2*s6        +    2*vth*vth*(n-th)  -    4*vth*s3   ;
   c2= -2*s5        +    2*vth*s4        ;

   linear_solver(a1,a2,b1,b2,c1,c2,&m1,&m2) ;              //a1,b1,c1 ,a2,b2,c2 along with address of global variables m1 and m2 are given to linear solver
}

void linear_solver(double a1,double a2,double b1,double b2,double c1,double c2,double *m1,double *m2){ 
    *m1  = (b1*c2 - b2*c1) / (a1*b2 - a2*b1) ;        //solving the system of partial differential equations to get m1 and m2                                                 
    *m2  = (a1*c2 - a2*c1) / (b1*a2 -b2*a1)   ;                                                         
}

//this function takes in values of vd , id in the form of pointers and position of any arbitary vth and returns the error for that arbitary vth
double error_calc(int th,double*x1,double*y1)  //x1 represents vd  ,y1 represents id for a particular vg ,th is the position of arbitary vth (between o and 401)
{
    int j, k ;                   //i,j,k are variables used for iteration
    double e1, e2,E;             // E is the error square ,e1 stores error for x1[i]<vth ,e2 stores error for x1[i]>=vth
    int N=401 ;                  //N is total no of terms
    double *vd =x1 ;             // pointer x1 is assigned to pointer vd 
    
    E = 0;                       // E is being calculated 
    for(j = 0; j < th-a; j++)      // square of e1 is calculated and stored in E
    {
        e1 = m1*x1[j] - y1[j];
        E  = E + e1*e1;
    }
    for(k = th+b; k < N; k++)      // square of e2 is calculated and stored in E
    {
        e2 = m1*vd[th] + m2*(x1[j]-vd[th]) - y1[j];
        E = E + e2*e2;
    }
    return E;                    //E is returned
}
