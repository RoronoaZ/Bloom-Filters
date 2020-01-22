#include <iostream>
#include <algorithm>
#include <vector>
#include <array>
#include <time.h>
#include "common.h"
#include <cstdlib>
#include "omp.h"
#include <chrono>
#include "BFilter.h"
#include <string.h>
#include <vector>

using namespace std;
using namespace std::chrono;

int main(int argc, char** argv){

        cout << "Testing for elements using parallelism (openmp)" << endl;
        cout << "-------------------------------------" << endl;

	        // Initialize the filter's parameters
        double fpp = 0.05;
        uint32_t n =  (1 << 25);
        uint32_t size = (-1)*(n * log(fpp)) / (pow(log(2), 2));
        //uint8_t num_hash = 2;

        // Instances of the standard Bloom filter
        BFilter<uint32_t> bloom_filter_uint(size, 4, fpp);

	bloom_filter_uint.std_bloom_filter(argc, argv);


}
