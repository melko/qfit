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

#include "fittools.h"

using namespace std;

int main(int argc, char** argv)
{
  bool error = true;
  
  if(argc)

  for (int i=0;i<argc;i++){
    if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0){
//       display_usage();
      return(0);
    }
    if(strcmp(argv[i], "-e") == 0 || strcmp(argv[i], "--no-errors") == 0)
      error = false;
    
  }
  
  cout << "Hello World!" << endl;
  return 0;
}

/* vim: set ts=2 sw=2 et: */
