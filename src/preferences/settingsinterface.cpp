/*
 * Copyright (C) 2011-2012 Luís Pereira <luis.artur.pereira@gmail.com>
 *
 * This file is part of QRegExp-Editor.
 *
 * QRegExp-Editor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QRegExp-Editor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QRegExp-Editor. If not, see <http://www.gnu.org/licenses/>.
 */

#include "settingsinterface.h"

#include <QtCore/QPointer>
#include <QtCore/QSettings>


class SettingsInterfacePrivate {
    Q_DECLARE_PUBLIC(SettingsInterface);

public:
    SettingsInterfacePrivate(SettingsInterface *q);
    ~SettingsInterfacePrivate() {}

    QPointer<QSettings> s;
    QString group;
private:

    SettingsInterface *const q_ptr;
};


SettingsInterfacePrivate::SettingsInterfacePrivate(SettingsInterface *q)
    : q_ptr(q)
{
}


SettingsInterface::SettingsInterface(QSettings *s, const QString &group)
    : QObject(0),
      d_ptr(new SettingsInterfacePrivate(this))
{
    Q_D(SettingsInterface);

    d->s = s;
    d->group = group;
}

QString SettingsInterface::group() const
{
    Q_D(const SettingsInterface);
    return d->group;
}

QSettings *SettingsInterface::settings() const
{
    Q_D(const SettingsInterface);
    return d->s;
}

#include "settingsinterface.moc"
