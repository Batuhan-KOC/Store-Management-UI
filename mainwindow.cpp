#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set Alert of at the beginning
    SetAlertOff();

    // Set store elements widget column sizes
    ui->GeneralStoreList->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->GeneralStoreList->setColumnWidth(0, 220);
    ui->GeneralStoreList->setColumnWidth(1, 150);
    ui->GeneralStoreList->setColumnWidth(2, 150);
    ui->GeneralStoreList->setColumnWidth(3, 659);
    ui->GeneralStoreList->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->KitContentList->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->KitContentList->setColumnWidth(0,220);
    ui->KitContentList->setColumnWidth(1,150);
    ui->KitContentList->setColumnWidth(2,150);

    RegisterRead();

    UpdateGeneralStoreItemList();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_AddItemButton_clicked(bool checked)
{
    Q_UNUSED(checked)

    QString* N_itemName = new QString();
    QString* N_itemUnitSuffix = new QString();
    QString* N_itemDescription = new QString();
    double* N_itemUnitCost = new double();

    MaterialDialog* materialDialog = new MaterialDialog();

    materialDialog->AssignDialogPtr(N_itemName, N_itemUnitSuffix, N_itemDescription, N_itemUnitCost, false);

    this->setEnabled(false);

    int dialogResult = materialDialog->exec();

    this->setEnabled(true);

    if(dialogResult == MATERIAL_DIALOG_ACCEPT){
        // Create a new item
        GeneralStoreItemType newStoreItem;
        newStoreItem.ItemName = *N_itemName;
        newStoreItem.UnitCost = *N_itemUnitCost;
        newStoreItem.UnitSuffix = *N_itemUnitSuffix;
        newStoreItem.Description = *N_itemDescription;

        bool OverlappingItem = false;

        // Can't add an existing store item!
        for(GeneralStoreItemType item: GeneralStoreItems){
            if(item.ItemName == newStoreItem.ItemName){
                OverlappingItem = true;
                break;
            }
        }

        if(OverlappingItem){
            return;
        }

        GeneralStoreItems.append(newStoreItem);

        UpdateGeneralStoreItemList();

        SetAlertOn();
    }
    else if(dialogResult == MATERIAL_DIALOG_CANCEL){
        // do nothing

        return;
    }
}

void MainWindow::on_RemoveItemButton_clicked(bool checked)
{
    Q_UNUSED(checked)

    if(CurrentStoreItemIndex < 0){
        return;
    }

    if(CurrentStoreItemIndex >= GeneralStoreItems.count()){
        return;
    }

    int Ensure = EnsureDialogOpen();

    if(Ensure != 1){
        return;
    }

    GeneralStoreItems.removeAt(CurrentStoreItemIndex);

    UpdateGeneralStoreItemList();

    ClearSelectedStoreItemData();

    SetAlertOn();

    CurrentStoreItemIndex = -1;
}

void MainWindow::on_EditItemButton_clicked(bool checked)
{
    Q_UNUSED(checked)

    if(CurrentStoreItemIndex < 0){
        return;
    }
    else if(CurrentStoreItemIndex >= GeneralStoreItems.count()){
        return;
    }

    QString* N_itemName = new QString();
    QString* N_itemUnitSuffix = new QString();
    QString* N_itemDescription = new QString();
    double* N_itemUnitCost = new double();

    GeneralStoreItemType currentItem = GeneralStoreItems.at(CurrentStoreItemIndex);

    *N_itemName = currentItem.ItemName;
    *N_itemUnitSuffix = currentItem.UnitSuffix;
    *N_itemDescription = currentItem.Description;
    *N_itemUnitCost = currentItem.UnitCost;

    MaterialDialog* materialDialog = new MaterialDialog();

    materialDialog->AssignDialogPtr(N_itemName, N_itemUnitSuffix, N_itemDescription, N_itemUnitCost, true);

    this->setEnabled(false);

    int dialogResult = materialDialog->exec();

    this->setEnabled(true);

    if(dialogResult == MATERIAL_DIALOG_ACCEPT){

        if(CurrentStoreItemIndex < 0){
            return;
        }

        if(CurrentStoreItemIndex >= GeneralStoreItems.count()){
            return;
        }

        // Create a new item
        GeneralStoreItemType newStoreItem;
        newStoreItem.ItemName = *N_itemName;
        newStoreItem.UnitCost = *N_itemUnitCost;
        newStoreItem.UnitSuffix = *N_itemUnitSuffix;
        newStoreItem.Description = *N_itemDescription;

        GeneralStoreItems.replace(CurrentStoreItemIndex, newStoreItem);

        UpdateGeneralStoreItemList();

        ClearSelectedStoreItemData();

        SetAlertOn();
    }
    else if(dialogResult == MATERIAL_DIALOG_CANCEL){
        // do nothing

        return;
    }
}

