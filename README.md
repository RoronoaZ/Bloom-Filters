# Bloom-Filters
This project is the implementation of several Bloom filter variants starting from the very first one, the *Standard Bloom filter*.
It also includes a parallelized implementation of the Bloom filter using *OpenMP*.

## Standard Bloom filter (Example)
The elements insertion process is demonstarted in the following animation, in Bloom filter, there are two main operations, the insertion and the querying, for the *deletion* operation, there are many Bloom filter variants that perform it. 
In the example below, we suppose that number of the hash functions used is *k=2*, and the data stream contains alphabetical characters as a small demonstration. 
<p align="center">
<img align="center" width="600" height="193" src="https://github.com/RoronoaZ/Bloom-Filters/blob/master/bloom-filter.gif">
</p>

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

### Build
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
Example: ```./par-bloom-filter 28 18 0.01 0.003 8 1 1.1 1 ```
(Note: it the running scheme will be discared in further modificaitons)

## Counting Bloom filter
It is known that the *Counting Bloom filter (CBF)* has the property of using counters instead of bits in the *Standard Bloom filter* in order to support item deletion from the filter by decrementing the corresponding counters and returning the counter with the minimum value while querying the existence of an item. 
### Build
Follow the same previous procedure of uncommenting the compiling part of the *CBF* in ```Makefile```.
### Running it
```
./countingBF
```

## Spectral Bloom filter
The *Spectral Bloom filter (SBF)* is quite similar to the (CBF) structure with a change in the way of incrementing the filter counters, SBF increments only the minimum i<sup>th</sup> counters in order to minimize the rate of false positives.
Example: 

```C++
Spectral_BF_MI<uint32_t> sbf(size, num_hash, fpp);
uint32_t item2 = 44;
uint16_t query_res = sbf.Query(&item, 32, sbf.get_bit_vector());

sbf.insert(&item2, 32);
query_res = sbf.Query(&item, 32, sbf.get_bit_vector());
sbf.insert(&item2, 32);
sbf.insert(&item2, 32);

cout << "Testing the Spectral BF" << endl;
cout << "Query result for item " << item << " is: " << query_res << endl;
sbf.insert(&item, 32);

query_res = sbf.Query(&item, 32, sbf.get_bit_vector());
cout << "Query result for item " << item << " is: " << query_res << endl;
```
The result of the previous code:
```

```
