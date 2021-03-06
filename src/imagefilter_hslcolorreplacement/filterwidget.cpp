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

#include <math.h>

#include "filterwidget.h"
#include "ui_filterwidget.h"

FilterWidget::FilterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FilterWidget),
    mEmitSignals(true),
    mHueCurvesPaintDelegate(0),
    mSaturationCurvesPaintDelegate(0),
    mLightnessCurvesPaintDelegate(0),
    mHueZoomFactorIndex(0),
    mSaturationZoomFactorIndex(0),
    mLightnessZoomFactorIndex(0)
{
    ui->setupUi(this);

    mHueCurvesPaintDelegate = new HSLColorReplacementCurvesPaintDelegate(this);
    mHueCurvesPaintDelegate->setChannel(ColorChannel_Hue);
    ui->mWidgetHueCurve->setPaintDelegate(mHueCurvesPaintDelegate);
    ui->mWidgetHueCurve->setPeriodic(true);
    ui->mSpinHueInValue->setEnabled(false);
    ui->mSpinHueOutValue->setEnabled(false);

    mSaturationCurvesPaintDelegate = new HSLColorReplacementCurvesPaintDelegate(this);
    mSaturationCurvesPaintDelegate->setChannel(ColorChannel_Saturation);
    ui->mWidgetSaturationCurve->setPaintDelegate(mSaturationCurvesPaintDelegate);
    ui->mSpinSaturationInValue->setEnabled(false);
    ui->mSpinSaturationOutValue->setEnabled(false);

    mLightnessCurvesPaintDelegate = new HSLColorReplacementCurvesPaintDelegate(this);
    mLightnessCurvesPaintDelegate->setChannel(ColorChannel_Lightness);
    ui->mWidgetLightnessCurve->setPaintDelegate(mLightnessCurvesPaintDelegate);
    ui->mSpinLightnessInValue->setEnabled(false);
    ui->mSpinLightnessOutValue->setEnabled(false);

    ui->mSliderAbsHue->setVisible(false);
    ui->mSpinAbsHue->setVisible(false);
    ui->mSliderAbsSaturation->setVisible(false);
    ui->mSpinAbsSaturation->setVisible(false);

    mZoomFactors << 1. << 1.2 << 1.4 << 1.6 << 1.8 << 2.;
}

FilterWidget::~FilterWidget()
{
    delete ui;
}

void FilterWidget::setHueKnots(const Interpolator1DKnots & k)
{
    if (ui->mWidgetHueCurve->knots() == k)
        return;

    mEmitSignals = false;
    ui->mWidgetHueCurve->setKnots(k);
    mEmitSignals = true;

    emit hueKnotsChanged(k);
}

void FilterWidget::setHueInterpolationMode(Filter::InterpolationMode im)
{
    if ((im == Filter::Flat && ui->mButtonHueInterpolationModeFlat->isChecked()) ||
        (im == Filter::Linear && ui->mButtonHueInterpolationModeLinear->isChecked()) ||
        (im == Filter::Smooth && ui->mButtonHueInterpolationModeSmooth->isChecked()))
        return;

    mEmitSignals = false;
    if (im == Filter::Flat)
        ui->mButtonHueInterpolationModeFlat->setChecked(true);
    else if (im == Filter::Linear)
        ui->mButtonHueInterpolationModeLinear->setChecked(true);
    else if (im == Filter::Smooth)
        ui->mButtonHueInterpolationModeSmooth->setChecked(true);
    mEmitSignals = true;

    emit hueInterpolationModeChanged(im);
}

void FilterWidget::setHueInverted(bool i)
{
    if (i == ui->mButtonHueInverted->isChecked())
        return;

    mEmitSignals = false;
    ui->mButtonHueInverted->setChecked(i);
    mEmitSignals = true;

    emit hueInvertedChanged(i);
}

void FilterWidget::setSaturationKnots(const Interpolator1DKnots & k)
{
    if (ui->mWidgetSaturationCurve->knots() == k)
        return;

    mEmitSignals = false;
    ui->mWidgetSaturationCurve->setKnots(k);
    mEmitSignals = true;

    emit saturationKnotsChanged(k);
}