void MainWindow::UpdateGeneralStoreItemList()
{
    CurrentStoreItemIndex = -1;

    ui->GeneralStoreList->blockSignals(true);
    ui->StoreItemSelect->blockSignals(true);

    ui->GeneralStoreList->clearFocus();
    ui->GeneralStoreList->setRowCount(0);

    ui->StoreItemSelect->clear();

    ui->GeneralStoreList->setRowCount(GeneralStoreItems.count());

    int rowIdx = 0;
    for(GeneralStoreItemType item: GeneralStoreItems){
        QTableWidgetItem* itemNameWidget, *itemUnitSuffixWidget, *itemUnitCostWidget, *itemDescriptionWidget;

        itemNameWidget = new QTableWidgetItem();
        itemUnitSuffixWidget = new QTableWidgetItem();
        itemUnitCostWidget = new QTableWidgetItem();
        itemDescriptionWidget = new QTableWidgetItem();

        itemNameWidget->setText(" " + item.ItemName);
        itemUnitSuffixWidget->setText(item.UnitSuffix);
        itemUnitCostWidget->setText(QString::number(item.UnitCost) + QString(" $"));
        itemDescriptionWidget->setText("  " + item.Description);

        ui->GeneralStoreList->setItem(rowIdx, 0, itemNameWidget);
        ui->GeneralStoreList->setItem(rowIdx, 1, itemUnitSuffixWidget);
        ui->GeneralStoreList->setItem(rowIdx, 2, itemUnitCostWidget);
        ui->GeneralStoreList->setItem(rowIdx, 3, itemDescriptionWidget);
        rowIdx++;

        ui->StoreItemSelect->addItem(item.ItemName);
    }

    UpdateKitList();

    ui->GeneralStoreList->blockSignals(false);
    ui->StoreItemSelect->blockSignals(false);
}

void MainWindow::UpdateSelectedStoreItemData(QString name, QString unitName, QString unitCost, QString description)
{
    ui->EditStoreItemName->setText(name);
    ui->EditUnit->setText(unitName);
    ui->EditUnitCost->setText(unitCost + " $");
    ui->EditDescription->setText(description);
}

void MainWindow::ClearSelectedStoreItemData()
{
    CurrentStoreItemIndex = -1;

    ui->EditStoreItemName->clear();
    ui->EditUnit->clear();
    ui->EditUnitCost->clear();
    ui->EditDescription->clear();
}

void MainWindow::UpdateKitList()
{
    CurrentKitIndex = -1;

    CurrentKitContentIndex = -1;

    ClearKitContentList();

    ui->KitList->blockSignals(true);

    ui->KitList->clearContents();

    // Remove all empty rows
    ui->KitList->setRowCount(0);

    // Create new rows
    ui->KitList->setRowCount(Kits.count());

    int kitIdx = 0;

    QList<KitType>::iterator kitIter = Kits.begin();

    for(kitIter = Kits.begin() ; kitIter != Kits.end() ; kitIter++){

        QTableWidgetItem* kitItemWidget = new QTableWidgetItem();

        kitIter->TotalCost = 0.0;

        // Analyze kit
        int contentIdx = 0;

        QList<KitContentType>::iterator contentIter = kitIter->KitContent.begin();

        QList<int> ErasedContents;

        for(; contentIter != kitIter->KitContent.end() ; contentIter++){

            bool Found = false;

            for(GeneralStoreItemType storeItem: GeneralStoreItems){
                if(storeItem.ItemName == contentIter->contentName){
                    Found = true;

                    // Update cost
                    if(contentIter->contentUnitCost != storeItem.UnitCost){
                        SetAlertOn();
                        contentIter->contentUnitCost = storeItem.UnitCost;
                    }
                    if(contentIter->contentUnitSuffix != storeItem.UnitSuffix){
                        SetAlertOn();
                        contentIter->contentUnitSuffix = storeItem.UnitSuffix;
                    }
                    if(contentIter->contentCost != contentIter->contentUnitCost * contentIter->contentAmount){
                        SetAlertOn();
                        contentIter->contentCost = contentIter->contentUnitCost * contentIter->contentAmount;
                    }

                    kitIter->TotalCost += contentIter->contentCost;
                }
            }

            if(!Found){
                SetAlertOn();
                ErasedContents.append(contentIdx);

            }

            contentIdx++;
        }

        for(int erasingIdx : ErasedContents){
            kitIter->KitContent.removeAt(erasingIdx);
        }

        kitItemWidget->setText(kitIter->KitName + " [" + QString::number(kitIter->TotalCost) + " $]");

        ui->KitList->setItem(kitIdx, 0, kitItemWidget);

        kitIdx++;
    }

    ui->KitList->blockSignals(false);
}

