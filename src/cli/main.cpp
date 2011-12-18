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
#include <iomanip>
#include <cstring>
#include <vector>

#include "fittools.h"
#include "libfile.h"

using namespace std;

int display_usage(const char* name);
int display_version();

int main(int argc, char** argv)
{
  char* file_path = NULL;
  double error = -1;
  FitTools::FitFunction fit_type = FitTools::LINEAR_FIT;

  if(argc<2){
    display_usage(argv[0]);
    return(-1);
  }

  /* scorrimento dei parametri */
  for (int i=1;i<argc;i++){
    if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0){
      display_usage(argv[0]);
      return(0);
    }
    else if(strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0){
      return(display_version());
    }
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
    else if(strcmp(argv[i], "-lo") == 0 || strcmp(argv[i], "--logarithmic") == 0)
      fit_type = FitTools::LOGARITHMIC_FIT;
    else
      file_path = argv[i];
  }

  if(file_path == NULL)
    return(display_usage(argv[0]));
  
  vector<double> xdata, ydata, yerrors;
  if(Data::ReadFile(file_path, xdata, ydata, yerrors, error))
    return(-2);
  
  fit_type = FitTools::LINEAR_FIT; //TODO remove this after implementing other fit methods

  /* start fittools */
  FitTools *fit;
  if(error<=0){
    fit = new FitTools(xdata, ydata, yerrors, fit_type);
  }
  else{
    fit = new FitTools(xdata, ydata, error, fit_type);
  }
  fit->Fit();
  fit->printResult(cout);
  return 0;
}

int display_version()
{
  cout << "cfit " << QFIT_VERSION_MAJOR << "." << QFIT_VERSION_MINOR << "." << QFIT_VERSION_PATCH
       << "\nCopyright (C) 2011 Paolo Cretaro <lorddarthfener@gmail.com>.\n"
       << "License GPLv3+: GNU GPL version 3 <http://gnu.org/licenses/gpl.html>.\n"
       << "This is free software: you are free to change and redistribute it.\n"
       << "There is NO WARRANTY, to the extent permitted by law.\n\n"
       << "Written by Paolo Cretaro.\n";
  return -1;
}
int display_usage(const char* name)
{
  cout << "Usage: " << name << " [OPTIONS]... FILE\n\n"
       << "  -h, --help\t\tprint this help\n"
       << "  -e, --error NUM\tuse NUM as custom error value\n"
       << "  -li, --linear\t\tdo a linear regression (default)\n"
       << "\n" //TODO add other fit types
       << "FILE must be a text file with three columns (or two if you use a custom error),\n"
       << "the first is used for x data, the second for y data and the third for y errors.\n"
       << "When using a custom error (-e,--error), the third column is ignored.\n"
       << "\n"
       << "Examples:\n"
       << "  " << name << " foo.txt\n"
       << "  " << name << " -e 0.426 foo.txt\n"
       << "\n"
       << "Report bugs to: https://github.com/melko/qfit/issues\n"
       << "                <lorddarthfener@gmail.com>\n"
       << "QFit home page: https://github.com/melko/qfit\n";
  return -1;
}

/* vim: set ts=2 sw=2 et: */
