/*
 * Copyright (c) 2010 Mark Liversedge (liversedge@gmail.com)
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

#ifndef _GC_LTMTool_h
#define _GC_LTMTool_h 1
#include "GoldenCheetah.h"

#include "Context.h"
#include "Season.h"
#include "RideMetric.h"
#include "LTMSettings.h"
#include "PDModel.h"

#include "SearchFilterBox.h"

#include <QDir>
#include <QFileDialog>
#include <QtGui>
#include <QTableWidget>
#include <QButtonGroup>
#include <QStackedWidget>
#include <QTextEdit>
#include <QCheckBox>
#include <QFormLayout>
#include <QHeaderView>
#include <QColorDialog>

// tree widget types
#define ROOT_TYPE   1
#define DATE_TYPE   2
#define METRIC_TYPE 3

#define SYS_DATE    1
#define USER_DATE   2


class LTMTool : public QWidget
{
    Q_OBJECT
    G_OBJECT


    public:

        LTMTool(Context *context, LTMSettings *settings);
        void hideBasic();

        QList<MetricDetail> metrics;
        MetricDetail* metricDetails(QString symbol);

        // apply settings to the metric selector
        void applySettings();
        void setUseSelected(int);;
        int useSelected();

        bool isFiltered() { return _amFiltered; }
        QStringList &filters() { return filenames; }

        LTMSettings *settings;

        SearchFilterBox *searchBox;

        // basic tab: accessed by LTMWindow hence public
        QComboBox *groupBy;
        QCheckBox *usePreset;
        QCheckBox *shadeZones;
        QCheckBox *showLegend;
        QCheckBox *showData;
        QCheckBox *showEvents;
        QCheckBox *showStack;
        QSlider *stackSlider;
        QPushButton *saveButton;
        QPushButton *applyButton;
        QPushButton *newButton;
        QTreeWidget *charts;

        DateSettingsEdit *dateSetting;

        static QList<MetricDetail> providePMmetrics();

    signals:

        void curvesChanged();
        void filterChanged();
        void useCustomRange(DateRange); // use the range passed...
        void useStandardRange();        // fall back to standard date range...
        void useThruToday();        // fall back to standard date range thru today

    private slots:
        void editMetric();
        void doubleClicked( int row, int column );
        void addMetric();
        void deleteMetric();
        void moveMetricUp();
        void moveMetricDown();

        void clearFilter();
        void setFilter(QStringList);

        void presetsChanged();   // presets changed in the athlete class
        void usePresetChanged(); // we changed the checkbox

        void addCurrent();

    private:

        // Helper function for default charts translation
        QwtPlotCurve::CurveStyle curveStyle(RideMetric::MetricType);
        QwtSymbol::Style symbolStyle(RideMetric::MetricType);

        Context *context;
        bool active; // ignore season changed signals since we triggered them

        bool _amFiltered; // is a filter appling?
        bool editing;
        QStringList filenames; // filters

        QTabWidget *tabs;
        QWidget *basicsettings, *basic, *custom;

        // preset tab:
        QWidget *presetWidget;
        QLineEdit *chartName;

        // custom tab:
        QTableWidget *customTable;
        QPushButton *editCustomButton, *addCustomButton, *deleteCustomButton;
#ifndef Q_OS_MAC
        QToolButton *upCustomButton, *downCustomButton;
#else
        QPushButton *upCustomButton, *downCustomButton;
#endif
        void refreshCustomTable(int indexSelectedItem = -1); // refreshes the table from LTMSettings
};

class EditMetricDetailDialog : public QDialog
{
    Q_OBJECT
    G_OBJECT


    public:
        EditMetricDetailDialog(Context *, LTMTool *, MetricDetail *);

    public slots:
        void colorClicked();
        void applyClicked();
        void cancelClicked();

        void metricSelected();
        int indexMetric(MetricDetail *metric);

        void typeChanged();
        void bestName();
        void stressName();
        void estimateName();

        void modelChanged();
        void estimateChanged();

    private:
        Context *context;
        LTMTool *ltmTool;
        MetricDetail *metricDetail;

        QRadioButton *chooseMetric, *chooseBest, *chooseEstimate, *chooseStress;
        QButtonGroup *group;
        QWidget *metricWidget, *bestWidget, *estimateWidget, *stressWidget;
        QStackedWidget *typeStack;

        // bests
        QDoubleSpinBox *duration;
        QComboBox *durationUnits;
        QComboBox *dataSeries;

        // metric
        QTreeWidget *metricTree;
        QLineEdit *userName,
                  *userUnits;

        // estimates
        QList<PDModel*>models;
        QComboBox *modelSelect;     // select 2p, 3p, multi etc
        QComboBox *estimateSelect;  // select w', cp, ftp, pmax .. whichever is suported by model
        QDoubleSpinBox *estimateDuration;
        QComboBox *estimateDurationUnits;
        QRadioButton *abs, *wpk;

        // stress
        QComboBox *stressTypeSelect; // STS, LTS, SB, RR et al

        QComboBox *curveStyle,
                  *curveSymbol;
        QCheckBox *stack;
        QPushButton *curveColor;
        QCheckBox *fillCurve;
        QCheckBox *labels;
        QDoubleSpinBox *showBest,
                       *showLowest,
                       *showOut,
                       *baseLine;
        QCheckBox *curveSmooth;
        QComboBox *trendType; // replaces above with a selection of trend line types
        QPushButton *applyButton, *cancelButton;

        QColor penColor; // chosen from color Picker
        void setButtonIcon(QColor);

        QList<RideFile::SeriesType> seriesList;
};

#endif // _GC_LTMTool_h