void FilterWidget::setSaturationInterpolationMode(Filter::InterpolationMode im)
{
    if ((im == Filter::Flat && ui->mButtonSaturationInterpolationModeFlat->isChecked()) ||
        (im == Filter::Linear && ui->mButtonSaturationInterpolationModeLinear->isChecked()) ||
        (im == Filter::Smooth && ui->mButtonSaturationInterpolationModeSmooth->isChecked()))
        return;

    mEmitSignals = false;
    if (im == Filter::Flat)
        ui->mButtonSaturationInterpolationModeFlat->setChecked(true);
    else if (im == Filter::Linear)
        ui->mButtonSaturationInterpolationModeLinear->setChecked(true);
    else if (im == Filter::Smooth)
        ui->mButtonSaturationInterpolationModeSmooth->setChecked(true);
    mEmitSignals = true;

    emit saturationInterpolationModeChanged(im);
}

void FilterWidget::setSaturationInverted(bool i)
{
    if (i == ui->mButtonSaturationInverted->isChecked())
        return;

    mEmitSignals = false;
    ui->mButtonSaturationInverted->setChecked(i);
    mEmitSignals = true;

    emit saturationInvertedChanged(i);
}

void FilterWidget::setLightnessKnots(const Interpolator1DKnots & k)
{
    if (ui->mWidgetLightnessCurve->knots() == k)
        return;

    mEmitSignals = false;
    ui->mWidgetLightnessCurve->setKnots(k);
    mEmitSignals = true;

    emit lightnessKnotsChanged(k);
}

void FilterWidget::setLightnessInterpolationMode(Filter::InterpolationMode im)
{
    if ((im == Filter::Flat && ui->mButtonLightnessInterpolationModeFlat->isChecked()) ||
        (im == Filter::Linear && ui->mButtonLightnessInterpolationModeLinear->isChecked()) ||
        (im == Filter::Smooth && ui->mButtonLightnessInterpolationModeSmooth->isChecked()))
        return;

    mEmitSignals = false;
    if (im == Filter::Flat)
        ui->mButtonLightnessInterpolationModeFlat->setChecked(true);
    else if (im == Filter::Linear)
        ui->mButtonLightnessInterpolationModeLinear->setChecked(true);
    else if (im == Filter::Smooth)
        ui->mButtonLightnessInterpolationModeSmooth->setChecked(true);
    mEmitSignals = true;

    emit lightnessInterpolationModeChanged(im);
}

void FilterWidget::setLightnessInverted(bool i)
{
    if (i == ui->mButtonLightnessInverted->isChecked())
        return;

    mEmitSignals = false;
    ui->mButtonLightnessInverted->setChecked(i);
    mEmitSignals = true;

    emit lightnessInvertedChanged(i);
}

void FilterWidget::setOutputMode(Filter::OutputMode om)
{
    if ((om == Filter::CorrectedImage && ui->mButtonOutputModeKeyedImage->isChecked()) ||
        (om == Filter::Mask && ui->mButtonOutputModeMatte->isChecked()))
        return;

    mEmitSignals = false;
    if (om == Filter::CorrectedImage)
        ui->mButtonOutputModeKeyedImage->setChecked(true);
    else
        ui->mButtonOutputModeMatte->setChecked(true);
    mEmitSignals = true;

    emit outputModeChanged(om);
}

void FilterWidget::setPreblurRadius(double pbr)
{
    if (ui->mSpinPreblurRadius->value() == pbr)
        return;
    mEmitSignals = false;
    ui->mSpinPreblurRadius->setValue(pbr);
    mEmitSignals = true;
    emit preblurRadiusChanged(pbr);
}

void FilterWidget::setRelHue(int v)
{
    if (ui->mSpinRelHue->value() == v)
        return;
    mEmitSignals = false;
    ui->mSpinRelHue->setValue(v);
    mEmitSignals = true;
    emit relHueChanged(v);
}

