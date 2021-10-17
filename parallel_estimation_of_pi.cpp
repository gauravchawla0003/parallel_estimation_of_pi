//
//  parallel_calculation_of_pi.cpp
//
//  Created by Gaurav Chawla on 17/10/21.
//

#include "parallel_estimation_of_pi.h"


#define INTERVAL 10000000
int noOfProcessors = 8;

/* Serial C++ program for estimation of Pi value using Monte Carlo Simulation */

void serial(){
    using namespace chrono;
    auto startTime = high_resolution_clock::now();

    long unsigned int i = 0;
    double rand_x=0.0, rand_y=0.0, origin_dist=0.0, pi=0;
    long unsigned int circle_points = 0, square_points = 0;
  
    // Initializing rand()
    srand((unsigned int)(time(NULL)));
    
    // Total Random numbers generated = possible x
    // values * possible y values
    for (i = 0; i < (INTERVAL); i++) {
        // Randomly generated x and y values
        rand_x = double(rand()) / RAND_MAX;
        rand_y = double(rand()) / RAND_MAX;
  
        // Distance between (x, y) from the origin
        origin_dist = rand_x * rand_x + rand_y * rand_y;
  
        // Checking if (x, y) lies inside the define
        // circle with R=1
        if (origin_dist <= 1)
            circle_points++;
  
        // Total number of points generated
        square_points++;
        
    }
  
    // estimated pi after this iteration
    pi = double(4 * circle_points) / square_points;
    
    
    auto endTime = high_resolution_clock::now();
    auto timTaken = duration_cast<microseconds>(endTime - startTime);
    // Final Estimated Value
    cout << "Final estimated value of Pi = " << pi <<endl;
    cout<<"[Serial] Total time taken in microseconds    : "<< timTaken.count()<<" us"<<endl;
}


/* Parallel C++ program for estimation of Pi value using Monte Carlo Simulation */

void parallel(){
    using namespace chrono;
    
    thread thArray[noOfProcessors];
    promise<double> p[noOfProcessors];
    future<double> f[noOfProcessors];
    double returnValueFromThread[noOfProcessors];
    
    auto startTime = high_resolution_clock::now();

    //set the future field for every promise
    for(int i=0; i< noOfProcessors; ++i){
        f[i] = p[i].get_future();
    }

    for(int i=0; i<noOfProcessors; ++i){
        thArray[i] = thread(EstimatePi , std::move(p[i]) );
    }

    for(int i=0; i<noOfProcessors; ++i){
        thArray[i].join();
        returnValueFromThread[i] = f[i].get();
    }
    
    auto endTime = high_resolution_clock::now();
    auto timTaken = duration_cast<microseconds>(endTime - startTime);
    cout<<"[Parallel] Total time taken in microseconds  : "<< timTaken.count()<<" us"<<endl;

    // to print the return value of every thread
    //for(auto x : returnValueFromThread){
    //    cout<<"pi : "<<x<<endl;
    //}
}

void EstimatePi( promise<double> && p ){
    long unsigned int i= 0;
    double rand_x=0.0, rand_y=0.0, origin_dist=0.0, pi=0;
    long unsigned int square_points = 0;
    long unsigned int circle_points = 0;

    // Initializing local seed for every thread
    unsigned int mySeed = (unsigned int) time(NULL);
    unsigned int Total_ITR = INTERVAL/noOfProcessors;

    // Total Random numbers generated = possible x
    // values * possible y values
    for (i = 0; i < Total_ITR; i++) {
        
        // Randomly generated x and y values
        rand_x = double(rand_r(&mySeed)) / RAND_MAX;
        rand_y = double(rand_r(&mySeed)) / RAND_MAX;
  
        // Distance between (x, y) from the origin
        origin_dist = rand_x * rand_x + rand_y * rand_y;
  
        // Checking if (x, y) lies inside the define
        // circle with R=1
        if (origin_dist <= 1)
            circle_points++;
  
        // Total number of points generated
        square_points++;

    }

    pi = double(4 * circle_points) / square_points;
    p.set_value(pi);
}

