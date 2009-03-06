#include <QtGui/QToolButton>
#include <QtGui/QAction>
#include "Menu.h"
#include "TabWidget.h"
#include "TabsWidget.h"

TabsWidget::TabsWidget(DatabaseCenter *dbCenter,QWidget *parent) : QTabWidget(parent) {
	databaseCenter = dbCenter;
	updateTranslationInterval = 0;
	showTips = true;
	
	tipsMenu = new Menu(true);
	tipsMenu->setHeaderText(tr("Tips"));
	tipsMenu->setHeaderIcon(QIcon(":/icons/tip.png"));
	
	hideAllTipsAction = new QAction(tipsMenu);
	hideAllTipsAction->setText(tr("Hide tips in all tabs"));
	hideAllTipsAction->setIcon(QIcon(":/icons/close.png"));
	connect(hideAllTipsAction,SIGNAL(triggered()),this,SLOT(hideAllTips()));
	
	nextTipAction = new QAction(tipsMenu);
	nextTipAction->setText(tr("Show next tip"));
	nextTipAction->setIcon(QIcon(":/icons/forward.png"));
	connect(nextTipAction,SIGNAL(triggered()),this,SLOT(showNextTipInCurrentTab()));
	
	previousTipAction = new QAction(tipsMenu);
	previousTipAction->setText(tr("Show previous tip"));
	previousTipAction->setIcon(QIcon(":/icons/backward.png"));
	connect(previousTipAction,SIGNAL(triggered()),this,SLOT(showPreviousTipInCurrentTab()));
	
	tipsMenu->addAction(nextTipAction);
	tipsMenu->addAction(previousTipAction);
	tipsMenu->addAction(hideAllTipsAction);
	
	newTabButton = new QToolButton;
	newTabButton->setAutoRaise(true);
	newTabButton->setIcon(QIcon(":/icons/tab-new.png"));
	newTabButton->setToolTip(tr("Open new tab"));
	newTabButton->setIconSize(QSize(16,16));
	connect(newTabButton,SIGNAL(clicked()),this,SLOT(openNewTab()));
	
	closeCurrentTabButton = new QToolButton;
	closeCurrentTabButton->setAutoRaise(true);
	closeCurrentTabButton->setIcon(QIcon(":/icons/tab-close.png"));
	closeCurrentTabButton->setToolTip(tr("Close tab"));
	closeCurrentTabButton->setIconSize(QSize(16,16));
	connect(closeCurrentTabButton,SIGNAL(clicked()),this,SIGNAL(closeTabButtonClicked()));
	
	setCornerWidget(newTabButton,Qt::TopLeftCorner);
	setCornerWidget(closeCurrentTabButton,Qt::TopRightCorner);
	
	connect(this,SIGNAL(currentChanged(int)),this,SLOT(currentTabChanged(int)));
}

TabsWidget::~TabsWidget() {
	blockSignals(true);
	foreach (TabWidget *tab,tabs)
		delete tab;
	delete hideAllTipsAction;
	delete nextTipAction;
	delete previousTipAction;
	delete tipsMenu;
	delete newTabButton;
	delete closeCurrentTabButton;
}

TabWidget* TabsWidget::openNewTab(const QString& tabTitle) {
	TabWidget *newTabWidget = new TabWidget(tabTitle,databaseCenter,tabs.count(),updateTranslationInterval);
	tabs << newTabWidget;
	newTabWidget->setEditorMenu(editorMenu);
	newTabWidget->setTipsMenu(tipsMenu);
	setCurrentIndex(addTab(newTabWidget,tabTitle.isEmpty() ? "(" + tr("Unnamed") + ")" : tabTitle));
	newTabWidget->setTipsHidden(!showTips);
	
	connect(newTabWidget,SIGNAL(renameTab(int,const QString&)),this,SLOT(renameTab(int,const QString&)));
	connect(newTabWidget,SIGNAL(showStatusMessage(const QString&)),this,SIGNAL(showStatusMessage(const QString&)));
	
	return newTabWidget;
}

void TabsWidget::closeCurrentTab() {
	tabs.removeAt(currentIndex());
	removeTab(currentIndex());
}

void TabsWidget::renameTab(int index,const QString& name) {
	setTabText(index,name.isEmpty() ? "(" + tr("Unnamed") + ")" : name);
}

void TabsWidget::currentTabChanged(int index) {
	if (index >= 0)
		tabs[index]->setFocusAtThisTab();
}

void TabsWidget::moveNextTab() {
	if (count() <= 1)
		return;
	int nextIndex = 0;
	if (currentIndex() < count() - 1)
		nextIndex = currentIndex() + 1;
	setCurrentIndex(nextIndex);
}

void TabsWidget::movePreviousTab() {
	if (count() <= 1)
		return;
	int nextIndex = 0;
	if (currentIndex() == 0)
		nextIndex = count() - 1;
	else
		nextIndex = currentIndex() - 1;
	setCurrentIndex(nextIndex);
}

void TabsWidget::setEditorMenu(Menu *menu) {
	editorMenu = menu;
}

void TabsWidget::setFocusOnCurrentTab() {
	tabs[currentIndex()]->setFocusAtThisTab();
}

void TabsWidget::setUpdateTranslationInterval(int interval) {
	updateTranslationInterval = interval;
	foreach (TabWidget *tab,tabs)
		tab->setUpdateTranslationInterval(updateTranslationInterval);
}

void TabsWidget::setAllTipsHidden(bool toHide) {
	foreach (TabWidget *tab,tabs)
		tab->setTipsHidden(toHide);
	showTips = !toHide;
}

void TabsWidget::hideAllTips() {
	setAllTipsHidden(true);
}

void TabsWidget::showFindWidgetInCurrentTab() {
	tabs[currentIndex()]->showSearchingPanel();
}

void TabsWidget::showNextTipInCurrentTab() {
	tabs[currentIndex()]->showNextTip();
}

void TabsWidget::showPreviousTipInCurrentTab() {
	tabs[currentIndex()]->showPreviousTip();
}
