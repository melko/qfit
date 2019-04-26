/*
 * about.h
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


#ifndef ABOUT_H
#define ABOUT_H

#include <QtWidgets/QDialog>

class About : public QDialog
{
    Q_OBJECT
public:
    About(QWidget *parent = 0);
    virtual ~About();
public slots:
    void on_clickLink(const QString &link);
};

#endif // ABOUT_H

/* vim: set ts=4 sw=4 et: */
