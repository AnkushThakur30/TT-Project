#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QSpinBox>
#include <QTableWidget>
#include <QGroupBox>
#include <QScrollArea>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPixmap>
#include <QListWidget>
#include <QSplitter>

#include "database.h"

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
    void showBuses();
    void showBookings();
    void bookBus();
    void refreshData();

private:
    Ui::MainWindow *ui;
    Database *m_database;

    // UI components
    QTabWidget *m_tabWidget;
    QWidget *m_busesTab;
    QWidget *m_bookingsTab;

    // Buses tab
    QScrollArea *m_busesScrollArea;
    QWidget *m_busesContainer;
    QGridLayout *m_busesLayout;

    // Bookings tab
    QTableWidget *m_bookingsTable;

    // Booking dialog components
    QDialog *m_bookingDialog;
    QLineEdit *m_customerNameEdit;
    QLineEdit *m_customerEmailEdit;
    QDateEdit *m_bookingDateEdit;
    QSpinBox *m_quantitySpinBox;
    QLabel *m_totalPriceLabel;

    void setupUI();
    void setupBusesTab();
    void setupBookingsTab();
    void setupBookingDialog();

    void populateBuses();
    void populateBookings();

    void createBusCard(const Bus& bus, int row, int col);

    void showBookingDialog(int itemId, const QString& itemName, double price);
    void confirmBooking();

    QString getItemName(int id);
};

#endif // MAINWINDOW_H