void FilterWidget::setRelSaturation(int v)
{
    if (ui->mSpinRelSaturation->value() == v)
        return;
    mEmitSignals = false;
    ui->mSpinRelSaturation->setValue(v);
    mEmitSignals = true;
    emit relSaturationChanged(v);
}

void FilterWidget::setRelLightness(int v)
{
    if (ui->mSpinRelLightness->value() == v)
        return;
    mEmitSignals = false;
    ui->mSpinRelLightness->setValue(v);
    mEmitSignals = true;
    emit relLightnessChanged(v);
}

void FilterWidget::setAbsHue(int v)
{
    if (ui->mSpinAbsHue->value() == v)
        return;
    mEmitSignals = false;
    ui->mSpinAbsHue->setValue(v);
    mEmitSignals = true;
    emit absHueChanged(v);
}

void FilterWidget::setAbsSaturation(int v)
{
    if (ui->mSpinAbsSaturation->value() == v)
        return;
    mEmitSignals = false;
    ui->mSpinAbsSaturation->setValue(v);
    mEmitSignals = true;
    emit absSaturationChanged(v);
}

void FilterWidget::setColorize(bool v)
{
    if (ui->mButtonColorize->isChecked() == v)
        return;
    mEmitSignals = false;
    ui->mButtonColorize->setChecked(v);
    mEmitSignals = true;
    emit relHueChanged(v);
}

void FilterWidget::on_mButtonHueInterpolationModeFlat_toggled(bool v)
{
    if (!v)
        return;

    ui->mWidgetHueCurve->setInterpolationMode(anitools::widgets::Curves::NearestNeighbor);

    if (mEmitSignals)
        emit hueInterpolationModeChanged(Filter::Flat);
}
void FilterWidget::on_mButtonHueInterpolationModeLinear_toggled(bool v)
{
    if (!v)
        return;

    ui->mWidgetHueCurve->setInterpolationMode(anitools::widgets::Curves::Linear);

    if (mEmitSignals)
        emit hueInterpolationModeChanged(Filter::Linear);
}
void FilterWidget::on_mButtonHueInterpolationModeSmooth_toggled(bool v)
{
    if (!v)
        return;

    ui->mWidgetHueCurve->setInterpolationMode(anitools::widgets::Curves::Cubic);

    if (mEmitSignals)
        emit hueInterpolationModeChanged(Filter::Smooth);
}

void FilterWidget::on_mWidgetHueCurve_knotsChanged(const Interpolator1DKnots & k)
{
    if (ui->mWidgetHueCurve->selectedKnotIndex() == -1)
    {
        ui->mSpinHueInValue->setEnabled(false);
        ui->mSpinHueOutValue->setEnabled(false);
    }
    else
    {
        Interpolator1DKnot kn = k.at(ui->mWidgetHueCurve->selectedKnotIndex());
        ui->mSpinHueInValue->setValue(round(kn.x() * 255.0));
        ui->mSpinHueOutValue->setValue(round(kn.y() * 255.0));
        ui->mSpinHueInValue->setEnabled(true);
        ui->mSpinHueOutValue->setEnabled(true);

        HSL color = mSaturationCurvesPaintDelegate->color();
        color.h = round(kn.x() * 255.0);
        mSaturationCurvesPaintDelegate->setColor(color);
        color = mLightnessCurvesPaintDelegate->color();
        color.h = round(kn.x() * 255.0);
        mLightnessCurvesPaintDelegate->setColor(color);
    }

    if (mEmitSignals)
        emit hueKnotsChanged(k);
}