void MainWindow::UpdataKitContentList()
{
    ui->KitContentList->blockSignals(true);

    if(CurrentKitIndex < 0){
        return;
    }

    if(CurrentKitIndex >= Kits.count()){
        return;
    }

    CurrentKitContentIndex = -1;

    // Get current selected kit
    KitType selectedKit = Kits.at(CurrentKitIndex);

    QList<KitContentType>::iterator contentIter = selectedKit.KitContent.begin();

    // Erase list first
    ui->KitContentList->setRowCount(0);

    // Then create empty rows
    ui->KitContentList->setRowCount(selectedKit.KitContent.count());

    int contentIdx = 0;
    for(; contentIter != selectedKit.KitContent.end(); contentIter++){
        QTableWidgetItem* contentNameWidget, *contentAmountWidget, *contentCostWidget;

        contentNameWidget = new QTableWidgetItem();
        contentAmountWidget = new QTableWidgetItem();
        contentCostWidget = new QTableWidgetItem();

        contentNameWidget->setText(contentIter->contentName);
        contentAmountWidget->setText(QString::number(contentIter->contentAmount) + " " +contentIter->contentUnitSuffix);
        contentCostWidget->setText(QString::number(contentIter->contentAmount * contentIter->contentUnitCost) + " $");

        ui->KitContentList->setItem(contentIdx, 0, contentNameWidget);
        ui->KitContentList->setItem(contentIdx, 1, contentAmountWidget);
        ui->KitContentList->setItem(contentIdx, 2, contentCostWidget);

        contentIdx++;
    }

    ui->KitContentList->blockSignals(false);
}

void MainWindow::ClearKitContentList()
{
    ui->KitContentList->blockSignals(true);

    ui->KitContentList->clearContents();
    ui->KitContentList->setRowCount(0);

    ui->EditKitName->clear();
    ui->EditKitTotalCost->clear();

    ui->KitContentList->blockSignals(false);
}

void MainWindow::SetAlertOn()
{
    ui->AlertLabel->setVisible(true);
    ui->AlertText->setVisible(true);
}

void MainWindow::SetAlertOff()
{
    ui->AlertLabel->setVisible(false);
    ui->AlertText->setVisible(false);
}

bool MainWindow::RegisterFileExists()
{
    QFileInfo check_file(QString("Register.REG"));
    // check if file exists and if yes: Is it really a file and no directory?
    if (check_file.exists() && check_file.isFile()) {
        return true;
    } else {
        return false;
    }
}

void MainWindow::DeleteRegisterFile()
{
    QFile file ("Register.REG");
    file.remove();
}

