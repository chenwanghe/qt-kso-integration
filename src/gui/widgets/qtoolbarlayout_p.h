/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtGui module of the Qt Toolkit.
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

#ifndef QTOOLBARLAYOUT_P_H
#define QTOOLBARLAYOUT_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtGui/qlayout.h>
#include <private/qlayoutengine_p.h>
#include <QVector>

QT_BEGIN_NAMESPACE

#ifndef QT_NO_TOOLBAR

class QAction;
class QToolBarExtension;
class QMenu;
class QAbstractButton;

class QToolBarItem : public QWidgetItem
{
public:
    QToolBarItem(QWidget *widget);
    bool isEmpty() const;

	QSize sizeHint(Qt::Orientation o) const;
	QSize minimumSize(Qt::Orientation o) const;
	QSize maximumSize(Qt::Orientation o) const;
	void setGeometry(Qt::Orientation o, const QRect& rc);

public:
    QAction *action;
    bool customWidget;
};

class QToolBarLayout : public QLayout
{
    Q_OBJECT

public:
    QToolBarLayout(QWidget *parent = 0);
    ~QToolBarLayout();

    void addItem(QLayoutItem *item);
    QLayoutItem *itemAt(int index) const;
    QLayoutItem *takeAt(int index);
    int count() const;

    bool isEmpty() const;
    void invalidate();
    Qt::Orientations expandingDirections() const;

    void setGeometry(const QRect &r);
    QSize minimumSize() const;
    QSize sizeHint() const;

	QSize dockSizeHint(Qt::Orientation o) const;
	QSize dockMinimumSize(Qt::Orientation o) const;

    void insertAction(int index, QAction *action);
    int indexOf(QAction *action) const;
    int indexOf(QWidget *widget) const { return QLayout::indexOf(widget); }

    bool layoutActions(const QSize &size);
    QSize expandedSize(const QSize &size) const;
    bool expanded, animating;

    void setUsePopupMenu(bool set); // Yeah, there's no getter, but it's internal.
    void checkUsePopupMenu();

    bool movable() const;
    void updateMarginAndSpacing();
    bool hasExpandFlag() const;
	int titleHeight() const;
	QSize getMargins(Qt::Orientation o, bool bFloating) const;
	QRect dragRect() const;

public Q_SLOTS:
    void setExpanded(bool b);
	void setExpanded(bool bExp, bool bAnimating);

private:
    QList<QToolBarItem*> items;
    QSize hint, minSize;
	QSize dockHint, dockMinSize;
	QSize dockHintVert, dockMinSizeVert;
    bool dirty, expanding, empty, expandFlag;
    QVector<QLayoutStruct> geomArray;
    QRect handRect;
    QToolBarExtension *extension;

    void updateGeomArray() const;
	void updateDockSize();

    QToolBarItem *createItem(QAction *action);
    QMenu *popupMenu;
	QAbstractButton* closebutton;
};

#endif // QT_NO_TOOLBAR

QT_END_NAMESPACE

#endif // QTOOLBARLAYOUT_P_H
