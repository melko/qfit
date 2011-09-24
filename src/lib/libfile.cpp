/*
    libfile.cpp
    
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


#include "libfile.h"
#include <stdlib.h>

int Data::ReadFile(char* path,				/* percorso del file */
		   vector< double >& xdata,
		   vector< double >& ydata, 
		   vector< double >& yerrors, 
		   double err)				/* true == array yerrors */
{
  ifstream file(path);
  
  if(!file.is_open()){
    cout << "Errore nell'apertura del file\n";
    exit(-2);
  }
  
  string line;
  while(getline(file, line)){
    stringstream str(line);
    double tmp;
    
    str >> tmp;
    xdata.push_back(tmp);
    
    str >> tmp;
    ydata.push_back(tmp);
    
    if(err < 0){
      str >> tmp;
      yerrors.push_back(tmp);
    }
  }
  
  file.close();

}