void MainWindow::RegisterSave()
{
    bool FileExist = RegisterFileExists();

    if(FileExist){
        DeleteRegisterFile();
    }

    std::ofstream registerFile ("Register.REG");

    /* General Store Item Type
     *
     * QString ItemName;
     * QString UnitSuffix;
     * QString Description;
     * double UnitCost;
    */

    std::string itemCount = std::to_string(GeneralStoreItems.count());

    registerFile << itemCount << std::endl;

    // Save items
    for(GeneralStoreItemType item : GeneralStoreItems){
        registerFile << item.ItemName.toStdString() << std::endl;
        registerFile << item.UnitSuffix.toStdString() << std::endl;
        registerFile << item.Description.toStdString() << std::endl;
        registerFile << std::to_string(item.UnitCost) << std::endl;
    }

    // Save kits
    std::string kitCount = std::to_string(Kits.count());

    registerFile << kitCount << std::endl;

    for(KitType kit : Kits){
        registerFile << kit.KitName.toStdString() << std::endl;
        registerFile << std::to_string(kit.TotalCost) << std::endl;
        registerFile << std::to_string(kit.KitContent.count()) << std::endl;

        for(KitContentType content: kit.KitContent){
            registerFile << content.contentName.toStdString() << std::endl;
            registerFile << content.contentUnitSuffix.toStdString() << std::endl;
            registerFile << std::to_string(content.contentAmount) << std::endl;
            registerFile << std::to_string(content.contentUnitCost) << std::endl;
            registerFile << std::to_string(content.contentCost) << std::endl;
        }
    }

    registerFile.close();

    SetAlertOff();
}

void MainWindow::RegisterRead()
{
    std::ifstream registerFile("Register.REG");

    GeneralStoreItems.clear();
    Kits.clear();

    std::string Reading;

    if(!registerFile.is_open() || registerFile.eof()){
        return;
    }

    // Read item count
    std::getline(registerFile, Reading);

    int itemCount = std::stoi(Reading);

    if(itemCount > 0){
        for(int itemIdx = 0; itemIdx < itemCount ; itemIdx++){
            GeneralStoreItemType newItem;

            // name - suffix - description - cost

            // read name
            std::getline(registerFile, Reading);
            newItem.ItemName = QString::fromStdString(Reading);

            // read suffix
            std::getline(registerFile, Reading);
            newItem.UnitSuffix = QString::fromStdString(Reading);

            // read description
            std::getline(registerFile, Reading);
            newItem.Description = QString::fromStdString(Reading);

            // read unit cost
            std::getline(registerFile, Reading);
            newItem.UnitCost = std::stod(Reading);

            GeneralStoreItems.append(newItem);
        }
    }

    // Read kit count
    std::getline(registerFile, Reading);

    int kitCount = std::stoi(Reading);

    if(kitCount > 0){
        for(int kitIdx = 0 ; kitIdx < kitCount ; kitIdx++){
            KitType newKit;

            // Read kit name
            std::getline(registerFile, Reading);
            newKit.KitName = QString::fromStdString(Reading);

            // Read total cost
            std::getline(registerFile, Reading);
            newKit.TotalCost = std::stod(Reading);

            // Read Kit Content Count
            std::getline(registerFile, Reading);
            int kitContentCount = std::stoi(Reading);

            if(kitContentCount > 0){
                for(int contentIdx = 0 ; contentIdx < kitContentCount ; contentIdx++){
                    KitContentType newContent;

                    /*
                    registerFile << content.contentName.toStdString() << std::endl;
                    registerFile << content.contentUnitSuffix.toStdString() << std::endl;
                    registerFile << std::to_string(content.contentAmount) << std::endl;
                    registerFile << std::to_string(content.contentUnitCost) << std::endl;
                    registerFile << std::to_string(content.contentCost) << std::endl;
                    */

                    // Read content name
                    std::getline(registerFile, Reading);
                    newContent.contentName = QString::fromStdString(Reading);

                    // Read content unit suffix
                    std::getline(registerFile, Reading);
                    newContent.contentUnitSuffix = QString::fromStdString(Reading);

                    // Read content amount
                    std::getline(registerFile, Reading);
                    newContent.contentAmount = std::stod(Reading);

                    // Read content unit cost
                    std::getline(registerFile, Reading);
                    newContent.contentUnitCost = std::stod(Reading);

                    // Read content cost
                    std::getline(registerFile, Reading);
                    newContent.contentCost = std::stod(Reading);

                    newKit.KitContent.append(newContent);
                }
            }

            Kits.append(newKit);
        }
    }

    registerFile.close();

    UpdateGeneralStoreItemList();
}

