/***************************************************************************
 *   Copyright (C) 2011 by Peter Penz <peter.penz19@gmail.com>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA            *
 ***************************************************************************/

#ifndef KFILEITEMLISTWIDGET_H
#define KFILEITEMLISTWIDGET_H

#include "dolphin_export.h"

#include <kitemviews/kstandarditemlistwidget.h>

class DOLPHIN_EXPORT KFileItemListWidgetInformant : public KStandardItemListWidgetInformant
{
public:
    KFileItemListWidgetInformant();
    virtual ~KFileItemListWidgetInformant();

protected:
    virtual QString itemText(int index, const KItemListView* view) const Q_DECL_OVERRIDE;
    virtual bool itemIsLink(int index, const KItemListView* view) const Q_DECL_OVERRIDE;
    virtual QString roleText(const QByteArray& role, const QHash<QByteArray, QVariant>& values) const Q_DECL_OVERRIDE;
    virtual QFont customizedFontForLinks(const QFont& baseFont) const Q_DECL_OVERRIDE;
};

class DOLPHIN_EXPORT KFileItemListWidget : public KStandardItemListWidget
{
    Q_OBJECT

public:
    KFileItemListWidget(KItemListWidgetInformant* informant, QGraphicsItem* parent);
    virtual ~KFileItemListWidget();

    static KItemListWidgetInformant* createInformant();

protected:
    virtual bool isRoleRightAligned(const QByteArray& role) const Q_DECL_OVERRIDE;
    virtual bool isHidden() const Q_DECL_OVERRIDE;
    virtual QFont customizedFont(const QFont& baseFont) const Q_DECL_OVERRIDE;

    /**
     * @return Selection length without MIME-type extension
     */
    virtual int selectionLength(const QString& text) const Q_DECL_OVERRIDE;
};

#endif


