/*
 * qfit.cpp
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

#include "qfit.h"
#include "about.h"

#include <libfile.h>
#include <fitconfig.h>
#include <fittools.h>

#include <vector>
#include <QtGui/QLabel>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QAction>
#include <QtGui/QImageWriter>
#include <QtGui/QFileDialog>
#include <qwt_plot_canvas.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_renderer.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_zoomer.h>
#include <qwt_interval_symbol.h>
#include <qwt_symbol.h>

qfit::qfit()
{
  fit_type = FitTools::LINEAR_FIT;
  data_plot = NULL;
  range_plot = NULL;
  model_plot = NULL;
  fit = NULL;

  setupUi(this);
  
  
  setupGui();
  
  connect(openFileButton, SIGNAL(clicked(bool)), this, SLOT(openFile()));
  connect(customError, SIGNAL(stateChanged(int)), this, SLOT(toggleCustomError(int)));
  connect(startFit, SIGNAL(clicked(bool)), this, SLOT(startFitClicked()));
  connect(selectFit, SIGNAL(currentIndexChanged(int)), this, SLOT(changeFitType(int)));
  connect(cleanLogButton, SIGNAL(clicked(bool)), this, SLOT(cleanLog()));
  connect(infoButton, SIGNAL(clicked(bool)), this, SLOT(displayInfo()));
  connect(savePlotButton, SIGNAL(clicked(bool)), this, SLOT(savePlot()));
}

qfit::~qfit()
{}

void qfit::openFile()
{
  filePath->setText(QFileDialog::getOpenFileName(this, tr("Apri"), "", trUtf8("File di Testo (*.txt);;Tutti i File (*)")));
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

void qfit::savePlot()
{
  QString imageFilter;
  const QList<QByteArray> imageFormats = QImageWriter::supportedImageFormats();
  if(imageFormats.size() > 0){
    for(int i=0;i<imageFormats.size();i++){
      imageFilter += "Immagine ";
      imageFilter += imageFormats[i];
      imageFilter += " (*.";
      imageFilter += imageFormats[i];
      imageFilter += ");;";
    }
  }
#ifndef QWT_NO_SVG
  imageFilter += "Documento SVG (*.svg)";
#endif
  imageFilter += ";;Documento PDF (*.pdf);;Documento Postscript (*.ps)";
  QString fileName = QFileDialog::getSaveFileName(this, trUtf8("Esporta"), "",
						  imageFilter, NULL, QFileDialog::DontConfirmOverwrite);
  if(!fileName.isEmpty()){
    QwtPlotRenderer renderer;
    renderer.setDiscardFlag(QwtPlotRenderer::DiscardBackground, false);
    renderer.renderDocument(qwtPlot, fileName, QSizeF(300,200), 85);
  }
}

void qfit::toggleCustomError(int state)
{
  // if the box is checked the state is 2
  if(state)
    errorValue->setReadOnly(false);
  else
    errorValue->setReadOnly(true);
}

void qfit::changeFitType(int state)
{
  switch(state){
    case FitTools::LINEAR_FIT:
      fit_type = FitTools::LINEAR_FIT;
      appendLog("Impostato fit lineare");
      break;
    case FitTools::SLOPE_FIT:
      fit_type = FitTools::SLOPE_FIT;
      appendLog("Impostato fit pendenza");
      break;
    case FitTools::HORIZONTAL_FIT:
      fit_type = FitTools::HORIZONTAL_FIT;
      appendLog("Impostato fit orizzontale");
      break;
    case FitTools::EXPONENTIAL_FIT:
      fit_type = FitTools::EXPONENTIAL_FIT;
      appendLog("Impostato fit esponenziale");
      break;
    case FitTools::LOGARITMIC_FIT:
      fit_type = FitTools::LOGARITMIC_FIT;
      appendLog("Impostato fit logaritmico");
      break;
  }
}


void qfit::startFitClicked()
{
  if(filePath->text().isEmpty()){
    appendLog("Non è stato selezionato nessun file da aprire!");
    return;
  }
  double error = -1;

  // retrive error from the QLineEdit
  if(customError->isChecked()){
    error = errorValue->text().toDouble();
    if(error <= 0.0){
      appendLog("L'errore inserito non è valido, verrà usato un errore standard pari a 1.0");
      error = 1;
    }
  }

  if(Data::ReadFile(filePath->text().toLatin1().data(), xdata, ydata, yerrors, error)){
    appendLog("Errore nell'apertura del file");
    return;
  }

  // start fittools
  delete fit;
  if(error<=0)
    fit = new FitTools(xdata, ydata, yerrors, fit_type);
  else
    fit = new FitTools(xdata, ydata, error, fit_type);
  fit->Fit();
  printResult();
  
  plotData();
}

int qfit::plotData()
{
  delete data_plot;
  delete range_plot;
  delete model_plot;
  
  switch(fit_type){
    case FitTools::LINEAR_FIT:
    case FitTools::HORIZONTAL_FIT:
    case FitTools::SLOPE_FIT:
      plotLinearData();
      break;
      //TODO fare gli altri casi
    case FitTools::EXPONENTIAL_FIT:
    case FitTools::LOGARITMIC_FIT:
      return(1);
      break;
  }
  qwtPlot->setAxisAutoScale(QwtPlot::xBottom);
  qwtPlot->setAxisAutoScale(QwtPlot::yLeft);
  qwtPlot->replot();
  savePlotButton->setEnabled(true);
  return(0);
}


int qfit::plotLinearData()
{
  /* standard data */
  data_plot = new QwtPlotCurve("data");
  data_plot->setSamples(&xdata.at(0), &ydata.at(0), xdata.size());
  data_plot->setSymbol(new QwtSymbol(QwtSymbol::XCross, Qt::NoBrush, QPen(Qt::black), QSize(8,8)));
  data_plot->setStyle(QwtPlotCurve::NoCurve);
  data_plot->setRenderHint(QwtPlotItem::RenderAntialiased);
  
  /* error bars */
  range_plot = new QwtPlotIntervalCurve("range");
  
  QVector<QwtIntervalSample> range(xdata.size());
  for(int i=0;i<(int)xdata.size();i++){
    range[i] = QwtIntervalSample(xdata.at(i), ydata.at(i)-yerrors.at(i), ydata.at(i)+yerrors.at(i));
  }
  
  QwtIntervalSymbol *errorbar = new QwtIntervalSymbol(QwtIntervalSymbol::Bar);
  errorbar->setPen(QPen(Qt::black, 1));
  range_plot->setSamples(range);
  range_plot->setSymbol(errorbar);
  range_plot->setStyle(QwtPlotIntervalCurve::NoCurve);
  
  /* model */
  FitTools::LinearFitResult tmp = fit->getLinearResult();
  double x[2], y[2];
  x[0] = xdata.at(0);
  x[1] = xdata.back();
  y[0] = tmp.m*x[0] + tmp.q;
  y[1] = tmp.m*x[1] + tmp.q;
  
  model_plot = new QwtPlotCurve("y=mx+q");
  model_plot->setSamples(x, y, 2);
  model_plot->setPen(QPen(Qt::red, 1));
  model_plot->setRenderHint(QwtPlotItem::RenderAntialiased);
  
  data_plot->attach(qwtPlot);
  range_plot->attach(qwtPlot);
  model_plot->attach(qwtPlot);
  return(0);
}


