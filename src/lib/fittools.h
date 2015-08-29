/*
   fittools.h

   Copyright (C) 2011-2012  Paolo Cretaro <lorddarthfener@gmail.com>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef FITTOOLS_H
#define FITTOOLS_H

#include "log.h"
#include "fitconfig.h"

#include <math.h>
#include <vector>

#define SQUARE(x) ((x)*(x))

using namespace std;

class FitTools
{

public:
    static inline double mean(const std::vector<double> &data, const std::vector<double> &weight)
    {
        double result = 0, sum_weight = 0;
        for(int i = 0; i < (int)data.size(); i++) {
            result += data.at(i) * weight.at(i);
            sum_weight += weight.at(i);
        }

        return result / sum_weight;

    }

    static inline double mean(const std::vector<double> &data)
    {
        double result = 0;
        for(int i = 0; i < (int)data.size(); i++) {
            result += data.at(i);
        }

        return result / data.size();
    }

    static inline vector<double> vector_product(const vector<double> &v1, const vector<double> &v2)
    {
        vector<double> result(v1.size());

        for(int i = 0; i < (int)v1.size(); i++)
            result.at(i) = v1.at(i) * v2.at(i);

        return result;

    }

    static inline pair<int,int> minmax_index(const std::vector<double> &vec)
    {
        int imin = 0, imax = 0;
        double min = vec.front();
        double max = vec.front();
        double tmp;

        for(unsigned i = 1; i < vec.size(); i++){
            tmp = vec.at(i);
            if(tmp < min) {
                min = tmp;
                imin = i;
            }
            else if(tmp > max) {
                max = tmp;
                imax = i;
            }
        }

        return make_pair(imin, imax);
    }

    struct LinearFitResult {
        double m, m_error;
        double q, q_error;
        double cov;
        double chi_square;
        double rss;
    };

    struct SlopeFitResult {
        double m, m_error;
        double chi_square;
        double rss;
    };

    struct HorizontalFitResult {
        double q, q_error;
        double chi_square;
        double rss;
    };

    struct TrapIntegrationResult {
        double area;
    };

    //TODO exponential and logarithmic result

    enum FitFunction {
        LINEAR_FIT = 0,
        TRAP_INTEGRATION,
        SLOPE_FIT, // not implemented starting from here
        HORIZONTAL_FIT,
        EXPONENTIAL_FIT,
        LOGARITHMIC_FIT
    };

    union FitResult {
        LinearFitResult _linear_result;
        TrapIntegrationResult _trap_integration_result;
        //TODO exponential and logarithmic
    };

    LinearFitResult getLinearResult();
    SlopeFitResult getSlopeResult();
    HorizontalFitResult getHorizontalResult();
    TrapIntegrationResult getTrapIntegrationResult();
    FitTools(vector<double> &x_array, vector<double> &y_array, vector<double> &errors_array, FitFunction fit_type);
    FitTools(vector<double> &x_array, vector<double> &y_array, double error, FitFunction fit_type);

    int printResult(ostream &sout);
    FitResult Fit();

private:
    vector<double> _xdata;
    vector<double> _ydata;
    vector<double> _yerrors;
    vector<double> _yweights;
    double _sum_weight;
    FitResult _fit_result;
    FitFunction _fit_type;


    int _prepare_fit();
    int _fit_linear();
    int _fit_slope();
    int _fit_horizontal();
    int _trap_integration();

    int _chi_square(FitFunction fit_type);
    int _print_linear(ostream &sout);
    int _print_slope(ostream &sout);
    int _print_horizontal(ostream &sout); //TODO print exp and log
    int _print_int_trap_integration(ostream &sout);
};

#endif // FITTOOLS_H

/* vim: set ts=4 sw=4 et: */
