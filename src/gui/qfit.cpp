/*
 * qfit.cpp
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

#include "qfit.h"
#include "about.h"

#include <libfile.h>
#include <fitconfig.h>
#include <fittools.h>

#include <vector>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QAction>
#include <QtGui/QImageWriter>
#include <QtWidgets/QFileDialog>

#ifdef WITH_QWT
#include <qwt_plot_canvas.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_renderer.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_zoomer.h>
#include <qwt_interval_symbol.h>
#include <qwt_symbol.h>
#endif

qfit::qfit()
{
    fit_type = FitTools::LINEAR_FIT;

#ifdef WITH_QWT
    data_plot = NULL;
    range_plot = NULL;
    model_plot = NULL;
#endif

    fit = NULL;

    setupUi(this);


    setupGui();

    connect(openFileButton, SIGNAL(clicked(bool)), this, SLOT(openFile()));
    connect(customError, SIGNAL(stateChanged(int)), this, SLOT(toggleCustomError(int)));
    connect(startFit, SIGNAL(clicked(bool)), this, SLOT(startFitClicked()));
    connect(selectFit, SIGNAL(currentIndexChanged(int)), this, SLOT(changeFitType(int)));
    connect(cleanLogButton, SIGNAL(clicked(bool)), this, SLOT(cleanLog()));
    connect(infoButton, SIGNAL(clicked(bool)), this, SLOT(displayInfo()));

#ifdef WITH_QWT
    connect(savePlotButton, SIGNAL(clicked(bool)), this, SLOT(savePlot()));
#endif
}

qfit::~qfit()
{}

void qfit::openFile()
{
    filePath->setText(QFileDialog::getOpenFileName(this, tr("Open"), "", trUtf8("Text File (*.txt);;All Files (*)")));
}

void qfit::cleanLog()
{
    logOutput->clear();
}

void qfit::displayInfo()
{
    About *about = new About(this);
    about->exec();
}

#ifdef WITH_QWT
void qfit::savePlot()
{
    QString imageFilter;
    const QList<QByteArray> imageFormats = QImageWriter::supportedImageFormats();
    if(imageFormats.size() > 0) {
        for(int i = 0; i < imageFormats.size(); i++) {
            imageFilter += "Image ";
            imageFilter += imageFormats[i];
            imageFilter += " (*.";
            imageFilter += imageFormats[i];
            imageFilter += ");;";
        }
    }
#ifndef QWT_NO_SVG
    imageFilter += "File SVG (*.svg)";
#endif
    imageFilter += ";;File PDF (*.pdf);;File Postscript (*.ps)";
    QString fileName = QFileDialog::getSaveFileName(this, trUtf8("Export"), "",
                       imageFilter, NULL, QFileDialog::DontConfirmOverwrite);
    if(!fileName.isEmpty()) {
        QwtPlotRenderer renderer;
        renderer.setDiscardFlag(QwtPlotRenderer::DiscardBackground, false);
        renderer.renderDocument(qwtPlot, fileName, QSizeF(300, 200), 85);
    }
}
#endif

void qfit::toggleCustomError(int state)
{
    // NOTE: if the box is checked the state is 2
    if(state)
        errorValue->setReadOnly(false);
    else
        errorValue->setReadOnly(true);
}

void qfit::changeFitType(int state)
{
    switch(state) {
    case FitTools::LINEAR_FIT:
        fit_type = FitTools::LINEAR_FIT;
        appendLog("Fit Linear set");
        break;
    case FitTools::SLOPE_FIT:
        fit_type = FitTools::SLOPE_FIT;
        appendLog("Fit Slope set");
        break;
    case FitTools::HORIZONTAL_FIT:
        fit_type = FitTools::HORIZONTAL_FIT;
        appendLog("Fit Horizontal set");
        break;
    case FitTools::EXPONENTIAL_FIT:
        fit_type = FitTools::EXPONENTIAL_FIT;
        appendLog("Fit Exponential set");
        break;
    case FitTools::LOGARITHMIC_FIT:
        fit_type = FitTools::LOGARITHMIC_FIT;
        appendLog("Fit Logarithmic set");
        break;
    case FitTools::TRAP_INTEGRATION:
        fit_type = FitTools::TRAP_INTEGRATION;
        appendLog("Trapezoidal Integration set");
        appendLog("WARNING: data is not sorted when calculating the area!");
        break;
    }
}


void qfit::startFitClicked()
{
    if(filePath->text().isEmpty()) {
        appendLog("Select a file to open!");
        return;
    }
    double error = -1;

    // retrive error from the QLineEdit
    if(customError->isChecked()) {
        error = errorValue->text().toDouble();
        if(error <= 0.0) {
            appendLog("Custom error is not valid, a default one of 1.0 will be used");
            error = 1;
        }
    }

    if(Data::ReadFile(filePath->text().toLatin1().data(), xdata, ydata, yerrors, error)) {
        appendLog("Error opening file");
        return;
    }

    // start fittools
    delete fit;
    if(error <= 0)
        fit = new FitTools(xdata, ydata, yerrors, fit_type);
    else
        fit = new FitTools(xdata, ydata, error, fit_type);
    fit->Fit();
    printResult();

#ifdef WITH_QWT
    plotData();
#endif
}

#ifdef WITH_QWT
int qfit::plotData()
{
    delete data_plot;
    delete range_plot;
    delete model_plot;

    plotScatter();

    switch(fit_type) {
    case FitTools::LINEAR_FIT:
    case FitTools::HORIZONTAL_FIT:
    case FitTools::SLOPE_FIT:
        plotLinearModel();
        break;
        //TODO fare gli altri casi
    case FitTools::TRAP_INTEGRATION:
        break;
    case FitTools::EXPONENTIAL_FIT:
    case FitTools::LOGARITHMIC_FIT:
        return(1);
        break;
    }
    qwtPlot->setAxisAutoScale(QwtPlot::xBottom);
    qwtPlot->setAxisAutoScale(QwtPlot::yLeft);
    qwtPlot->replot();
    savePlotButton->setEnabled(true);
    return(0);
}

int qfit::plotScatter()
{
    /* standard data */
    data_plot = new QwtPlotCurve("data");
    data_plot->setSamples(&xdata.at(0), &ydata.at(0), xdata.size());
    data_plot->setSymbol(new QwtSymbol(QwtSymbol::Ellipse, QBrush(Qt::black), QPen(Qt::black), QSize(8, 8)));
    data_plot->setStyle(QwtPlotCurve::NoCurve);
    data_plot->setRenderHint(QwtPlotItem::RenderAntialiased);

    /* error bars */
    range_plot = new QwtPlotIntervalCurve("range");

    QVector<QwtIntervalSample> range(xdata.size());
    for(int i = 0; i < (int)xdata.size(); i++) {
        range[i] = QwtIntervalSample(xdata.at(i), ydata.at(i) - yerrors.at(i), ydata.at(i) + yerrors.at(i));
    }

    QwtIntervalSymbol *errorbar = new QwtIntervalSymbol(QwtIntervalSymbol::Bar);
    errorbar->setPen(QPen(Qt::black, 1));
    range_plot->setSamples(range);
    range_plot->setSymbol(errorbar);
    range_plot->setStyle(QwtPlotIntervalCurve::NoCurve);

    data_plot->attach(qwtPlot);
    range_plot->attach(qwtPlot);
    return(0);
}

