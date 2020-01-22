//#pragma once
//#include "pch.h"
//#include "BFilter.h"
#include <vector>
#include <time.h>
#include <random>
#include <algorithm>
#include <string.h>
#include "omp.h"
#include <math.h>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <math.h>
#include "common.h"
#include "generator.h"
#include "MurmurHash3.h"
#include <array>
#include <chrono>

using namespace std;
using namespace std::chrono;

unsigned long long no_stream, no_unique;
unsigned no_max_threads;
double epsilon, delta;
int distNum;
double zipfAlp;

template <class chosenType>
class BFilter
{

private:
	uint8_t n_num_hashes;
	std::vector<uint8_t> m_bits;    // Getting only a vector of bits which has one bit for each item (space efficiency)
	double fpp;

public:
	//BFilter(uint64_t BF_size, uint8_t num_hashes);

	//void insert(const uint8_t *item, std::size_t length);
	//bool query(const uint8_t *item, std::size_t length) const;

	std::array<uint64_t, 2> hashFunc(chosenType *item, std::size_t length) {

		std::array<uint64_t, 2> hashValue;
		MurmurHash3_x64_128(item, length, 0, hashValue.data());

		return hashValue;

	}

	uint64_t hash_to_vec(uint8_t n,
		uint64_t hashA,
		uint64_t hashB,
		uint64_t filterSize) {
		return (hashA + n * hashB) % filterSize;
	}

	std::vector<uint32_t> static generate_sizes(std::vector<uint32_t> elements_num, double fpp) {
		std::vector<uint32_t> sizes;
		for (int i = 0; i < elements_num.size(); i++) {
			sizes.push_back((-1)*(elements_num[i] * log(fpp)) / (pow(log(2), 2)));
		}

		return sizes;
	}


	std::vector<int> static optimal_hash_num(std::vector<uint32_t> m, std::vector<uint32_t> n) {
		std::vector<int> hash_funcs;
		for (int i = 0; i < m.size(); i++) {
			hash_funcs.push_back((m[i] / n[i]) * log(2));
		}

		return hash_funcs;
	}



	//=================Filter's functions====================//
	void insert(chosenType *item, std::size_t len) {
		auto hashValues = hashFunc(item, len);

		for (int n = 0; n < n_num_hashes; n++) {
			m_bits[hash_to_vec(n, hashValues[0], hashValues[1], m_bits.size())] = 1;
		}
	}

	bool query(chosenType *item, std::size_t len) {
		auto hashValues = hashFunc(item, len);

		for (int n = 0; n < n_num_hashes; n++) {
			if (m_bits[hash_to_vec(n, hashValues[0], hashValues[1], m_bits.size())] != 1) {
				return false;
			}
		}

		return true;
	}


	int static parseInputs(int argc, char** argv) {
		if (argc >= 6) {
			no_stream = pow(2, atoi(argv[1]));
			no_unique = pow(2, atoi(argv[2]));
			epsilon = atof(argv[3]);
			delta = atof(argv[4]);
			no_max_threads = atoi(argv[5]);
			distNum = atoi(argv[6]);
			zipfAlp = atof(argv[7]);
			return 0;
		}
		else {
			return -1;
		}
	}

	int static std_bloom_filter(int argc, char** argv) {

		if (parseInputs(argc, argv) == -1) {
			std::cout << "Wrong number of arguments" << std::endl;
			std::cout << "Usage: executable [log2(stream size)] [log2(universal set size)] [epsilon] [delta] [no max threads] [distribution no] " << std::endl;
			std::cout << "\tNORMAL 1\n\tUNIFORM 2\n\tPOISSON 3\n\tEXPONENTIAL 4" << std::endl;
			return -1;
		}

		//std::vector<uint32_t> sizes = { 6236, 12471, 31176, 62352, 623522, 6235224, 12470448,
			//12470448, 40265318, 40265318, 40265318, 187056726, 120000000, 120000000 };
		std::vector<uint32_t> elements_num = { 1 << 10, 2 * 1 << 10, 4 * 1 << 10, 8 * 1 << 10, 1 << 20, 2 * 1 << 20, 4 * 1 << 20, 8 * 1 << 20, 1 << 25, 1 << 28};
		//std::vector<int> num_hashes = { 4, 4, 4, 4, 4, 4, 4, 5, 16, 5, 3, 4, 4, 2 };
		const float fpp = 0.05;
		clock_t time;
		dtype* data;

		//Generate the various filter sizes and its corresponding hash function numbers
		std::vector<uint32_t> sizes = BFilter::generate_sizes(elements_num, fpp);
		std::vector<int> num_hashes = BFilter::optimal_hash_num(sizes, elements_num);

		// Generate the data
		generateData(*&data, no_stream, no_unique, distNum, zipfAlp);
		cout << "The number of items is:" << no_stream << endl;


		std::ofstream Log;
		Log.open("log.csv");
		Log << "Number of inserted elements(n)" << "," << "Filter Size (m) in bits" << "," << "Filter Size (m) in bytes" << ","
			<< "Filter Size (m) in Mbytes" << "," << "Number of Hash Funcs(k)" << "," << "Building Time(s)" << "," << "FPP = 5% "
			<< endl;
		uint32_t a = 20;
		uint32_t b = 432;

		//Build the filter for different sizes
		for (int iter = 0; iter < sizes.size(); iter++) {

			BFilter bf(sizes[iter], num_hashes[iter], 0.05); // (m,k)
			//time = clock();
			auto start = high_resolution_clock::now();
			bf.build_filter(*&data, 128, elements_num[iter]); //(,,n) 
			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<microseconds>(stop - start);
//			time = clock() - time;
			cout << "It took " << duration.count()*pow(10, -6) /*((float)time) / CLOCKS_PER_SEC*/ << " seconds to build the filter" << endl;
			cout << "The memory used for " << elements_num[iter] << " is " << sizes[iter]/(pow(10, 6)*8) << " megabytes, and using "
				<< num_hashes[iter] << " hash functions" << endl;
			float size_bits = (-1)*(elements_num[iter] * (log(0.05) / (pow(log(2), 2))));
			float size_bytes = size_bits / 8;
			float size_Mbytes = size_bits / (8 * 1000 * 1000);
			Log << elements_num[iter] << "," << size_bits << "," << size_bytes << "," << size_Mbytes << "," << num_hashes[iter]
				<< "," << ((float)time) / CLOCKS_PER_SEC << "," << fpp << endl;

		}


		Log.close();

	}


	void build_filter(dtype* data, std::size_t len, uint32_t n) {
		//BFilter bf(n, 3);
		cout << "======================================================" << endl;
		cout << "The number of inserted items is " << n << endl;
#pragma omp parallel num_threads(80)
#pragma omp for
		for (int i = 0; i < n; i++) {
			insert((chosenType *)&data[i], len);
		}
		//return bf;
	}

	BFilter(uint64_t size, uint8_t numHashes, double falsePosProb) :
		n_num_hashes(numHashes),
		m_bits(size),
		fpp(falsePosProb)
	{
		//cout << "Bloom filter Creation" << endl << "--------------------" << endl;

	};

	virtual ~BFilter()
	{};


};