void FilterWidget::on_mWidgetHueCurve_selectedKnotChanged(int i)
{
    Q_UNUSED(i)

    if (i == -1)
    {
        ui->mSpinHueInValue->setEnabled(false);
        ui->mSpinHueOutValue->setEnabled(false);
    }
    else
    {
        Interpolator1DKnot kn = ui->mWidgetHueCurve->selectedKnot();
        ui->mSpinHueInValue->setValue(round(kn.x() * 255.0));
        ui->mSpinHueOutValue->setValue(round(kn.y() * 255.0));
        ui->mSpinHueInValue->setEnabled(true);
        ui->mSpinHueOutValue->setEnabled(true);

        HSL color = mSaturationCurvesPaintDelegate->color();
        color.h = round(kn.x() * 255.0);
        mSaturationCurvesPaintDelegate->setColor(color);
        color = mLightnessCurvesPaintDelegate->color();
        color.h = round(kn.x() * 255.0);
        mLightnessCurvesPaintDelegate->setColor(color);
    }
}

void FilterWidget::on_mSpinHueInValue_valueChanged(double v)
{
    if (ui->mWidgetHueCurve->selectedKnotIndex() == -1)
        return;

    Interpolator1DKnot k = ui->mWidgetHueCurve->selectedKnot();
    v = v / 255.0;
    if (v == k.x()) return;
    ui->mWidgetHueCurve->setSelectedKnot(v, k.y());

    if (mEmitSignals)
        emit hueKnotsChanged(ui->mWidgetHueCurve->knots());
}

void FilterWidget::on_mSpinHueOutValue_valueChanged(double v)
{
    if (ui->mWidgetHueCurve->selectedKnotIndex() == -1)
        return;

    Interpolator1DKnot k = ui->mWidgetHueCurve->selectedKnot();
    v = v / 255.0;
    if (v == k.y()) return;
    ui->mWidgetHueCurve->setSelectedKnot(k.x(), v);

    if (mEmitSignals)
        emit hueKnotsChanged(ui->mWidgetHueCurve->knots());
}

void FilterWidget::on_mButtonHueInverted_toggled(bool v)
{
    mHueCurvesPaintDelegate->setInverted(v);

    if (mEmitSignals)
        emit hueInvertedChanged(v);
}

void FilterWidget::on_mButtonHueZoomIn_clicked()
{
    if (mHueZoomFactorIndex == mZoomFactors.size() - 1)
        return;
    mHueZoomFactorIndex++;
    ui->mWidgetHueCurve->setZoomFactor(mZoomFactors[mHueZoomFactorIndex]);
}

void FilterWidget::on_mButtonHueZoomOut_clicked()
{
    if (mHueZoomFactorIndex == 0)
        return;
    mHueZoomFactorIndex--;
    ui->mWidgetHueCurve->setZoomFactor(mZoomFactors[mHueZoomFactorIndex]);
}

void FilterWidget::on_mButtonHueZoom100_clicked()
{
    mHueZoomFactorIndex = 0;
    ui->mWidgetHueCurve->fit();
}

void FilterWidget::on_mButtonSaturationInterpolationModeFlat_toggled(bool v)
{
    if (!v)
        return;

    ui->mWidgetSaturationCurve->setInterpolationMode(anitools::widgets::Curves::NearestNeighbor);

    if (mEmitSignals)
        emit saturationInterpolationModeChanged(Filter::Flat);
}
void FilterWidget::on_mButtonSaturationInterpolationModeLinear_toggled(bool v)
{
    if (!v)
        return;

    ui->mWidgetSaturationCurve->setInterpolationMode(anitools::widgets::Curves::Linear);

    if (mEmitSignals)
        emit saturationInterpolationModeChanged(Filter::Linear);
}
void FilterWidget::on_mButtonSaturationInterpolationModeSmooth_toggled(bool v)
{
    if (!v)
        return;

    ui->mWidgetSaturationCurve->setInterpolationMode(anitools::widgets::Curves::Cubic);

    if (mEmitSignals)
        emit saturationInterpolationModeChanged(Filter::Smooth);
}

