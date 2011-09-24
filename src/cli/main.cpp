/*
 * main.cpp
 *
 * Copyright (C) 2011 by Paolo Cretaro <lorddarthfener@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <iostream>
#include <cstring>
#include <vector>

#include "fittools.h"
#include "libfile.h"

using namespace std;

int display_usage();
int print_result(FitTools::FitResult fit_result, FitTools::FitFunction fit_type);

int main(int argc, char** argv)
{
  char* file_path = NULL;
  double error = -1;
  FitTools::FitFunction fit_type = FitTools::LINEAR_FIT;

  if(argc<2){
    display_usage();
    return(-1);
  }

  /* scorrimento dei parametri */
  for (int i=1;i<argc;i++){
    if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
      return(display_usage());
    else if(strcmp(argv[i], "-e") == 0 || strcmp(argv[i], "--error") == 0){
      stringstream tmp(argv[1+i++]);
      tmp >> error;
      if(error<=0){
	cout << "Inserito un errore non valido!\n";
	return(-3);
      }
    }
    else if(strcmp(argv[i], "-li") == 0 || strcmp(argv[i], "--linear") == 0)
      fit_type = FitTools::LINEAR_FIT;
    else if(strcmp(argv[i], "-sl") == 0 || strcmp(argv[i], "--slope") == 0)
      fit_type = FitTools::SLOPE_FIT;
    else if(strcmp(argv[i], "-ho") == 0 || strcmp(argv[i], "--horizontal") == 0)
      fit_type = FitTools::HORIZONTAL_FIT;
    else if(strcmp(argv[i], "-ex") == 0 || strcmp(argv[i], "--exponential") == 0)
      fit_type = FitTools::EXPONENTIAL_FIT;
    else if(strcmp(argv[i], "-lo") == 0 || strcmp(argv[i], "--logaritmic") == 0)
      fit_type = FitTools::LOGARITMIC_FIT;
    else
      file_path = argv[i];
  }

  if(file_path == NULL)
    return(display_usage());
  
  vector<double> xdata, ydata, yerrors;
  Data::ReadFile(file_path, xdata, ydata, yerrors, error);
  
  fit_type = FitTools::LINEAR_FIT; //TODO remove this after implementing other fit methods

  /* start fittools */
  FitTools *fit;
  if(error<0){
    fit = new FitTools(xdata, ydata, yerrors, fit_type);
  }
  else{
    fit = new FitTools(xdata, ydata, error, fit_type);
  }
  FitTools::FitResult result = fit->Fit();
  print_result(result, fit_type);
  return 0;
}

int display_usage()
{
  cout << "Display "
       << "usage\n";
  return -1;
}

int print_result(FitTools::FitResult fit_result, FitTools::FitFunction fit_type)
{
  switch(fit_type){
    case FitTools::LINEAR_FIT:
      cout << "m = " << fit_result._linear_result.m << "\ts(m) = " << fit_result._linear_result.m_error << endl
           << "q = " << fit_result._linear_result.q << "\ts(q) = " << fit_result._linear_result.q_error << endl
           << "cov(m,q) = " << fit_result._linear_result.cov << endl
           << "X² = " << fit_result._linear_result.chi_square << endl;
      break;
    case FitTools::SLOPE_FIT: //TODO
    case FitTools::HORIZONTAL_FIT:
    case FitTools::EXPONENTIAL_FIT:
    case FitTools::LOGARITMIC_FIT:
      break;
  }
  return 0;
}

/* vim: set ts=2 sw=2 et: */