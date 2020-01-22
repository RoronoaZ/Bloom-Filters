# Bloom-Filters
This project is the implementation of several Bloom filter variants starting from the very first one, the *Standard Bloom filter*.
It also includes a parallelized implementation of the Bloom filter using *OpenMP*.

## Standard Bloom filter (Example)
```C++
double fpp = 0.03;
uint32_t n =  (1 << 10);
uint32_t size = (-1)*(n * log(fpp)) / (pow(log(2), 2));
uint8_t num_hash = 2;

BFilter<uint32_t> bloom_filter_uint(size*2, num_hash, fpp);
uint32_t e = 1235;
uint32_t test = 254788;

cout << "Testing for elements of type uint32_t" << endl;
cout << "-------------------------------------" << endl;
bloom_filter_uint.insert(&e, 32);
cout << "The answer for element " << e << " is: " << bloom_filter_uint.query(&e, 32) << endl;
cout << "The answer for element " << test << " is: " << bloom_filter_uint.query(&test, 32) << endl;
```

The result of the previous code would be:
```
The answer for element 1235 is: 1
The answer for element 254788 is: 0
```

## Building the project
First, you have to select which script you want to compile, the simple Bloom filter or its parallelized version, 
then all you have to do is to run the command ```make```
### Running it
```./bloom-filter```

## Parallelized Bloom filter
The **OpenMP** C++ library is used in this project to implement the parallelized Bloom filter. You can change the number of the 
threads from [BFilter.h](Bloom-Filters/Bloom-filter/BFilter.h).
For building the project in the parallelized verions, make sure to comment the simple version in the makefile and uncomment parallellized 
one.

### Running it
```
./par-bloom-filter [set size][number of distinct items][epsilon][delta][No threads][Zipf dist(Normal/Uniform)][Zipf degree][Bloom filter=1]
```
Example: ./par-bloom-filter 28 18 0.01 0.003 8 1 1.1 1
(Note: it the running scheme will be discared in further modificaitons)
