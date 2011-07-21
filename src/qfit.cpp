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

#include <QtGui/QLabel>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QAction>

qfit::qfit()
{
  QLabel* l = new QLabel( this );
  l->setText( "Hello World!" );
  setCentralWidget( l );
  QAction* a = new QAction(this);
  a->setText( "Quit" );
  connect(a, SIGNAL(triggered()), SLOT(close()) );
  menuBar()->addMenu( "File" )->addAction( a );
}

qfit::~qfit()
{}

#include "qfit.moc"

/* vim: set ts=2 sw=2 et: */
