/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.3.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
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
    QTabWidget *tabWidgetClients;
    QWidget *tabVive;
    QWidget *formLayoutWidget_4;
    QFormLayout *formLayout_4;
    QLabel *label_11;
    QLabel *label_12;
    QLineEdit *lineEditViveHost;
    QLineEdit *lineEditVivePort;
    QLineEdit *lineEditViveStatus;
    QLabel *label_13;
    QPushButton *pushButtonViveConnect;
    QSpacerItem *horizontalSpacer;
    QLabel *label_21;
    QWidget *tabVicon;
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
    QLabel *label_6;
    QCheckBox *checkBoxViconYUp;
    QLabel *label_16;
    QWidget *tabNP;
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
    QLabel *label_17;
    QWidget *tabStub;
    QPushButton *pushButtonStub;
    QLineEdit *lineEditStubStatus;
    QLabel *label_5;
    QLabel *label_18;
    QWidget *tabTree;
    QVBoxLayout *verticalLayout_3;
    QTreeView *treeViewData;
    QWidget *tabRaw;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_19;
    QSpacerItem *horizontalSpacer_5;
    QCheckBox *checkBoxRawEnable;
    QTextEdit *textEditData;
    QWidget *tabFile;
    QVBoxLayout *verticalLayout_6;
    QLabel *label_20;
    QHBoxLayout *horizontalLayout;
    QLineEdit *lineEditFilePath;
    QPushButton *pushButtonFileChoose;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButtonFileRead;
    QPushButton *pushButtonFileWrite;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_15;
    QLineEdit *lineEditFileStatus;
    QSpacerItem *verticalSpacer;
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
    QLineEdit *lineEditListenPort;
    QLabel *label_14;
    QPushButton *pushButtonServerToggle;
    QPlainTextEdit *plainTextEditLog;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWin)
    {
        if (MainWin->objectName().isEmpty())
            MainWin->setObjectName(QStringLiteral("MainWin"));
        MainWin->resize(819, 611);
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
        tabWidgetClients = new QTabWidget(splitter);
        tabWidgetClients->setObjectName(QStringLiteral("tabWidgetClients"));
        tabWidgetClients->setEnabled(true);
        tabVive = new QWidget();
        tabVive->setObjectName(QStringLiteral("tabVive"));
        formLayoutWidget_4 = new QWidget(tabVive);
        formLayoutWidget_4->setObjectName(QStringLiteral("formLayoutWidget_4"));
        formLayoutWidget_4->setGeometry(QRect(10, 30, 211, 141));
        formLayout_4 = new QFormLayout(formLayoutWidget_4);
        formLayout_4->setSpacing(6);
        formLayout_4->setContentsMargins(11, 11, 11, 11);
        formLayout_4->setObjectName(QStringLiteral("formLayout_4"));
        formLayout_4->setContentsMargins(0, 0, 0, 0);
        label_11 = new QLabel(formLayoutWidget_4);
        label_11->setObjectName(QStringLiteral("label_11"));

        formLayout_4->setWidget(0, QFormLayout::LabelRole, label_11);

        label_12 = new QLabel(formLayoutWidget_4);
        label_12->setObjectName(QStringLiteral("label_12"));

        formLayout_4->setWidget(1, QFormLayout::LabelRole, label_12);

        lineEditViveHost = new QLineEdit(formLayoutWidget_4);
        lineEditViveHost->setObjectName(QStringLiteral("lineEditViveHost"));

        formLayout_4->setWidget(0, QFormLayout::FieldRole, lineEditViveHost);

        lineEditVivePort = new QLineEdit(formLayoutWidget_4);
        lineEditVivePort->setObjectName(QStringLiteral("lineEditVivePort"));

        formLayout_4->setWidget(1, QFormLayout::FieldRole, lineEditVivePort);

        lineEditViveStatus = new QLineEdit(formLayoutWidget_4);
        lineEditViveStatus->setObjectName(QStringLiteral("lineEditViveStatus"));

        formLayout_4->setWidget(2, QFormLayout::FieldRole, lineEditViveStatus);

        label_13 = new QLabel(formLayoutWidget_4);
        label_13->setObjectName(QStringLiteral("label_13"));

        formLayout_4->setWidget(2, QFormLayout::LabelRole, label_13);

        pushButtonViveConnect = new QPushButton(formLayoutWidget_4);
        pushButtonViveConnect->setObjectName(QStringLiteral("pushButtonViveConnect"));

        formLayout_4->setWidget(3, QFormLayout::FieldRole, pushButtonViveConnect);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        formLayout_4->setItem(3, QFormLayout::LabelRole, horizontalSpacer);

        label_21 = new QLabel(tabVive);
        label_21->setObjectName(QStringLiteral("label_21"));
        label_21->setGeometry(QRect(10, 10, 311, 16));
        tabWidgetClients->addTab(tabVive, QString());
        tabVicon = new QWidget();
        tabVicon->setObjectName(QStringLiteral("tabVicon"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tabVicon->sizePolicy().hasHeightForWidth());
        tabVicon->setSizePolicy(sizePolicy);
        formLayoutWidget = new QWidget(tabVicon);
        formLayoutWidget->setObjectName(QStringLiteral("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(10, 30, 211, 126));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setLabelAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        formLayout->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(formLayoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy1);
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(0, QFormLayout::LabelRole, label_2);

        lineEditViconHost = new QLineEdit(formLayoutWidget);
        lineEditViconHost->setObjectName(QStringLiteral("lineEditViconHost"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(lineEditViconHost->sizePolicy().hasHeightForWidth());
        lineEditViconHost->setSizePolicy(sizePolicy2);
        lineEditViconHost->setMinimumSize(QSize(50, 0));
        lineEditViconHost->setMaximumSize(QSize(16777215, 16777215));

        formLayout->setWidget(0, QFormLayout::FieldRole, lineEditViconHost);

        label = new QLabel(formLayoutWidget);
        label->setObjectName(QStringLiteral("label"));
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(1, QFormLayout::LabelRole, label);

        lineEditViconPort = new QLineEdit(formLayoutWidget);
        lineEditViconPort->setObjectName(QStringLiteral("lineEditViconPort"));
        lineEditViconPort->setMinimumSize(QSize(50, 0));

        formLayout->setWidget(1, QFormLayout::FieldRole, lineEditViconPort);

        label_4 = new QLabel(formLayoutWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        sizePolicy1.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy1);
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(3, QFormLayout::LabelRole, label_4);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        formLayout->setItem(2, QFormLayout::LabelRole, horizontalSpacer_2);

        pushButtonConnectVicon = new QPushButton(formLayoutWidget);
        pushButtonConnectVicon->setObjectName(QStringLiteral("pushButtonConnectVicon"));

        formLayout->setWidget(2, QFormLayout::FieldRole, pushButtonConnectVicon);

        lineEditViconStatus = new QLineEdit(formLayoutWidget);
        lineEditViconStatus->setObjectName(QStringLiteral("lineEditViconStatus"));
        sizePolicy2.setHeightForWidth(lineEditViconStatus->sizePolicy().hasHeightForWidth());
        lineEditViconStatus->setSizePolicy(sizePolicy2);
        lineEditViconStatus->setMinimumSize(QSize(50, 0));
        lineEditViconStatus->setMaximumSize(QSize(16777215, 16777215));

        formLayout->setWidget(3, QFormLayout::FieldRole, lineEditViconStatus);

        label_6 = new QLabel(formLayoutWidget);
        label_6->setObjectName(QStringLiteral("label_6"));

        formLayout->setWidget(4, QFormLayout::LabelRole, label_6);

        checkBoxViconYUp = new QCheckBox(formLayoutWidget);
        checkBoxViconYUp->setObjectName(QStringLiteral("checkBoxViconYUp"));
        checkBoxViconYUp->setChecked(true);

        formLayout->setWidget(4, QFormLayout::FieldRole, checkBoxViconYUp);

        label_16 = new QLabel(tabVicon);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setGeometry(QRect(10, 10, 281, 16));
        tabWidgetClients->addTab(tabVicon, QString());
        tabNP = new QWidget();
        tabNP->setObjectName(QStringLiteral("tabNP"));
        formLayoutWidget_3 = new QWidget(tabNP);
        formLayoutWidget_3->setObjectName(QStringLiteral("formLayoutWidget_3"));
        formLayoutWidget_3->setGeometry(QRect(10, 30, 241, 181));
        formLayout_3 = new QFormLayout(formLayoutWidget_3);
        formLayout_3->setSpacing(6);
        formLayout_3->setContentsMargins(11, 11, 11, 11);
        formLayout_3->setObjectName(QStringLiteral("formLayout_3"));
        formLayout_3->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
        formLayout_3->setContentsMargins(0, 0, 0, 0);
        label_7 = new QLabel(formLayoutWidget_3);
        label_7->setObjectName(QStringLiteral("label_7"));
        sizePolicy1.setHeightForWidth(label_7->sizePolicy().hasHeightForWidth());
        label_7->setSizePolicy(sizePolicy1);
        label_7->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_3->setWidget(0, QFormLayout::LabelRole, label_7);

        lineEditNPHost = new QLineEdit(formLayoutWidget_3);
        lineEditNPHost->setObjectName(QStringLiteral("lineEditNPHost"));
        sizePolicy2.setHeightForWidth(lineEditNPHost->sizePolicy().hasHeightForWidth());
        lineEditNPHost->setSizePolicy(sizePolicy2);
        lineEditNPHost->setMinimumSize(QSize(50, 0));
        lineEditNPHost->setMaximumSize(QSize(16777215, 16777215));

        formLayout_3->setWidget(0, QFormLayout::FieldRole, lineEditNPHost);

        label_10 = new QLabel(formLayoutWidget_3);
        label_10->setObjectName(QStringLiteral("label_10"));
        sizePolicy1.setHeightForWidth(label_10->sizePolicy().hasHeightForWidth());
        label_10->setSizePolicy(sizePolicy1);
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
        sizePolicy1.setHeightForWidth(label_9->sizePolicy().hasHeightForWidth());
        label_9->setSizePolicy(sizePolicy1);
        label_9->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_3->setWidget(3, QFormLayout::LabelRole, label_9);

        lineEditNPStatus = new QLineEdit(formLayoutWidget_3);
        lineEditNPStatus->setObjectName(QStringLiteral("lineEditNPStatus"));
        sizePolicy2.setHeightForWidth(lineEditNPStatus->sizePolicy().hasHeightForWidth());
        lineEditNPStatus->setSizePolicy(sizePolicy2);
        lineEditNPStatus->setMinimumSize(QSize(50, 0));
        lineEditNPStatus->setMaximumSize(QSize(16777215, 16777215));

        formLayout_3->setWidget(3, QFormLayout::FieldRole, lineEditNPStatus);

        label_17 = new QLabel(tabNP);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setGeometry(QRect(10, 10, 251, 16));
        tabWidgetClients->addTab(tabNP, QString());
        tabStub = new QWidget();
        tabStub->setObjectName(QStringLiteral("tabStub"));
        pushButtonStub = new QPushButton(tabStub);
        pushButtonStub->setObjectName(QStringLiteral("pushButtonStub"));
        pushButtonStub->setGeometry(QRect(90, 30, 75, 23));
        lineEditStubStatus = new QLineEdit(tabStub);
        lineEditStubStatus->setObjectName(QStringLiteral("lineEditStubStatus"));
        lineEditStubStatus->setGeometry(QRect(90, 60, 111, 20));
        label_5 = new QLabel(tabStub);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(30, 60, 53, 19));
        label_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_18 = new QLabel(tabStub);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setGeometry(QRect(10, 10, 281, 16));
        tabWidgetClients->addTab(tabStub, QString());
        tabTree = new QWidget();
        tabTree->setObjectName(QStringLiteral("tabTree"));
        tabTree->setEnabled(true);
        verticalLayout_3 = new QVBoxLayout(tabTree);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        treeViewData = new QTreeView(tabTree);
        treeViewData->setObjectName(QStringLiteral("treeViewData"));

        verticalLayout_3->addWidget(treeViewData);

        tabWidgetClients->addTab(tabTree, QString());
        tabRaw = new QWidget();
        tabRaw->setObjectName(QStringLiteral("tabRaw"));
        verticalLayout_2 = new QVBoxLayout(tabRaw);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_19 = new QLabel(tabRaw);
        label_19->setObjectName(QStringLiteral("label_19"));

        horizontalLayout_4->addWidget(label_19);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_5);

        checkBoxRawEnable = new QCheckBox(tabRaw);
        checkBoxRawEnable->setObjectName(QStringLiteral("checkBoxRawEnable"));

        horizontalLayout_4->addWidget(checkBoxRawEnable);

        horizontalLayout_4->setStretch(0, 1);

        verticalLayout_2->addLayout(horizontalLayout_4);

        textEditData = new QTextEdit(tabRaw);
        textEditData->setObjectName(QStringLiteral("textEditData"));

        verticalLayout_2->addWidget(textEditData);

        tabWidgetClients->addTab(tabRaw, QString());
        tabFile = new QWidget();
        tabFile->setObjectName(QStringLiteral("tabFile"));
        verticalLayout_6 = new QVBoxLayout(tabFile);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        label_20 = new QLabel(tabFile);
        label_20->setObjectName(QStringLiteral("label_20"));

        verticalLayout_6->addWidget(label_20);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        lineEditFilePath = new QLineEdit(tabFile);
        lineEditFilePath->setObjectName(QStringLiteral("lineEditFilePath"));

        horizontalLayout->addWidget(lineEditFilePath);

        pushButtonFileChoose = new QPushButton(tabFile);
        pushButtonFileChoose->setObjectName(QStringLiteral("pushButtonFileChoose"));

        horizontalLayout->addWidget(pushButtonFileChoose);


        verticalLayout_6->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        pushButtonFileRead = new QPushButton(tabFile);
        pushButtonFileRead->setObjectName(QStringLiteral("pushButtonFileRead"));

        horizontalLayout_2->addWidget(pushButtonFileRead);

        pushButtonFileWrite = new QPushButton(tabFile);
        pushButtonFileWrite->setObjectName(QStringLiteral("pushButtonFileWrite"));

        horizontalLayout_2->addWidget(pushButtonFileWrite);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);


        verticalLayout_6->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_15 = new QLabel(tabFile);
        label_15->setObjectName(QStringLiteral("label_15"));

        horizontalLayout_3->addWidget(label_15);

        lineEditFileStatus = new QLineEdit(tabFile);
        lineEditFileStatus->setObjectName(QStringLiteral("lineEditFileStatus"));

        horizontalLayout_3->addWidget(lineEditFileStatus);


        verticalLayout_6->addLayout(horizontalLayout_3);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer);

        tabWidgetClients->addTab(tabFile, QString());
        splitter->addWidget(tabWidgetClients);
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

        lineEditListenPort = new QLineEdit(formLayoutWidget_2);
        lineEditListenPort->setObjectName(QStringLiteral("lineEditListenPort"));

        formLayout_2->setWidget(2, QFormLayout::FieldRole, lineEditListenPort);

        label_14 = new QLabel(formLayoutWidget_2);
        label_14->setObjectName(QStringLiteral("label_14"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, label_14);

        pushButtonServerToggle = new QPushButton(formLayoutWidget_2);
        pushButtonServerToggle->setObjectName(QStringLiteral("pushButtonServerToggle"));

        formLayout_2->setWidget(3, QFormLayout::FieldRole, pushButtonServerToggle);

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
        menuBar->setGeometry(QRect(0, 0, 819, 21));
        MainWin->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWin);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWin->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWin);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWin->setStatusBar(statusBar);
        QWidget::setTabOrder(tabWidgetClients, lineEditViconHost);
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

        tabWidgetClients->setCurrentIndex(6);
        tabWidget_2->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWin);
    } // setupUi

    void retranslateUi(QMainWindow *MainWin)
    {
        MainWin->setWindowTitle(QApplication::translate("MainWin", "MainWindow", 0));
        label_11->setText(QApplication::translate("MainWin", "Host:", 0));
        label_12->setText(QApplication::translate("MainWin", "Port:", 0));
        label_13->setText(QApplication::translate("MainWin", "Fps:", 0));
        pushButtonViveConnect->setText(QApplication::translate("MainWin", "Connect", 0));
        label_21->setText(QApplication::translate("MainWin", "Exchange data with a remote vive system:", 0));
        tabWidgetClients->setTabText(tabWidgetClients->indexOf(tabVive), QApplication::translate("MainWin", "Vive", 0));
        label_2->setText(QApplication::translate("MainWin", "Host:", 0));
        label->setText(QApplication::translate("MainWin", "Port:", 0));
        label_4->setText(QApplication::translate("MainWin", "Fps:", 0));
        pushButtonConnectVicon->setText(QApplication::translate("MainWin", "Connect", 0));
        label_6->setText(QApplication::translate("MainWin", "Y-up", 0));
        checkBoxViconYUp->setText(QString());
        label_16->setText(QApplication::translate("MainWin", "Read data from a Vicon realtime (Blade/Tracker) stream:", 0));
        tabWidgetClients->setTabText(tabWidgetClients->indexOf(tabVicon), QApplication::translate("MainWin", "Vicon", 0));
        label_7->setText(QApplication::translate("MainWin", "Host:", 0));
        label_10->setText(QApplication::translate("MainWin", "Port:", 0));
        pushButtonNPConnect->setText(QApplication::translate("MainWin", "Connect", 0));
        label_9->setText(QApplication::translate("MainWin", "Fps:", 0));
        label_17->setText(QApplication::translate("MainWin", "Read data from a Naturalpoint (Motive) system:", 0));
        tabWidgetClients->setTabText(tabWidgetClients->indexOf(tabNP), QApplication::translate("MainWin", "NaturalPoint", 0));
        pushButtonStub->setText(QApplication::translate("MainWin", "Start", 0));
        label_5->setText(QApplication::translate("MainWin", "fps:", 0));
        label_18->setText(QApplication::translate("MainWin", "Test data generator:", 0));
        tabWidgetClients->setTabText(tabWidgetClients->indexOf(tabStub), QApplication::translate("MainWin", "Stub", 0));
        tabWidgetClients->setTabText(tabWidgetClients->indexOf(tabTree), QApplication::translate("MainWin", "Tree", 0));
        label_19->setText(QApplication::translate("MainWin", "Sample output data (updated once a second)", 0));
        checkBoxRawEnable->setText(QApplication::translate("MainWin", "Enable", 0));
        tabWidgetClients->setTabText(tabWidgetClients->indexOf(tabRaw), QApplication::translate("MainWin", "Raw", 0));
        label_20->setText(QApplication::translate("MainWin", "Read or write the data stream to a file (for playback later)", 0));
        pushButtonFileChoose->setText(QApplication::translate("MainWin", "Choose", 0));
        pushButtonFileRead->setText(QApplication::translate("MainWin", "Read", 0));
        pushButtonFileWrite->setText(QApplication::translate("MainWin", "Write", 0));
        label_15->setText(QApplication::translate("MainWin", "Status:", 0));
        tabWidgetClients->setTabText(tabWidgetClients->indexOf(tabFile), QApplication::translate("MainWin", "File", 0));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tabConnections), QApplication::translate("MainWin", "Connections", 0));
        label_3->setText(QApplication::translate("MainWin", "TCP Server FPS", 0));
        label_8->setText(QApplication::translate("MainWin", "Local Server FPS", 0));
        label_14->setText(QApplication::translate("MainWin", "ListenPort:", 0));
        pushButtonServerToggle->setText(QApplication::translate("MainWin", "Stop", 0));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tabStatus), QApplication::translate("MainWin", "Status", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWin: public Ui_MainWin {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
