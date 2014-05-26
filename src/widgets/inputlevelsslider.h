/****************************************************************************
**
** Copyright (C) 2014 Deif Lou
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

#ifndef ANITOOLS_WIDGETS_INPUTLEVELSSLIDER_H
#define ANITOOLS_WIDGETS_INPUTLEVELSSLIDER_H

#include <QWidget>

namespace anitools {
namespace widgets {

class InputLevelsSlider : public QWidget
{
    Q_OBJECT

private:
    static const double LOG_1OVER2 = -0.69314718055994530941723212145818;
    static const double POW_1OVER2_TEN = 0.0009765625;
    static const int LEFT_MARGIN = 4;
    static const int TOP_MARGIN = 0;
    static const int RIGHT_MARGIN = 4;
    static const int BOTTOM_MARGIN = 6;
    static const int HANDLE_HEIGHT = 8;
    static const int HANDLE_WIDTH = 8;

    double mBlackPoint, mWhitePoint, mGammaCorrection;
    QImage mFunction;
    int mHandlePressed, mHandleSelected;

    void makeFunction();
    void paintHandle(QPainter & p, const QPoint & pos, const QColor & c, bool selected = false);

protected:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void focusInEvent(QFocusEvent *e);
    void focusOutEvent(QFocusEvent *e);
    void keyPressEvent(QKeyEvent *);

public:
    explicit InputLevelsSlider(QWidget *parent = 0);

    double blackPoint();
    double whitePoint();
    double gammaCorrection();
    
signals:
    void blackPointChanged(double v);
    void whitePointChanged(double v);
    void gammaCorrectionChanged(double v);

public slots:
    void setBlackPoint(double v);
    void setWhitePoint(double v);
    void setGammaCorrection(double v);
    void setValues(double b, double w, double g);
};

}}
#endif // ANITOOLS_WIDGETS_INPUTLEVELSSLIDER_H
