/*
 * Copyright (C) 2010-2012 M. Mehdi Salem Naraghi <momesana@googlemail.com>
 * Copyright (C) 2010-2012 Luís Pereira <luis.artur.pereira@gmail.com>
 *
 * This file is part of QRegExp-Editor.
 *
 * Regexp-Editor is free software: you can redistribute it and/or modify
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
 * along with QRegExp-Editor.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mainwindow.h"
#include "qregexp_editor-version.h"

// Qt
#include <QtCore/QTranslator>
#include <QtCore/QLibraryInfo>
#include <QtCore/QLocale>

#include <QtGui/QApplication>

// Borrowed from QtCreator (qt-creator/src/app/main.cpp)
#ifdef Q_OS_MAC
#  define SHARE_PATH "/../Resources"
#else
#  define SHARE_PATH "/../share/qregexp-editor"
#endif

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("QRegExp-Editor");
    QCoreApplication::setOrganizationDomain(
        QLatin1String("http://qregexp-editor.sourceforge.net"));
    QCoreApplication::setApplicationVersion(QLatin1String(version_string));
    QCoreApplication::setApplicationName("QRegExp-Editor");

    const QString &appTrPath = QApplication::applicationDirPath()
                               + QLatin1String(SHARE_PATH "/translations");

    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),
                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);

    QTranslator appTranslator;
    appTranslator.load(QLatin1String("qregexp-editor_") + QLocale::system().name(), appTrPath);
    app.installTranslator(&appTranslator);

    MainWindow mw;
    mw.show();
    return app.exec();
}

