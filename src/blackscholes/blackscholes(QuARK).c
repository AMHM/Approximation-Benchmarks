// Copyright (c) 2007 Intel Corp.

// Black-Scholes
// Analytical method for calculating European Options
//
// 
// Reference Source: Options, Futures, and Other Derivatives, 3rd Edition, Prentice 
// Hall, John C. Hull,

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#ifdef ENABLE_PARSEC_HOOKS
#include <hooks.h>
#endif

// Multi-threaded pthreads header
#ifdef ENABLE_THREADS
#define MAX_THREADS 128
// Add the following line so that icc 9.0 is compatible with pthread lib.
#define __thread __threadp  
MAIN_ENV
#undef __thread
#endif

//EnerC
//#include<enerc.h>


// Multi-threaded OpenMP header
#ifdef ENABLE_OPENMP
#include <omp.h>
#endif

// Multi-threaded header for Windows
#ifdef WIN32
#pragma warning(disable : 4305)
#pragma warning(disable : 4244)
#include <windows.h>
#define MAX_THREADS 128
#endif

//Precision to use for calculations
#define fptype float

#ifdef AMHM_APPROXIMATION
#include "../../shared_lib/approximations.h"
int reliability_level = 0;
#endif

#include <math.h>

int NUM_RUNS = 10;

fptype *prices;
int numOptions;

int    * otype;
fptype * sptprice;
fptype * strike;
fptype * rate;
fptype * volatility;
fptype * otime;
int numError = 0;
int nThreads;

////////////////////////////////////////////////////////////////////////////////
// Cumulative Normal Distribution Function
// See Hull, Section 11.8, P.243-244
fptype inv_sqrt_2xPI = 0.39894228040143270286;
fptype CNDF ( fptype InputX ) 
{
    int sign;

    fptype OutputX;
    fptype xInput;
    fptype xNPrimeofX;
    fptype expValues;
    fptype xK2;
    fptype xK2_2, xK2_3;
    fptype xK2_4, xK2_5;
    fptype xLocal, xLocal_1;
    fptype xLocal_2, xLocal_3;

    // Check for negative value of InputX
    if (InputX < 0.0) {
        InputX = -InputX;
        sign = 1;
    } else 
        sign = 0;

    xInput = InputX;
 
    // Compute NPrimeX term common to both four & six decimal accuracy calcs
    expValues = exp(-0.5f * InputX * InputX);
    xNPrimeofX = expValues;
    xNPrimeofX = xNPrimeofX * inv_sqrt_2xPI;

    xK2 = 0.2316419 * xInput;
    xK2 = 1.0 + xK2;
    xK2 = 1.0 / xK2;
    xK2_2 = xK2 * xK2;
    xK2_3 = xK2_2 * xK2;
    xK2_4 = xK2_3 * xK2;
    xK2_5 = xK2_4 * xK2;
    
    xLocal_1 = xK2 * 0.319381530;
    xLocal_2 = xK2_2 * (-0.356563782);
    xLocal_3 = xK2_3 * 1.781477937;
    xLocal_2 = xLocal_2 + xLocal_3;
    xLocal_3 = xK2_4 * (-1.821255978);
    xLocal_2 = xLocal_2 + xLocal_3;
    xLocal_3 = xK2_5 * 1.330274429;
    xLocal_2 = xLocal_2 + xLocal_3;

    xLocal_1 = xLocal_2 + xLocal_1;
    xLocal   = xLocal_1 * xNPrimeofX;
    xLocal   = 1.0 - xLocal;

    OutputX  = xLocal;
    
    if (sign) {
        OutputX = 1.0 - OutputX;
    }
    
    return OutputX;
} 

