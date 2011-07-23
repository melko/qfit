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

FitTools::FitTools(std::vector< double >& x_array, std::vector< double >& y_array, std::vector< double >& errors_array, FitTools::FitFunction fit_type)
{
  //TODO
}

FitTools::FitTools(std::vector< double >& x_array, std::vector< double >& y_array, double error, FitTools::FitFunction fit_type)
{
  //TODO
  _yerrors = vector<double>(y_array.size(), error);
}

FitTools::FitResult FitTools::Fit()
{
  switch(_fit_type){
    case LINEAR_FIT:
      _fit_linear();
      break;
    case SLOPE_FIT:
      _fit_slope();
      break;
    case HORIZONTAL_FIT:
      _fit_horizontal();
      break;
    case PARABOLIC_FIT: //TODO
    case EXPONENTIAL_FIT:
    case LOGARITMIC_FIT:
      break;
  }
  
  return _fit_result;
}


int FitTools::_fit_linear()
{
  double x_mean, y_mean, xy_mean, x2_mean;
  vector<double> x2 = vec_product(_xdata, _xdata);
  vector<double> xy = vec_product(_xdata, _ydata);
  
  x_mean = mean(_xdata, _yweights);
  y_mean = mean(_ydata, _yweights);
  xy_mean = mean(xy, _yweights);
  x2_mean = mean(x2, _yweights);
  
  _fit_result._linear_result.m = (xy_mean - x_mean*y_mean)/(x2_mean - x_mean*x_mean);
  _fit_result._linear_result.m_error = 1/sqrt((x2_mean - x_mean*x_mean)*_sum_weight);
  _fit_result._linear_result.q = (x2_mean*y_mean - x_mean*xy_mean)/(x2_mean - x_mean*x_mean);
  _fit_result._linear_result.q_error = _fit_result._linear_result.m_error*sqrt(x2_mean);
  _fit_result._linear_result.cov = -x_mean/((x2_mean - x_mean*x_mean)*_sum_weight);
  
  this->_fit_type = LINEAR_FIT;
  return 0;
}

int FitTools::_fit_slope()
{
  //TODO
  return 0;
}

int FitTools::_fit_horizontal()
{
  //TODO
  return 0;
}

double FitTools::mean(std::vector< double >& data, std::vector< double >& weight)
{
  double result = 0, sum_weight = 0;
  for(int i=0;i<(int)data.size();i++){
    result += data.at(i)*weight.at(i);
    sum_weight += weight.at(i);
  }
  
  return result/sum_weight;

}

double FitTools::mean(std::vector< double >& data)
{
  double result = 0;
  for(int i=0;i<(int)data.size();i++){
    result += data.at(i);
  }
  
  return result/data.size();
}

vector< double > FitTools::vec_product(std::vector< double >& v1, std::vector< double >& v2)
{
  vector<double> result(v1.size());
  
  for(int i=0;i<(int)v1.size();i++)
    result.at(i) = v1.at(i)*v2.at(i);
  
  return result;

}

int FitTools::_chi_square_linear()
{
  _fit_result._linear_result.chi_square = 0;
  
  for(int i=0;i<(int)_xdata.size();i++){
    _fit_result._linear_result.chi_square += SQUARE((_ydata.at(i) - _fit_result._linear_result.m * _xdata.at(i) - _fit_result._linear_result.q)/_yerrors.at(i));
  }
  
  return 0;
}

int FitTools::_chi_square_slope()
{
  _fit_result._slope_result.chi_square = 0;
  
  for(int i=0;i<(int)_xdata.size();i++){
    _fit_result._slope_result.chi_square += SQUARE((_ydata.at(i) - _fit_result._slope_result.m * _xdata.at(i))/_yerrors.at(i));
  }
  
  return 0;
}

int FitTools::_chi_square_horizontal()
{
  _fit_result._horizontal_result.chi_square = 0;
  
  for(int i=0;i<(int)_xdata.size();i++){
    _fit_result._horizontal_result.chi_square += SQUARE((_ydata.at(i) - _fit_result._horizontal_result.q)/_yerrors.at(i));
  }

  return 0;
}


/* vim: set ts=2 sw=2 et: */