//
//  parallel_calculation_of_pi.h
//
//  Created by Gaurav Chawla on 17/10/21.
//

#ifndef parallel_calculation_of_pi_h
#define parallel_calculation_of_pi_h

#include <iostream>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <stdlib.h>
#include <thread>
#include <iomanip>
#include <future>
#include <sys/time.h>

using namespace std;

void serial();
void EstimatePi( promise<double> && p );
void parallel();

#endif /* parallel_calculation_of_pi_h */
