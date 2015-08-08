/*
 * Copyright (c) 2012 Mark Liversedge (liversedge@gmail.com)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc., 51
 * Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "SearchBox.h"
#include "Context.h"
#include "Tab.h"
#include "Athlete.h"
#include "NamedSearch.h"
#include "RideNavigator.h"
#include "GcSideBarItem.h"
#include "AnalysisSidebar.h"
#include <QToolButton>
#include <QInputDialog>

#include <QStyle>
#include <QMenu>
#include <QDebug>

SearchBox::SearchBox(Context *context, QWidget *parent, bool nochooser)
    : QLineEdit(parent), context(context), filtered(false), nochooser(nochooser)
{
    setFixedHeight(21);
    //clear button
    clearButton = new QToolButton(this);
    clearButton->setStyleSheet("QToolButton { border: none; padding: 0px; }");
    QIcon pixmap(":images/toolbar/popbutton.png");
    clearButton->setIcon(QIcon(pixmap));
    clearButton->setIconSize(QSize(12,12));
    clearButton->setCursor(Qt::ArrowCursor);
    clearButton->hide();
    //connect(clearButton, SIGNAL(clicked()), this, SLOT(clear()));
    connect(clearButton, SIGNAL(clicked()), this, SLOT(clearClicked()));
    connect(this, SIGNAL(textChanged(const QString&)), this, SLOT(updateCloseButton(const QString&)));

    // make sure its underneath the toggle button
    toolButton = new QToolButton(this);
    toolButton->setFixedSize(QSize(16,16));
    toolButton->setStyleSheet("QToolButton { border: none; padding: 0px; }");
    toolButton->setCursor(Qt::ArrowCursor);
    toolButton->setPopupMode(QToolButton::InstantPopup);

    dropMenu = new QMenu(this);
    toolButton->setMenu(dropMenu);
    connect(dropMenu, SIGNAL(aboutToShow()), this, SLOT(setMenu()));
    connect(dropMenu, SIGNAL(triggered(QAction*)), this, SLOT(runMenu(QAction*)));

    // search button
    searchButton = new QToolButton(this);
    QIcon search = iconFromPNG(":images/toolbar/search3.png", false);
    searchButton->setStyleSheet("QToolButton { border: none; padding: 0px; }");
    searchButton->setIcon(search);
    searchButton->setIconSize(QSize(11,11));
    searchButton->setCursor(Qt::ArrowCursor);
    connect(searchButton, SIGNAL(clicked()), this, SLOT(toggleMode()));

#ifdef Q_OS_MAC
    setAttribute(Qt::WA_MacShowFocusRect, 0);
#endif
    setObjectName("SearchBox");
    setPlaceholderText(tr("Search..."));
    mode = Search;
    setDragEnabled(true);
    checkMenu();
    connect(this, SIGNAL(returnPressed()), this, SLOT(searchSubmit()));
    connect(context, SIGNAL(configChanged(qint32)), this, SLOT(configChanged(qint32)));

    // set colors and curviness
    configChanged(CONFIG_APPEARANCE);
}

void
SearchBox::configChanged(qint32)
{
    int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
    QColor color = QPalette().color(QPalette::Highlight);

    // flat mode has square corners
    if (GCColor::isFlat()) {

        setStyleSheet(QString( //"QLineEdit { padding-right: %1px; } "
                          "QLineEdit#SearchBox {"
                          "    border-radius: 3px; "
                          "    border: 1px solid rgba(127,127,127,127);"
                          "    padding: 0px %1px;"
                          "}"
                          "QLineEdit#SearchBox:focus {"
                          "    border-radius: 3px; "
#ifdef WIN32
                          "    border: 1px solid rgba(%2,%3,%4,255);"
#else
                          "    border: 2px solid rgba(%2,%3,%4,255);"
#endif
                          "    padding: 0px %5px;"
                          "}"
                 ).arg(clearButton->sizeHint().width() + frameWidth + 12)
                  .arg(color.red()).arg(color.green()).arg(color.blue())
                  .arg(clearButton->sizeHint().width() + frameWidth + 12));

    } else { 
        setStyleSheet(QString( //"QLineEdit { padding-right: %1px; } "
                          "QLineEdit#SearchBox {"
                          "    border-radius: 10px; "
                          "    border: 1px solid rgba(127,127,127,127);"
                          "    padding: 0px %1px;"
                          "}"
                          "QLineEdit#SearchBox:focus {"
                          "    border-radius: 10px; "
#ifdef WIN32
                          "    border: 1px solid rgba(%2,%3,%4,255);"
#else
                          "    border: 2px solid rgba(%2,%3,%4,255);"
#endif
                          "    padding: 0px %5px;"
                          "}"
                 ).arg(clearButton->sizeHint().width() + frameWidth + 12)
                  .arg(color.red()).arg(color.green()).arg(color.blue())
                  .arg(clearButton->sizeHint().width() + frameWidth + 12));

    }
}

void SearchBox::resizeEvent(QResizeEvent *)
{
    QSize sz = clearButton->sizeHint();
    int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
    clearButton->move(rect().right() - frameWidth - sz.width() - 1, 3);
    searchButton->move(5, 3);
#ifndef Q_OS_MAC
    toolButton->move(15, 0);
#else
    toolButton->move(13, 0);
#endif

    //container->move(rect().left(), rect().bottom() + 3); // named dialog...
    checkMenu();
}

void SearchBox::toggleMode()
{
    clear(); // clear whatever is there first
    if (mode == Search) setMode(Filter);
    else setMode(Search);
}

void SearchBox::setMode(SearchBoxMode mode)
{
    switch (mode) {

        case Filter:
        {
            QIcon filter = iconFromPNG(":images/toolbar/filter3.png", false);
            searchButton->setIcon(filter);
            searchButton->setIconSize(QSize(11,11));
            setPlaceholderText(tr("Filter..."));
        }
        break;

        case Search:
        default:
        {
            QIcon search = iconFromPNG(":images/toolbar/search3.png", false);
            searchButton->setIcon(search);
            searchButton->setIconSize(QSize(11,11));
            setPlaceholderText(tr("Search..."));
        }
        break;
    }
    this->mode = mode;
}

void SearchBox::updateCloseButton(const QString& text)
{
    if (clearButton->isVisible() && text.isEmpty()) mode == Search ? clearQuery() : clearFilter();
    clearButton->setVisible(!text.isEmpty());

    //REMOVED SINCE TOO HEAVY NOW AFFECTS CHARTS TOO
    //if (mode == Search) searchSubmit(); // only do search as you type in search mode

    setGood(); // if user changing then don't stay red - wait till resubmitted
    checkMenu();
}

void SearchBox::searchSubmit()
{
    // return hit / key pressed
    if (text() != "") {
        filtered = true;
        mode == Search ? submitQuery(text()) : submitFilter(text());
    }
}

void SearchBox::clearClicked()
{
    setText("");
    filtered = false;
    //mode == Search ? clearQuery() : clearFilter();
    setGood();
}

void SearchBox::checkMenu()
{
    if (context->athlete->namedSearches->getList().count() || text() != "") toolButton->show();
    else toolButton->hide();
}

void SearchBox::setMenu()
{
    dropMenu->clear();
    if (text() != "") dropMenu->addAction(tr("Add to Named Filters"));
    if (context->athlete->namedSearches->getList().count()) {
        if (text() != "") dropMenu->addSeparator();
        foreach(NamedSearch x, context->athlete->namedSearches->getList()) {
            dropMenu->addAction(x.name);
        }
        dropMenu->addSeparator();
        dropMenu->addAction(tr("Manage Filters"));
    }
    if (!nochooser) dropMenu->addAction(tr("Column Chooser"));
}

void SearchBox::runMenu(QAction *x)
{
    // just qdebug for now
    if (x->text() == tr("Add to Named Filters")) addNamed();
    else if (x->text() == tr("Manage Filters")) {

        EditNamedSearches *editor = new EditNamedSearches(this, context);
        editor->move(QCursor::pos()-QPoint(230,-5));
        editor->show();

    } else if (x->text() == tr("Column Chooser")) {

        ColumnChooser *selector = new ColumnChooser(context->tab->rideNavigator()->logicalHeadings);
        selector->show();

    } else {
        NamedSearch get = context->athlete->namedSearches->get(x->text());
        if (get.name == x->text()) {
            setMode(static_cast<SearchBox::SearchBoxMode>(get.type));
            setText(get.text);
        }
    }
}

void SearchBox::setBad(QStringList errors)
{
    QPalette pal;
    pal.setColor(QPalette::Text, Qt::red);
    setPalette(pal);

    setToolTip(errors.join(tr(" and ")));
}

void SearchBox::setGood()
{
    QPalette pal;
    pal.setColor(QPalette::Text, Qt::black);
    setPalette(pal);

    setToolTip("");
}



// Drag and drop columns from the chooser...
void
SearchBox::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->data("application/x-columnchooser") != "")
        event->acceptProposedAction(); // whatever you wanna drop we will try and process!
    else
        event->ignore();
}

void
SearchBox::dropEvent(QDropEvent *event)
{
    QByteArray rawData = event->mimeData()->data("application/x-columnchooser");
    QDataStream stream(&rawData, QIODevice::ReadOnly);
    stream.setVersion(QDataStream::Qt_4_6);
    QString name;
    stream >> name;

    // fugly, but it works for BikeScore with the (TM) in it... so...
    // independent of Latin1 or UTF-8 coming from "Column Chooser" the "TM" special sign is not recognized by the parser,
    // so strip the "TM" off for this case (only)
    if (name.startsWith("BikeScore")) name = QString("BikeScore");

    //  Always use the "internalNames" in Filter expressions
    SpecialFields sp;
    name = sp.internalName(name);

    // we do very little to the name, just space to _ and lower case it for now...
    name.replace(' ', '_');
    insert(name);
}

void
SearchBox::setText(QString s)
{
    QLineEdit::setText(s);
    if (s != "") searchSubmit();
}

void
SearchBox::addNamed()
{
     bool ok;
     QString text = QInputDialog::getText(this, tr("Add new search"),
                                          tr("Name:"), QLineEdit::Normal, QString(""), &ok);

    if (ok && !text.isEmpty()) {
        NamedSearch x;
        x.name = text;
        x.text = this->text();
        x.type = mode;
        x.count = 0;
        context->athlete->namedSearches->getList().append(x);
        context->athlete->namedSearches->write();
    }
}
