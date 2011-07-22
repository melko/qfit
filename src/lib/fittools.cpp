/*
   fittools.cpp

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


#include "fittools.h"

FitTools::FitTools(std::vector< double >* x_array, std::vector< double >* y_array, std::vector< double >* errors_array)
{

}

FitTools::FitTools(std::vector< double >* x_array, std::vector< double >* y_array, double error)
{
  yerrors = vector<double>(y_array->size(), error);
}


FitTools::LinearFitResult FitTools::_fit_linear()
{
  LinearFitResult result;
  double x_mean, y_mean, xy_mean, x2_mean, sum_weight;
  vector<double> x2 = vec_product(&xdata, &xdata);
  vector<double> xy = vec_product(&xdata, &ydata);
  
  x_mean = mean(&xdata, &yweights);
  y_mean = mean(&ydata, &yweights);
  xy_mean = mean(&xy, &yweights);
  x2_mean = mean(&x2, &yweights);
  
  result.m = (xy_mean - x_mean*y_mean)/(x2_mean - x_mean*x_mean);
  result.m_error = 1/sqrt((x2_mean - x_mean*x_mean)*sum_weight);
  result.q = (x2_mean*y_mean - x_mean*xy_mean)/(x2_mean - x_mean*x_mean);
  result.q_error = result.m_error*sqrt(x2_mean);
  result.cov = -x_mean/((x2_mean - x_mean*x_mean)*sum_weight);
  
  return result;
}

FitTools::SlopeFitResult FitTools::_fit_slope()
{

}

FitTools::HorizontalFitResult FitTools::_fit_horizontal()
{

}

double FitTools::mean(vector< double > *data, vector< double > *weight)
{
  double result = 0, sum_weight = 0;
  for(int i=0;i<data->size();i++){
    result += data->at(i)*weight->at(i);
    sum_weight += weight->at(i);
  }
  
  return result/sum_weight;

}

double FitTools::mean(vector< double > *data)
{
  double result = 0;
  for(int i=0;i<data->size();i++){
    result += data->at(i);
  }
  
  return result/data->size();
}

vector< double > FitTools::vec_product(vector< double >* v1, vector< double >* v2)
{
  vector<double> result(v1->size());
  
  for(int i=0;i<v1->size();i++)
    result.at(i) = v1->at(i)*v2->at(i);
  
  return result;

}


/* vim: set ts=2 sw=2 et: */
