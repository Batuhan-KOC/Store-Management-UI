#ifndef MATERIALDIALOG_H
#define MATERIALDIALOG_H

#include <QDialog>
#include <QTimer>
#include "SMSDataTypes.h"

#define MATERIAL_DIALOG_ACCEPT 99
#define MATERIAL_DIALOG_CANCEL 1

namespace Ui {
class MaterialDialog;
}

class MaterialDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MaterialDialog(QWidget *parent = nullptr);
    ~MaterialDialog();

    void AssignDialogPtr(QString* itemNamePtr, QString* itemUnitSuffixPtr, QString* itemDescriptionPtr, double* itemUnitCostPtr, bool isEdit);

private slots:
    void AlarmTimer();

    void on_SetItemName_textEdited(const QString &arg1);

    void on_SetUnitSuffix_textEdited(const QString &arg1);

    void on_SetUnitCost_valueChanged(double arg1);

    void on_SetItemDescription_textEdited(const QString &arg1);

    void on_AcceptButton_clicked(bool checked);

    void on_CancelButton_clicked(bool checked);

private:
    Ui::MaterialDialog *ui;

    QTimer* warningTimer;

    QString itemName = "";
    QString itemUnitSuffix = "";
    QString itemDescription = "";
    double itemUnitCost = 0.0;

    QString *A_itemName;
    QString *A_itemUnitSuffix;
    QString *A_itemDescription;
    double  *A_itemUnitCost;

    bool isEditDialog = false;
};

#endif // MATERIALDIALOG_H
