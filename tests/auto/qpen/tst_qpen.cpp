/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the test suite of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/


#include <QtTest/QtTest>

#include "qpen.h"
#include "qbrush.h"
#include "qcomplexstroker.h"

#include <qdebug.h>

//TESTED_CLASS=
//TESTED_FILES=

class tst_QPen : public QObject
{
    Q_OBJECT

public:
    tst_QPen();

private slots:
    void getSetCheck();
    void operator_eq_eq();
    void operator_eq_eq_data();

    void stream();
    void stream_data();

    void constructor();
    void constructor_data();
    
private:
    QPen getPen();
};

// Testing get/set functions
void tst_QPen::getSetCheck()
{
    QPen obj1;
    // qreal QPen::miterLimit()
    // void QPen::setMiterLimit(qreal)
    obj1.setMiterLimit(0.0);
    QCOMPARE(0.0, obj1.miterLimit());
    obj1.setMiterLimit(qreal(1.1));
    QCOMPARE(qreal(1.1), obj1.miterLimit());

    // qreal QPen::widthF()
    // void QPen::setWidthF(qreal)
    obj1.setWidthF(0.0);
    QCOMPARE(0.0, obj1.widthF());
    obj1.setWidthF(qreal(1.1));
    QCOMPARE(qreal(1.1), obj1.widthF());

    // int QPen::width()
    // void QPen::setWidth(int)
    for (int i = 0; i < 100; ++i) {
        obj1.setWidth(i);
        QCOMPARE(i, obj1.width());
    }

    QVector<qreal> compounds;
    compounds << 0 << 0.2 << 0.5 << 0.7 << 0.9 << 1.0;
    obj1.setCompoundArray(compounds);
    QCOMPARE(obj1.compoundArray(), compounds);
    obj1.setStartAnchorStyle(Qt::DiamondAnchor);
    obj1.setStartAnchor(QCustomLineAnchor(Qt::DiamondAnchor));

    obj1.setStartAnchorStyle(Qt::DiamondAnchor);
    QCOMPARE(obj1.startAnchorStyle(), Qt::DiamondAnchor);
    obj1.setStartAnchor(QCustomLineAnchor(Qt::DiamondAnchor));
    QCOMPARE(obj1.startAnchor(), QCustomLineAnchor(Qt::DiamondAnchor));

    obj1.setEndAnchorStyle(Qt::DiamondAnchor);
    QCOMPARE(obj1.endAnchorStyle(), Qt::DiamondAnchor);
    obj1.setEndAnchor(QCustomLineAnchor(Qt::DiamondAnchor));
    QCOMPARE(obj1.endAnchor(), QCustomLineAnchor(Qt::DiamondAnchor));

    obj1.setAlignment(Qt::PenAlignmentInset);
    QCOMPARE(obj1.alignment(), Qt::PenAlignmentInset);

    obj1.setStartCapStyle(Qt::RoundCap);
    QCOMPARE(obj1.startCapStyle(), Qt::RoundCap);
    obj1.setEndCapStyle(Qt::SquareCap);
    QCOMPARE(obj1.endCapStyle(), Qt::SquareCap);
    obj1.setDashCapStyle(Qt::TriangleCap);
    QCOMPARE(obj1.dashCapStyle(), Qt::TriangleCap);

    obj1.setCapStyle(Qt::RoundCap);
    QCOMPARE(obj1.capStyle(), Qt::RoundCap);
    QCOMPARE(obj1.startCapStyle(), Qt::RoundCap);
    QCOMPARE(obj1.endCapStyle(), Qt::RoundCap);
    QCOMPARE(obj1.dashCapStyle(), Qt::RoundCap);
}

Q_DECLARE_METATYPE(QPen)
Q_DECLARE_METATYPE(QBrush)

tst_QPen::tst_QPen()

{
}

void tst_QPen::operator_eq_eq_data()
{
    QTest::addColumn<QPen>("pen1");
    QTest::addColumn<QPen>("pen2");
    QTest::addColumn<bool>("isEqual");

    QTest::newRow("differentColor") << QPen(Qt::red)
				 << QPen(Qt::blue)
				 << bool(FALSE);
    QTest::newRow("differentWidth") << QPen(Qt::red, 2)
				 << QPen(Qt::red, 3)
				 << bool(FALSE);
    QTest::newRow("differentPenStyle") << QPen(Qt::red, 2, Qt::DashLine)
				    << QPen(Qt::red, 2, Qt::DotLine)
				    << bool(FALSE);
    QTest::newRow("differentCapStyle") << QPen(Qt::red, 2, Qt::DashLine, Qt::RoundCap, Qt::BevelJoin)
				    << QPen(Qt::red, 2, Qt::DashLine, Qt::SquareCap, Qt::BevelJoin)
				    << bool(FALSE);
    QTest::newRow("differentJoinStyle") << QPen(Qt::red, 2, Qt::DashLine, Qt::RoundCap, Qt::BevelJoin)
				     << QPen(Qt::red, 2, Qt::DashLine, Qt::RoundCap, Qt::MiterJoin)
				     << bool(FALSE);
    QTest::newRow("same") << QPen(Qt::red, 2, Qt::DashLine, Qt::RoundCap, Qt::BevelJoin)
		       << QPen(Qt::red, 2, Qt::DashLine, Qt::RoundCap, Qt::BevelJoin)
		       << bool(TRUE);
    QPen pen1 = getPen();
    pen1.setStartAnchor(QCustomLineAnchor(Qt::DiamondAnchor));
    QPen pen2 = getPen();
    pen2.setStartAnchor(QCustomLineAnchor(Qt::SquareAnchor));
    QTest::newRow("differentStartAnchor") << pen1 << pen2 << false;

    pen2.setStartAnchor(QCustomLineAnchor(Qt::DiamondAnchor));
    QTest::newRow("same pen") << pen1 << pen2 << true;

    pen2 = pen1;
    pen1.setAlignment(Qt::PenAlignmentInset);
    pen2.setAlignment(Qt::PenAlignmentOutset);
    QTest::newRow("differentAlignment") << pen1 << pen2 << false;

    pen2 = pen1;
    pen1.setDashCapStyle(Qt::SquareCap);
    pen2.setDashCapStyle(Qt::RoundCap);
    QTest::newRow("differentDashCap") << pen1 << pen2 << false;

    pen2 = pen1;
    QVector<qreal> comps;
    comps << 0 << 0.2 << 0.5 << 1.0;
    pen1.setCompoundArray(comps);
    comps[2] = 0.6;
    pen2.setCompoundArray(comps);
    QTest::newRow("differentCompundArray") << pen1 << pen2 << false;
}

