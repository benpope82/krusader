/***************************************************************************
                               kcmdline.cpp
                            -------------------
   copyright            : (C) 2000 by Shie Erlich & Rafi Yanai
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

                                                    S o u r c e    F i l e

***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/


#include "kcmdline.h"
#include "stdlib.h"
#include <unistd.h>
#include <qmessagebox.h>
#include <kprocess.h>
#include <qiconset.h>
#include <qwhatsthis.h>
#include <unistd.h>
#include "../krusader.h"
#include "../kicons.h"
#include "../krslots.h"
#include "../resources.h"
#include "../defaults.h"
#include "../krusaderview.h"
#include "../Panel/listpanel.h"
#include <qdir.h>
#include <klocale.h>
#include <kglobalsettings.h>
#include <qfontmetrics.h>
#include <qimage.h>
#include <qstringlist.h>
#include <qsizepolicy.h>


KCMDLine::KCMDLine( QWidget *parent, const char *name ) : QWidget( parent, name ) {
  QGridLayout * layout = new QGridLayout( this, 1, 3 );
  path = new QLabel( this );
  QWhatsThis::add
    ( path, i18n( "Name of directory where command will be processed." ) );
  path->setAlignment( Qt::AlignRight );
  path->setFrameStyle( QFrame::Box | QFrame::Sunken );
  path->setLineWidth( 1 );
  path->setFont( KGlobalSettings::generalFont() );
  path->setMaximumHeight( QFontMetrics( path->font() ).height() + 4 );
  path->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred));
  layout->addWidget( path, 0, 0 );

  // and editable command line
  completion.setMode( KURLCompletion::FileCompletion );
  cmdLine = new KrHistoryCombo( this );
  cmdLine->setMaxCount(100);  // remember 100 commands
  cmdLine->setFont( KGlobalSettings::generalFont() );
  cmdLine->setMaximumHeight( QFontMetrics( cmdLine->font() ).height() + 4 );
  cmdLine->setCompletionObject( &completion );
  cmdLine->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred));
  // load the history
  KConfigGroupSaver grpSvr( krConfig, "Private" );
  QStringList list = krConfig->readListEntry( "cmdline history" );
  cmdLine->setHistoryItems( list );

  connect( cmdLine, SIGNAL( activated( const QString& ) ), this, SLOT( setEditText( const QString& ) ) );
  connect( cmdLine, SIGNAL( returnPressed(const QString& ) ), this, SLOT( slotRun( const QString& ) ) );
  connect( cmdLine, SIGNAL( returnPressed(const QString &) ), cmdLine, SLOT( clearEdit() ) );
  connect( cmdLine, SIGNAL( returnToPanel() ), this, SLOT( slotReturnFocus() ));

  QWhatsThis::add
    ( cmdLine, i18n( "<qt>Well, it's quite simple actually: You write "
                     "you command in here, and Krusader obeys.<p>"
                     "<Tip:>Move in command-line history with &lt;Up&gt; "
                     "and &lt;Down&gt; arrows.</qt>" ) );
  layout->addWidget( cmdLine, 0, 1 );

  // a run in terminal button
  terminal = new QToolButton( this );
  terminal->setFixedSize( cmdLine->maximumHeight(), cmdLine->maximumHeight() );
  terminal->setTextLabel( i18n( "If pressed, Krusader executes command line in a terminal." ) );
  terminal->setToggleButton( true );
  terminal->setOn( false );
  QImage im = krLoader->loadIcon( "konsole", KIcon::Panel ).convertToImage();
  terminal->setPixmap( im.scale( terminal->height(), terminal->height() ) );
  QWhatsThis::add
    ( terminal, i18n( "The 'run in terminal' button allows Krusader "
                      "to run console (or otherwise non-graphical) "
                      "programs in a terminal of your choice. If it's "
                      "pressed, terminal mode is active." ) );
  layout->addWidget( terminal, 0, 2 );
  layout->activate();
}

void KCMDLine::setCurrent( const QString &p ) {
  path->setText( p + ">" );
  completion.setDir( p );
}


KCMDLine::~KCMDLine() {
   KConfigGroupSaver grpSvr( krConfig, "Private" );
   QStringList list = cmdLine->historyItems();
   kdWarning() << list[0] << endl;
   krConfig->writeEntry( "cmdline history", list );
   krConfig->sync();
}

void KCMDLine::slotRun(const QString &command1) {
  //cmdLine->clearEdit(); // clean the line edit

  if ( command1.isEmpty() )
    return ;
  krConfig->setGroup( "General" );
  QString panelPath = path->text().left( path->text().length() - 1 );

  cmdLine->addToHistory(command1);

  if ( command1.simplifyWhiteSpace().left( 3 ) == "cd " ) { // cd command effect the active panel
    QString dir = command1.right( command1.length() - command1.find( " " ) ).stripWhiteSpace();
    if ( dir == "~" )
      dir = QDir::homeDirPath();
    else
      if ( dir.left( 1 ) != "/" && !dir.contains( ":/" ) )
        dir = panelPath + ( panelPath == "/" ? "" : "/" ) + dir;
    SLOTS->refresh( dir );
  } else {
    QString save = getcwd( 0, 0 );
    KShellProcess proc;
    chdir( panelPath.local8Bit() );
    // run in a terminal ???
    if ( terminal->isOn() )
      proc << krConfig->readEntry( "Terminal", _Terminal )
      << "-e";
    proc << command1;
    proc.start( KProcess::DontCare );

    chdir( save.local8Bit() );
  }
}


void KCMDLine::slotReturnFocus() {
  Krusader::App->mainView->cmdLineUnFocus();
}

void KrHistoryCombo::keyPressEvent( QKeyEvent *e ) {
   switch (e->key()) {
      case Key_Up:
         if (e->state() == ControlButton) {
            emit returnToPanel();
            return;
         }
      default:
      KHistoryCombo::keyPressEvent(e);
   }
}

#include "kcmdline.moc"

