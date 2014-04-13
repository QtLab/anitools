#include <QDebug>

#include "colorpicker.h"
#include "ui_colorpicker.h"
#include "../imgproc/types.h"

namespace anitools {
namespace widgets {

using namespace anitools::imgproc;

#define INIT_WIDGETS(arg1, arg2, arg3) \
    ui->mSlider##arg1->setColorModel(arg3); \
    ui->mSlider##arg1->setColorChannel(arg2);

ColorPicker::ColorPicker(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ColorPicker),
    mCanUpdate(false)
{
    ui->setupUi(this);

    ui->mBoxColor->setColorModel(ColorModel_HSV);
    ui->mBoxColor->setColorChannel(ColorChannel_Hue);
    INIT_WIDGETS(Big, ColorChannel_Hue, ColorModel_HSV)

    INIT_WIDGETS(Blue, ColorChannel_Blue, ColorModel_BGR)
    INIT_WIDGETS(Green, ColorChannel_Green, ColorModel_BGR)
    INIT_WIDGETS(Red, ColorChannel_Red, ColorModel_BGR)
    INIT_WIDGETS(Alpha, ColorChannel_Alpha, ColorModel_BGR)

    INIT_WIDGETS(Hue1, ColorChannel_Hue, ColorModel_HSV)
    INIT_WIDGETS(Saturation1, ColorChannel_Saturation, ColorModel_HSV)
    INIT_WIDGETS(Value, ColorChannel_Value, ColorModel_HSV)

    INIT_WIDGETS(Hue2, ColorChannel_Hue, ColorModel_HSL)
    INIT_WIDGETS(Saturation2, ColorChannel_Saturation, ColorModel_HSL)
    INIT_WIDGETS(Lightness, ColorChannel_Lightness, ColorModel_HSL)

    INIT_WIDGETS(CIEL, ColorChannel_CIEL, ColorModel_Lab)
    INIT_WIDGETS(CIEa, ColorChannel_CIEa, ColorModel_Lab)
    INIT_WIDGETS(CIEb, ColorChannel_CIEb, ColorModel_Lab)

    INIT_WIDGETS(Cyan, ColorChannel_Cyan, ColorModel_CMYK)
    INIT_WIDGETS(Magenta, ColorChannel_Magenta, ColorModel_CMYK)
    INIT_WIDGETS(Yellow, ColorChannel_Yellow, ColorModel_CMYK)
    INIT_WIDGETS(Black, ColorChannel_Black, ColorModel_CMYK)

    mMainSlider = ui->mSliderHue1;

    setColor(QColor(255, 0, 0, 255));

    mCanUpdate = true;
}

ColorPicker::~ColorPicker()
{
    delete ui;
}

QColor ColorPicker::color() const
{
    return ui->mSliderRed->color();
}

void ColorPicker::setColor(const QColor &color)
{
    unsigned int srcColor = color.rgba();
    unsigned int srcColor2 = 0xFF000000 | (srcColor & 0xFFFFFF);

    mCanUpdate = false;
    ui->mSliderRed->setColor(srcColor2);
    ui->mSliderGreen->setColor(srcColor2);
    ui->mSliderBlue->setColor(srcColor2);
    ui->mSliderAlpha->setColor(srcColor);
    ui->mSpinRed->setValue(ui->mSliderRed->value());
    ui->mSpinGreen->setValue(ui->mSliderGreen->value());
    ui->mSpinBlue->setValue(ui->mSliderBlue->value());
    ui->mSpinAlpha->setValue(ui->mSliderAlpha->value());
    rgbChanged(srcColor);
    unsigned int mainSliderColor;
    mMainSlider->color(&mainSliderColor);
    ui->mSliderBig->setColor(mainSliderColor);
    ui->mBoxColor->setColor(mainSliderColor);
    mCanUpdate = true;
}

void ColorPicker::on_mSliderBig_valueChanged(int v)
{
    if (!mCanUpdate)
        return;
    mMainSlider->setValue(v);
}

#define CHANGE_BGR \
    ui->mSliderRed->setColor(*dstColor2); \
    ui->mSliderGreen->setColor(*dstColor2); \
    ui->mSliderBlue->setColor(*dstColor2); \
    ui->mSpinRed->setValue(dstColor[ColorChannel_Red]); \
    ui->mSpinGreen->setValue(dstColor[ColorChannel_Green]); \
    ui->mSpinBlue->setValue(dstColor[ColorChannel_Blue]);
#define CHANGE_HSV \
    ui->mSliderHue1->setColor(*dstColor2); \
    ui->mSliderSaturation1->setColor(*dstColor2); \
    ui->mSliderValue->setColor(*dstColor2); \
    ui->mSpinHue1->setValue(qRound(dstColor[ColorChannel_Hue] * 359 / 255.)); \
    ui->mSpinSaturation1->setValue(qRound(dstColor[ColorChannel_Saturation] * 100 / 255.)); \
    ui->mSpinValue->setValue(qRound(dstColor[ColorChannel_Value] * 100 / 255.));
#define CHANGE_HSL \
    ui->mSliderHue2->setColor(*dstColor2); \
    ui->mSliderSaturation2->setColor(*dstColor2); \
    ui->mSliderLightness->setColor(*dstColor2); \
    ui->mSpinHue2->setValue(qRound(dstColor[ColorChannel_Hue] * 359 / 255.)); \
    ui->mSpinSaturation2->setValue(qRound(dstColor[ColorChannel_Saturation] * 100 / 255.)); \
    ui->mSpinLightness->setValue(qRound(dstColor[ColorChannel_Lightness] * 100 / 255.));
#define CHANGE_Lab \
    ui->mSliderCIEL->setColor(*dstColor2); \
    ui->mSliderCIEa->setColor(*dstColor2); \
    ui->mSliderCIEb->setColor(*dstColor2); \
    ui->mSpinCIEL->setValue(qRound(dstColor[ColorChannel_CIEL] * 100 / 255.)); \
    ui->mSpinCIEa->setValue(dstColor[ColorChannel_CIEa] - 128); \
    ui->mSpinCIEb->setValue(dstColor[ColorChannel_CIEb] - 128);
#define CHANGE_CMYK \
    ui->mSliderCyan->setColor(*dstColor2); \
    ui->mSliderMagenta->setColor(*dstColor2); \
    ui->mSliderYellow->setColor(*dstColor2); \
    ui->mSliderBlack->setColor(*dstColor2); \
    ui->mSpinCyan->setValue(qRound(dstColor[ColorChannel_Cyan] * 100 / 255.)); \
    ui->mSpinMagenta->setValue(qRound(dstColor[ColorChannel_Magenta] * 100 / 255.)); \
    ui->mSpinYellow->setValue(qRound(dstColor[ColorChannel_Yellow] * 100 / 255.)); \
    ui->mSpinBlack->setValue(qRound(dstColor[ColorChannel_Black] * 100 / 255.));

#define COLORGROUP_CHANGED(arg1, arg2, arg3, arg4, arg5, arg6) \
void ColorPicker::arg1##Changed(unsigned int srcColor) \
{ \
    unsigned char dstColor[4]; \
    unsigned int * dstColor2 = (unsigned int *)dstColor; \
 \
    convert##arg2##To##arg3((unsigned char *)&srcColor, dstColor, 1); \
    CHANGE_##arg3 \
    convert##arg2##To##arg4((unsigned char *)&srcColor, dstColor, 1); \
    CHANGE_##arg4 \
    convert##arg2##To##arg5((unsigned char *)&srcColor, dstColor, 1); \
    CHANGE_##arg5 \
    convert##arg2##To##arg6((unsigned char *)&srcColor, dstColor, 1); \
    CHANGE_##arg6 \
}

COLORGROUP_CHANGED(rgb, BGR, HSV, HSL, Lab, CMYK)
COLORGROUP_CHANGED(hsv, HSV, BGR, HSL, Lab, CMYK)
COLORGROUP_CHANGED(hsl, HSL, BGR, HSV, Lab, CMYK)
COLORGROUP_CHANGED(lab, Lab, BGR, HSV, HSL, CMYK)
COLORGROUP_CHANGED(cmyk, CMYK, BGR, HSV, HSL, Lab)

#define SLIDER_CODE(arg1, arg2, arg3, arg4, arg5) \
void ColorPicker::on_mSlider##arg1##_valueChanged(int v) \
{ \
    if (!mCanUpdate) \
        return; \
    mCanUpdate = false; \
    unsigned int srcColor; \
    ui->mSlider##arg1->color(&srcColor); \
    ui->mSlider##arg2->setColor(srcColor); \
    ui->mSlider##arg3->setColor(srcColor); \
    ui->mSpin##arg1->setValue(qRound((double)v arg5)); \
    arg4##Changed(srcColor); \
    ui->mSliderAlpha->setColor(ui->mSliderBlue->value(), ui->mSliderGreen->value(), \
                               ui->mSliderRed->value(), ui->mSliderAlpha->value()); \
    unsigned int mainSliderColor; \
    mMainSlider->color(&mainSliderColor); \
    ui->mSliderBig->setColor(mainSliderColor); \
    ui->mBoxColor->setColor(mainSliderColor); \
    mCanUpdate = true; \
    QColor c = ui->mSliderRed->color(); \
    c.setAlpha(ui->mSliderAlpha->value()); \
    emit colorChanged(c); \
}
#define SLIDER_CODE2(arg1, arg2, arg3, arg4, arg5, arg6) \
    void ColorPicker::on_mSlider##arg1##_valueChanged(int v) \
{ \
    if (!mCanUpdate) \
    return; \
    mCanUpdate = false; \
    unsigned int srcColor; \
    ui->mSlider##arg1->color(&srcColor); \
    ui->mSlider##arg2->setColor(srcColor); \
    ui->mSlider##arg3->setColor(srcColor); \
    ui->mSlider##arg4->setColor(srcColor); \
    ui->mSpin##arg1->setValue(qRound((double)v arg6)); \
    arg5##Changed(srcColor); \
    ui->mSliderAlpha->setColor(ui->mSliderBlue->value(), ui->mSliderGreen->value(), \
                               ui->mSliderRed->value(), ui->mSliderAlpha->value()); \
    unsigned int mainSliderColor; \
    mMainSlider->color(&mainSliderColor); \
    ui->mSliderBig->setColor(mainSliderColor); \
    ui->mBoxColor->setColor(mainSliderColor); \
    mCanUpdate = true; \
    QColor c = ui->mSliderRed->color(); \
    c.setAlpha(ui->mSliderAlpha->value()); \
    emit colorChanged(c); \
}

SLIDER_CODE(Red, Green, Blue, rgb, )
SLIDER_CODE(Green, Red, Blue, rgb, )
SLIDER_CODE(Blue, Red, Green, rgb, )
SLIDER_CODE(Hue1, Saturation1, Value, hsv, * 359 / 255)
SLIDER_CODE(Saturation1, Hue1, Value, hsv, * 100 / 255)
SLIDER_CODE(Value, Hue1, Saturation1, hsv, * 100 / 255)
SLIDER_CODE(Hue2, Saturation2, Lightness, hsl, * 359 / 255)
SLIDER_CODE(Saturation2, Hue2, Lightness, hsl, * 100 / 255)
SLIDER_CODE(Lightness, Hue2, Saturation2, hsl, * 100 / 255)
SLIDER_CODE(CIEL, CIEa, CIEb, lab, * 100 / 255)
SLIDER_CODE(CIEa, CIEL, CIEb, lab, - 128)
SLIDER_CODE(CIEb, CIEL, CIEa, lab, - 128)
SLIDER_CODE2(Cyan, Magenta, Yellow, Black, cmyk, * 100 / 255)
SLIDER_CODE2(Magenta, Cyan, Yellow, Black, cmyk, * 100 / 255)
SLIDER_CODE2(Yellow, Cyan, Magenta, Black, cmyk, * 100 / 255)
SLIDER_CODE2(Black, Cyan, Magenta, Yellow, cmyk, * 100 / 255)

void ColorPicker::on_mSliderAlpha_valueChanged(int v)
{
    if (!mCanUpdate)
        return;
    mCanUpdate = false;
    unsigned int srcColor;
    ui->mSliderAlpha->color(&srcColor);
    ui->mSpinAlpha->setValue(v);
    mCanUpdate = true;
    QColor c = ui->mSliderRed->color();
    c.setAlpha(v);
    emit colorChanged(c);
}

#define SPIN_CODE(arg1, arg2) \
void ColorPicker::on_mSpin##arg1##_valueChanged(int v) \
{ \
    ui->mSlider##arg1->setValue(qRound((double)v arg2)); \
}

SPIN_CODE(Red, )
SPIN_CODE(Green, )
SPIN_CODE(Blue, )
SPIN_CODE(Alpha, )
SPIN_CODE(Hue1, * 255 / 359)
SPIN_CODE(Saturation1, * 255 / 100)
SPIN_CODE(Value, * 255 / 100)
SPIN_CODE(Hue2, * 255 / 359)
SPIN_CODE(Saturation2, * 255 / 100)
SPIN_CODE(Lightness, * 255 / 100)
SPIN_CODE(CIEL, * 255 / 100)
SPIN_CODE(CIEa, + 128)
SPIN_CODE(CIEb, + 128)
SPIN_CODE(Cyan, * 255 / 100)
SPIN_CODE(Magenta, * 255 / 100)
SPIN_CODE(Yellow, * 255 / 100)
SPIN_CODE(Black, * 255 / 100)

#define BUTTON_CODE(arg1) \
void ColorPicker::on_mButton##arg1##_toggled(bool c) \
{ \
    if (!c) \
        return; \
    mCanUpdate = false; \
    ui->mSliderBig->setColorModel(ui->mSlider##arg1->colorModel()); \
    ui->mSliderBig->setColorChannel(ui->mSlider##arg1->colorChannel()); \
    ui->mBoxColor->setColorModel(ui->mSlider##arg1->colorModel()); \
    ui->mBoxColor->setColorChannel(ui->mSlider##arg1->colorChannel()); \
    mMainSlider = ui->mSlider##arg1; \
    unsigned int mainSliderColor; \
    mMainSlider->color(&mainSliderColor); \
    ui->mSliderBig->setColor(mainSliderColor); \
    ui->mBoxColor->setColor(mainSliderColor); \
    mCanUpdate = true; \
}

BUTTON_CODE(Red)
BUTTON_CODE(Green)
BUTTON_CODE(Blue)
BUTTON_CODE(Hue1)
BUTTON_CODE(Saturation1)
BUTTON_CODE(Value)
BUTTON_CODE(Hue2)
BUTTON_CODE(Saturation2)
BUTTON_CODE(Lightness)
BUTTON_CODE(CIEL)
BUTTON_CODE(CIEa)
BUTTON_CODE(CIEb)
BUTTON_CODE(Cyan)
BUTTON_CODE(Magenta)
BUTTON_CODE(Yellow)

#define BOX_CODE(arg1, arg2, arg3, arg4, arg5, arg6, arg7) \
    { \
        ui->mSlider##arg1->setColor(*colorUInt); \
        ui->mSlider##arg2->setColor(*colorUInt); \
        ui->mSlider##arg3->setColor(*colorUInt); \
        ui->mSpin##arg1->setValue(qRound((double)ui->mSlider##arg1->value() arg5)); \
        ui->mSpin##arg2->setValue(qRound((double)ui->mSlider##arg2->value() arg6)); \
        ui->mSpin##arg3->setValue(qRound((double)ui->mSlider##arg3->value() arg7)); \
        arg4##Changed(*colorUInt); \
    }

void ColorPicker::on_mBoxColor_colorChanged()
{
    if (!mCanUpdate)
        return;

    unsigned char color[4] = { 0 };
    unsigned int * colorUInt = (unsigned int *)color;
    ui->mBoxColor->color(colorUInt);

    mCanUpdate = false;

    if (mMainSlider == ui->mSliderRed || mMainSlider == ui->mSliderGreen || mMainSlider == ui->mSliderBlue)

    BOX_CODE(Red, Green, Blue, rgb, , , )

    else if (mMainSlider == ui->mSliderHue1 || mMainSlider == ui->mSliderSaturation1 ||
             mMainSlider == ui->mSliderValue)

    BOX_CODE(Hue1, Saturation1, Value, hsv, * 359 / 255, * 100 / 255, * 100 / 255)

    else if (mMainSlider == ui->mSliderHue2 || mMainSlider == ui->mSliderSaturation2 ||
             mMainSlider == ui->mSliderLightness)

    BOX_CODE(Hue2, Saturation2, Lightness, hsl, * 359 / 255, * 100 / 255, * 100 / 255)

    else if (mMainSlider == ui->mSliderCIEL || mMainSlider == ui->mSliderCIEa || mMainSlider == ui->mSliderCIEb)

    BOX_CODE(CIEL, CIEa, CIEb, lab, * 100 / 255, - 128, - 128)

    else

    BOX_CODE(Cyan, Magenta, Yellow, cmyk, * 100 / 255, * 100 / 255, * 100 / 255)

    unsigned int mainSliderColor;
    mMainSlider->color(&mainSliderColor);
    ui->mSliderBig->setColor(mainSliderColor);
    ui->mSliderAlpha->setColor(ui->mSliderBlue->value(), ui->mSliderGreen->value(),
                               ui->mSliderRed->value(), ui->mSliderAlpha->value());
    mCanUpdate = true;
}

}}