int qfit::plotLinearModel()
{
    FitTools::LinearFitResult model = fit->getLinearResult();
    double x[2], y[2];
    pair< int, int> minmax_pair = FitTools::minmax_index(xdata);
    x[0] = xdata.at(minmax_pair.first);
    x[1] = xdata.at(minmax_pair.second);
    y[0] = model.m * x[0] + model.q;
    y[1] = model.m * x[1] + model.q;

    model_plot = new QwtPlotCurve("y=mx+q");
    model_plot->setSamples(x, y, 2);
    model_plot->setPen(QPen(Qt::red, 1));
    model_plot->setRenderHint(QwtPlotItem::RenderAntialiased);

    model_plot->attach(qwtPlot);
    return(0);
}

#endif

void qfit::appendLog(const char *c)
{
    logOutput->appendPlainText(trUtf8(c));
}

int qfit::printResult()
{
    stringstream sout;
    fit->printResult(sout);
    appendLog(sout.str().c_str());
    return 0;
}

int qfit::setupGui()
{
//   if(QApplication::arguments().size() > 1)
//     filePath->setText(QApplication::arguments().at(1));
    if(QCoreApplication::arguments().size() > 1) {
        filePath->setText(QCoreApplication::arguments().at(1));
    }
    selectFit->addItem(tr("Fit Linear"));
    selectFit->addItem(tr("Trapezoidal Integration"));
    /*TODO add once are available in FitTools
    selectFit->addItem(tr("Fit Slope"));
    selectFit->addItem(tr("Fit Horizontal"));
    selectFit->addItem(tr("Fit Exponential"));
    selectFit->addItem(tr("Fit Logarithmic"));*/

#ifdef WITH_QWT
    // the latest version of qwt now returns a QWidget when asking for the canvas
    QwtPlotCanvas *canvas = (QwtPlotCanvas*) qwtPlot->canvas();

    // plot stuff
    // panning with the middle mouse button
    QwtPlotPanner *panner = new QwtPlotPanner(canvas);
    panner->setMouseButton(Qt::MidButton);
    // zoom in/out with the wheel
    (void) new QwtPlotMagnifier(canvas);
    // zoom an area with left button
    QwtPlotZoomer *zoomer = new QwtPlotZoomer(canvas);
    zoomer->setRubberBandPen(QColor(Qt::black));
    zoomer->setTrackerPen(QColor(Qt::black));
    zoomer->setMousePattern(QwtEventPattern::MouseSelect2, Qt::RightButton, Qt::ControlModifier);
    zoomer->setMousePattern(QwtEventPattern::MouseSelect3, Qt::RightButton);

    // canvas
    canvas->setLineWidth(1);
    canvas->setFrameStyle(QFrame::Box | QFrame::Plain);
    canvas->setStyleSheet(
        "border: 2px solid Black;"
        "border-radius: 10px;"
        "background-color: qlineargradient( x1: 0, y1: 0, x2: 0, y2: 1,"
            "stop: 0 LemonChiffon, stop: 1 PaleGoldenrod );"
    );

    //grid
    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->setMajorPen(QPen(Qt::gray, 0, Qt::DotLine));
//   grid->setMinorPen(QPen(Qt::gray, 0, Qt::DotLine));
    grid->attach(qwtPlot);

    // save button tooltip
    savePlotButton->setToolTip(trUtf8("Save plot"));
#endif

    // tooltips
    infoButton->setToolTip(trUtf8("About"));
    openFileButton->setToolTip(trUtf8("Open file"));
    selectFit->setToolTip(trUtf8("Fit typology"));
    startFit->setToolTip(trUtf8("GO!"));
    cleanLogButton->setToolTip(trUtf8("Clear log"));

    return(0);
}

/* vim: set ts=4 sw=4 et: */
