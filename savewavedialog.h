#ifndef SAVEWAVEDIALOG_H
#define SAVEWAVEDIALOG_H

#include <QDialog>
#include <QtCharts>
#include <memory>

using std::shared_ptr;

namespace Ui {
class SaveWaveDialog;
}


struct WaveSettings;
class QtCharts::QLineSeries;
class QIntValidator;

class SaveWaveDialog : public QDialog
{
    Q_OBJECT

public:
    SaveWaveDialog(shared_ptr<WaveSettings> settings, QtCharts::QLineSeries& series, QWidget *parent = 0);
    ~SaveWaveDialog();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_btnBrowse_clicked();

private:
    Ui::SaveWaveDialog *ui;
    shared_ptr<WaveSettings> settings;
    QtCharts::QLineSeries& series;
    QString filePath;

    QIntValidator* validator;
};

#endif // SAVEWAVEDIALOG_H
