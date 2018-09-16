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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QRegExp-Editor. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SETTINGSINTERFACE_H
#define SETTINGSINTERFACE_H

class QSettings;
class SettingsInterfacePrivate;

#include <QtCore/QObject>
#include <QtCore/QString>


class SettingsInterface  : public QObject {
    Q_OBJECT
    Q_DECLARE_PRIVATE(SettingsInterface);

public:
    virtual ~SettingsInterface() {};
    virtual void toSettings() const = 0;
    virtual void fromSettings() = 0;
    QString group() const;
    QSettings *settings() const;

Q_SIGNALS:
    void settingsChanged(const QString &settingsGroup);

protected:

    SettingsInterface(QSettings *s, const QString &group);
private:

    SettingsInterfacePrivate *const d_ptr;
};

#endif // SETTINGSINTERFACE_H
