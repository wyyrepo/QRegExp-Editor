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
#include "ui_mainwindow.h"
#include "constants.h"
#include "regexpmodel.h"
#include "aboutdialog.h"
#include "escapedpatterndialog.h"
#include "parentheseshighlighter.h"

#include "preferences/preferencesdialog.h"
#include "preferences/regexppreferencespage.h"
#include "preferences/regexpsettings.h"


// Qt
#include <QtCore/QCoreApplication>
#include <QtCore/QSettings>
#include <QtCore/QDebug>

#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_aboutDialog(0)
    , m_escapedPatternDialog(0)
    , m_maxRecentFiles(10)
    , m_settings(QSettings::IniFormat, QSettings::UserScope, QLatin1String("QRegExp-Editor"), QLatin1String("QRegExp-Editor"))
    , m_searchSettings()
    , m_regexpSettings(new RegexpSettings(&m_settings, QLatin1String("regexp")))
    , m_preferencesDialog(new PreferencesDialog(this))
{
    ui->setupUi(this);
    setWindowTitle(qApp->applicationName());

    setIcons();
    populateComboBoxes();

    toolbars.append(ui->inputEditToolbar);
    toolbars.append(ui->regexpEditToolbar);
    toolbars.append(ui->filtersToolbar);
    populateToolbarMenu();

    // shortcuts
    ui->quitAct->setShortcut(QKeySequence::Quit);

    makeSignalConnections(); // UI widgets (exception: recent files)
    createRegExpModel();
    createRecentFileActions();
    createStatusBar();
    createPreferencesDialog();

    readSettings();
    updateRecentFileActions();
    updateUiStatus();
    updateRegexpSettingsUi();
    updateRegexpDialogSettingsUi();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    m_searchSettings.append(searchSettings());
    writeSettings();
    e->accept();
}

static const char groupM[] = "mainwindow";
static const char geometryKeyC[] = "geometry";
static const char stateKeyC[] = "state";
static const char recentFilesKeyC[] = "recentfiles";
static const char resultViewWidthsKeyC[] = "resultview";

void MainWindow::readSettings()
{
    m_settings.beginGroup(QLatin1String(groupM));
    restoreGeometry(m_settings.value(
                        QLatin1String(geometryKeyC)).toByteArray());
    restoreState(m_settings.value(QLatin1String(stateKeyC)).toByteArray());
    m_recentFiles = m_settings.value(QLatin1String(recentFilesKeyC),
                                     QStringList()).toStringList();
    QStringList widths = m_settings.value(QLatin1String(
            resultViewWidthsKeyC)).toString().split(' ');
    for (int i = 0; i < widths.count(); ++i) {
        ui->resultView->setColumnWidth(i, qMax(widths.at(i).toInt(), 0));
    }

    m_settings.endGroup();

    m_searchSettings.fromSettings(&m_settings);
    m_searchSettings.setHistoryLength(1);
    setSearchSettings(&m_searchSettings);

    m_regexpSettings->fromSettings();
}

void MainWindow::writeSettings()
{
    m_settings.beginGroup(QLatin1String(groupM));
    m_settings.setValue(QLatin1String(geometryKeyC), saveGeometry());
    m_settings.setValue(QLatin1String(stateKeyC), saveState());
    m_settings.setValue(QLatin1String(recentFilesKeyC), m_recentFiles);

    QString widths;
    for (int i = 0; i < m_model->columnCount(QModelIndex()) - 1; ++i) {
        widths.append(QString::number(ui->resultView->columnWidth(i)));
        widths.append(' ');
    }
    m_settings.setValue(QLatin1String(resultViewWidthsKeyC), widths);
    m_settings.endGroup();
    m_searchSettings.toSettings(&m_settings);
    m_regexpSettings->toSettings();
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()) {
        loadFile(fileName);
    }
}

void MainWindow::openRecentFile()
{
    QAction *act = qobject_cast<QAction *>(sender());
    this->loadFile(act->data().toString());
}

void MainWindow::clearAllRecentFiles()
{
    m_recentFiles.clear();
    updateRecentFileActions();
}


void MainWindow::about()
{
    if (!m_aboutDialog) {
        m_aboutDialog = new AboutDialog(this);
        m_aboutDialog->setWindowTitle(tr("About %1").arg(qApp->applicationName()));
    }

    m_aboutDialog->show();
    m_aboutDialog->raise();
    m_aboutDialog->activateWindow();
}

void MainWindow::escapedPattern()
{
    if (!m_escapedPatternDialog) {
        m_escapedPatternDialog = new EscapedPatternDialog(this);
        m_escapedPatternDialog->setWindowTitle(tr("Escaped Pattern"));
    }

    m_escapedPatternDialog->setPattern(ui->regexpEdit->toPlainText());
    m_escapedPatternDialog->show();
    m_escapedPatternDialog->raise();
    m_escapedPatternDialog->activateWindow();
}

