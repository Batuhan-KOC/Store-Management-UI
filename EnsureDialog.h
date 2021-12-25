#ifndef ENSUREDIALOG_H
#define ENSUREDIALOG_H

#include <QDialog>

#define ENSURE_DIALOG_YES 77
#define ENSURE_DIALOG_NO 3

namespace Ui {
class EnsureDialog;
}

class EnsureDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EnsureDialog(QWidget *parent = nullptr);
    ~EnsureDialog();

private slots:
    void on_AcceptButton_clicked(bool checked);

    void on_CancelButton_clicked(bool checked);

private:
    Ui::EnsureDialog *ui;
};

#endif // ENSUREDIALOG_H