void tst_QPen::operator_eq_eq()
{
    QFETCH(QPen, pen1);
    QFETCH(QPen, pen2);
    QFETCH(bool, isEqual);
    QCOMPARE(pen1 == pen2, isEqual);
}


void tst_QPen::constructor_data()
{
    QTest::addColumn<QPen>("pen");
    QTest::addColumn<QBrush>("brush");
    QTest::addColumn<double>("width");
    QTest::addColumn<int>("style");
    QTest::addColumn<int>("capStyle");
    QTest::addColumn<int>("joinStyle");

    QTest::newRow("solid_black") << QPen() << QBrush(Qt::black) << 0. << (int)Qt::SolidLine
                              << (int) Qt::SquareCap << (int)Qt::BevelJoin;
    QTest::newRow("solid_red") << QPen(Qt::red) << QBrush(Qt::red) << 0. << (int)Qt::SolidLine
                            << (int)Qt::SquareCap << (int)Qt::BevelJoin;
    QTest::newRow("full") << QPen(QBrush(QLinearGradient(0, 0, 100, 100)), 10,
                               Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin)
                       << QBrush(QLinearGradient(0, 0, 100, 100)) << 10. << (int)Qt::SolidLine
                       << (int)Qt::RoundCap << (int)Qt::MiterJoin;

}


void tst_QPen::constructor()
{
    QFETCH(QPen, pen);
    QFETCH(QBrush, brush);
    QFETCH(double, width);
    QFETCH(int, style);
    QFETCH(int, capStyle);
    QFETCH(int, joinStyle);

    QCOMPARE(pen.style(), Qt::PenStyle(style));
    QCOMPARE(pen.capStyle(), Qt::PenCapStyle(capStyle));
    QCOMPARE(pen.joinStyle(), Qt::PenJoinStyle(joinStyle));
    QCOMPARE(pen.widthF(), width);
    QCOMPARE(pen.brush(), brush);
}


void tst_QPen::stream_data()
{
    QTest::addColumn<QPen>("pen");

    QTest::newRow("solid_black") << QPen();
    QTest::newRow("solid_red") << QPen(Qt::red);
    QTest::newRow("full") << QPen(QBrush(QLinearGradient(0, 0, 100, 100)), 10, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
}


void tst_QPen::stream()
{
    QFETCH(QPen, pen);

    QByteArray bytes;

    {
        QDataStream stream(&bytes, QIODevice::WriteOnly);
        stream << pen;
    }

    QPen cmp;
    {
        QDataStream stream(&bytes, QIODevice::ReadOnly);
        stream >> cmp;
    }

    QCOMPARE(pen.widthF(), cmp.widthF());
    QCOMPARE(pen.style(), cmp.style());
    QCOMPARE(pen.capStyle(), cmp.capStyle());
    QCOMPARE(pen.joinStyle(), cmp.joinStyle());
    QCOMPARE(pen.brush(), cmp.brush());

    QCOMPARE(pen, cmp);
}

QPen tst_QPen::getPen()
{
    QPen pen;
    QVector<qreal> compounds;
    compounds << 0 << 0.2 << 0.5 << 0.7 << 0.9 << 1.0;
    pen.setCompoundArray(compounds);
    pen.setStartAnchorStyle(Qt::DiamondAnchor);
    pen.setStartAnchor(QCustomLineAnchor(Qt::DiamondAnchor));
    pen.setEndAnchorStyle(Qt::DiamondAnchor);
    pen.setStartAnchor(QCustomLineAnchor(Qt::DiamondAnchor));
    pen.setEndAnchorStyle(Qt::DiamondAnchor);
    pen.setEndAnchor(QCustomLineAnchor(Qt::DiamondAnchor));
    pen.setAlignment(Qt::PenAlignmentInset);
    pen.setStartCapStyle(Qt::RoundCap);
    pen.setEndCapStyle(Qt::SquareCap);
    pen.setDashCapStyle(Qt::TriangleCap);

    return pen;
}

QTEST_APPLESS_MAIN(tst_QPen)
#include "tst_qpen.moc"
