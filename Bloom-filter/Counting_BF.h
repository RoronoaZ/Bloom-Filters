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

	uint32_t query(chosenType *item, std::size_t len, std::vector<uint16_t> m_bits){
		auto hashValues = BFilter<chosenType>::hashFunc(item, len);
		auto min = 1 << 20;

		for (int n =0 ; n < num_hashes; n++){
			if ( m_bits[BFilter<chosenType>::hash_to_vec(n, hashValues[0], hashValues[1], m_bits.size())] < min)
				min = m_bits[BFilter<chosenType>::hash_to_vec(n, hashValues[0], hashValues[1], m_bits.size())];
		}

		return min;

	}

	std::vector<uint16_t> get_bit_vector(){
		return this->m_bits;
	}

};


template <class chosenType>
class Spectral_BF_MI : public Counting_BF<chosenType>{


protected:
	uint8_t num_hashes;
	std::vector<uint16_t> bit_vector;
	double fpp;

public:
	Spectral_BF_MI(uint64_t size, uint8_t numHashes, double falsefp):
		num_hashes(numHashes),
		bit_vector(size),
		fpp(falsefp), Counting_BF<chosenType>(size, numHashes,falsefp){};



	void insert(chosenType *item, std::size_t len){
		auto hashValues = BFilter<chosenType>::hashFunc(item, len);
		//auto min = std::distance(bit_vector.begin(), std::min_element(bit_vector.begin(), bit_vector.end()));
		auto min = 1 << 20;
		for (int n = 0; n < num_hashes; n++) {

			if(bit_vector[BFilter<chosenType>::hash_to_vec(n, hashValues[0], hashValues[1], bit_vector.size())] < min)
				min = bit_vector[BFilter<chosenType>::hash_to_vec(n, hashValues[0], hashValues[1], bit_vector.size())];
		}


		for (int n = 0; n < num_hashes; n++) {

			if(bit_vector[BFilter<chosenType>::hash_to_vec(n, hashValues[0], hashValues[1], bit_vector.size())] == min)
				bit_vector[BFilter<chosenType>::hash_to_vec(n, hashValues[0], hashValues[1], bit_vector.size())] += 1;
		}
	


	}

	std::vector<uint16_t> get_bit_vector(){
//		cout << "Entered to the SBF version "<< endl;
		return this->bit_vector;
	}



	uint32_t Query(chosenType *item, std::size_t len, std::vector<uint16_t> bit_vector){
		return this->query(item, len, bit_vector);

	}

};
