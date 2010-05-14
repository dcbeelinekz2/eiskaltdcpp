/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 3 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QDockWidget>
#include <QLabel>
#include <QList>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QStatusBar>
#include <QCloseEvent>
#include <QShowEvent>
#include <QTabBar>
#include <QToolBar>
#include <QHash>
#include <QSessionManager>
#include <QShortcut>
#include <QKeySequence>
#include <QToolButton>
#include <QRegExp>
#include <QTreeView>

#include "dcpp/stdinc.h"
#include "dcpp/DCPlusPlus.h"
#include "dcpp/ConnectionManager.h"
#include "dcpp/DownloadManager.h"
#include "dcpp/LogManager.h"
#include "dcpp/QueueManager.h"
#include "dcpp/TimerManager.h"
#include "dcpp/UploadManager.h"
#include "dcpp/FavoriteManager.h"
#include "dcpp/ShareManager.h"
#include "dcpp/SettingsManager.h"
#include "dcpp/Download.h"
#include "dcpp/version.h"

#include "ArenaWidget.h"
#include "ArenaWidgetContainer.h"
#include "HistoryInterface.h"
#include "LineEdit.h"
#include "Func.h"

#include "ui_UIAbout.h"

using namespace dcpp;

class FavoriteHubs;
class DownloadQueue;
class ToolBar;
class MainWindow;

class QProgressBar;

class MainWindowCustomEvent: public QEvent{
public:
    static const QEvent::Type Event = static_cast<QEvent::Type>(1210);

    MainWindowCustomEvent(FuncBase *f = NULL): QEvent(Event), f(f)
    {}
    virtual ~MainWindowCustomEvent(){ delete f; }

    FuncBase *func() { return f; }
private:
    FuncBase *f;
};

class About:
        public QDialog,
        public Ui::UIAbout
{
Q_OBJECT

public:
    About(QWidget *parent): QDialog(parent){ setupUi(this); }
};

