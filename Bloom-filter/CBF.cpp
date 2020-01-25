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
	uint32_t q_result = cbf.query(&item, 32, cbf.get_bit_vector());
	if (q_result == 0)
		cout << "The item " << item << " does not exist and query result is: " << cbf.query(&item, 32, cbf.get_bit_vector()) << endl;
	else
		cout << "The item " << item << " exists and query result is: " << cbf.query(&item, 32, cbf.get_bit_vector()) << endl;

	Spectral_BF_MI<uint32_t> sbf(size, num_hash, fpp);
	uint32_t item2 = 44;
	uint16_t query_res = sbf.Query(&item, 32, sbf.get_bit_vector());

	sbf.insert(&item, 32);
	query_res = sbf.Query(&item, 32, sbf.get_bit_vector());
	sbf.insert(&item, 32);
	sbf.insert(&item, 32);

	cout << "Testing the Spectral BF" << endl;
	cout << "Query result for item " << item << " is: " << query_res << endl;
	sbf.insert(&item, 32);

	query_res = sbf.Query(&item, 32, sbf.get_bit_vector());
	cout << "Query result for item " << item << " is: " << query_res << endl;
}
