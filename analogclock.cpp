/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>
#include <QtCore/qmath.h>

#include "analogclock.h"

//! [0] //! [1]
AnalogClock::AnalogClock(QWidget *parent)
//! [0] //! [2]
    : QWidget(parent)
    //! [2] //! [3]
{
    //! [3] //! [4]
    QTimer *timer = new QTimer(this);
    //! [4] //! [5]
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    //! [5] //! [6]
    timer->start(1000);
    //! [6]

    setWindowTitle(tr("Analog Clock"));
    setWindowOpacity(1);
    setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setCursor(Qt::OpenHandCursor);

    QDesktopWidget* desktop = QApplication::desktop();
    move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);

    resize(400, 400);
    //! [7]
}
//! [1] //! [7]

//! [8] //! [9]
void AnalogClock::paintEvent(QPaintEvent *)
//! [8] //! [10]
{
    static const QPoint hourHand[3] = {
        QPoint(7, 8),
        QPoint(-7, 8),
        QPoint(0, -40)
    };
    static const QPoint minuteHand[3] = {
        QPoint(7, 8),
        QPoint(-7, 8),
        QPoint(0, -55)
    };
    static const QPoint secondHand[3] = {
        QPoint(4, 8),
        QPoint(-4, 8),
        QPoint(0, -70)
    };

    QColor hourColor(127, 0, 127);
    QColor minuteColor(0, 127, 127, 191);
    QColor secondColor(0xff, 0x22, 0, 191);

    int side = qMin(width(), height());
    QTime time = QTime::currentTime();
    QDate date = QDate::currentDate();
    //! [10]

    //! [11]
    QPainter painter(this);
    //! [11] //! [12]
    painter.setRenderHint(QPainter::Antialiasing);
    //! [12] //! [13]
    painter.translate(width() / 2, height() / 2);
    //! [13] //! [14]
    painter.scale(side / 200.0, side / 200.0);
    //! [9] //! [14]

    painter.setPen(minuteColor);
    painter.setBrush(QColor(255, 255, 255, 128));
    painter.drawEllipse(QPointF(0, 0), 96, 96);

    painter.setPen(QColor(0, 0, 0, 128));
    QString t;
    t.sprintf("%02d:%02d:%02d", time.hour(), time.minute(), time.second());
    painter.drawText(QRectF(-50, -50, 100, 100), Qt::AlignHCenter | Qt::AlignTop, t);
    t.sprintf("%04d-%02d-%02d", date.year(), date.month(), date.day());
    painter.drawText(QRectF(-50, 30, 100, 100), Qt::AlignHCenter | Qt::AlignTop, t);

    //! [15]
    painter.setPen(Qt::NoPen);
    //! [15] //! [16]
    painter.setBrush(hourColor);
    //! [16]

    //! [17] //! [18]
    painter.save();
    //! [17] //! [19]
    painter.rotate(30.0 * (time.hour() + time.minute() / 60.0 + time.second() / 3600.0));
    painter.drawConvexPolygon(hourHand, 3);
    painter.restore();
    //! [18] //! [19]

    //! [20]
    painter.setPen(hourColor);
    //! [20] //! [21]

    for (int i = 0; i < 12; ++i) {
        painter.drawLine(88, 0, 96, 0);
        painter.rotate(30.0);
    }
    //! [21]

    //! [22]
    painter.setPen(Qt::NoPen);
    //! [22] //! [23]
    painter.setBrush(minuteColor);

    //! [24]
    painter.save();
    painter.rotate(6.0 * (time.minute() + time.second() / 60.0));
    painter.drawConvexPolygon(minuteHand, 3);
    painter.restore();
    //! [23] //! [24]

    //! [25]
    painter.setPen(minuteColor);
    //! [25] //! [26]

    //! [27]
    for (int j = 0; j < 60; ++j) {
        if ((j % 5) != 0)
            painter.drawLine(92, 0, 96, 0);
        painter.rotate(6.0);
    }
    //! [27]

    painter.setPen(Qt::NoPen);
    painter.setBrush(secondColor);
    painter.save();
    painter.rotate(6.0 * time.second());
    painter.drawConvexPolygon(secondHand, 3);
    painter.drawEllipse(QPointF(0, -55), 3, 3);
    painter.restore();

    painter.save();
    painter.setBrush(Qt::NoBrush);
    painter.setPen(hourColor);
    qreal rangle = 0.0, x, y;
    for (int i = 0; i < 12; ++i) {
        rangle = (i*30.0-60.0)*3.14159265/180.0;
        x = 78.0*qCos(rangle);
        y = 78.0*qSin(rangle);
        painter.drawText(x-18.0, y-10.0, 40, 40, Qt::AlignHCenter | Qt::AlignTop, QString("%1").arg(i+1));
        //painter.drawRect(QRect(x-20.0, y-20.0, 40, 40));
    }
    painter.restore();
    painter.setBrush(QColor(255,255,255, 191));
    painter.drawEllipse(QPointF(0, 0), 3, 3);
}
//! [26]

void AnalogClock::mousePressEvent(QMouseEvent *event){
    windowPos = this->pos();
    mousePos = event->globalPos();
    dPos = mousePos - windowPos;
    setCursor(Qt::ClosedHandCursor);
}
void AnalogClock::mouseMoveEvent(QMouseEvent *event){
    move(event->globalPos() - dPos);
}
void AnalogClock::mouseReleaseEvent(QMouseEvent *event){
    setCursor(Qt::OpenHandCursor);
}
