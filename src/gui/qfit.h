/*
 * qfit.h
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

#ifndef qfit_H
#define qfit_H

#include <fittools.h>

#include <QString>
#include <QtGui/QWidget>
#include "ui_maindialog.h"

class qfit : public QWidget, private Ui_Maindialog
{
Q_OBJECT
public:
  qfit();
  virtual ~qfit();
  int printResult(FitTools* fit);
public slots:
  void appendLog(const char* c);
  void changeFitType(int state);
  void startFitClicked();
  void toggleCustomError(int);
  void openFile();
  void cleanLog();
private:
  FitTools::FitFunction fit_type;
};

#endif // qfit_H

/* vim: set ts=2 sw=2 et: */
