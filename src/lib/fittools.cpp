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
  _xdata = x_array;
  _ydata = y_array;
  _yerrors = errors_array;
  _fit_type = fit_type;
  _sum_weight = 0;
}

FitTools::FitTools(std::vector< double >& x_array, std::vector< double >& y_array, double error, FitTools::FitFunction fit_type)
{
  _xdata = x_array;
  _ydata = y_array;
  _yerrors = vector<double>(y_array.size(), error);
  _fit_type = fit_type;
  _sum_weight = 0;
}

int FitTools::_prepare_fit()
{
  _sum_weight = 0;
  for(int i=0;i<(int)_ydata.size();i++){
    _yweights.push_back(SQUARE(1.0/_yerrors.at(i)));
    _sum_weight += _yweights.at(i);
  }
  
  return 0;
}

/*
 * chiama la funzione di fit appropriata a seconda della variabile _fit_type
*/ 
FitTools::FitResult FitTools::Fit()
{
  _prepare_fit();
  
  switch(_fit_type){
    case LINEAR_FIT:
      _fit_linear();
      _chi_square_linear();
      break;
    case SLOPE_FIT:
      _fit_slope();
      _chi_square_slope();
      break;
    case HORIZONTAL_FIT:
      _fit_horizontal();
      _chi_square_horizontal();
      break;
    case EXPONENTIAL_FIT: //TODO
    case LOGARITMIC_FIT: //TODO
      break;
  }
  
  return _fit_result;
}

/*
 * fit lineare
*/
int FitTools::_fit_linear()
{
  double x_mean, y_mean, xy_mean, x2_mean;
  vector<double> x2 = vector_product(_xdata, _xdata);
  vector<double> xy = vector_product(_xdata, _ydata);
  
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

/*
 * fit lineare solo pendenza
*/
int FitTools::_fit_slope()
{
  //TODO
  return 0;
}

/*
 * fit lineare solo altezza
*/
int FitTools::_fit_horizontal()
{
  //TODO
  return 0;
}

/*
 * calcola la media pesata di un vettore
*/
double FitTools::mean(const std::vector< double >& data,	/* vettore di cui calcolare la media */
		      const std::vector< double >& weight)	/* vettore dei pesi */
{
  double result = 0, sum_weight = 0;
  for(int i=0;i<(int)data.size();i++){
    result += data.at(i)*weight.at(i);
    sum_weight += weight.at(i);
  }

  return result/sum_weight;

}

/*
 * calcola la media aritmetica di un vettore
*/
double FitTools::mean(const std::vector< double >& data)
{
  double result = 0;
  for(int i=0;i<(int)data.size();i++){
    result += data.at(i);
  }
  
  return result/data.size();
}

/*
 * calcola un vettore le cui componenti sono il prodotto delle componenti dei
 * vettori in entrata
*/
vector< double > FitTools::vector_product(const std::vector< double >& v1, const std::vector< double >& v2)
{
  vector<double> result(v1.size());
  
  for(int i=0;i<(int)v1.size();i++)
    result.at(i) = v1.at(i)*v2.at(i);

  return result;

}

/*
 * calcola il chi quadro su un'aspettativa lineare
*/
inline int FitTools::_chi_square_linear()
{
  _fit_result._linear_result.chi_square = 0;
  
  for(int i=0;i<(int)_xdata.size();i++){
    _fit_result._linear_result.chi_square += SQUARE((_ydata.at(i) - _fit_result._linear_result.m * _xdata.at(i) - _fit_result._linear_result.q)/_yerrors.at(i));
  }
  
  return 0;
}

/*
 * calcola il chi quadro su un'aspettativa lineare solo pendenza
*/
inline int FitTools::_chi_square_slope()
{
  _fit_result._slope_result.chi_square = 0;
  
  for(int i=0;i<(int)_xdata.size();i++){
    _fit_result._slope_result.chi_square += SQUARE((_ydata.at(i) - _fit_result._slope_result.m * _xdata.at(i))/_yerrors.at(i));
  }
  
  return 0;
}

/*
 * calcola il chi quadro su un'aspettativa lineare solo altezza
*/
inline int FitTools::_chi_square_horizontal()
{
  _fit_result._horizontal_result.chi_square = 0;
  
  for(int i=0;i<(int)_xdata.size();i++){
    _fit_result._horizontal_result.chi_square += SQUARE((_ydata.at(i) - _fit_result._horizontal_result.q)/_yerrors.at(i));
  }

  return 0;
}


/* vim: set ts=2 sw=2 et: */