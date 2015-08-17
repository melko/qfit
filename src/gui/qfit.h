/*
 * qfit.h
 *
 * Copyright (C) 2011-2012 by Paolo Cretaro <lorddarthfener@gmail.com>
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

#ifdef WITH_QWT
#include <qwt_plot_curve.h>
#include <qwt_plot_intervalcurve.h>

#include "ui_maindialog.h"
#else
#include "ui_maindialog_noqwt.h"
#endif

class qfit : public QWidget, private Ui_Maindialog
{
    Q_OBJECT
public:
    qfit();
    virtual ~qfit();
    int printResult();

#ifdef WITH_QWT
    int plotData();
#endif

public slots:
    void appendLog(const char *c);
    void changeFitType(int state);
    void startFitClicked();
    void toggleCustomError(int);
    void openFile();
    void cleanLog();
    void displayInfo();

#ifdef WITH_QWT
    void savePlot();
#endif

private:
    vector<double> xdata;
    vector<double> ydata;
    vector<double> yerrors;
    FitTools *fit;
    FitTools::FitFunction fit_type;

#ifdef WITH_QWT
    QwtPlotCurve *data_plot;
    QwtPlotIntervalCurve *range_plot;
    QwtPlotCurve *model_plot;
    int plotLinearData();
#endif

    int setupGui();
};

#endif // qfit_H

/* vim: set ts=4 sw=4 et: */
