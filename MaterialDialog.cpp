#include "MaterialDialog.h"
#include "ui_MaterialDialog.h"

MaterialDialog::MaterialDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MaterialDialog)
{
    ui->setupUi(this);

    ui->AlertText->setVisible(false);

    warningTimer = new QTimer(this);

    connect(warningTimer, &QTimer::timeout, this, &MaterialDialog::AlarmTimer);
}

MaterialDialog::~MaterialDialog()
{
    delete ui;
}

void MaterialDialog::AssignDialogPtr(QString *itemNamePtr, QString *itemUnitSuffixPtr, QString *itemDescriptionPtr, double *itemUnitCostPtr, bool isEdit)
{
    A_itemName = itemNamePtr;
    A_itemUnitSuffix = itemUnitSuffixPtr;
    A_itemDescription = itemDescriptionPtr;
    A_itemUnitCost = itemUnitCostPtr;

    isEditDialog = isEdit;

    if(!isEditDialog){
        *A_itemName = "";
        *A_itemUnitSuffix = "";
        *A_itemDescription = "";
        *A_itemUnitCost = 0.0;
    }

    if(isEditDialog){
        ui->SetItemName->setText(*A_itemName);
        ui->SetUnitSuffix->setText(*A_itemUnitSuffix);
        ui->SetItemDescription->setText(*A_itemDescription);
        ui->SetUnitCost->setValue(*A_itemUnitCost);
    }

    itemName = *A_itemName;
    itemUnitSuffix = *A_itemUnitSuffix;
    itemDescription = *A_itemDescription;
    itemUnitCost = *A_itemUnitCost;
}

void MaterialDialog::AlarmTimer()
{
    ui->AlertText->setVisible(false);

    warningTimer->stop();
}

void MaterialDialog::on_SetItemName_textEdited(const QString &arg1)
{
    itemName = arg1;
}

void MaterialDialog::on_SetUnitSuffix_textEdited(const QString &arg1)
{
    itemUnitSuffix = arg1;
}

void MaterialDialog::on_SetUnitCost_valueChanged(double arg1)
{
    itemUnitCost = arg1;
}

void MaterialDialog::on_SetItemDescription_textEdited(const QString &arg1)
{
    itemDescription = arg1;
}

void MaterialDialog::on_AcceptButton_clicked(bool checked)
{
    Q_UNUSED(checked)

    bool AlarmNeeded = false;

    if(ui->SetItemName->text() == ""){
        AlarmNeeded = true;
    }
    else if(ui->SetUnitSuffix->text() == ""){
        AlarmNeeded = true;
    }

    if(AlarmNeeded){
        ui->AlertText->setVisible(true);

        warningTimer->start(2000);

        return;
    }

    *A_itemName        = itemName;
    *A_itemUnitSuffix  = itemUnitSuffix;
    *A_itemDescription = itemDescription;
    *A_itemUnitCost    = itemUnitCost;

    done(MATERIAL_DIALOG_ACCEPT);
}

void MaterialDialog::on_CancelButton_clicked(bool checked)
{
    Q_UNUSED(checked)

    done(MATERIAL_DIALOG_CANCEL);
}
