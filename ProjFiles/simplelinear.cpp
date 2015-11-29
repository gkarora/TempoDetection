//
//  simplelinear.cpp
//  FYDP
//
//  Created by Canmanie Ponnambalam on 2015-11-24.
//  Copyright © 2015 Canmanie Ponnambalam. All rights reserved.
//

#include "simplelinear.hpp"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cmath>
using namespace std;

double calcSD(double slopes[], int j){
    double sd = 0;
    double mean = 0;
    double sum = 0;
    
    for (int i = 0; i<j; i++) {
        sum+=slopes[i];
    }
    
    mean = sum/j;
    
    for (int i = 0; i<j; i++) {
        sd = sqrt(pow((slopes[i]-mean),2)/(j-1));
    }
    
    return sd;
//    cout << "\n" << "standard deviation = ";
//    cout << sd;
}

double linSlope(double x[], double y[], int n)
{
    double intersect = 0;
    double slope = 0;
    
    double sumx = 0;
    double sumy = 0;
    double sumxsquared = 0;
    double sumxy = 0;
    
    for (int i = 0; i<n; i++) {
        sumx+=x[i];
        sumy+=y[i];
        sumxsquared+=pow(x[i],2);
        sumxy+=x[i]*y[i];
    }

    slope = ((n*sumxy)-(sumx*sumy))/((n*sumxsquared)-pow(sumx,2));
    intersect = ((sumy*sumxsquared)-sumx*sumxy)/((n*sumxsquared)-pow(sumx,2));

    return slope; //return slope
}