void FilterWidget::on_mWidgetSaturationCurve_knotsChanged(const Interpolator1DKnots & k)
{
    if (ui->mWidgetSaturationCurve->selectedKnotIndex() == -1)
    {
        ui->mSpinSaturationInValue->setEnabled(false);
        ui->mSpinSaturationOutValue->setEnabled(false);
    }
    else
    {
        Interpolator1DKnot kn = k.at(ui->mWidgetSaturationCurve->selectedKnotIndex());
        ui->mSpinSaturationInValue->setValue(round(kn.x() * 255.0));
        ui->mSpinSaturationOutValue->setValue(round(kn.y() * 255.0));
        ui->mSpinSaturationInValue->setEnabled(true);
        ui->mSpinSaturationOutValue->setEnabled(true);
    }

    if (mEmitSignals)
        emit saturationKnotsChanged(k);
}

void FilterWidget::on_mWidgetSaturationCurve_selectedKnotChanged(int i)
{
    Q_UNUSED(i)

    if (i == -1)
    {
        ui->mSpinSaturationInValue->setEnabled(false);
        ui->mSpinSaturationOutValue->setEnabled(false);
    }
    else
    {
        Interpolator1DKnot kn = ui->mWidgetSaturationCurve->selectedKnot();
        ui->mSpinSaturationInValue->setValue(round(kn.x() * 255.0));
        ui->mSpinSaturationOutValue->setValue(round(kn.y() * 255.0));
        ui->mSpinSaturationInValue->setEnabled(true);
        ui->mSpinSaturationOutValue->setEnabled(true);
    }
}

void FilterWidget::on_mSpinSaturationInValue_valueChanged(double v)
{
    if (ui->mWidgetSaturationCurve->selectedKnotIndex() == -1)
        return;

    Interpolator1DKnot k = ui->mWidgetSaturationCurve->selectedKnot();
    v = v / 255.0;
    if (v == k.x()) return;
    ui->mWidgetSaturationCurve->setSelectedKnot(v, k.y());

    if (mEmitSignals)
        emit saturationKnotsChanged(ui->mWidgetSaturationCurve->knots());
}

void FilterWidget::on_mSpinSaturationOutValue_valueChanged(double v)
{
    if (ui->mWidgetSaturationCurve->selectedKnotIndex() == -1)
        return;

    Interpolator1DKnot k = ui->mWidgetSaturationCurve->selectedKnot();
    v = v / 255.0;
    if (v == k.y()) return;
    ui->mWidgetSaturationCurve->setSelectedKnot(k.x(), v);

    if (mEmitSignals)
        emit saturationKnotsChanged(ui->mWidgetSaturationCurve->knots());
}

void FilterWidget::on_mButtonSaturationInverted_toggled(bool v)
{
    mSaturationCurvesPaintDelegate->setInverted(v);

    if (mEmitSignals)
        emit saturationInvertedChanged(v);
}

void FilterWidget::on_mButtonSaturationZoomIn_clicked()
{
    if (mSaturationZoomFactorIndex == mZoomFactors.size() - 1)
        return;
    mSaturationZoomFactorIndex++;
    ui->mWidgetSaturationCurve->setZoomFactor(mZoomFactors[mSaturationZoomFactorIndex]);
}

void FilterWidget::on_mButtonSaturationZoomOut_clicked()
{
    if (mSaturationZoomFactorIndex == 0)
        return;
    mSaturationZoomFactorIndex--;
    ui->mWidgetSaturationCurve->setZoomFactor(mZoomFactors[mSaturationZoomFactorIndex]);
}

void FilterWidget::on_mButtonSaturationZoom100_clicked()
{
    mSaturationZoomFactorIndex = 0;
    ui->mWidgetSaturationCurve->fit();
}

void FilterWidget::on_mButtonLightnessInterpolationModeFlat_toggled(bool v)
{
    if (!v)
        return;

    ui->mWidgetLightnessCurve->setInterpolationMode(anitools::widgets::Curves::NearestNeighbor);

    if (mEmitSignals)
        emit lightnessInterpolationModeChanged(Filter::Flat);
}
void FilterWidget::on_mButtonLightnessInterpolationModeLinear_toggled(bool v)
{
    if (!v)
        return;

    ui->mWidgetLightnessCurve->setInterpolationMode(anitools::widgets::Curves::Linear);

    if (mEmitSignals)
        emit lightnessInterpolationModeChanged(Filter::Linear);
}
void FilterWidget::on_mButtonLightnessInterpolationModeSmooth_toggled(bool v)
{
    if (!v)
        return;

    ui->mWidgetLightnessCurve->setInterpolationMode(anitools::widgets::Curves::Cubic);

    if (mEmitSignals)
        emit lightnessInterpolationModeChanged(Filter::Smooth);
}

