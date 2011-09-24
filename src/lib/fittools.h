/*
   fittools.h

   Copyright (C) 2011  Paolo Cretaro <lorddarthfener@gmail.com>

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

#include <math.h>
#include <vector>

#define SQUARE(x) (x*x)

using namespace std;

class FitTools
{ 

public:
  static inline double mean(const std::vector< double >& data, const std::vector< double >& weight);
  static inline double mean(const std::vector< double >& data);
  static inline vector<double> vector_product(const vector<double> &v1, const vector<double> &v2);
  
  struct LinearFitResult {
    double m, m_error;
    double q, q_error;
    double cov;
    double chi_square;
  };
  
  struct SlopeFitResult {
    double m, m_error;
    double chi_square;
  };
  
  struct HorizontalFitResult {
    double q, q_error;
    double chi_square;
  };

  enum FitFunction {
    LINEAR_FIT,
    SLOPE_FIT,
    HORIZONTAL_FIT,
    EXPONENTIAL_FIT,
    LOGARITMIC_FIT
  };
  
  union FitResult{
    LinearFitResult _linear_result;
    SlopeFitResult _slope_result;
    HorizontalFitResult _horizontal_result;
  };

  FitTools(vector<double> &x_array, vector<double> &y_array, vector<double> &errors_array, FitFunction fit_type);
  FitTools(vector<double> &x_array, vector<double> &y_array, double error, FitFunction fit_type);

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
  
  inline int _chi_square_linear();
  inline int _chi_square_slope();
  inline int _chi_square_horizontal();
};

#endif // FITTOOLS_H

/* vim: set ts=2 sw=2 et: */
