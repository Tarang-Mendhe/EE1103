/**************************************************************************************

Date: 17th February, 2021

Authors: ME20B178, EE20B075, EE20B080 (Group 4C)

Purpose: To create a noisy sine wave and evaluate the integral using Midpoint Theorem,
         Trapezoid Method and Simpsons Method and check where they converge 
Inputs: M (No of cycles of the wave),N (Maximum No of points taken per cycle).
        The inputs are accepted in the same order
        
Outputs: The values of no of points per cycle,and errors due to estimation of Integrals
         by all the 3 Methods
***************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

float* create(float n,float M){ //Function to create noisy sine wave

     float* z;
     float* y;
     float* x;
     float U1, U2;
    if (( z = (float*)malloc(n*sizeof(float))) == NULL) { //dynamic allocation of arrays
        printf ("\nProblem in alloc z");
        exit (0);
     }
    if (( y = (float*)malloc(n*sizeof(float))) == NULL) {
        printf ("\nProblem in alloc y");
        exit (0);
     }
    if (( x = (float*)malloc(n*sizeof(float))) == NULL) {
        printf ("\nProblem in alloc x");
        exit (0);
     }
     
      if(x == NULL || y == NULL || z == NULL){ //Checking if Memory allocation is proper

         printf("Problem in Func ");
         exit(0);
     }
     for(int j=0;j<n;j++) { //Finding points for sinx;
    
       x[j] = sin(2 * M_PI * j/M);
     
     }
   
     for(int i = 0; i < n; i++) { //Finding Noise and adding it to the sine wave

        U1 = rand() / (float)RAND_MAX;
        U2 = rand() / (float)RAND_MAX;
        z[i] = 0.2 * 2 * sqrt(-2 * log (U1)) * cos(2 * M_PI * U2);
        y[i] = x[i] + z[i]; 
    }   

    return y;
}

float midptapp (float* z,float n){ //Finding integral using midpoint method

    float int_value = 0,del_x = 2*M_PI/n;
    for(int i = 1;i<n;i++){
       
      int_value = int_value + del_x*z[i];
      
    }
   
    return int_value;

}

float trapezoidapp(float* z,float n) { //Finding integral using trapezoid method

    float int_value = 0,del_x = 2*M_PI/n;
    for(int i = 1;i<=n;i++){

       int_value = int_value + 0.5*del_x*(z[i-1]+z[i]);

   }
   
   return int_value;

} 

float simpsonsapp(float* z,float n) { //Finding integral using simpsons rule

   float int_value = 0,del_x = 2*M_PI/n,m;
   for(int i = 0;i<=n;i++){ 
   
     if((i ==0)||(i == n)){ //The below 3 conditions are for assigning the coeffecients properly
   
       m = 1;

     }
     
     else if(i%2 == 1){

       m = 4;

     }

     else{

       m = 2;

     }

     int_value = int_value + (del_x*m*z[i])/3; //adding the value here

  }

  return int_value;
} 

int main(int argc,char **argv){
  
   int M,N,flag = 0;
   float Em,Et,Es,tol,Em0 = 0,Et0 = 0,Es0 = 0;
   
   if(argc != 3){ //If the command line arguments are not proper

     printf("Please enter only M and N in order !");
     exit(0);
   
   }
   
   M = atoi(argv[1]); //Getting the number of cycles
   N = atoi(argv[2]); //Number of points per cycle

   for(int n = 4;n<=N;n++){ //Loop to run for different values of n

     float *z;
     z = (float*)malloc((M*n)*sizeof(float)); //Dynamic allocation to get the noisy sine wave points
    
     if(z == NULL){ //To prevent errors in memory allocation

       printf("prblm in main()");
       exit(0);

     }
     
     z = create(M*n,M); //Creating the noisy sine wave
    
     Em = midptapp(z,M*n); //Approximating the integral using mid point method
     
     Et = trapezoidapp(z,M*n);//Approximating the integral using mid point method

     Es = simpsonsapp(z,M*n);//Approximating the integral using mid point method

     printf("(%d ,%f ,%f ,%f )\n",n,fabs(Em),fabs(Et),fabs(Es)); //Loop for checking the convergence within tolerance of 0.0001

     if(fabs(Em-Em0)>fabs(Et-Et0)){ //Evaluating the maximum tolerance
   
         tol = fabs(Em - Em0); 

     }
     else{

         tol = fabs(Et - Et0);

     }

    if(tol<fabs(Es-Es0)){

         tol = fabs(Es - Es0);

     } 
    
    
     if(fabs(tol)<0.0001){ //Condition for stopping the loop
        
        flag = 1;
        break;

     }
    
     Em0 = Em; //Assigning the new values to the old variables
     Et0 = Et;
     Es0 = Es; 
     
   } 
 
   if(flag == 1){
   printf("The Errors have converged here successfully\n");}
   else{
   printf("They dont converge due to small N\n");}
   return 0;

}