void MainWindow::match()
{
    m_model->evaluate(ui->inputEdit->toPlainText(), m_rx);

    // Resize the columns to it's contents
    for (int i = 0; i < m_model->columnCount(QModelIndex()) - 1; ++i) {
        ui->resultView->resizeColumnToContents(i);
    }

    // usability: expand everything when we have one match only
    const int matchesCount = m_model->rowCount(QModelIndex());
    if (matchesCount == 1)
        ui->resultView->expandAll();
}

void MainWindow::updateUiStatus()
{
    bool b = isSearchPossible();
    ui->escapedPatternAct->setEnabled(m_rx.isValid() && !ui->regexpEdit->toPlainText().isEmpty());

    if (m_rx.isValid()) {
        ui->regexpEdit->setStyleSheet("");
        statusBar()->showMessage(tr("Valid expression"));
    } else {
        ui->regexpEdit->setStyleSheet("QPlainTextEdit { background: #F7E7E7; }");
        statusBar()->showMessage(
            tr("Invalid expression: %1").arg(m_rx.errorString()));
    }

    ui->matchButton->setEnabled(b);
    ui->matchAct->setEnabled(b);
}

void MainWindow::updateRegExpPattern()
{
    QString s = QString(ui->regexpEdit->toPlainText());
    QString pattern = patternFilter.filtered(s);
    m_rx.setPattern(pattern);
    updateUiStatus();
}

void MainWindow::updateRegExpOptions()
{
    int index = ui->syntaxComboBox->currentIndex();
    // we use the data as enum value for the pattern syntax of the regexp
    int data = ui->syntaxComboBox->itemData(index, Qt::UserRole).toInt();
    m_rx.setPatternSyntax((QRegExp::PatternSyntax)data);
    m_rx.setMinimal(ui->minimalCheckBox->isChecked());
    m_rx.setCaseSensitivity(ui->caseSensitivityCheckBox->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);

    updateUiStatus();
}
void MainWindow::clearInputEdit()
{
    ui->inputEdit->clear();
}

void MainWindow::clearRegExpEdit()
{
    ui->regexpEdit->clear();
}

void MainWindow::updateStatus(const QString &message)
{
    m_statusLabel->setText(message);
}

void MainWindow::toggleWarningWidget(bool on)
{
    ui->warningWidget->setVisible(on);
}

bool MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Error opening file"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    m_recentFiles.removeAll(fileName);
    m_recentFiles.prepend(fileName);
    updateRecentFileActions();
    QTextStream inFile(&file);
    ui->inputEdit->setPlainText(inFile.readAll());
    return true;
}

void MainWindow::updateRecentFileActions()
{
    ui->recentFilesMenu->setEnabled(m_recentFiles.size());
    QMutableStringListIterator i(m_recentFiles);
    while (i.hasNext()) {
        if (!QFile::exists(i.next())) {
            i.remove();
        }
    }

    QFontMetrics fm = this->fontMetrics();
    for (int i = 0; i < m_recentFileActions.size(); ++i) {
        if (i >= m_maxRecentFiles || i >= m_recentFiles.size()) {
            m_recentFileActions[i]->setVisible(false);
        } else {
            const QString path = m_recentFiles.at(i);
            QAction *act = m_recentFileActions[i];
            act->setText(QString("%1 [%2]")
                         .arg(QFileInfo(path).fileName())
                         .arg(fm.elidedText(path, Qt::ElideMiddle, width() / 2))); // TODO find more intelligent way to calculate the avail. width
            act->setData(path);
            act->setVisible(true);
        }
    }
}

void MainWindow::populateComboBoxes()
{
    ui->syntaxComboBox->addItem(tr("RegExp"), QRegExp::RegExp);
    ui->syntaxComboBox->addItem(tr("RegExp2"), QRegExp::RegExp2);
    ui->syntaxComboBox->addItem(tr("Wildcard"), QRegExp::Wildcard);
    ui->syntaxComboBox->addItem(tr("WildcardUnix"), QRegExp::WildcardUnix);
    ui->syntaxComboBox->addItem(tr("FixedString"), QRegExp::FixedString);
    ui->syntaxComboBox->addItem(tr("W3CXmlSchema11"), QRegExp::W3CXmlSchema11);
}

void MainWindow::populateToolbarMenu()
{
    for (int i = 0; i < toolbars.size(); ++i) {
        ui->toolbarsMenu->addAction(toolbars.at(i)->toggleViewAction());
    }
}

