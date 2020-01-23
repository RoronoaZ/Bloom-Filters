#include <iostream>
#include <algorithm>
#include <vector>
#include <array>
#include <time.h>
#include "common.h"
#include <cstdlib>
#include "omp.h"
#include <chrono>
//#include "BFilter.h"
#include <string.h>
#include <vector>
#include "Counting_BF.h"

using namespace std;

int main(int argc, char** argv){


	double fpp = 0.05;
	uint32_t n = (1 << 10);
	uint32_t size = (-1)*(n * log(fpp)) / (pow(log(2), 2));
	uint8_t num_hash = 4;

	Counting_BF<uint32_t> cbf(size, num_hash, fpp);

	uint32_t item = 435;
	cbf.insert(&item, 32);
	cbf.insert(&item, 32);
	cbf.insert(&item, 32);
	uint32_t q_result = cbf.query(&item, 32);
	if (q_result == 0)
		cout << "The item " << item << " does not exist and query result is: " << cbf.query(&item, 32) << endl;
	else
		cout << "The item " << item << " exists and query result is: " << cbf.query(&item, 32) << endl;

}