void qfit::appendLog(const char* c)
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
  selectFit->addItem(tr("Fit Lineare"));/*TODO add once are available in FitTools
  selectFit->addItem(tr("Fit Pendenza"));
  selectFit->addItem(tr("Fit Orizzontale"));
  selectFit->addItem(tr("Fit Esponenziale"));
  selectFit->addItem(tr("Fit Logaritmico"));*/
  
  // plot stuff
  // panning with the middle mouse button
  QwtPlotPanner *panner = new QwtPlotPanner(qwtPlot->canvas());
  panner->setMouseButton(Qt::MidButton);
  // zoom in/out with the wheel
  (void) new QwtPlotMagnifier(qwtPlot->canvas());
  // zoom an area with left button
  QwtPlotZoomer *zoomer = new QwtPlotZoomer(qwtPlot->canvas());
  zoomer->setRubberBandPen(QColor(Qt::black));
  zoomer->setTrackerPen(QColor(Qt::black));
  zoomer->setMousePattern(QwtEventPattern::MouseSelect2, Qt::RightButton, Qt::ControlModifier);
  zoomer->setMousePattern(QwtEventPattern::MouseSelect3, Qt::RightButton);
  
  // canvas
  qwtPlot->canvas()->setLineWidth(1);
  qwtPlot->canvas()->setFrameStyle(QFrame::Box | QFrame::Plain);
  qwtPlot->canvas()->setBorderRadius(10);
  qwtPlot->canvas()->setPalette(Qt::darkGray);
  //grid
  QwtPlotGrid *grid = new QwtPlotGrid();
  grid->setMajPen(QPen(Qt::white, 0, Qt::DotLine));
//   grid->setMinPen(QPen(Qt::gray, 0, Qt::DotLine));
  grid->attach(qwtPlot);
  
  // tooltips
  infoButton->setToolTip(trUtf8("Info"));
  openFileButton->setToolTip(trUtf8("Apri un file"));
  selectFit->setToolTip(trUtf8("Tipologia del fit"));
  startFit->setToolTip(trUtf8("Via"));
  cleanLogButton->setToolTip(trUtf8("Svuota il log"));
  savePlotButton->setToolTip(trUtf8("Salva il grafico"));
  return(0);
}

#include "qfit.moc"

/* vim: set ts=2 sw=2 et: */