class MainWindow:
        public QMainWindow,
        public dcpp::Singleton<MainWindow>,
        private LogManagerListener,
        private TimerManagerListener,
        private QueueManagerListener
{
    Q_OBJECT

friend class dcpp::Singleton<MainWindow>;

    public:

        typedef QList<QAction*> ActionList;
        typedef QList<ArenaWidget*> ArenaWidgetList;
        typedef QMap<ArenaWidget*, QWidget*> ArenaWidgetMap;

        void beginExit();

        /** */
        void setStatusMessage(QString);

        /** */
        void newHubFrame(QString, QString);

        /** */
        void browseOwnFiles();

        /** */
        void redrawToolPanel();

        /** */
        void startSocket();

        /** */
        void autoconnect();

        /** */
        void parseCmdLine();
        /** */
        void parseInstanceLine(QString);

        /** */
        void retranslateUi();

        /** */
        void reloadSomeSettings();

        /** */
        void setUnload(bool b){ isUnload = b; }

        ArenaWidget *widgetForRole(ArenaWidget::Role) const;

    public slots:
        /** Allow widget to be mapped on arena*/
        void addArenaWidget(ArenaWidget*);
        /** Disallow widget to be mapped on arena*/
        void remArenaWidget(ArenaWidget*);
        /** Show widget on arena */
        void mapWidgetOnArena(ArenaWidget*);
        /** Remove widget from arena*/
        void remWidgetFromArena(ArenaWidget*);

        /** */
        void addArenaWidgetOnToolbar(ArenaWidget*, bool keepFocus = false);
        /** */
        void remArenaWidgetFromToolbar(ArenaWidget*);

        void slotChatClear();

    protected:
        virtual void closeEvent(QCloseEvent*);
        virtual void showEvent(QShowEvent *);
        virtual void hideEvent(QHideEvent *);
        virtual void customEvent(QEvent *);
        virtual bool eventFilter(QObject *, QEvent *);

    private slots:
        void slotFileOpenLogFile();
        void slotFileOpenDownloadDirectory();
        void slotFileBrowseFilelist();
        void slotFileBrowseOwnFilelist();
        void slotFileRefreshShare();
        void slotFileHashProgress();
        void slotHubsReconnect();
        void slotHubsFavoriteHubs();
        void slotHubsPublicHubs();
        void slotHubsFavoriteUsers();
        void slotToolsDownloadQueue();
        void slotToolsFinishedDownloads();
        void slotToolsFinishedUploads();
        void slotToolsSpy();
        void slotToolsAntiSpam();
        void slotToolsIPFilter();
        void slotToolsSwitchAway();
        void slotToolsAutoAway();
        void slotToolsSearch();
        void slotToolsADLS();
        void slotToolsSettings();
        void slotToolsTransfer(bool);
        void slotPanelMenuActionClicked();
        void slotWidgetsToggle();
        void slotQC();
        void slotHideMainMenu();
        void slotHideWindow();
        void slotHideProgressSpace();
        void slotHideLastStatus();
        void slotHideUsersStatistics();
        void slotSidebarContextMenu();
        void slotSidebarHook(const QModelIndex&);
        void slotSelectSidebarIndex(const QModelIndex&);
        void slotExit();

        void slotCloseCurrentWidget();

        void slotUnixSignal(int);

        void nextMsg();
        void prevMsg();

        void slotFind();
        void slotDel();
        void slotChatDisable();

        void slotAboutClient();
        void slotAboutQt();

    private:
        MainWindow (QWidget *parent=NULL);
        virtual ~MainWindow();

        /** LogManagerListener */
        virtual void on(dcpp::LogManagerListener::Message, time_t t, const std::string&) throw();
        /** TimerManagerListener */
        virtual void on(dcpp::TimerManagerListener::Second, uint32_t) throw();
        /** QueueManagerListener */
        virtual void on(dcpp::QueueManagerListener::Finished, QueueItem*, const std::string&, int64_t) throw();
        //
        void showShareBrowser(dcpp::UserPtr, QString, QString);

        // Interface setup functions
        void init();
        void loadSettings();
        void saveSettings();

        void initActions();
        void initMenuBar();
        void initStatusBar();
        void initSearchBar();
        void initToolbar();
        void initSideBar();
        void initHotkeys();

        void toggleSingletonWidget(ArenaWidget *a);

        void updateStatus(QMap<QString,QString>);
#ifdef FREE_SPACE_BAR_C
        static bool FreeDiscSpace ( std::string path, unsigned long long * res, unsigned long long * res2);
#endif
        bool isUnload;
        bool exitBegin;

        // position and geometry
        bool showMax;
        int w;
        int h;
        int xPos;
        int yPos;

        // Widgets
        QDockWidget *arena;
        QDockWidget *transfer_dock;
        QDockWidget *sideDock;

        QTreeView *sideTree;

        ArenaWidgetContainer *wcontainer;

        ToolBar *tBar;//for tabs
        ToolBar *fBar;//for actions
        ToolBar *sBar;//for fast search

        LineEdit   *searchLineEdit;

        QLabel *statusLabel;
        QLabel *statusDSPLabel;
        QLabel *statusUSPLabel;
        QLabel *statusDLabel;
        QLabel *statusULabel;
        QLabel *statusTRLabel;
        QLabel *msgLabel;
        QProgressBar *progressSpace;

        QMenu   *menuFile;
        QAction *fileFileListBrowser;
        QAction *fileFileListBrowserLocal;
        QAction *fileFileListRefresh;
        QAction *fileHashProgress;
        QAction *fileOpenLogFile;
        QAction *fileOpenDownloadDirectory;
        QAction *fileHideWindow;
        QAction *fileQuit;

        QMenu   *menuHubs;
        QAction *hubsHubReconnect;
        QAction *hubsQuickConnect;
        QAction *hubsFavoriteHubs;
        QAction *hubsPublicHubs;
        QAction *hubsFavoriteUsers;

        QMenu   *menuTools;
        QAction *toolsSearch;
        QAction *toolsADLS;
        QAction *toolsTransfers;
        QAction *toolsDownloadQueue;
        QAction *toolsFinishedDownloads;
        QAction *toolsFinishedUploads;
        QAction *toolsSpy;
        QAction *toolsAntiSpam;
        QAction *toolsIPFilter;
        QAction *menuAwayAction;
        // submenu
        QMenu   *menuAway;
        QActionGroup *awayGroup;
        QAction *toolsAwayOn;
        QAction *toolsAwayOff;
        QAction *toolsAutoAway;
        // end
        QAction *toolsHideProgressSpace;
        QAction *toolsHideLastStatus;
        QAction *toolsHideUsersStatisctics;
        QAction *toolsOptions;

        QMenu   *menuPanels;
        QAction *panelsWidgets;
        QAction *panelsTools;
        QAction *panelsSearch;

        // Standalone shortcuts
        QShortcut *ctrl_pgup;
        QShortcut *ctrl_pgdown;
        QShortcut *ctrl_up;
        QShortcut *ctrl_down;
        QShortcut *ctrl_w;
        QShortcut *ctrl_m;
        QShortcut *del;

        QAction *chatDisable;
        QAction *findInWidget;
        QAction *chatClear;

        QMenu *menuWidgets;
        QList<QAction*> menuWidgetsActions;
        QHash<QAction*, ArenaWidget*> menuWidgetsHash;

        QMenu   *menuAbout;
        QAction *aboutClient;
        QAction *aboutQt;

        ActionList toolBarActions;
        ActionList fileMenuActions;
        ActionList hubsMenuActions;
        ActionList toolsMenuActions;
        ArenaWidgetList arenaWidgets;
        ArenaWidgetMap arenaMap;
};

class EiskaltApp: public QApplication{
Q_OBJECT
public:
    EiskaltApp(int argc, char *argv[]): QApplication(argc, argv){}

    void commitData(QSessionManager& manager){
        if (MainWindow::getInstance()){
            MainWindow::getInstance()->beginExit();
            MainWindow::getInstance()->close();
        }

        manager.release();
    }

    void saveState(QSessionManager &){ /** Do nothing */ }
};

#endif //MAINWINDOW_H_
