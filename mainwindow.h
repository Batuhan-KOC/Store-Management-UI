#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QList>
#include <QFileInfo>
#include <QFile>
#include <iostream>
#include <fstream>
#include "KitDialog.h"
#include "MaterialDialog.h"
#include "EnsureDialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    /*GENERAL STORE ITEM TAB*/
    void on_AddItemButton_clicked(bool checked);

    void on_RemoveItemButton_clicked(bool checked);

    void on_EditItemButton_clicked(bool checked);

    void on_GeneralStoreList_cellClicked(int row, int column);

    /*KITS TAB*/
    void on_AddKitButton_clicked(bool checked);

    void on_RemoveKitButton_clicked(bool checked);

    void on_EditKitButton_clicked(bool checked);

    void on_KitList_cellClicked(int row, int column);

    void on_AddStoreItemButton_clicked(bool checked);

    void on_RemoveSelectedStoreItemButton_clicked(bool checked);

    void on_KitContentList_cellClicked(int row, int column);

    /*SAVE BUTTON*/
    void on_SaveButton_clicked(bool checked);

private:
    // Window User Interface File
    Ui::MainWindow *ui;

    QList<GeneralStoreItemType> GeneralStoreItems;
    QList<KitType> Kits;

    /***************************************************
     * GENERAL STORE TAB
    ***************************************************/

    int CurrentStoreItemIndex = -1;

    // Update General Store Tab Item List
    void UpdateGeneralStoreItemList();

    // Show selected store item data on first window
    void UpdateSelectedStoreItemData(QString name, QString unitName, QString unitCost, QString description);

    // Clear selected store item widgets
    void ClearSelectedStoreItemData();

    /***************************************************
     * KITS TAB
    ***************************************************/

    int CurrentKitIndex = -1;
    int CurrentKitContentIndex = -1;

    void UpdateKitList();

    void UpdataKitContentList();

    void ClearKitContentList();

    /***************************************************
     * ALERT WIDGETS
    ***************************************************/

    void SetAlertOn();

    void SetAlertOff();

    /***************************************************
     * FILE REGISTRATION
    ***************************************************/

    bool RegisterFileExists();

    void DeleteRegisterFile();

    void RegisterSave();

    void RegisterRead();

    /* Ensure Dialog */

    int EnsureDialogOpen();
};
#endif // MAINWINDOW_H
