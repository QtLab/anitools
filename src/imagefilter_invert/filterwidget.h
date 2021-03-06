/****************************************************************************
**
** Copyright (C) 2014 - 2015 Deif Lou
**
** This file is part of Anitools
**
** Anitools is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#ifndef FILTERWIDGET_H
#define FILTERWIDGET_H

#include <QWidget>

#include "filter.h"

namespace Ui {
class FilterWidget;
}

class FilterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FilterWidget(QWidget *parent = 0);
    ~FilterWidget();

private:
    Ui::FilterWidget *ui;
    bool mEmitSignals;

signals:
    void redChannelToggled(bool v);
    void greenChannelToggled(bool v);
    void blueChannelToggled(bool v);
    void alphaChannelToggled(bool v);

public slots:
    void setRedChannel(bool v);
    void setGreenChannel(bool v);
    void setBlueChannel(bool v);
    void setAlphaChannel(bool v);

private slots:
    void on_mButtonRedChannel_toggled(bool c);
    void on_mButtonGreenChannel_toggled(bool c);
    void on_mButtonBlueChannel_toggled(bool c);
    void on_mButtonAlphaChannel_toggled(bool c);
};

#endif // FILTERWIDGET_H
