/********************************************************************************
** Form generated from reading ui file 'AGHMainWindow.ui'
**
** Created: Fri Dec 10 15:34:40 2010
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_AGHMAINWINDOW_H
#define UI_AGHMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOuvrir;
    QAction *actionFermer;
    QAction *actionQuit;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *param_layout;
    QLabel *parametr_label;
    QVBoxLayout *vitesse_layout;
    QLabel *label;
    QSpinBox *vitesse_spinbox;
    QFrame *rendererFrame;
    QMenuBar *menubar;
    QMenu *menuFichier;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(640, 480);
        actionOuvrir = new QAction(MainWindow);
        actionOuvrir->setObjectName(QString::fromUtf8("actionOuvrir"));
        actionFermer = new QAction(MainWindow);
        actionFermer->setObjectName(QString::fromUtf8("actionFermer"));
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        param_layout = new QHBoxLayout();
        param_layout->setObjectName(QString::fromUtf8("param_layout"));
        parametr_label = new QLabel(centralwidget);
        parametr_label->setObjectName(QString::fromUtf8("parametr_label"));

        param_layout->addWidget(parametr_label);

        vitesse_layout = new QVBoxLayout();
        vitesse_layout->setObjectName(QString::fromUtf8("vitesse_layout"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));

        vitesse_layout->addWidget(label);

        vitesse_spinbox = new QSpinBox(centralwidget);
        vitesse_spinbox->setObjectName(QString::fromUtf8("vitesse_spinbox"));
        vitesse_spinbox->setMaximum(100);

        vitesse_layout->addWidget(vitesse_spinbox);


        param_layout->addLayout(vitesse_layout);


        verticalLayout->addLayout(param_layout);

        rendererFrame = new QFrame(centralwidget);
        rendererFrame->setObjectName(QString::fromUtf8("rendererFrame"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(rendererFrame->sizePolicy().hasHeightForWidth());
        rendererFrame->setSizePolicy(sizePolicy);
        rendererFrame->setFrameShape(QFrame::StyledPanel);
        rendererFrame->setFrameShadow(QFrame::Raised);

        verticalLayout->addWidget(rendererFrame);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 640, 25));
        menuFichier = new QMenu(menubar);
        menuFichier->setObjectName(QString::fromUtf8("menuFichier"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuFichier->menuAction());
        menuFichier->addAction(actionOuvrir);
        menuFichier->addAction(actionFermer);
        menuFichier->addSeparator();
        menuFichier->addAction(actionQuit);

        retranslateUi(MainWindow);
        QObject::connect(actionQuit, SIGNAL(triggered()), MainWindow, SLOT(close()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionOuvrir->setText(QApplication::translate("MainWindow", "Ouvrir", 0, QApplication::UnicodeUTF8));
        actionOuvrir->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        actionFermer->setText(QApplication::translate("MainWindow", "Fermer", 0, QApplication::UnicodeUTF8));
        actionFermer->setShortcut(QApplication::translate("MainWindow", "Ctrl+F", 0, QApplication::UnicodeUTF8));
        actionQuit->setText(QApplication::translate("MainWindow", "Quit", 0, QApplication::UnicodeUTF8));
        actionQuit->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
        parametr_label->setText(QApplication::translate("MainWindow", "Param\303\250tres ", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Vitesse", 0, QApplication::UnicodeUTF8));
        menuFichier->setTitle(QApplication::translate("MainWindow", "Fichier", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AGHMAINWINDOW_H