// For debugging
void print_xmm(fptype in, char* s) {
    printf("%s: %f\n", s, in);
}

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
fptype BlkSchlsEqEuroNoDiv( fptype sptprice,
                            fptype strike, fptype rate, fptype volatility,
                            fptype time, int otype, float timet )
{
    fptype OptionPrice;

    // local private working variables for the calculation
    fptype xStockPrice;
    fptype xStrikePrice;
    fptype xRiskFreeRate;
    fptype xVolatility;
    fptype xTime;
    fptype xSqrtTime;

    fptype logValues;
    fptype xLogTerm;
    fptype xD1; 
    fptype xD2;
    fptype xPowerTerm;
    fptype xDen;
    fptype d1;
    fptype d2;
    fptype FutureValueX;
    fptype NofXd1;
    fptype NofXd2;
    fptype NegNofXd1;
    fptype NegNofXd2;    
    
    xStockPrice = sptprice; xStrikePrice = strike; xRiskFreeRate = rate; xVolatility = volatility; 
    xTime = time;
    xSqrtTime = sqrt(xTime);
    
    //AMHM Begin
    logValues = log(sptprice / (strike == 0 ? 1 : strike)); xLogTerm = logValues; 
    //AMHM End
    
    xPowerTerm = xVolatility * xVolatility;
    xPowerTerm = xPowerTerm * 0.5;
        
    xD1 = xRiskFreeRate + xPowerTerm;
    xD1 = xD1 * xTime;
    xD1 = xD1 + xLogTerm;

    xDen = xVolatility * xSqrtTime;
    
    //AMHM Begin
    xD1 = xD1 / (xDen == 0 ? 1 : xDen);
    //AMHM End
    
    xD2 = xD1 -  xDen;

    d1 = xD1;
    d2 = xD2;
    
    NofXd1 = CNDF(d1);
    NofXd2 = CNDF(d2);

    FutureValueX = strike * ( exp( -(rate)*(time) ) );        
    if (otype == 0) {            
        OptionPrice = (sptprice * NofXd1) - (FutureValueX * NofXd2);
    } else { 
        NegNofXd1 = (1.0 - NofXd1);
        NegNofXd2 = (1.0 - NofXd2);
        OptionPrice = (FutureValueX * NegNofXd2) - (sptprice * NegNofXd1);
    }
    
    return OptionPrice;
}

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
#ifdef WIN32
DWORD WINAPI bs_thread(LPVOID tid_ptr){
#else
int bs_thread(void *tid_ptr) {
#endif
    int i;
    int j;
    fptype price;
    fptype priceDelta;
    int tid = *(int *)tid_ptr;
    int start = tid * (numOptions / nThreads);
    int end = start + (numOptions / nThreads);

    int _dummy = 0;
    for (j=0; j<NUM_RUNS; j++) {
        _dummy = 1; // ACCEPT: Prohibit perforation of this loop.
#ifdef ENABLE_OPENMP
#pragma omp parallel for
        for (i=0; i<numOptions; i++) {
#else  //ENABLE_OPENMP
        for (i=start; i<end; i++) {
#endif //ENABLE_OPENMP
            /* Calling main function to calculate option value based on 
             * Black & Sholes's equation.
             */
            price = BlkSchlsEqEuroNoDiv( sptprice[i], strike[i],
                                         rate[i], volatility[i], otime[i], 
                                         otype[i], 0);
            prices[i] = price;
            
#ifdef ERR_CHK   
            priceDelta = data_DGrefval[i] - price;
            if( fabs(priceDelta) >= 1e-4 ){
                printf("Error on %d. Computed=%.5f, Ref=%.5f, Delta=%.5f\n",
                       i, price, data_DGrefval[i], priceDelta);
                numError ++;
            }
#endif
        }
    }
    _dummy = _dummy;

    return 0;
}

int main (int argc, char **argv)
{
    FILE *file;
    int i;
    int loopnum;
    fptype * buffer;
    int * buffer2;
    int rv;
    
    fptype *data_s;          // spot price
    fptype *data_strike;     // strike price
    fptype *data_r;          // risk-free interest rate
    fptype *data_divq;       // dividend rate
    fptype *data_v;          // volatility
    fptype *data_t;          // time to maturity or option expiration in years 
			//     (1yr = 1.0, 6mos = 0.5, 3mos = 0.25, ..., etc)  
    char *data_OptionType;   // Option type.  "P"=PUT, "C"=CALL
    fptype *data_divs;       // dividend vals (not used in this test)
    fptype *data_DGrefval;   // DerivaGem Reference Value

#ifdef PARSEC_VERSION
#define __PARSEC_STRING(x) #x
#define __PARSEC_XSTRING(x) __PARSEC_STRING(x)
        printf("PARSEC Benchmark Suite Version "__PARSEC_XSTRING(PARSEC_VERSION)"\n");
	fflush(NULL);
#else
        printf("PARSEC Benchmark Suite\n");
	fflush(NULL);
#endif //PARSEC_VERSION
#ifdef ENABLE_PARSEC_HOOKS
   __parsec_bench_begin(__parsec_blackscholes);
#endif
   
#ifdef AMHM_APPROXIMATION
   if (argc != 5)
        {
                printf("Usage:\n\t%s <nthreads> <inputFile> <outputFile> <reliability_level>\n", argv[0]);
                exit(1);
        }
#endif

#ifndef AMHM_APPROXIMATION
   if (argc != 4)
        {
                printf("Usage:\n\t%s <nthreads> <inputFile> <outputFile>\n", argv[0]);
                exit(1);
        }
#endif

    nThreads = atoi(argv[1]);
    char *inputFile = argv[2];
    char *outputFile = argv[3];

#ifdef AMHM_APPROXIMATION
    reliability_level = atoi(argv[4]);
#endif
    
    //Read input data from file
    file = fopen(inputFile, "r");
    if(file == NULL) {
      printf("ERROR: Unable to open file `%s'.\n", inputFile);
      exit(1);
    }
    rv = fscanf(file, "%i", &numOptions);
    if(rv != 1) {
      printf("ERROR: Unable to read from file `%s'.\n", inputFile);
      fclose(file);
      exit(1);
    }
    if(nThreads > numOptions) {
      printf("WARNING: Not enough work, reducing number of threads to match number of options.\n");
      nThreads = numOptions;
    }

#if !defined(ENABLE_THREADS) && !defined(ENABLE_OPENMP)
    if(nThreads != 1) {
        printf("Error: <nthreads> must be 1 (serial version)\n");
        exit(1);
    }
#endif

    // alloc spaces for the option data
    data_s = (fptype*)malloc(numOptions*sizeof(fptype));          // spot price
    data_strike = (fptype*)malloc(numOptions*sizeof(fptype));      // strike price
    data_r = (fptype*)malloc(numOptions*sizeof(fptype));          // risk-free interest rate
    data_divq = (fptype*)malloc(numOptions*sizeof(fptype));       // dividend rate
    data_v = (fptype*)malloc(numOptions*sizeof(fptype));          // volatility
    data_t = (fptype*)malloc(numOptions*sizeof(fptype));         // time to maturity or option expiration in years 
    data_OptionType = (char*)malloc(numOptions*sizeof(char));   // Option type.  "P"=PUT, "C"=CALL
    data_divs = (fptype*)malloc(numOptions*sizeof(fptype));       // dividend vals (not used in this test)
    data_DGrefval = (fptype*)malloc(numOptions*sizeof(fptype));   // DerivaGem Reference Value
    prices = (fptype*)malloc(numOptions*sizeof(fptype));

    for ( loopnum = 0; loopnum < numOptions; ++ loopnum )
    {
        rv = fscanf(file, "%f %f %f %f %f %f %c %f %f", &data_s[loopnum], &data_strike[loopnum], &data_r[loopnum], &data_divq[loopnum], &data_v[loopnum], &data_t[loopnum], &data_OptionType[loopnum], &data_divs[loopnum], &data_DGrefval[loopnum]);
        if(rv != 9) {
          printf("ERROR: Unable to read from file `%s'.\n", inputFile);
          fclose(file);
          exit(1);
        }
    }
    rv = fclose(file);
    if(rv != 0) {
      printf("ERROR: Unable to close file `%s'.\n", inputFile);
      exit(1);
    }
    
#ifdef AMHM_APPROXIMATION

    m5_add_approx(  (uint32_t)&prices[0], (uint32_t)(&prices[numOptions] + sizeof(fptype) - 1), reliability_level);
    m5_add_approx(  (uint32_t)&data_s[0], (uint32_t)(&data_s[numOptions] + sizeof(fptype) - 1), reliability_level);
    m5_add_approx(  (uint32_t)&data_strike[0], (uint32_t)(&data_strike[numOptions] + sizeof(fptype) - 1), reliability_level);
    m5_add_approx(  (uint32_t)&data_r[0], (uint32_t)(&data_r[numOptions] + sizeof(fptype) - 1), reliability_level);
    m5_add_approx(  (uint32_t)&data_divq[0], (uint32_t)(&data_divq[numOptions] + sizeof(fptype) - 1), reliability_level);
    m5_add_approx(  (uint32_t)&data_v[0], (uint32_t)(&data_v[numOptions] + sizeof(fptype) - 1), reliability_level);
    m5_add_approx(  (uint32_t)&data_t[0], (uint32_t)(&data_t[numOptions] + sizeof(fptype) - 1), reliability_level);
    //m5_add_approx(  (uint32_t)&data_OptionType[0], (uint32_t)(&data_OptionType[numOptions] + sizeof(char) - 1), reliability_level);
    m5_add_approx(  (uint32_t)&data_divs[0], (uint32_t)(&data_divs[numOptions] + sizeof(fptype) - 1), reliability_level);
    m5_add_approx(  (uint32_t)&data_DGrefval[0], (uint32_t)(&data_DGrefval[numOptions] + sizeof(fptype) - 1), reliability_level);
#endif

#ifdef ENABLE_THREADS
    MAIN_INITENV(,8000000,nThreads);
#endif
    printf("Num of Options: %d\n", numOptions);
    printf("Num of Runs: %d\n", NUM_RUNS);

#define PAD 256
#define LINESIZE 64

    buffer = (fptype *) malloc(5 * numOptions * sizeof(fptype) + PAD);

#ifdef AMHM_APPROXIMATION
    m5_add_approx(  (uint32_t)&buffer[0], (uint32_t)(&buffer[5 * numOptions] + sizeof(fptype) + PAD - 1), reliability_level);
#endif

    sptprice = (fptype *) (((unsigned long long)buffer + PAD) & ~(LINESIZE - 1));
    strike = sptprice + numOptions;
    rate = strike + numOptions;
    volatility = rate + numOptions;
    otime = volatility + numOptions;

    buffer2 = (int *) malloc(numOptions * sizeof(fptype) + PAD);
    otype = (int *) (((unsigned long long)buffer2 + PAD) & ~(LINESIZE - 1));

    for (i=0; i<numOptions; i++) {
        otype[i]      = (data_OptionType[i] == 'P') ? 1 : 0;
        sptprice[i]   = data_s[i];
        strike[i]     = data_strike[i];
        rate[i]       = data_r[i];
        volatility[i] = data_v[i];    
        otime[i]      = data_t[i];
    }

    printf("Size of data: %d\n", numOptions * (sizeof(fptype) + sizeof(fptype) + sizeof(fptype) + sizeof(fptype) + sizeof(fptype) + sizeof(fptype) + sizeof(fptype) + sizeof(fptype) + sizeof(char) + sizeof(int)));

#ifdef ENABLE_PARSEC_HOOKS
    __parsec_roi_begin();
#endif
#ifdef ENABLE_THREADS
    int tids[nThreads];
    for(i=0; i<nThreads; i++) {
        tids[i]=i;
        CREATE_WITH_ARG(bs_thread, &tids[i]);
    }
    WAIT_FOR_END(nThreads);
#else//ENABLE_THREADS
#ifdef ENABLE_OPENMP
    {
        int tid=0;
        omp_set_num_threads(nThreads);
        bs_thread(&tid);
    }
#else //ENABLE_OPENMP
#ifdef WIN32 
    if (nThreads > 1)
    {
        HANDLE threads[MAX_THREADS];
                int nums[MAX_THREADS];
                for(i=0; i<nThreads; i++) {
                        nums[i] = i;
                        threads[i] = CreateThread(0, 0, bs_thread, &nums[i], 0, 0);
                }
                WaitForMultipleObjects(nThreads, threads, TRUE, INFINITE);
    } else
#endif
    {
        int tid=0;
        bs_thread(&tid);
    }
#endif //ENABLE_OPENMP
#endif //ENABLE_THREADS
#ifdef ENABLE_PARSEC_HOOKS
    __parsec_roi_end();
#endif

    //Write prices to output file
    file = fopen(outputFile, "w");
    if(file == NULL) {
      printf("ERROR: Unable to open file `%s'.\n", outputFile);
      exit(1);
    }
    rv = fprintf(file, "%i\n", numOptions);
    if(rv < 0) {
      printf("ERROR: Unable to write to file `%s'.\n", outputFile);
      fclose(file);
      exit(1);
    }
    for(i=0; i<numOptions; i++) {
      rv = fprintf(file, "%.18f\n", prices[i]);
      if(rv < 0) {
        printf("ERROR: Unable to write to file `%s'.\n", outputFile);
        fclose(file);
        exit(1);
      }
    }
    rv = fclose(file);
    if(rv != 0) {
      printf("ERROR: Unable to close file `%s'.\n", outputFile);
      exit(1);
    }

#ifdef ERR_CHK
    printf("Num Errors: %d\n", numError);
#endif
//     free(data_s);
//     free(data_strike);
//     free(data_r);
//     free(data_divq);
//     free(data_v);
//     free(data_t);
//     free(data_OptionType);
//     free(data_divs);
//     free(data_DGrefval);
//     free(prices);

#ifdef ENABLE_PARSEC_HOOKS
    __parsec_bench_end();
#endif

    return 0;
}