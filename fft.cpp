
#include <fstream>
#include <string>
#include <cmath>
#include <complex>
#include <iostream>
#include <iomanip>
#include <ctime>


#define n 1024
using namespace std;

extern void fft(double *x_in, std::complex<double> *x_out, int N);
void fft_rec(std::complex<double> *x, int N);
double diffclock(clock_t start, clock_t end);


double diffclock(clock_t start, clock_t end)
{
    double delta_t = end - start;
    double delta_t_ms=(delta_t)/(CLOCKS_PER_SEC/1000);
    return delta_t_ms;
}


void fft(double *x_in, std::complex<double>* x_out, int N)
{
	// Make copy of array and apply window
    
	for (int i = 0; i < N; i++)
        x_out[i] = std::complex<double>(x_in[i], 0);
 
	
    clock_t start = clock(); //timer start
	fft_rec(x_out, N); // start recursion
    clock_t end = clock(); //tiemer end
    std::cout << diffclock(start,end) << std::endl; //print fft() time in ms
}


void fft_rec(std::complex<double> *x, int N) 
{
	// Check if it is splitted enough
	if(N <= 1) return;
	
	// split even and odd
	std::complex<double> odd[N/2];
	std::complex<double> even[N/2];
	for(int i = 0; i < N/2; i++){
		even[i] = x[i*2];
		odd[i] = x[i*2+1];
	}

	// recursively splits the odd and even functions - results in 
  // array of bit reversal order
	fft_rec(even, N/2);
	fft_rec(odd, N/2);
    
	// calculate FFT()
	for(int k = 0; k < N/2; k++){
        std::complex<double> G2 = exp(std::complex<double>(0, (-2*M_PI*k)/N)) * odd[k];
		x[k] = even[k] + G2;
		x[(N/2) + k]= even[k] - G2;
	}
}


int main ( void )
{
	std::complex<double> out[n];
    double signal[n];

    int count = 0;
    
    /* prepare a cosine wave off the fly  */
    for (int i = 0; i < n; i++)
        signal[i] = cos(512 * 2*M_PI*i/n);
    
    fft(signal, out, n); //compute the fft()
    
    //stream data to .txt file
    std::ofstream outfile ("data.txt");
    for(int i = 0; i < n ; i++)
          outfile << abs(out[i]) << std::endl;
    outfile.close();

	return 0;
}