void MainWindow::setIcons()
{
    QApplication::setWindowIcon(QIcon(QLatin1String(ICON_QREGEXP_LOGO_128)));
    ui->openAct->setIcon(QIcon::fromTheme("document-open", QIcon(":/images/document-open.png")));
    ui->quitAct->setIcon(QIcon::fromTheme("application-exit", QIcon(":/images/application-exit.png")));
    ui->clearInputEditAct->setIcon(QIcon::fromTheme("edit-clear", QIcon(":/images/edit-clear.png")));
    ui->showTabsAndSpacesAct->setIcon(
        QIcon(QLatin1String(ICON_SHOW_TABS_AND_SPACES)));
    ui->showNewlinesAct->setIcon(
        QIcon(QLatin1String(ICON_SHOW_NEWLINES)));
    ui->showParenthesesMatchAct->setIcon(
        QIcon(QLatin1String(ICON_PARENTHESES_HIGHLIGHTER)));

    ui->preferencesAct->setIcon(QIcon::fromTheme(QLatin1String("configure"),
                                QIcon(QLatin1String(ICON_CONFIGURE))));
    ui->aboutAct->setIcon(QIcon::fromTheme("help-about"));
    QIcon warningIcon = style()->standardIcon(QStyle::SP_MessageBoxWarning);
    warningIcon = QIcon::fromTheme("dialog-warning", warningIcon);
    ui->emptyStringMatchedIconLabel->setPixmap(warningIcon.pixmap(32));
}

void MainWindow::makeSignalConnections()
{
    connect(ui->openAct, SIGNAL(triggered()), this, SLOT(open()));
    connect(ui->quitAct, SIGNAL(triggered()), SLOT(close()));
    connect(ui->regexpEdit, SIGNAL(textChanged()), SLOT(updateRegExpPattern()));
    connect(ui->clearRegExpEditAct, SIGNAL(triggered()), SLOT(clearRegExpEdit()));
    connect(ui->inputEdit, SIGNAL(textChanged()), SLOT(updateUiStatus()));
    connect(ui->clearInputEditAct, SIGNAL(triggered()), SLOT(clearInputEdit()));
    connect(ui->showTabsAndSpacesAct, SIGNAL(triggered(bool)),
            SLOT(showTabsAndSpaces(bool)));
    connect(ui->showNewlinesAct, SIGNAL(triggered(bool)),
            SLOT(showNewlines(bool)));
    connect(ui->showParenthesesMatchAct, SIGNAL(triggered(bool)),
            SLOT(showParenthesesMatch(bool)));
    connect(ui->syntaxComboBox, SIGNAL(currentIndexChanged(int)), SLOT(updateRegExpOptions()));
    connect(ui->caseSensitivityCheckBox, SIGNAL(toggled(bool)), SLOT(updateRegExpOptions()));
    connect(ui->minimalCheckBox, SIGNAL(toggled(bool)), SLOT(updateRegExpOptions()));
    connect(ui->filterNewlinesAct, SIGNAL(triggered(bool)),
            SLOT(filterNewlines(bool)));
    connect(ui->filterTrailingWhitespacesAct, SIGNAL(triggered(bool)),
            SLOT(filterTrailingWhitespaces(bool)));
    connect(ui->matchAct, SIGNAL(triggered()), SLOT(match()));
    connect(ui->matchButton, SIGNAL(released()), SLOT(match()));
    connect(ui->aboutAct, SIGNAL(triggered()), SLOT(about()));
    connect(ui->escapedPatternAct, SIGNAL(triggered()), SLOT(escapedPattern()));
    connect(ui->preferencesAct, SIGNAL(triggered()), SLOT(showPreferencesDialog()));

    connect(m_regexpSettings, SIGNAL(settingsChanged(const QString &)),
            SLOT(updateRegexpSettingsUi()));
}

void MainWindow::createPreferencesDialog()
{
    m_preferencesDialog = new PreferencesDialog(this, tr("Preferences"));
    RegexpPreferencesPage *rp = new RegexpPreferencesPage(
        m_preferencesDialog, m_regexpSettings);
    m_preferencesDialog->addPage(
        rp,
        QIcon::fromTheme(QLatin1String("document-properties"),
                         QIcon(QLatin1String(ICON_DOCUMENT_PROPERTIES))),
        tr("RegExp"));

    connect(m_regexpSettings, SIGNAL(settingsChanged(const QString &)),
            SLOT(updateRegexpDialogSettingsUi()));
}

void MainWindow::createStatusBar()
{
    m_statusLabel = new QLabel;
    statusBar()->addPermanentWidget(m_statusLabel);
    connect(m_model, SIGNAL(statusChanged(QString)), this, SLOT(updateStatus(QString)));
    ui->warningWidget->hide();
}

void MainWindow::createRegExpModel()
{
    m_model = new RegExpModel(this);
    ui->resultView->setModel(m_model);
    connect(m_model, SIGNAL(emptyStringMatched(bool)), SLOT(toggleWarningWidget(bool)));
}

