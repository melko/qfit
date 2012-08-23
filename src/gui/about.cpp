/*
 * about.cpp
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


#include "about.h"

#include <fitconfig.h>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QTextEdit>
#include <QtGui/QPixmap>
#include <QtGui/QSpacerItem>
#include <QtGui/QDesktopServices>
#include <QtCore/QUrl>

About::About(QWidget *parent /*= 0*/) : QDialog(parent)
{
//   resize(500, 400);
//   QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//   sizePolicy.setHorizontalStretch(0);
//   sizePolicy.setVerticalStretch(0);
//   setSizePolicy(sizePolicy);
    setFixedSize(500, 400);

    QVBoxLayout *mainlayout = new QVBoxLayout();
    QHBoxLayout *hlayout = new QHBoxLayout();

    QLabel *picture =  new QLabel();
    QPixmap(QString::fromUtf8(":/icons/resources/curve_to.png"));
    picture->setPixmap(QPixmap(QString::fromUtf8(":/icons/resources/curve_to.png")).scaled(150, 150));

    QSpacerItem *spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QLabel *info = new QLabel();
    info->setText(trUtf8("<p><b>QFit</b></p><p>Version: %1.%2.%3</p>"
                         "<p>Home Page: <a href=\"https://github.com/melko/qfit\">https://github.com/melko/qfit</a></p>")
                  .arg(QFIT_VERSION_MAJOR)
                  .arg(QFIT_VERSION_MINOR)
                  .arg(QFIT_VERSION_PATCH));
    connect(info, SIGNAL(linkActivated(QString)), this, SLOT(on_clickLink(QString)));

    QTextEdit *other = new QTextEdit();
    other->setReadOnly(true);
    other->append(trUtf8("<p><b>Author:</b> Paolo Cretaro (lorddarthfener@gmail.com)</p>"
                         "<p><b>Thanks To:<br/></p>"
                         "<p>This software is released under the terms of the <i>GNU General Public License v3</i>.</p>"));

    hlayout->addWidget(picture);
    hlayout->addWidget(info);
    hlayout->addItem(spacer);
    mainlayout->addLayout(hlayout);
    mainlayout->addWidget(other);
    setLayout(mainlayout);
}

About::~About()
{

}

void About::on_clickLink(const QString &link)
{
    QDesktopServices::openUrl(QUrl(link));
}

#include "about.moc"

/* vim: set ts=4 sw=4 et: */
