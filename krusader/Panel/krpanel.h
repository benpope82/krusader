/***************************************************************************
                               krpanel.h
                            -------------------
   copyright            : (C) 2010 by Jan Lepper
   e-mail               : krusader@users.sourceforge.net
   web site             : http://krusader.sourceforge.net
---------------------------------------------------------------------------
 Description
***************************************************************************

 A

    db   dD d8888b. db    db .d8888.  .d8b.  d8888b. d88888b d8888b.
    88 ,8P' 88  `8D 88    88 88'  YP d8' `8b 88  `8D 88'     88  `8D
    88,8P   88oobY' 88    88 `8bo.   88ooo88 88   88 88ooooo 88oobY'
    88`8b   88`8b   88    88   `Y8b. 88~~~88 88   88 88~~~~~ 88`8b
    88 `88. 88 `88. 88b  d88 db   8D 88   88 88  .8D 88.     88 `88.
    YP   YD 88   YD ~Y8888P' `8888Y' YP   YP Y8888D' Y88888P 88   YD

                                                    H e a d e r    F i l e

***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/


#ifndef KRPANEL_H
#define KRPANEL_H

// QtCore
#include <QUrl>

class AbstractPanelManager;
class ListPanelFunc;
class ListPanel;
class KrView;

class KrPanel
{
public:
    KrPanel(AbstractPanelManager *manager, ListPanel *panel, ListPanelFunc *func) :
        gui(panel), func(func), view(0), _manager(manager) {}
    virtual ~KrPanel() {}
    QUrl virtualPath() const; // the current directory path of this panel
    AbstractPanelManager *manager() {
        return _manager;
    }
    KrPanel *otherPanel();
    bool isLeft() const;
    virtual void otherPanelChanged() = 0;

    ListPanel *gui;
    ListPanelFunc *func;
    KrView *view;

protected:
    AbstractPanelManager *_manager;
};

#endif
