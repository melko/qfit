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

FitTools::FitTools(std::vector< double > x_array, std::vector< double > y_array, std::vector< double > errors_array)
{
  this->noerror = false;
}

FitTools::FitTools(std::vector< double > x_array, std::vector< double > y_array)
{
  this->noerror = true;
}


FitTools::LinearFitResult FitTools::_fit_linear()
{
  LinearFitResult result;
  
  
}

FitTools::SlopeFitResult FitTools::_fit_slope()
{

}

FitTools::HorizontalFitResult FitTools::_fit_horizontal()
{

}

double FitTools::mean(std::vector< double > data, std::vector< double > weight)
{

}

double FitTools::mean(std::vector< double > data)
{
  
}


/* vim: set ts=2 sw=2 et: */
