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

#ifndef REGEXPSETTINGS_H
#define REGEXPSETTINGS_H

#include "settingsinterface.h"
#include <QtCore/QObject>

#include "regexpoptions.h"

#include <QtCore/QPointer>
#include <QtCore/QString>

class QSettings;

class RegexpSettings : public SettingsInterface  {
    Q_OBJECT

public:
    RegexpSettings(QSettings *s, const QString &name);
    virtual ~RegexpSettings();
    void toSettings() const;
    void fromSettings();

    RegexpOptions options() const;
    void setOptions(const RegexpOptions &ao);

    QString group() const;

public slots:
    void updateSettings();

private:
    RegexpOptions m_options;
};

#endif // REGEXPSETTINGS_H

