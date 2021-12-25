#include "EnsureDialog.h"
#include "ui_EnsureDialog.h"

EnsureDialog::EnsureDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EnsureDialog)
{
    ui->setupUi(this);
}

EnsureDialog::~EnsureDialog()
{
    delete ui;
}

void EnsureDialog::on_AcceptButton_clicked(bool checked)
{
    Q_UNUSED(checked)

    done(ENSURE_DIALOG_YES);
}

void EnsureDialog::on_CancelButton_clicked(bool checked)
{
    Q_UNUSED(checked)

    done(ENSURE_DIALOG_NO);
}
