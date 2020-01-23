#mainmake: test-simple-Bloom-filter.cpp common.h generator.h MurmurHash3.h MurmurHash3.cpp BFilter.h
#	g++ -std=c++11 -o bloom-filter test-simple-Bloom-filter.cpp common.h generator.h MurmurHash3.h MurmurHash3.cpp BFilter.h -O3 -fopenmp
#mainmake: test-parallel_bloom_filter.cpp common.h generator.h MurmurHash3.h MurmurHash3.cpp BFilter.h
#	g++ -std=c++11 -o par-bloom-filter test-parallel_bloom_filter.cpp common.h generator.h MurmurHash3.h MurmurHash3.cpp BFilter.h -O3 -fopenmp
mainmake: CBF.cpp MurmurHash3.h MurmurHash3.cpp BFilter.h
	g++ CBF.cpp Counting_BF.h MurmurHash3.cpp -o countingBF -O3 -fopenmp

