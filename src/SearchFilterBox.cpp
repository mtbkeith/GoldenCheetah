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

#include "SearchFilterBox.h"
#include "Context.h"
#include "FreeSearch.h"
#include "DataFilter.h"
#include "SearchBox.h"

SearchFilterBox::SearchFilterBox(QWidget *parent, Context *context, bool nochooser) : QWidget(parent), context(context)
{

    setContentsMargins(0,0,0,0);
    QVBoxLayout *contents = new QVBoxLayout(this);
    contents->setSpacing(0);
    contents->setContentsMargins(0,0,0,0);

    // no column chooser if my parent widget is a modal widget
    searchbox = new SearchBox(context, this, nochooser);
    contents->addWidget(searchbox);

    freeSearch = new FreeSearch(this, context);
    datafilter = new DataFilter(this,context);

    // text searching
    connect(searchbox, SIGNAL(submitQuery(QString)), freeSearch, SLOT(search(QString)));
    connect(freeSearch, SIGNAL(results(QStringList)), this, SIGNAL(searchResults(QStringList)));
    connect(searchbox, SIGNAL(clearQuery()), this, SIGNAL(searchClear()));

    // data filtering
    connect(searchbox, SIGNAL(submitFilter(QString)), datafilter, SLOT(parseFilter(QString)));
    connect(datafilter, SIGNAL(results(QStringList)), this, SIGNAL(searchResults(QStringList)));
    connect(searchbox, SIGNAL(clearFilter()), this, SIGNAL(searchClear()));
    connect(searchbox, SIGNAL(clearFilter()), datafilter, SLOT(clearFilter()));

    // syntax check
    connect(datafilter, SIGNAL(parseGood()), searchbox, SLOT(setGood()));
    connect(datafilter, SIGNAL(parseBad(QStringList)), searchbox, SLOT(setBad(QStringList)));
}

QString
SearchFilterBox::filter()
{
    return QString("%1:%2").arg(searchbox->getMode() == SearchBox::Search ? "search" : "filter")
                           .arg(searchbox->text());
}

void
SearchFilterBox::setFilter(QString spec)
{
    if (spec.startsWith("filter:")) {
        searchbox->setMode(SearchBox::Filter);
    } else {
        searchbox->setMode(SearchBox::Search);
    }
    searchbox->setText(spec.mid(7, spec.length()-7));
}
