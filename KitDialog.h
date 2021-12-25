#ifndef KITDIALOG_H
#define KITDIALOG_H

#include <QDialog>
#include <QTimer>
#include "SMSDataTypes.h"

#define KIT_DIALOG_ACCEPT 88
#define KIT_DIALOG_CANCEL 2

namespace Ui {
class KitDialog;
}

class KitDialog : public QDialog
{
    Q_OBJECT

public:
    explicit KitDialog(QWidget *parent = nullptr);
    ~KitDialog();

    void AssignDialogPtr(QString* kitNamePtr, bool isEdit);

private slots:
    void AlarmTimer();

    void on_SetItemName_textEdited(const QString &arg1);

    void on_AcceptButton_clicked(bool checked);

    void on_CancelButton_clicked(bool checked);

private:
    Ui::KitDialog *ui;

    QTimer* warningTimer;

    QString kitName = "";

    QString* A_kitName;

    bool isEditDialog = false;
};

#endif // KITDIALOG_H
