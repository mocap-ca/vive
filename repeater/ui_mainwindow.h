/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWin
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_4;
    QSplitter *splitter_2;
    QSplitter *splitter;
    QTabWidget *tabWidget;
    QWidget *ViconTab;
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *label_2;
    QLineEdit *lineEditViconHost;
    QLabel *label;
    QLineEdit *lineEditViconPort;
    QLabel *label_4;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButtonConnectVicon;
    QLineEdit *lineEditViconStatus;
    QWidget *NPTab;
    QWidget *formLayoutWidget_3;
    QFormLayout *formLayout_3;
    QLabel *label_7;
    QLineEdit *lineEditNPHost;
    QLabel *label_10;
    QLineEdit *lineEditNPPort;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pushButtonNPConnect;
    QLabel *label_9;
    QLineEdit *lineEditNPStatus;
    QWidget *StubTab;
    QPushButton *pushButtonStub;
    QLineEdit *lineEditStubStatus;
    QLabel *label_5;
    QWidget *TreeTab;
    QVBoxLayout *verticalLayout_3;
    QTreeView *treeViewData;
    QWidget *RawTab;
    QVBoxLayout *verticalLayout_2;
    QTextEdit *textEditData;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget_2;
    QWidget *tabConnections;
    QVBoxLayout *verticalLayout_5;
    QListView *listViewConnections;
    QWidget *tabStatus;
    QWidget *formLayoutWidget_2;
    QFormLayout *formLayout_2;
    QLabel *label_3;
    QLineEdit *lineEditServerFPS;
    QLabel *label_8;
    QLineEdit *lineEditLocalFPS;
    QPlainTextEdit *plainTextEditLog;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWin)
    {
        if (MainWin->objectName().isEmpty())
            MainWin->setObjectName(QStringLiteral("MainWin"));
        MainWin->resize(776, 611);
        centralWidget = new QWidget(MainWin);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout_4 = new QVBoxLayout(centralWidget);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        splitter_2 = new QSplitter(centralWidget);
        splitter_2->setObjectName(QStringLiteral("splitter_2"));
        splitter_2->setOrientation(Qt::Vertical);
        splitter = new QSplitter(splitter_2);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        tabWidget = new QTabWidget(splitter);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        ViconTab = new QWidget();
        ViconTab->setObjectName(QStringLiteral("ViconTab"));
        formLayoutWidget = new QWidget(ViconTab);
        formLayoutWidget->setObjectName(QStringLiteral("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(10, 10, 211, 119));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setLabelAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        formLayout->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(formLayoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(0, QFormLayout::LabelRole, label_2);

        lineEditViconHost = new QLineEdit(formLayoutWidget);
        lineEditViconHost->setObjectName(QStringLiteral("lineEditViconHost"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lineEditViconHost->sizePolicy().hasHeightForWidth());
        lineEditViconHost->setSizePolicy(sizePolicy1);
        lineEditViconHost->setMinimumSize(QSize(50, 0));
        lineEditViconHost->setMaximumSize(QSize(16777215, 16777215));

        formLayout->setWidget(0, QFormLayout::FieldRole, lineEditViconHost);

        label = new QLabel(formLayoutWidget);
        label->setObjectName(QStringLiteral("label"));
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(1, QFormLayout::LabelRole, label);

        lineEditViconPort = new QLineEdit(formLayoutWidget);
        lineEditViconPort->setObjectName(QStringLiteral("lineEditViconPort"));
        lineEditViconPort->setMinimumSize(QSize(50, 0));

        formLayout->setWidget(1, QFormLayout::FieldRole, lineEditViconPort);

        label_4 = new QLabel(formLayoutWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        sizePolicy.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy);
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(3, QFormLayout::LabelRole, label_4);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        formLayout->setItem(2, QFormLayout::LabelRole, horizontalSpacer_2);

        pushButtonConnectVicon = new QPushButton(formLayoutWidget);
        pushButtonConnectVicon->setObjectName(QStringLiteral("pushButtonConnectVicon"));

        formLayout->setWidget(2, QFormLayout::FieldRole, pushButtonConnectVicon);

        lineEditViconStatus = new QLineEdit(formLayoutWidget);
        lineEditViconStatus->setObjectName(QStringLiteral("lineEditViconStatus"));
        sizePolicy1.setHeightForWidth(lineEditViconStatus->sizePolicy().hasHeightForWidth());
        lineEditViconStatus->setSizePolicy(sizePolicy1);
        lineEditViconStatus->setMinimumSize(QSize(50, 0));
        lineEditViconStatus->setMaximumSize(QSize(16777215, 16777215));

        formLayout->setWidget(3, QFormLayout::FieldRole, lineEditViconStatus);

        tabWidget->addTab(ViconTab, QString());
        NPTab = new QWidget();
        NPTab->setObjectName(QStringLiteral("NPTab"));
        formLayoutWidget_3 = new QWidget(NPTab);
        formLayoutWidget_3->setObjectName(QStringLiteral("formLayoutWidget_3"));
        formLayoutWidget_3->setGeometry(QRect(30, 10, 241, 225));
        formLayout_3 = new QFormLayout(formLayoutWidget_3);
        formLayout_3->setSpacing(6);
        formLayout_3->setContentsMargins(11, 11, 11, 11);
        formLayout_3->setObjectName(QStringLiteral("formLayout_3"));
        formLayout_3->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
        formLayout_3->setContentsMargins(0, 0, 0, 0);
        label_7 = new QLabel(formLayoutWidget_3);
        label_7->setObjectName(QStringLiteral("label_7"));
        sizePolicy.setHeightForWidth(label_7->sizePolicy().hasHeightForWidth());
        label_7->setSizePolicy(sizePolicy);
        label_7->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_3->setWidget(0, QFormLayout::LabelRole, label_7);

        lineEditNPHost = new QLineEdit(formLayoutWidget_3);
        lineEditNPHost->setObjectName(QStringLiteral("lineEditNPHost"));
        sizePolicy1.setHeightForWidth(lineEditNPHost->sizePolicy().hasHeightForWidth());
        lineEditNPHost->setSizePolicy(sizePolicy1);
        lineEditNPHost->setMinimumSize(QSize(50, 0));
        lineEditNPHost->setMaximumSize(QSize(16777215, 16777215));

        formLayout_3->setWidget(0, QFormLayout::FieldRole, lineEditNPHost);

        label_10 = new QLabel(formLayoutWidget_3);
        label_10->setObjectName(QStringLiteral("label_10"));
        sizePolicy.setHeightForWidth(label_10->sizePolicy().hasHeightForWidth());
        label_10->setSizePolicy(sizePolicy);
        label_10->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_3->setWidget(1, QFormLayout::LabelRole, label_10);

        lineEditNPPort = new QLineEdit(formLayoutWidget_3);
        lineEditNPPort->setObjectName(QStringLiteral("lineEditNPPort"));
        lineEditNPPort->setMinimumSize(QSize(50, 0));

        formLayout_3->setWidget(1, QFormLayout::FieldRole, lineEditNPPort);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        formLayout_3->setItem(2, QFormLayout::LabelRole, horizontalSpacer_3);

        pushButtonNPConnect = new QPushButton(formLayoutWidget_3);
        pushButtonNPConnect->setObjectName(QStringLiteral("pushButtonNPConnect"));

        formLayout_3->setWidget(2, QFormLayout::FieldRole, pushButtonNPConnect);

        label_9 = new QLabel(formLayoutWidget_3);
        label_9->setObjectName(QStringLiteral("label_9"));
        sizePolicy.setHeightForWidth(label_9->sizePolicy().hasHeightForWidth());
        label_9->setSizePolicy(sizePolicy);
        label_9->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_3->setWidget(3, QFormLayout::LabelRole, label_9);

        lineEditNPStatus = new QLineEdit(formLayoutWidget_3);
        lineEditNPStatus->setObjectName(QStringLiteral("lineEditNPStatus"));
        sizePolicy1.setHeightForWidth(lineEditNPStatus->sizePolicy().hasHeightForWidth());
        lineEditNPStatus->setSizePolicy(sizePolicy1);
        lineEditNPStatus->setMinimumSize(QSize(50, 0));
        lineEditNPStatus->setMaximumSize(QSize(16777215, 16777215));

        formLayout_3->setWidget(3, QFormLayout::FieldRole, lineEditNPStatus);

        tabWidget->addTab(NPTab, QString());
        StubTab = new QWidget();
        StubTab->setObjectName(QStringLiteral("StubTab"));
        pushButtonStub = new QPushButton(StubTab);
        pushButtonStub->setObjectName(QStringLiteral("pushButtonStub"));
        pushButtonStub->setGeometry(QRect(90, 10, 75, 23));
        lineEditStubStatus = new QLineEdit(StubTab);
        lineEditStubStatus->setObjectName(QStringLiteral("lineEditStubStatus"));
        lineEditStubStatus->setGeometry(QRect(90, 40, 111, 20));
        label_5 = new QLabel(StubTab);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(30, 40, 53, 19));
        label_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        tabWidget->addTab(StubTab, QString());
        TreeTab = new QWidget();
        TreeTab->setObjectName(QStringLiteral("TreeTab"));
        verticalLayout_3 = new QVBoxLayout(TreeTab);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        treeViewData = new QTreeView(TreeTab);
        treeViewData->setObjectName(QStringLiteral("treeViewData"));

        verticalLayout_3->addWidget(treeViewData);

        tabWidget->addTab(TreeTab, QString());
        RawTab = new QWidget();
        RawTab->setObjectName(QStringLiteral("RawTab"));
        verticalLayout_2 = new QVBoxLayout(RawTab);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        textEditData = new QTextEdit(RawTab);
        textEditData->setObjectName(QStringLiteral("textEditData"));

        verticalLayout_2->addWidget(textEditData);

        tabWidget->addTab(RawTab, QString());
        splitter->addWidget(tabWidget);
        verticalLayoutWidget = new QWidget(splitter);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        tabWidget_2 = new QTabWidget(verticalLayoutWidget);
        tabWidget_2->setObjectName(QStringLiteral("tabWidget_2"));
        tabConnections = new QWidget();
        tabConnections->setObjectName(QStringLiteral("tabConnections"));
        verticalLayout_5 = new QVBoxLayout(tabConnections);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        listViewConnections = new QListView(tabConnections);
        listViewConnections->setObjectName(QStringLiteral("listViewConnections"));

        verticalLayout_5->addWidget(listViewConnections);

        tabWidget_2->addTab(tabConnections, QString());
        tabStatus = new QWidget();
        tabStatus->setObjectName(QStringLiteral("tabStatus"));
        formLayoutWidget_2 = new QWidget(tabStatus);
        formLayoutWidget_2->setObjectName(QStringLiteral("formLayoutWidget_2"));
        formLayoutWidget_2->setGeometry(QRect(20, 10, 201, 142));
        formLayout_2 = new QFormLayout(formLayoutWidget_2);
        formLayout_2->setSpacing(6);
        formLayout_2->setContentsMargins(11, 11, 11, 11);
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));
        formLayout_2->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        formLayout_2->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(formLayoutWidget_2);
        label_3->setObjectName(QStringLiteral("label_3"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, label_3);

        lineEditServerFPS = new QLineEdit(formLayoutWidget_2);
        lineEditServerFPS->setObjectName(QStringLiteral("lineEditServerFPS"));
        lineEditServerFPS->setEnabled(false);

        formLayout_2->setWidget(0, QFormLayout::FieldRole, lineEditServerFPS);

        label_8 = new QLabel(formLayoutWidget_2);
        label_8->setObjectName(QStringLiteral("label_8"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, label_8);

        lineEditLocalFPS = new QLineEdit(formLayoutWidget_2);
        lineEditLocalFPS->setObjectName(QStringLiteral("lineEditLocalFPS"));
        lineEditLocalFPS->setEnabled(false);

        formLayout_2->setWidget(1, QFormLayout::FieldRole, lineEditLocalFPS);

        tabWidget_2->addTab(tabStatus, QString());

        verticalLayout->addWidget(tabWidget_2);

        splitter->addWidget(verticalLayoutWidget);
        splitter_2->addWidget(splitter);
        plainTextEditLog = new QPlainTextEdit(splitter_2);
        plainTextEditLog->setObjectName(QStringLiteral("plainTextEditLog"));
        splitter_2->addWidget(plainTextEditLog);

        verticalLayout_4->addWidget(splitter_2);

        MainWin->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWin);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 776, 21));
        MainWin->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWin);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWin->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWin);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWin->setStatusBar(statusBar);
        QWidget::setTabOrder(tabWidget, lineEditViconHost);
        QWidget::setTabOrder(lineEditViconHost, lineEditViconPort);
        QWidget::setTabOrder(lineEditViconPort, pushButtonConnectVicon);
        QWidget::setTabOrder(pushButtonConnectVicon, lineEditViconStatus);
        QWidget::setTabOrder(lineEditViconStatus, pushButtonStub);
        QWidget::setTabOrder(pushButtonStub, lineEditStubStatus);
        QWidget::setTabOrder(lineEditStubStatus, treeViewData);
        QWidget::setTabOrder(treeViewData, textEditData);
        QWidget::setTabOrder(textEditData, tabWidget_2);
        QWidget::setTabOrder(tabWidget_2, listViewConnections);
        QWidget::setTabOrder(listViewConnections, lineEditServerFPS);
        QWidget::setTabOrder(lineEditServerFPS, lineEditLocalFPS);
        QWidget::setTabOrder(lineEditLocalFPS, plainTextEditLog);

        retranslateUi(MainWin);

        tabWidget->setCurrentIndex(1);
        tabWidget_2->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWin);
    } // setupUi

    void retranslateUi(QMainWindow *MainWin)
    {
        MainWin->setWindowTitle(QApplication::translate("MainWin", "MainWindow", 0));
        label_2->setText(QApplication::translate("MainWin", "Host:", 0));
        label->setText(QApplication::translate("MainWin", "Port:", 0));
        label_4->setText(QApplication::translate("MainWin", "Fps:", 0));
        pushButtonConnectVicon->setText(QApplication::translate("MainWin", "Connect", 0));
        tabWidget->setTabText(tabWidget->indexOf(ViconTab), QApplication::translate("MainWin", "Vicon", 0));
        label_7->setText(QApplication::translate("MainWin", "Host:", 0));
        label_10->setText(QApplication::translate("MainWin", "Port:", 0));
        pushButtonNPConnect->setText(QApplication::translate("MainWin", "Connect", 0));
        label_9->setText(QApplication::translate("MainWin", "Fps:", 0));
        tabWidget->setTabText(tabWidget->indexOf(NPTab), QApplication::translate("MainWin", "NaturalPoint", 0));
        pushButtonStub->setText(QApplication::translate("MainWin", "Start", 0));
        label_5->setText(QApplication::translate("MainWin", "fps:", 0));
        tabWidget->setTabText(tabWidget->indexOf(StubTab), QApplication::translate("MainWin", "Stub", 0));
        tabWidget->setTabText(tabWidget->indexOf(TreeTab), QApplication::translate("MainWin", "Tree", 0));
        tabWidget->setTabText(tabWidget->indexOf(RawTab), QApplication::translate("MainWin", "Raw", 0));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tabConnections), QApplication::translate("MainWin", "Connections", 0));
        label_3->setText(QApplication::translate("MainWin", "TCP Server FPS", 0));
        label_8->setText(QApplication::translate("MainWin", "Local Server FPS", 0));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tabStatus), QApplication::translate("MainWin", "Status", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWin: public Ui_MainWin {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
