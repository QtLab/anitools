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

#include <QHash>

#include "filter.h"
#include "../imgproc/imagefilter.h"

using namespace anitools::imgproc;

#ifdef Q_OS_WIN32
#define ANITOOLS_EXPORT __declspec(dllexport)
#else
#define ANITOOLS_EXPORT
#endif

extern "C" ANITOOLS_EXPORT QHash<QString, QString> getAnitoolsPluginInfo()
{
    QHash<QString, QString> info;

    info.insert("id", "anitools.imagefilter.bilateralfilter");
    info.insert("version", "0.1.0");
    info.insert("name", QObject::tr("Bilateral Filter"));
    info.insert("description", QObject::tr("Blurs the image using a smooth gaussian "
                                           "distribution but preserving hard edges"));
    info.insert("tags", QObject::tr("Smooth, Noise"));
    info.insert("author", QObject::tr("Deif Lou"));
    info.insert("copyright", QObject::tr(""));
    info.insert("url", QObject::tr(""));
    return info;
}

extern "C" ANITOOLS_EXPORT ImageFilter * getImageFilterInstance()
{
    return new Filter();
}