void FilterWidget::on_mWidgetLightnessCurve_knotsChanged(const Interpolator1DKnots & k)
{
    if (ui->mWidgetLightnessCurve->selectedKnotIndex() == -1)
    {
        ui->mSpinLightnessInValue->setEnabled(false);
        ui->mSpinLightnessOutValue->setEnabled(false);
    }
    else
    {
        Interpolator1DKnot kn = k.at(ui->mWidgetLightnessCurve->selectedKnotIndex());
        ui->mSpinLightnessInValue->setValue(round(kn.x() * 255.0));
        ui->mSpinLightnessOutValue->setValue(round(kn.y() * 255.0));
        ui->mSpinLightnessInValue->setEnabled(true);
        ui->mSpinLightnessOutValue->setEnabled(true);
    }

    if (mEmitSignals)
        emit lightnessKnotsChanged(k);
}

void FilterWidget::on_mWidgetLightnessCurve_selectedKnotChanged(int i)
{
    Q_UNUSED(i)

    if (i == -1)
    {
        ui->mSpinLightnessInValue->setEnabled(false);
        ui->mSpinLightnessOutValue->setEnabled(false);
    }
    else
    {
        Interpolator1DKnot kn = ui->mWidgetLightnessCurve->selectedKnot();
        ui->mSpinLightnessInValue->setValue(round(kn.x() * 255.0));
        ui->mSpinLightnessOutValue->setValue(round(kn.y() * 255.0));
        ui->mSpinLightnessInValue->setEnabled(true);
        ui->mSpinLightnessOutValue->setEnabled(true);
    }
}

void FilterWidget::on_mSpinLightnessInValue_valueChanged(double v)
{
    if (ui->mWidgetLightnessCurve->selectedKnotIndex() == -1)
        return;

    Interpolator1DKnot k = ui->mWidgetLightnessCurve->selectedKnot();
    v = v / 255.0;
    if (v == k.x()) return;
    ui->mWidgetLightnessCurve->setSelectedKnot(v, k.y());

    if (mEmitSignals)
        emit lightnessKnotsChanged(ui->mWidgetLightnessCurve->knots());
}

void FilterWidget::on_mSpinLightnessOutValue_valueChanged(double v)
{
    if (ui->mWidgetLightnessCurve->selectedKnotIndex() == -1)
        return;

    Interpolator1DKnot k = ui->mWidgetLightnessCurve->selectedKnot();
    v = v / 255.0;
    if (v == k.y()) return;
    ui->mWidgetLightnessCurve->setSelectedKnot(k.x(), v);

    if (mEmitSignals)
        emit lightnessKnotsChanged(ui->mWidgetLightnessCurve->knots());
}

void FilterWidget::on_mButtonLightnessInverted_toggled(bool v)
{
    mLightnessCurvesPaintDelegate->setInverted(v);

    if (mEmitSignals)
        emit lightnessInvertedChanged(v);
}

void FilterWidget::on_mButtonLightnessZoomIn_clicked()
{
    if (mLightnessZoomFactorIndex == mZoomFactors.size() - 1)
        return;
    mLightnessZoomFactorIndex++;
    ui->mWidgetLightnessCurve->setZoomFactor(mZoomFactors[mLightnessZoomFactorIndex]);
}

void FilterWidget::on_mButtonLightnessZoomOut_clicked()
{
    if (mLightnessZoomFactorIndex == 0)
        return;
    mLightnessZoomFactorIndex--;
    ui->mWidgetLightnessCurve->setZoomFactor(mZoomFactors[mLightnessZoomFactorIndex]);
}

