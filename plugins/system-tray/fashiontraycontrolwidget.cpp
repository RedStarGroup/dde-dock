/*
 * Copyright (C) 2011 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     listenerri <listenerri@gmail.com>
 *
 * Maintainer: listenerri <listenerri@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "fashiontraycontrolwidget.h"

#include <QMouseEvent>
#include <QPainter>

#define ExpandedKey "fashion-tray-expanded"

FashionTrayControlWidget::FashionTrayControlWidget(Dock::Position position, QWidget *parent)
    : QWidget(parent),
      m_settings(new QSettings("deepin", "dde-dock-tray")),
      m_dockPosition(position),
      m_expanded(m_settings->value(ExpandedKey, true).toBool()),
      m_hover(false),
      m_pressed(false)
{
    setDockPostion(m_dockPosition);
    setExpanded(m_expanded);
}

void FashionTrayControlWidget::setDockPostion(Dock::Position pos)
{
    m_dockPosition = pos;
    refreshArrowPixmap();
}

bool FashionTrayControlWidget::expanded() const
{
    return m_expanded;
}

void FashionTrayControlWidget::setExpanded(const bool &expanded)
{
    if (m_expanded == expanded) {
        return;
    }

    m_expanded = expanded;
    refreshArrowPixmap();

    m_settings->setValue(ExpandedKey, m_expanded);

    Q_EMIT expandChanged(m_expanded);
}

void FashionTrayControlWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setOpacity(0.5);

    QColor color;
    if (m_expanded) {
        color = QColor::fromRgb(40, 40, 40);
        if (m_hover) {
            color = QColor::fromRgb(60, 60, 60);
        }
        if (m_pressed) {
            color = QColor::fromRgb(20, 20, 20);
        }
    } else {
        color = QColor::fromRgb(255, 255, 255);
        if (m_hover) {
            painter.setOpacity(0.6);
        }
        if (m_pressed) {
            painter.setOpacity(0.3);
        }
    }

    // draw background
    QPainterPath path;
    path.addRoundedRect(rect(), 10, 10);
    painter.fillPath(path, color);

    // draw arrow pixmap
    QRect r = QRect(QPoint(0, 0), m_arrowPix.size());
    r.moveCenter(rect().center());
    painter.drawPixmap(r, m_arrowPix);
}

void FashionTrayControlWidget::mouseReleaseEvent(QMouseEvent *event)
{
    m_pressed = false;
    update();

    if (event->button() == Qt::LeftButton) {
        event->accept();
        setExpanded(!m_expanded);
        return;
    }

    QWidget::mouseReleaseEvent(event);
}

void FashionTrayControlWidget::mousePressEvent(QMouseEvent *event)
{
    m_pressed = true;
    update();

    QWidget::mousePressEvent(event);
}

void FashionTrayControlWidget::enterEvent(QEvent *event)
{
    m_hover = true;
    update();

    QWidget::enterEvent(event);
}

void FashionTrayControlWidget::leaveEvent(QEvent *event)
{
    m_hover = false;
    update();

    QWidget::leaveEvent(event);
}

void FashionTrayControlWidget::refreshArrowPixmap()
{
    switch (m_dockPosition) {
    case Dock::Top:
    case Dock::Bottom:
        m_arrowPix.load(m_expanded ? ":/icons/resources/arrow_left_light.svg" : ":/icons/resources/arrow_right_dark.svg");
        break;
    case Dock::Left:
    case Dock::Right:
        m_arrowPix.load(m_expanded ? ":/icons/resources/arrow_up_light.svg" : ":/icons/resources/arrow_down_dark.svg");
        break;
    default:
        break;
    }

    update();
}
