//#pragma once
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

	// Initialize the filter's parameters
	double fpp = 0.03;
	uint32_t n =  (1 << 10);
	uint32_t size = (-1)*(n * log(fpp)) / (pow(log(2), 2));
	uint8_t num_hash = 2;

	// Instances of the standard Bloom filter
	BFilter<std::string> bloom_filter_str(size*2, num_hash, fpp);
	BFilter<uint32_t> bloom_filter_uint(size*2, num_hash, fpp);

	uint32_t e = 1235;
	uint32_t test = 254788;

	cout << "Testing for elements of type uint32_t" << endl;
	cout << "-------------------------------------" << endl;
	bloom_filter_uint.insert(&e, 32);
	cout << "The answer for element " << e << " is: " << bloom_filter_uint.query(&e, 32) << endl;
	cout << "The answer for element " << test << " is: " << bloom_filter_uint.query(&test, 32) << endl;

	cout << "--------------------------------------------------------------------------" << endl;


	cout << "Testing for elements of type string" << endl;
	cout << "-------------------------------------" << endl;
	std::vector<std::string> elements = {"IF100", "NS101", "CS201", "CS204", "CS210", "CS300", "CS301", "CS308", "EE404", "IE600"};

	for(int i=0; i < elements.size(); i++){

		bloom_filter_str.insert(&elements[i], 32);
	}

	std::string test1 = "CS407";
	cout << "The answer for element " << test1 << " is: " << bloom_filter_str.query(&test1, 128) << endl;

	for(int j =0; j < elements.size(); j++){
		bool query_res = bloom_filter_str.query(&elements[j], 32);
		if (query_res == 1)
			cout << "The element " << elements[j]  << " exists and the query result is: " << query_res << endl;
		else
			cout << "The element " << elements[j]  << " does not exist and the query result is: " << query_res << endl;

	}




}