int MainWindow::EnsureDialogOpen()
{
    this->setEnabled(false);

    EnsureDialog* newEnsureDialog = new EnsureDialog();

    int result = newEnsureDialog->exec();

    if(result == ENSURE_DIALOG_YES){
        result = 1;
    }
    else{
        result = 0;
    }

    this->setEnabled(true);

    return result;
}

void MainWindow::on_GeneralStoreList_cellClicked(int row, int column)
{
    Q_UNUSED(column)

    CurrentStoreItemIndex = row;

    GeneralStoreItemType selectedItem = GeneralStoreItems.at(row);

    QString itemName = selectedItem.ItemName;
    QString unitName = selectedItem.UnitSuffix;
    QString unitCost = QString::number(selectedItem.UnitCost);
    QString description = selectedItem.Description;

    UpdateSelectedStoreItemData(itemName, unitName, unitCost, description);
}

void MainWindow::on_AddKitButton_clicked(bool checked)
{
    Q_UNUSED(checked)

    CurrentKitIndex = -1;
    CurrentKitContentIndex = -1;

    QString* N_kitName = new QString();

    KitDialog* kitDialog = new KitDialog();

    kitDialog->AssignDialogPtr(N_kitName, false);

    this->setEnabled(false);

    int dialogResult = kitDialog->exec();

    this->setEnabled(true);

    if(dialogResult == KIT_DIALOG_ACCEPT){
        // create new kit
        KitType newkit;
        newkit.KitName = *N_kitName;
        newkit.TotalCost = 0.0;

        bool OverlappingItem = false;

        // Can't add an existing kit!
        for(KitType kit: Kits){
            if(kit.KitName == newkit.KitName){
                OverlappingItem = true;
                break;
            }
        }

        if(OverlappingItem){
            return;
        }

        Kits.append(newkit);

        UpdateKitList();

        SetAlertOn();
    }
    else if (KIT_DIALOG_CANCEL) {
        // do nothing

        return;
    }
}

void MainWindow::on_RemoveKitButton_clicked(bool checked)
{
    Q_UNUSED(checked)

    if(CurrentKitIndex < 0){
        return;
    }

    if(CurrentKitIndex >= Kits.count()){
        return;
    }

    int Ensure = EnsureDialogOpen();

    if(Ensure != 1){
        return;
    }

    Kits.removeAt(CurrentKitIndex);

    UpdateKitList();

    SetAlertOn();

    CurrentKitIndex = -1;
    CurrentKitContentIndex = -1;
}

void MainWindow::on_EditKitButton_clicked(bool checked)
{
    if(CurrentKitIndex < 0){
        return;
    }

    if(CurrentKitIndex >= Kits.count()){
        return;
    }

    Q_UNUSED(checked)

    QString* N_kitName = new QString();

    *N_kitName = Kits.at(CurrentKitIndex).KitName;

    KitDialog* kitDialog = new KitDialog();

    kitDialog->AssignDialogPtr(N_kitName, true);

    this->setEnabled(false);

    int dialogResult = kitDialog->exec();

    this->setEnabled(true);

    if(dialogResult == KIT_DIALOG_ACCEPT){

        QList<KitType>::Iterator kitIter = Kits.begin() + CurrentKitIndex;

        kitIter->KitName = *N_kitName;

        UpdateKitList();

        SetAlertOn();

    }
    else if (KIT_DIALOG_CANCEL) {
        // do nothing

        return;
    }
}

void MainWindow::on_KitList_cellClicked(int row, int column)
{
    Q_UNUSED(column)

    CurrentKitIndex = row;

    if(CurrentKitIndex >= Kits.count()){
        CurrentKitIndex = Kits.count() - 1 ;
    }
    else if (CurrentKitIndex < 0 ) {
        CurrentKitIndex = -1;
        return;
    }

    KitType selectedKit = Kits.at(CurrentKitIndex);

    ui->EditKitName->setText(selectedKit.KitName);
    ui->EditKitTotalCost->setText(QString::number(selectedKit.TotalCost) + " $");

    UpdataKitContentList();
}

