/*
   fittools.cpp

   Copyright (C) 2011-2012  Paolo Cretaro <lorddarthfener@gmail.com>

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

FitTools::FitTools(std::vector< double > &x_array, std::vector< double > &y_array, std::vector< double > &errors_array, FitTools::FitFunction fit_type)
    : _xdata(x_array),
      _ydata(y_array),
      _yerrors(errors_array),
      _sum_weight(0),
      _fit_type(fit_type)
{
}

FitTools::FitTools(std::vector< double > &x_array, std::vector< double > &y_array, double error, FitTools::FitFunction fit_type)
    : _xdata(x_array),
      _ydata(y_array),
      _yerrors(vector<double>(y_array.size(), error)),
      _sum_weight(0),
      _fit_type(fit_type)
{
}

/*
 * crea l'array dei pesi come 1/yerr^2
 */
int FitTools::_prepare_fit()
{
    _sum_weight = 0;
    for(int i = 0; i < (int)_ydata.size(); i++) {
        _yweights.push_back(SQUARE(1.0 / _yerrors.at(i)));
        _sum_weight += _yweights.at(i);
    }

    return 0;
}

/*
 * chiama la funzione di fit appropriata a seconda della variabile _fit_type
*/
FitTools::FitResult FitTools::Fit()
{

    switch(_fit_type) {
        case LINEAR_FIT:
            _prepare_fit();
            _fit_linear();
            break;
        case SLOPE_FIT:
            _prepare_fit();
            _fit_slope();
            break;
        case HORIZONTAL_FIT:
            _prepare_fit();
            _fit_horizontal();
            break;
        case TRAP_INTEGRATION:
            _trap_integration();
            break;
        default:
            throw invalid_argument("Invalid fit type");
            break;
    }
    _chi_square(_fit_type);
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

    _fit_result._linear_result.m = (xy_mean - x_mean * y_mean) / (x2_mean - x_mean * x_mean);
    _fit_result._linear_result.m_error = 1 / sqrt((x2_mean - x_mean * x_mean) * _sum_weight);
    _fit_result._linear_result.q = (x2_mean * y_mean - x_mean * xy_mean) / (x2_mean - x_mean * x_mean);
    _fit_result._linear_result.q_error = _fit_result._linear_result.m_error * sqrt(x2_mean);
    _fit_result._linear_result.cov = -x_mean / ((x2_mean - x_mean * x_mean) * _sum_weight);

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
 * integrazione col metodo dei trapezi
 */
int FitTools::_trap_integration()
{
    double area = 0;

    for(unsigned i = 0; i < (_xdata.size() - 1); i++) {
        area += 0.5 * (_ydata.at(i) + _ydata.at(i+1)) * (_xdata.at(i+1) - _xdata.at(i));
    }
    _fit_result._trap_integration_result.area = area;

    return 0;
}

FitTools::LinearFitResult FitTools::getLinearResult()
{
    LinearFitResult ris;
    ris.m = _fit_result._linear_result.m;
    ris.m_error = _fit_result._linear_result.m_error;
    ris.q = _fit_result._linear_result.q;
    ris.q_error = _fit_result._linear_result.q_error;
    ris.cov = _fit_result._linear_result.cov;
    ris.chi_square = _fit_result._linear_result.chi_square;
    return ris;
}

FitTools::SlopeFitResult FitTools::getSlopeResult()
{
    SlopeFitResult ris;
    ris.m = _fit_result._linear_result.m;
    ris.m_error = _fit_result._linear_result.m_error;
    ris.chi_square = _fit_result._linear_result.chi_square;
    return ris;
}

FitTools::HorizontalFitResult FitTools::getHorizontalResult()
{
    HorizontalFitResult ris;
    ris.q = _fit_result._linear_result.q;
    ris.q_error = _fit_result._linear_result.q_error;
    ris.chi_square = _fit_result._linear_result.chi_square;
    return ris;
}

FitTools::TrapIntegrationResult FitTools::getTrapIntegrationResult()
{
    TrapIntegrationResult ris;
    ris.area = _fit_result._trap_integration_result.area;
    return ris;
}

/*
 * calcola chi quadro in base al modello scelto
 */
int FitTools::_chi_square(FitTools::FitFunction fit_type)
{
    switch(fit_type) {
        case LINEAR_FIT: {
            LinearFitResult *f = &(_fit_result._linear_result);
            f->chi_square = 0;
            f->rss = 0;
            for(int i = 0; i < (int)_xdata.size(); i++) {
                double rs = SQUARE(_ydata.at(i) - f->m * _xdata.at(i) - f->q);
                f->chi_square += rs / SQUARE(_yerrors.at(i));
                f->rss += rs;
            }
            break;
        }
        case SLOPE_FIT: {
            LinearFitResult *f = &(_fit_result._linear_result);
            f->chi_square = 0;
            f->rss = 0;
            for(int i = 0; i < (int)_xdata.size(); i++) {
                double rs = SQUARE(_ydata.at(i) - f->m * _xdata.at(i));
                f->chi_square += rs / SQUARE(_yerrors.at(i));
                f->rss += rs;
            }
            break;
        }
        case HORIZONTAL_FIT: {
            LinearFitResult *f = &(_fit_result._linear_result);
            f->chi_square = 0;
            f->rss = 0;
            for(int i = 0; i < (int)_xdata.size(); i++) {
                double rs = SQUARE(_ydata.at(i) - f->q);
                f->chi_square += rs / SQUARE(_yerrors.at(i));
                f->rss += rs;
            }
            break;
        }
        default:
            return -1;
    }
    return 0;
}

/*
 * stampa i risultati su uno stream
 */
int FitTools::printResult(ostream &sout)
{
    switch(_fit_type) {
        case FitTools::LINEAR_FIT:
            _print_linear(sout);
            break;
        case FitTools::SLOPE_FIT:
            _print_slope(sout);
            break;
        case FitTools::HORIZONTAL_FIT:
            _print_horizontal(sout);
            break;
        case FitTools::TRAP_INTEGRATION:
            _print_int_trap_integration(sout);
            break;
        case FitTools::EXPONENTIAL_FIT: //TODO
        case FitTools::LOGARITHMIC_FIT:
            return 0;
            break;
    }
    return 0;
}

int FitTools::_print_linear(ostream &sout)
{
    LinearFitResult *f = &(_fit_result._linear_result);
    sout << scientific
         << "m = " << f->m << "\ts(m) = " << f->m_error << endl
         << "q = " << f->q << "\ts(q) = " << f->q_error << endl
         << "cov(m,q) = " << f->cov << endl
         << "X² = " << f->chi_square << "\tRSS = " << f->rss << endl;
    return 0;
}

int FitTools::_print_slope(ostream &sout)
{
    LinearFitResult *f = &(_fit_result._linear_result);
    sout << scientific
         << "m = " << f->m << "\ts(m) = " << f->m_error << endl
         << "X² = " << f->chi_square << "\tRSS = " << f->rss << endl;
    return 0;
}

int FitTools::_print_horizontal(ostream &sout)
{
    LinearFitResult *f = &(_fit_result._linear_result);
    sout << scientific
         << "q = " << f->q << "\ts(q) = " << f->q_error << endl
         << "X² = " << f->chi_square << "\tRSS = " << f->rss << endl;
    return 0;
}

int FitTools::_print_int_trap_integration(ostream &sout)
{
    TrapIntegrationResult *f = &(_fit_result._trap_integration_result);
    sout << scientific
         << "Area = " << f->area << endl;
    return 0;
}

/* vim: set ts=4 sw=4 et: */