void FilterWidget::on_mButtonLightnessZoom100_clicked()
{
    mLightnessZoomFactorIndex = 0;
    ui->mWidgetLightnessCurve->fit();
}

void FilterWidget::on_mButtonOutputModeKeyedImage_toggled(bool c)
{
    if (!c)
        return;

    if (mEmitSignals)
        emit outputModeChanged(Filter::CorrectedImage);
}

void FilterWidget::on_mButtonOutputModeMatte_toggled(bool c)
{
    if (!c)
        return;

    if (mEmitSignals)
        emit outputModeChanged(Filter::Mask);
}

void FilterWidget::on_mSliderPreblurRadius_valueChanged(int value)
{
    ui->mSpinPreblurRadius->setValue(value / 100.0);
    if (mEmitSignals)
        emit preblurRadiusChanged(value / 100.0);
}

void FilterWidget::on_mSpinPreblurRadius_valueChanged(double arg1)
{
    ui->mSliderPreblurRadius->setValue((int)round(arg1 * 100.0));
    if (mEmitSignals)
        emit preblurRadiusChanged(arg1);
}

void FilterWidget::on_mSliderRelHue_valueChanged(int v)
{
    ui->mSpinRelHue->setValue(v);
    if (mEmitSignals)
        emit relHueChanged(v);
}

void FilterWidget::on_mSpinRelHue_valueChanged(int v)
{
    ui->mSliderRelHue->setValue(v);
    mHueCurvesPaintDelegate->setCorrectedValue(ColorChannel_Hue, v);
    mSaturationCurvesPaintDelegate->setCorrectedValue(ColorChannel_Hue, v);
    mLightnessCurvesPaintDelegate->setCorrectedValue(ColorChannel_Hue, v);
    if (mEmitSignals)
        emit relHueChanged(v);
}

void FilterWidget::on_mSliderRelSaturation_valueChanged(int v)
{
    ui->mSpinRelSaturation->setValue(v);
    if (mEmitSignals)
        emit relSaturationChanged(v);
}

void FilterWidget::on_mSpinRelSaturation_valueChanged(int v)
{
    ui->mSliderRelSaturation->setValue(v);
    mHueCurvesPaintDelegate->setCorrectedValue(ColorChannel_Saturation, v);
    mSaturationCurvesPaintDelegate->setCorrectedValue(ColorChannel_Saturation, v);
    mLightnessCurvesPaintDelegate->setCorrectedValue(ColorChannel_Saturation, v);
    if (mEmitSignals)
        emit relSaturationChanged(v);
}

void FilterWidget::on_mSliderRelLightness_valueChanged(int v)
{
    ui->mSpinRelLightness->setValue(v);
    if (mEmitSignals)
        emit relLightnessChanged(v);
}

void FilterWidget::on_mSpinRelLightness_valueChanged(int v)
{
    ui->mSliderRelLightness->setValue(v);
    mHueCurvesPaintDelegate->setCorrectedValue(ColorChannel_Lightness, v);
    mSaturationCurvesPaintDelegate->setCorrectedValue(ColorChannel_Lightness, v);
    mLightnessCurvesPaintDelegate->setCorrectedValue(ColorChannel_Lightness, v);
    if (mEmitSignals)
        emit relLightnessChanged(v);
}

void FilterWidget::on_mSliderAbsHue_valueChanged(int v)
{
    ui->mSpinAbsHue->setValue(v);
    if (mEmitSignals)
        emit absHueChanged(v);
}

void FilterWidget::on_mSpinAbsHue_valueChanged(int v)
{
    ui->mSliderAbsHue->setValue(v);
    mHueCurvesPaintDelegate->setCorrectedValue(ColorChannel_Hue, v);
    mSaturationCurvesPaintDelegate->setCorrectedValue(ColorChannel_Hue, v);
    mLightnessCurvesPaintDelegate->setCorrectedValue(ColorChannel_Hue, v);
    if (mEmitSignals)
        emit absHueChanged(v);
}

