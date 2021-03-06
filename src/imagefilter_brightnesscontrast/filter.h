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

#ifndef FILTER_H
#define FILTER_H

#include <QObject>
#include <QHash>
#include <QVector>
#include <QString>
#include <QImage>
#include <QSettings>
#include <QWidget>
#include <QMutex>

#include "../imgproc/imagefilter.h"

using namespace anitools::imgproc;

class Filter : public ImageFilter
{
    Q_OBJECT

public:
    enum WorkingChannel
    {
        RGB, Red, Green, Blue, Alpha
    };
    enum BCParameter
    {
        Brightness = 0,
        Contrast = 1
    };

    Filter();
    ~Filter();
    ImageFilter * clone();
    QHash<QString, QString> info();
    QImage process(const QImage & inputImage);
    bool loadParameters(QSettings & s);
    bool saveParameters(QSettings & s);
    QWidget * widget(QWidget *parent = 0);

private:
    WorkingChannel mWorkingChannel;
    int mBrightnessContrast[5][2];
    bool mUseSoftMode;
    unsigned char mLuts[5][256];

    void makeLUT(WorkingChannel c);
    void makeSoftLUT(WorkingChannel c);
    void makeHardLUT(WorkingChannel c);
    void makeAllLUTs();

signals:
    void workingChannelChanged(Filter::WorkingChannel s);
    void brightnessChanged(int v);
    void contrastChanged(int v);
    void useSoftModeChanged(bool v);

public slots:
    void setWorkingChannel(Filter::WorkingChannel s);
    void setBrightness(int v);
    void setContrast(int v);
    void setUseSoftMode(bool v);
};

#endif // FILTER_H
