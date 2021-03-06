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

#include <QPainter>
#include <QStyle>
#include <QStyleOptionFrame>
#include <QWheelEvent>
#include <math.h>

#include "histogramviewer.h"
#include "../imgproc/types.h"
#include "../imgproc/lut.h"

namespace anitools {
namespace widgets {

using namespace anitools::imgproc;

HistogramViewer::HistogramViewer(QWidget *parent) :
    QWidget(parent),
    mHistMax(0),
    mHasHistogram(false),
    mIsMoving(false),
    mSelStartPos(-1),
    mSelEndPos(-1)
{
}

void HistogramViewer::paintEvent(QPaintEvent *e)
{
    QPainter p(this);

    if (mHasHistogram)
    {
        int linePos;
        double histV;
        QRect r = this->rect().adjusted(2, 2, -2, -2);
        p.fillRect(this->rect(), QColor(0, 0, 0));
        p.setPen(QColor(255, 255, 255));

        for (int i = 0; i < 256; i++)
        {
            linePos = i * (r.width() - 1) / 255;
            histV = (double)mHistogram[i] / mHistMax;
            p.drawLine(linePos + 2, r.bottom(), linePos + 2,
                       r.bottom() - histV * (r.height() - 1));
        }

        p.setCompositionMode(QPainter::CompositionMode_Difference);
        if (mSelStartPos != -1 && mSelStartPos != -1)
            p.fillRect(r.adjusted(qMin<int>(mSelStartPos, mSelEndPos) - r.left(),
                             0,
                             qMin<int>(mSelStartPos, mSelEndPos) - r.right(),
                             0),
                       QColor(255, 255, 255));
    }
    else
    {
        p.fillRect(this->rect(), this->palette().dark());

        p.setRenderHint(QPainter::Antialiasing);
        QPoint center(this->width() >> 1, this->height() >> 1);
        int radius = (qMin<int>(this->width(), this->height()) - 16) >> 1;
        int offset = (radius - radius / 6) * 0.7;

        p.setBrush(Qt::transparent);
        p.setPen(QPen(this->palette().mid(), radius / 3, Qt::SolidLine, Qt::FlatCap));

        p.drawEllipse(center, radius - radius / 6, radius - radius / 6);
        p.drawLine(center.x() - offset, center.y() - offset, center.x() + offset, center.y() + offset);
    }

    p.resetTransform();
    p.setCompositionMode(QPainter::CompositionMode_SourceOver);
    p.setRenderHint(QPainter::Antialiasing, false);
    p.setRenderHint(QPainter::SmoothPixmapTransform, false);
    p.setBrush(Qt::transparent);
    p.setPen(QPen(this->palette().shadow(), 1));
    p.drawRect(this->rect().adjusted(0, 0, -1, -1));
    p.setPen(QPen(this->palette().mid(), 1));
    p.drawRect(this->rect().adjusted(1, 1, -2, -2));

    e->accept();
}

void HistogramViewer::mousePressEvent(QMouseEvent *e)
{
    if ((e->buttons() & Qt::LeftButton) && mHasHistogram)
    {
        mSelStartPos = mSelEndPos = qBound<int>(2, e->pos().x(), this->rect().right() - 2);
        mIsMoving = true;
        update();
        e->accept();
    }
}
void HistogramViewer::mouseReleaseEvent(QMouseEvent *e)
{
    if ((e->button() == Qt::LeftButton) && mHasHistogram)
    {
        mSelStartPos = mSelEndPos = -1;
        mIsMoving = false;
        update();
        e->accept();
    }
}
void HistogramViewer::mouseMoveEvent(QMouseEvent *e)
{
    if ((e->buttons() & Qt::LeftButton) && mIsMoving && mHasHistogram)
    {
        mSelEndPos = qBound<int>(2, e->pos().x(), this->rect().right() - 2);
        update();
        e->accept();
    }
}

void HistogramViewer::setImage(const QImage & newImage)
{
    if (newImage.isNull())
    {
        mHasHistogram = false;
        update();
        return;
    }

    int gray;
    unsigned int totalPixels = newImage.width() * newImage.height();
    BGRA * bits = (BGRA*)newImage.bits();

    memset(mHistogram, 0, sizeof(mHistogram));
    mHistMax = 0;

    while (totalPixels--)
    {
        gray = round(bits->r * .2126 + bits->g * .7152 + bits->b * .0722);
        gray += lut01[255 - gray][255 - bits->a];
        mHistogram[gray]++;
        if (mHistogram[gray] > mHistMax)
            mHistMax = mHistogram[gray];
        bits++;
    }

    mHasHistogram = true;
    update();
}

void HistogramViewer::setHistogram(const QVector<unsigned int> &newHistogram)
{
    if (newHistogram.isEmpty() || newHistogram.size() != 256)
    {
        mHasHistogram = false;
        update();
        return;
    }
    mHistMax = 0;
    for (int i = 0; i < 256; i++)
    {
        mHistogram[i] = newHistogram.at(i);
        if (mHistogram[i] > mHistMax)
            mHistMax = mHistogram[i];
    }
    mHasHistogram = true;
    update();
}

}}
