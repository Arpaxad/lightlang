#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QtGui/QWidget>

class QLineEdit;
class TranslationEditor;
class QAction;
class DatabaseCenter;
class QToolButton;
class QTimer;
class Menu;
class EditorTipsWidget;

class TabWidget : public QWidget
{
	Q_OBJECT
	signals:
		void renameTab(int index,const QString& name);
		void showStatusMessage(const QString& message);
		void hideAllTips();
	public:
		TabWidget(DatabaseCenter *databaseCenter,int index,int updateTranslationInterval);
		~TabWidget();
		
		void setUpdateTranslationInterval(int interval);
		void hideTips();
	
		void setFocusAtThisTab();
		void setEditorMenu(Menu *menu);
	
		void setHtml(const QString& htmlText);
		void setReadOnly(bool readOnly);
	private slots:
		void textChanged(const QString& text);
		void updateTranslation();
		void addWord();
		void editWord();
		void removeWord();
		void translationChanged();
	private:
		void resetButtonsAccessibility();
		void formatSlStringIntoHtmlString(QString& str);
		void formatHtmlStringIntoSlString(QString& str);
	
		bool updateTranslationDuringEntering;
		QTimer *timer;
		
		EditorTipsWidget *editorTipsWidget;
	
		int tabIndex;
	
		DatabaseCenter *databaseCenter;
	
		QLineEdit *lineEdit;
		QToolButton *clearLineEditButton;
		
		TranslationEditor *textEdit;
	
		QToolButton *addWordButton;
		QToolButton *editWordButton;
		QToolButton *removeWordButton;
		QToolButton *updateTranslationButton;
};

#endif