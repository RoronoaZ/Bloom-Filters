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
#include "BFilter.h"

using namespace std;

template <class chosenType>
class Counting_BF : public BFilter<chosenType>
{


protected:
	uint8_t num_hashes;
	std::vector<uint16_t> m_bits;
	double fpp;

public:

	Counting_BF(uint64_t size, uint8_t numHashes, double falseposprob):
		num_hashes(numHashes),
		m_bits(size),
		fpp(falseposprob), BFilter<chosenType>(size, numHashes,falseposprob){};


	void insert(chosenType *item, std::size_t len){
		auto hashValues = BFilter<chosenType>::hashFunc(item, len);
//		cout << "Insert of CBF "<< endl;
		for (int n = 0; n < num_hashes; n++) {
			m_bits[BFilter<chosenType>::hash_to_vec(n, hashValues[0], hashValues[1], m_bits.size())] += 1;
		}

	}

	void deletion(chosenType *item, std::size_t len){
		auto hashValues = BFilter<chosenType>::hashFunc(item, len);

		for (int n =0; n < num_hashes; n++){
			m_bits[BFilter<chosenType>::hash_to_vec(n, hashValues[0], hashValues[1], m_bits.size())] -= 1;
		}

	}

	uint32_t query(chosenType *item, std::size_t len){
		auto hashValues = BFilter<chosenType>::hashFunc(item, len);
		uint32_t min = m_bits[BFilter<chosenType>::hash_to_vec(0, hashValues[0], hashValues[1], m_bits.size())];
		for (int n =1 ; n < num_hashes; n++){
			if ( m_bits[BFilter<chosenType>::hash_to_vec(n, hashValues[0], hashValues[1], m_bits.size())] < min)
				min = m_bits[BFilter<chosenType>::hash_to_vec(n, hashValues[0], hashValues[1], m_bits.size())];
		}

		return min;

	}


};
