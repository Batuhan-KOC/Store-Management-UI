#include "KitDialog.h"
#include "ui_KitDialog.h"

KitDialog::KitDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KitDialog)
{
    ui->setupUi(this);

    ui->AlertText->setVisible(false);

    warningTimer = new QTimer(this);

    connect(warningTimer, &QTimer::timeout, this, &KitDialog::AlarmTimer);
}

KitDialog::~KitDialog()
{
    delete ui;
}

void KitDialog::AssignDialogPtr(QString *kitNamePtr, bool isEdit)
{
    A_kitName = kitNamePtr;

    isEditDialog = isEdit;

    if(!isEditDialog){
        *A_kitName = "";
    }

    if(isEditDialog){
        ui->SetItemName->setText(*A_kitName);
    }

    kitName = *A_kitName;
}

void KitDialog::AlarmTimer()
{
    ui->AlertText->setVisible(false);

    warningTimer->stop();
}

void KitDialog::on_SetItemName_textEdited(const QString &arg1)
{
    kitName = arg1;
}

void KitDialog::on_AcceptButton_clicked(bool checked)
{
    Q_UNUSED(checked)

    bool AlarmNeeded = false;

    if(ui->SetItemName->text() == ""){
        AlarmNeeded = true;
    }

    if(AlarmNeeded){
        ui->AlertText->setVisible(true);

        warningTimer->start(2000);

        return;
    }

    *A_kitName = kitName;

    done(KIT_DIALOG_ACCEPT);
}

void KitDialog::on_CancelButton_clicked(bool checked)
{
    Q_UNUSED(checked)

    done(KIT_DIALOG_CANCEL);
}
