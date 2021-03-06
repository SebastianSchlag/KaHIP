/******************************************************************************
 * random_functions.h
 *
 * Source of KaHIP -- Karlsruhe High Quality Graph Partitioning 
 ******************************************************************************
 * Copyright (C) 2017 Christian Schulz 
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *****************************************************************************/

#ifndef RANDOM_FUNCTIONS_RMEPKWYT
#define RANDOM_FUNCTIONS_RMEPKWYT

#include <iostream>
#include <tr1/random>
#include <vector>

#include "definitions.h"
#include "partition_config.h"

typedef std::tr1::mt19937 MersenneTwister;

class random_functions {
        public:
                random_functions();
                virtual ~random_functions();

                template<typename sometype>
                        static void circular_permutation(std::vector<sometype> & vec) {
                                if(vec.size() < 2) return;
                                for( ULONG i = 0; i < vec.size(); i++) {
                                        vec[i] = i;
                                }                   

                                ULONG size = vec.size();
                                std::tr1::uniform_int<ULONG> A(0,size-1);
                                std::tr1::uniform_int<ULONG> B(0,size-1);

                                for( ULONG i = 0; i < size; i++) {
                                        ULONG posA = A(m_mt);
                                        ULONG posB = B(m_mt);

                                        while(posB == posA) {
                                                posB = B(m_mt);
                                        }

                                        if( posA != vec[posB] && posB != vec[posA]) {
                                                std::swap(vec[posA], vec[posB]); 
                                        }
                                } 

                         }

                template<typename sometype>
                        static void permutate_vector_fast(std::vector<sometype> & vec, bool init) {
                                if(init) {
                                        for( ULONG i = 0; i < vec.size(); i++) {
                                                vec[i] = i;
                                        }                   
                                }
                                
                                if(vec.size() < 10) return;
                                        
                                int distance = 20; 
                                std::tr1::uniform_int<unsigned int> A(0, distance);
                                ULONG size = vec.size()-4;
                                for( ULONG i = 0; i < size; i++) {
                                        ULONG posA = i;
                                        ULONG posB = (posA + A(m_mt))%size;
                                        std::swap(vec[posA], vec[posB]);
                                        std::swap(vec[posA+1], vec[posB+1]); 
                                        std::swap(vec[posA+2], vec[posB+2]); 
                                        std::swap(vec[posA+3], vec[posB+3]); 
                                }               
                        }

                template<typename sometype>
                        static void permutate_vector_good(std::vector<sometype> & vec, bool init) {
                                if(init) {
                                        for( ULONG i = 0; i < vec.size(); i++) {
                                                vec[i] = (sometype)i;
                                        }                   
                                }

                                if(vec.size() < 10) { 
                                        permutate_vector_good_small(vec);
                                        return;
                                }
                                ULONG size = vec.size();
                                std::tr1::uniform_int<ULONG> A(0,size - 4);
                                std::tr1::uniform_int<ULONG> B(0,size - 4);

                                for( ULONG i = 0; i < size; i++) {
                                        ULONG posA = A(m_mt);
                                        ULONG posB = B(m_mt);
                                        std::swap(vec[posA], vec[posB]); 
                                        std::swap(vec[posA+1], vec[posB+1]); 
                                        std::swap(vec[posA+2], vec[posB+2]); 
                                        std::swap(vec[posA+3], vec[posB+3]); 

                                } 
                        }

                template<typename sometype>
                        static void permutate_vector_good_small(std::vector<sometype> & vec) {
                                if(vec.size() < 2) return;
                                ULONG size = vec.size();
                                std::tr1::uniform_int<ULONG> A(0,size-1);
                                std::tr1::uniform_int<ULONG> B(0,size-1);

                                for( ULONG i = 0; i < size; i++) {
                                        ULONG posA = A(m_mt);
                                        ULONG posB = B(m_mt);
                                        std::swap(vec[posA], vec[posB]); 
                                } 
                        }

                template<typename sometype>
                        static void permutate_entries(const PPartitionConfig & partition_config, 
                                                      std::vector<sometype> & vec, 
                                                      bool init) {
                                if(init) {
                                        for( ULONG i = 0; i < vec.size(); i++) {
                                                vec[i] = i;
                                        }                   
                                }

                                switch(partition_config.permutation_quality) {
                                        case PERMUTATION_QUALITY_NONE: break;
                                        case PERMUTATION_QUALITY_FAST: permutate_vector_fast(vec, false); break;
                                        case PERMUTATION_QUALITY_GOOD: permutate_vector_good(vec, false); break;
                                }      

                        }

                static bool nextBool() {
                        std::tr1::uniform_int<short> A(0,1);
                        return (bool) A(m_mt); 
                }

                //including lb and rb
                template<typename sometype>
                        static sometype nextInt(sometype lb, sometype rb) {
                                std::tr1::uniform_int<sometype> A(lb,rb);
                                return A(m_mt); 
                        }


                static double nextDouble(double lb, double rb) {
                        double rnbr   = (double) rand() / (double) RAND_MAX; // rnd in 0,1
                        double length = rb - lb;
                        rnbr         *= length;
                        rnbr         += lb;

                        return rnbr; 
                }

                static void setSeed(int seed) {
                        m_seed = seed;
                        srand(seed);
                        m_mt.seed(m_seed);
                }

        private:
                static int m_seed;
                static MersenneTwister m_mt;
};

#endif /* end of include guard: RANDOM_FUNCTIONS_RMEPKWYT */