void FilterWidget::on_mSliderAbsSaturation_valueChanged(int v)
{
    ui->mSpinAbsSaturation->setValue(v);
    if (mEmitSignals)
        emit absSaturationChanged(v);
}

void FilterWidget::on_mSpinAbsSaturation_valueChanged(int v)
{
    ui->mSliderAbsSaturation->setValue(v);
    mHueCurvesPaintDelegate->setCorrectedValue(ColorChannel_Saturation, v);
    mSaturationCurvesPaintDelegate->setCorrectedValue(ColorChannel_Saturation, v);
    mLightnessCurvesPaintDelegate->setCorrectedValue(ColorChannel_Saturation, v);
    if (mEmitSignals)
        emit absSaturationChanged(v);
}

void FilterWidget::on_mButtonColorize_toggled(bool v)
{
    ui->mSliderRelHue->setVisible(!v);
    ui->mSpinRelHue->setVisible(!v);
    ui->mSliderRelSaturation->setVisible(!v);
    ui->mSpinRelSaturation->setVisible(!v);

    ui->mSliderAbsHue->setVisible(v);
    ui->mSpinAbsHue->setVisible(v);
    ui->mSliderAbsSaturation->setVisible(v);
    ui->mSpinAbsSaturation->setVisible(v);

    mHueCurvesPaintDelegate->setColorize(v);
    mSaturationCurvesPaintDelegate->setColorize(v);
    mLightnessCurvesPaintDelegate->setColorize(v);
    if (v)
    {
        mHueCurvesPaintDelegate->setCorrectedValue(ColorChannel_Hue, ui->mSpinAbsHue->value());
        mSaturationCurvesPaintDelegate->setCorrectedValue(ColorChannel_Hue, ui->mSpinAbsHue->value());
        mLightnessCurvesPaintDelegate->setCorrectedValue(ColorChannel_Hue, ui->mSpinAbsHue->value());
        mHueCurvesPaintDelegate->setCorrectedValue(ColorChannel_Saturation, ui->mSpinAbsSaturation->value());
        mSaturationCurvesPaintDelegate->setCorrectedValue(ColorChannel_Saturation, ui->mSpinAbsSaturation->value());
        mLightnessCurvesPaintDelegate->setCorrectedValue(ColorChannel_Saturation, ui->mSpinAbsSaturation->value());
        mHueCurvesPaintDelegate->setCorrectedValue(ColorChannel_Lightness, ui->mSpinRelLightness->value());
        mSaturationCurvesPaintDelegate->setCorrectedValue(ColorChannel_Lightness, ui->mSpinRelLightness->value());
        mLightnessCurvesPaintDelegate->setCorrectedValue(ColorChannel_Lightness, ui->mSpinRelLightness->value());
    }
    else
    {
        mHueCurvesPaintDelegate->setCorrectedValue(ColorChannel_Hue, ui->mSpinRelHue->value());
        mSaturationCurvesPaintDelegate->setCorrectedValue(ColorChannel_Hue, ui->mSpinRelHue->value());
        mLightnessCurvesPaintDelegate->setCorrectedValue(ColorChannel_Hue, ui->mSpinRelHue->value());
        mHueCurvesPaintDelegate->setCorrectedValue(ColorChannel_Saturation, ui->mSpinRelSaturation->value());
        mSaturationCurvesPaintDelegate->setCorrectedValue(ColorChannel_Saturation, ui->mSpinRelSaturation->value());
        mLightnessCurvesPaintDelegate->setCorrectedValue(ColorChannel_Saturation, ui->mSpinRelSaturation->value());
        mHueCurvesPaintDelegate->setCorrectedValue(ColorChannel_Lightness, ui->mSpinRelLightness->value());
        mSaturationCurvesPaintDelegate->setCorrectedValue(ColorChannel_Lightness, ui->mSpinRelLightness->value());
        mLightnessCurvesPaintDelegate->setCorrectedValue(ColorChannel_Lightness, ui->mSpinRelLightness->value());
    }

    if (mEmitSignals)
        emit colorizeChanged(v);
}
