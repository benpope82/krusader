/***************************************************************************
                       synchronizergui.h  -  description
                             -------------------
    copyright            : (C) 2003 by Csaba Karai
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

#ifndef __SYNCHRONIZERGUI_H__
#define __SYNCHRONIZERGUI_H__

#include "synchronizer.h"
#include <qdialog.h>
#include <qlistview.h>
#include <kcombobox.h>
#include <qpixmap.h>
#include <qcheckbox.h>
#include <qmap.h>
#include <qlabel.h>

class SynchronizerGUI : QDialog
{
   Q_OBJECT

public:
  class SyncViewItem : public QListViewItem
  {
    private:
      SynchronizerFileItem *syncItemRef;
      SyncViewItem         *lastItemRef;
            
    public:
      SyncViewItem( SynchronizerFileItem *item, QListView * parent, QListViewItem *after, QString label1,
                    QString label2 = QString::null, QString label3 = QString::null, QString label4 = QString::null,
                    QString label5 = QString::null, QString label6 = QString::null,
                    QString label7 = QString::null, QString label8 = QString::null ) :
                      QListViewItem( parent, after, label1, label2, label3, label4, label5, label6,
                                     label7, label8 ), syncItemRef( item ), lastItemRef( 0 )
      {
        item->setUserData( (void *)this );
      }
      
      SyncViewItem( SynchronizerFileItem *item, QListViewItem * parent, QListViewItem *after, QString label1,
                    QString label2 = QString::null, QString label3 = QString::null, QString label4 = QString::null,
                    QString label5 = QString::null, QString label6 = QString::null,
                    QString label7 = QString::null, QString label8 = QString::null ) :
                      QListViewItem( parent, after, label1, label2, label3, label4, label5, label6,
                                     label7, label8 ), syncItemRef( item ), lastItemRef( 0 )
      {
        item->setUserData( (void *)this );
      }

      ~SyncViewItem()
      {
        syncItemRef->setUserData( 0 );
      }

      inline SynchronizerFileItem * synchronizerItemRef()       {return syncItemRef;}
      inline SyncViewItem         * lastItem()                  {return lastItemRef;}
      inline void                   setLastItem(SyncViewItem*s) {lastItemRef = s;}
  };
   
public:
  // if rightDirectory is null, leftDirectory is actually the profile name to load
  SynchronizerGUI(QWidget* parent,  QString leftDirectory, QString rightDirectory = QString::null );
  ~SynchronizerGUI();

  inline bool wasSynchronization()    {return wasSync;}

public slots:
  void rightMouseClicked(QListViewItem *);
  void compare();
  void synchronize();
  void stop();
  void closeDialog();
  void refresh();
  void swapSides();
  void profiles();
  
protected slots:
  void reject();
  void addFile( SynchronizerFileItem * );
  void markChanged( SynchronizerFileItem * );
  void statusInfo( QString );
  void subdirsChecked( bool );
  void setPanelLabels();

private:
  QString convertTime(time_t time) const;
  void    setMarkFlags();
  void    disableMarkButtons();
  void    enableMarkButtons();
  QString encodeName( QString );
  QString decodeName( QString );
  void    loadProfile( QString );
  void    saveProfile( QString, int );
  
  void    rightMenuCompareFiles( KURL url1, KURL url2 );

protected:
  virtual void keyPressEvent( QKeyEvent * );
  virtual void resizeEvent( QResizeEvent *e );
  
  KHistoryCombo *leftLocation;
  KHistoryCombo *rightLocation;
  KHistoryCombo *fileFilter;
  
  QListView     *syncList;
  Synchronizer   synchronizer;
  
  QCheckBox     *cbSubdirs;
  QCheckBox     *cbSymlinks;
  QCheckBox     *cbByContent;
  QCheckBox     *cbIgnoreDate;
  QCheckBox     *cbAsymmetric;
  QCheckBox     *cbAutoScroll;
  
  QPushButton   *btnProfiles;
  QPushButton   *btnSwapSides;
  QPushButton   *btnCompareDirs;
  QPushButton   *btnStopComparing;
  QPushButton   *btnSynchronize;
  
  QPushButton   *btnLeftToRight;
  QPushButton   *btnEquals;
  QPushButton   *btnDifferents;
  QPushButton   *btnRightToLeft;
  QPushButton   *btnDeletable;
  QPushButton   *btnDuplicates;
  QPushButton   *btnSingles;

  QLabel        *statusLabel;
  QLabel        *leftDirLabel;
  QLabel        *rightDirLabel;
  
private:
  QPixmap        fileIcon;
  QPixmap        folderIcon;
  bool           isComparing;
  bool           wasClosed;
  bool           wasSync;
  bool           firstResize;
  SyncViewItem  *lastItem;
  
  int            sizeX;
  int            sizeY;
};

#endif /* __SYNCHRONIZERGUI_H__ */