void MainWindow::on_AddStoreItemButton_clicked(bool checked)
{
    Q_UNUSED(checked)

    if(CurrentKitIndex < 0){
        return;
    }

    if(CurrentKitIndex >= Kits.count()){
        return;
    }

    if(ui->StoreItemSelect->currentText() == ""){
        return;
    }

    if(ui->StoreItemAmountSelect->value() <= 0.0){
        return;
    }

    // Find the equivalent general store item first

    GeneralStoreItemType candidateStoreItem;

    QString storeItemName = ui->StoreItemSelect->currentText();

    QList<GeneralStoreItemType>::iterator storeItemIter = GeneralStoreItems.begin();

    bool Found = false;

    for(; storeItemIter != GeneralStoreItems.end() ; storeItemIter++){
        if(storeItemName == storeItemIter->ItemName){
            candidateStoreItem = *storeItemIter;
            Found = true;
            break;
        }
    }

    if(!Found){
        return;
    }

    // Now we have to find the right kit!

    QList<KitType>::iterator kitIter = Kits.begin() + CurrentKitIndex;

    // Check if there is another content with same name or not?

    QList<KitContentType>::iterator contentIter = kitIter->KitContent.begin();

    bool OverlappingContent = false;

    for(; contentIter != kitIter->KitContent.end(); contentIter++){
        if(contentIter->contentName == storeItemName){
            OverlappingContent = true;

            // just update the amount

            contentIter->contentAmount = ui->StoreItemAmountSelect->value();
            contentIter->contentCost = contentIter->contentAmount * contentIter->contentUnitCost;
            contentIter->contentUnitSuffix = candidateStoreItem.UnitSuffix;

            break;
        }
    }

    if(!OverlappingContent){
        // Create a new content type and add
        KitContentType newContent;
        newContent.contentName = candidateStoreItem.ItemName;
        newContent.contentAmount = ui->StoreItemAmountSelect->value();
        newContent.contentUnitCost = candidateStoreItem.UnitCost;
        newContent.contentUnitSuffix = candidateStoreItem.UnitSuffix;
        newContent.contentCost = newContent.contentAmount * newContent.contentUnitCost;

        kitIter->KitContent.append(newContent);
    }

    // Calculate kit price again
    contentIter = kitIter->KitContent.begin();

    kitIter->TotalCost = 0.0;

    for(; contentIter != kitIter->KitContent.end(); contentIter++){
        kitIter->TotalCost += contentIter->contentCost;
    }

    ui->EditKitTotalCost->setText(QString::number(kitIter->TotalCost) + " $");
    QTableWidgetItem* itemToEdit = ui->KitList->item(CurrentKitIndex,0);

    itemToEdit->setText(Kits.at(CurrentKitIndex).KitName + " [" + QString::number(kitIter->TotalCost) + " $]");

    UpdataKitContentList();
    SetAlertOn();
}

void MainWindow::on_RemoveSelectedStoreItemButton_clicked(bool checked)
{
    Q_UNUSED(checked)

    if(CurrentKitIndex < 0){
        return;
    }

    if(CurrentKitIndex >= Kits.count()){
        return;
    }

    KitType selectedKit = Kits.at(CurrentKitIndex);

    if(CurrentKitContentIndex < 0 ){
        return;
    }

    if(CurrentKitContentIndex >= selectedKit.KitContent.count()){
        return;
    }

    QList<KitType>::iterator kitIter = Kits.begin() + CurrentKitIndex;

    kitIter->KitContent.removeAt(CurrentKitContentIndex);

    QList<KitContentType>::iterator contentIter = kitIter->KitContent.begin();

    kitIter->TotalCost = 0.0;

    for(; contentIter != kitIter->KitContent.end(); contentIter++){
        kitIter->TotalCost += contentIter->contentCost;
    }

    UpdataKitContentList();

    ui->EditKitTotalCost->setText(QString::number(kitIter->TotalCost) + " $");
    QTableWidgetItem* itemToEdit = ui->KitList->item(CurrentKitIndex,0);

    itemToEdit->setText(Kits.at(CurrentKitIndex).KitName + " [" + QString::number(kitIter->TotalCost) + " $]");

    SetAlertOn();

}

void MainWindow::on_KitContentList_cellClicked(int row, int column)
{
    Q_UNUSED(column)

    CurrentKitContentIndex = row;
}

void MainWindow::on_SaveButton_clicked(bool checked)
{
    Q_UNUSED(checked)

    RegisterSave();
}