void MainWindow::createRecentFileActions()
{
    for (int i = 0; i < m_maxRecentFiles; ++i) {
        QAction *act = new QAction(this);
        connect(act, SIGNAL(triggered()), SLOT(openRecentFile()));
        m_recentFileActions << act;
        act->setVisible(false);
    }
    QAction *separator = ui->recentFilesMenu->addSeparator();
    ui->recentFilesMenu->addAction(tr("C&lear all"), this, SLOT(clearAllRecentFiles()));
    ui->recentFilesMenu->insertActions(separator, m_recentFileActions);
}

bool MainWindow::isSearchPossible()
{
    bool b = ui->inputEdit->toPlainText().isEmpty() ||
             ui->regexpEdit->toPlainText().isEmpty() ||
             !m_rx.isValid() ? false : true;
    return b;
}

SearchData MainWindow::searchSettings() const
{
    SearchData rc;
    rc.pattern = ui->regexpEdit->toPlainText();
    rc.syntax = ui->syntaxComboBox->currentIndex();
    rc.caseSensitivity = ui->caseSensitivityCheckBox->isChecked();
    rc.minimal = ui->minimalCheckBox->isChecked();
    return rc;
}

void MainWindow::setSearchSettings(SearchSettings *s) const
{
    QList<SearchData>searches = s->searchData();
    if (!searches.isEmpty()) {
        SearchData rc = searches.last();
        ui->regexpEdit->setPlainText(rc.pattern);
        ui->syntaxComboBox->setCurrentIndex(rc.syntax);
        ui->caseSensitivityCheckBox->setChecked(rc.caseSensitivity);
        ui->minimalCheckBox->setChecked(rc.minimal);
    }
}

void MainWindow::showPreferencesDialog()
{
    if (m_preferencesDialog) {
        m_preferencesDialog->show();
    }
}

void MainWindow::updateRegexpSettingsUi()
{
    qDebug() << Q_FUNC_INFO;
    RegexpOptions rc = m_regexpSettings->options();

    ui->showTabsAndSpacesAct->setChecked(rc.showTabsAndSpaces);
    ui->regexpEdit->setShowTabsAndSpacesEnabled(rc.showTabsAndSpaces);

    ui->showNewlinesAct->setChecked(rc.showNewlines);
    ui->regexpEdit->setShowLineAndParagraphSeparatorsEnabled(rc.showNewlines);

    ui->showParenthesesMatchAct->setChecked(rc.showParenthesesMatch);
    ui->regexpEdit->setHighlightEnabled(rc.showParenthesesMatch);

    ui->filterNewlinesAct->setChecked(
        rc.filters.testFlag(PatternFilter::FilterNewlines));
    ui->filterTrailingWhitespacesAct->setChecked(
        rc.filters.testFlag(PatternFilter::FilterTrailingWhitespaces));
    patternFilter.setFilters(rc.filters);

    updateRegExpPattern();
}

void MainWindow::showTabsAndSpaces(bool checked)
{
    RegexpOptions rc = m_regexpSettings->options();
    rc.showTabsAndSpaces = checked;
    m_regexpSettings->setOptions(rc);
}

void MainWindow::showNewlines(bool checked)
{
    RegexpOptions rc = m_regexpSettings->options();
    rc.showNewlines = checked;
    m_regexpSettings->setOptions(rc);
}

void MainWindow::showParenthesesMatch(bool checked)
{
    RegexpOptions rc = m_regexpSettings->options();
    rc.showParenthesesMatch = checked;
    m_regexpSettings->setOptions(rc);
}

void MainWindow::filterNewlines(bool checked)
{
    qDebug() << Q_FUNC_INFO;
    RegexpOptions rc = m_regexpSettings->options();

    if (checked) {
        rc.filters |= PatternFilter::FilterNewlines;
    } else {
        rc.filters &= ~PatternFilter::FilterNewlines;
    }
    qDebug() << Q_FUNC_INFO << "rc.filters: " << rc.filters;
    m_regexpSettings->setOptions(rc);
}

void MainWindow::filterTrailingWhitespaces(bool checked)
{
    qDebug() << Q_FUNC_INFO;
    RegexpOptions rc = m_regexpSettings->options();

    if (checked) {
        rc.filters |= PatternFilter::FilterTrailingWhitespaces;
    } else {
        rc.filters &= ~PatternFilter::FilterTrailingWhitespaces;
    }
    qDebug() << Q_FUNC_INFO << "rc.filters: " << rc.filters;
    m_regexpSettings->setOptions(rc);
}

void MainWindow::updateRegexpDialogSettingsUi()
{
    RegexpOptions rc = m_regexpSettings->options();
    ui->regexpEdit->setHighlightColor(rc.highlightMatchColor);
}
