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

#include <vector>

using namespace std;

class FitTools
{ 

public:
  static double mean(vector<double> data, vector<double> weight);
  static double mean(vector<double> data);
  
  struct LinearFitResult {
    double m, m_error;
    double q, q_error;
    double cov;
  };
  
  struct SlopeFitResult {
    double m, m_error;
  };
  
  struct HorizontalFitResult {
    double q, q_error;
  };

  enum FitFunction {
    LINEAR_FIT,
    SLOPE_FIT,
    HORIZONTAL_FIT,
    PARABOLIC_FIT,
    EXPONENTIAL_FIT,
    LOGARITMIC_FIT
  };

  FitTools(vector<double> x_array, vector<double> y_array, vector<double> errors_array);
  FitTools(vector<double> x_array, vector<double> y_array);

private:
  bool noerror;
  
  vector<long> xdata;
  vector<long> ydata;
  vector<long> yerrors;

  FitTools::LinearFitResult _fit_linear();
  FitTools::SlopeFitResult _fit_slope();
  FitTools::HorizontalFitResult _fit_horizontal();
};

#endif // FITTOOLS_H

/* vim: set ts=2 sw=2 et: */
