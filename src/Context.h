/*
 * Copyright (c) 2013 Mark Liversedge (liversedge@gmail.com)
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

#ifndef _GC_Context_h
#define _GC_Context_h 1

#include "TimeUtils.h" // for class DateRange
#include "RealtimeData.h" // for class RealtimeData

class RideFile;
class RideItem;
class ErgFile;

class Context;
class Athlete;
class MainWindow;

class Context : public QObject
{
    Q_OBJECT;

    public:
        Context(MainWindow *mainWindow);

        // ride item
        RideItem *rideItem() const { return ride; }
        const RideFile *currentRide();
        const RideItem *currentRideItem() { return ride; }

        // last date range selected in diary/home view
        DateRange currentDateRange() { return _dr; }


        // current selections
        MainWindow *mainWindow;
        Athlete *athlete;
        RideItem *ride;  // the currently selected ride
        DateRange _dr;   // the currently selected date range
        ErgFile *workout; // the currently selected workout file
        long now; // point in time during train session

        // *********************************************
        // APPLICATION EVENTS
        // *********************************************
        void notifyConfigChanged(); // used by ConfigDialog to notify Context *
                                    // when config has changed - and to get a
                                    // signal emitted to notify its children

        // realtime signals
        void notifyTelemetryUpdate(const RealtimeData &rtData) { telemetryUpdate(rtData); }
        void notifyErgFileSelected(ErgFile *x) { workout=x; ergFileSelected(x); }
        ErgFile *currentErgFile() { return workout; }
        void notifyMediaSelected( QString x) { mediaSelected(x); }
        void notifySelectVideo(QString x) { selectMedia(x); }
        void notifySelectWorkout(QString x) { selectWorkout(x); }
        void notifySetNow(long x) { now = x; setNow(x); }
        long getNow() { return now; }
        void notifyNewLap() { emit newLap(); }
        void notifyStart() { emit start(); }
        void notifyUnPause() { emit unpause(); }
        void notifyPause() { emit pause(); }
        void notifyStop() { emit stop(); }
        void notifySeek(long x) { emit seek(x); }

        void notifyRideClean() { rideClean(ride); }
        void notifyRideDirty() { rideDirty(ride); }

    signals:

        void configChanged();

        void rideDirty(RideItem*);
        void rideClean(RideItem*);

        // realtime
        void telemetryUpdate(RealtimeData rtData);
        void ergFileSelected(ErgFile *);
        void mediaSelected(QString);
        void selectWorkout(QString); // ask traintool to select this
        void selectMedia(QString); // ask traintool to select this
        void setNow(long);
        void seek(long);
        void newLap();
        void start();
        void unpause();
        void pause();
        void stop();

};
#endif // _GC_Context_h
