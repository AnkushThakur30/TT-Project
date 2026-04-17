#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_database(new Database(this))
{
    ui->setupUi(this);

    if (!m_database->initialize()) {
        QMessageBox::critical(this, "Database Error", "Failed to initialize database!");
        return;
    }

    setupUI();
    refreshData();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupUI()
{
    setWindowTitle("Bus Booking Management System");
    setMinimumSize(1000, 700);
    resize(1200, 800);

    m_tabWidget = new QTabWidget(this);
    setCentralWidget(m_tabWidget);

    setupBusesTab();
    setupBookingsTab();
    setupBookingDialog();

    // Connect refresh button (we'll add it to menu)
    QMenuBar *menuBar = this->menuBar();
    QMenu *fileMenu = menuBar->addMenu("&File");
    QAction *refreshAction = fileMenu->addAction("&Refresh");
    connect(refreshAction, &QAction::triggered, this, &MainWindow::refreshData);
}

void MainWindow::setupBusesTab()
{
    m_busesTab = new QWidget();
    m_tabWidget->addTab(m_busesTab, "Buses");

    QVBoxLayout *layout = new QVBoxLayout(m_busesTab);

    // Header
    QLabel *header = new QLabel("Available Buses");
    header->setStyleSheet("font-size: 18px; font-weight: bold; margin-bottom: 10px;");
    layout->addWidget(header);

    // Scroll area for buses
    m_busesScrollArea = new QScrollArea();
    m_busesScrollArea->setWidgetResizable(true);
    m_busesScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_busesScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    m_busesContainer = new QWidget();
    m_busesLayout = new QGridLayout(m_busesContainer);
    m_busesLayout->setSpacing(15);

    m_busesScrollArea->setWidget(m_busesContainer);
    layout->addWidget(m_busesScrollArea);
}

void MainWindow::setupBookingsTab()
{
    m_bookingsTab = new QWidget();
    m_tabWidget->addTab(m_bookingsTab, "My Bookings");

    QVBoxLayout *layout = new QVBoxLayout(m_bookingsTab);

    // Header
    QLabel *header = new QLabel("Your Bookings");
    header->setStyleSheet("font-size: 18px; font-weight: bold; margin-bottom: 10px;");
    layout->addWidget(header);

    // Bookings table
    m_bookingsTable = new QTableWidget();
    m_bookingsTable->setColumnCount(8);
    m_bookingsTable->setHorizontalHeaderLabels({"ID", "Type", "Item", "Name", "Email", "Date", "Quantity", "Total Price"});
    m_bookingsTable->setAlternatingRowColors(true);
    m_bookingsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_bookingsTable->horizontalHeader()->setStretchLastSection(true);

    layout->addWidget(m_bookingsTable);
}

void MainWindow::setupBookingDialog()
{
    m_bookingDialog = new QDialog(this);
    m_bookingDialog->setWindowTitle("Make Booking");
    m_bookingDialog->setModal(true);
    m_bookingDialog->setFixedSize(400, 300);

    QVBoxLayout *layout = new QVBoxLayout(m_bookingDialog);

    // Customer details
    QGroupBox *customerGroup = new QGroupBox("Customer Details");
    QFormLayout *customerLayout = new QFormLayout(customerGroup);

    m_customerNameEdit = new QLineEdit();
    m_customerEmailEdit = new QLineEdit();
    m_bookingDateEdit = new QDateEdit();
    m_bookingDateEdit->setDate(QDate::currentDate());
    m_quantitySpinBox = new QSpinBox();
    m_quantitySpinBox->setMinimum(1);
    m_quantitySpinBox->setMaximum(10);

    customerLayout->addRow("Name:", m_customerNameEdit);
    customerLayout->addRow("Email:", m_customerEmailEdit);
    customerLayout->addRow("Date:", m_bookingDateEdit);
    customerLayout->addRow("Quantity:", m_quantitySpinBox);

    layout->addWidget(customerGroup);

    // Total price
    m_totalPriceLabel = new QLabel("Total Price: $0.00");
    m_totalPriceLabel->setStyleSheet("font-weight: bold; font-size: 14px; color: #2e7d32;");
    layout->addWidget(m_totalPriceLabel);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *cancelButton = new QPushButton("Cancel");
    QPushButton *bookButton = new QPushButton("Book Now");
    bookButton->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; padding: 8px 16px; border: none; border-radius: 4px; }"
                              "QPushButton:hover { background-color: #45a049; }");

    buttonLayout->addStretch();
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(bookButton);

    layout->addLayout(buttonLayout);

    connect(cancelButton, &QPushButton::clicked, m_bookingDialog, &QDialog::reject);
    connect(bookButton, &QPushButton::clicked, this, &MainWindow::confirmBooking);
    connect(m_quantitySpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [this](int value) {
        // Update total price (this will be set when dialog is shown)
        double price = m_totalPriceLabel->text().split("$")[1].toDouble();
        if (price > 0) {
            double total = price / (value > 0 ? value : 1) * value; // Recalculate
            m_totalPriceLabel->setText(QString("Total Price: $%1").arg(total, 0, 'f', 2));
        }
    });
}

void MainWindow::populateHotels()
{
    // Clear existing widgets
    QLayoutItem *item;
    while ((item = m_hotelsLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    QVector<Hotel> hotels = m_database->getHotels();
    int row = 0, col = 0;
    const int maxCols = 3;

    for (const Hotel& hotel : hotels) {
        createHotelCard(hotel, row, col);
        col++;
        if (col >= maxCols) {
            col = 0;
            row++;
        }
    }
}

void MainWindow::populateBuses()
{
    // Clear existing widgets
    QLayoutItem *item;
    QVector<Booking> bookings = m_database->getBookings();

    for (const Booking& booking : bookings) {
        int row = m_bookingsTable->rowCount();
        m_bookingsTable->insertRow(row);

        m_bookingsTable->setItem(row, 0, new QTableWidgetItem(QString::number(booking.id)));
        m_bookingsTable->setItem(row, 1, new QTableWidgetItem(booking.type));
        m_bookingsTable->setItem(row, 2, new QTableWidgetItem(getItemName(booking.type, booking.itemId)));
        m_bookingsTable->setItem(row, 3, new QTableWidgetItem(booking.customerName));
        m_bookingsTable->setItem(row, 4, new QTableWidgetItem(booking.customerEmail));
        m_bookingsTable->setItem(row, 5, new QTableWidgetItem(booking.date));
        m_bookingsTable->setItem(row, 6, new QTableWidgetItem(QString::number(booking.quantity)));
        m_bookingsTable->setItem(row, 7, new QTableWidgetItem(QString("$%1").arg(booking.totalPrice, 0, 'f', 2)));
    }

    m_bookingsTable->resizeColumnsToContents();
}

void MainWindow::createHotelCard(const Hotel& hotel, int row, int col)
{
    QGroupBox *card = new QGroupBox();
    card->setFixedSize(300, 400);
    card->setStyleSheet("QGroupBox { border: 1px solid #ccc; border-radius: 5px; margin: 5px; }");

    QVBoxLayout *layout = new QVBoxLayout(card);

    // Image placeholder (in real app, load from URL)
    QLabel *imageLabel = new QLabel();
    imageLabel->setFixedSize(280, 150);
    imageLabel->setStyleSheet("border: 1px solid #ddd; background-color: #f0f0f0;");
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setText("Hotel Image\n(URL: " + hotel.imageUrl + ")");
    layout->addWidget(imageLabel);

    // Hotel details
    QLabel *nameLabel = new QLabel(hotel.name);
    nameLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
    layout->addWidget(nameLabel);

    QLabel *locationLabel = new QLabel("📍 " + hotel.location);
    layout->addWidget(locationLabel);
BusCard(const Bus& bus, int row, int col)
{
    QGroupBox *card = new QGroupBox();
    card->setFixedSize(300, 400);
    card->setStyleSheet("QGroupBox { border: 1px solid #ccc; border-radius: 5px; margin: 5px; }");

    QVBoxLayout *layout = new QVBoxLayout(card);

    // Image placeholder
    QLabel *imageLabel = new QLabel();
    imageLabel->setFixedSize(280, 150);
    imageLabel->setStyleSheet("border: 1px solid #ddd; background-color: #f0f0f0;");
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setText("Bus Image\n(URL: " + bus.imageUrl + ")");
    layout->addWidget(imageLabel);

    // Bus details
    QLabel *nameLabel = new QLabel(bus.name);
    nameLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
    layout->addWidget(nameLabel);

    QLabel *routeLabel = new QLabel("🚌 " + bus.route);
    layout->addWidget(routeLabel);

    QLabel *timeLabel = new QLabel("⏰ " + bus.departureTime + " (" + bus.duration + ")");
    layout->addWidget(timeLabel);

    QLabel *descLabel = new QLabel(bus.description);
    descLabel->setWordWrap(true);
    descLabel->setMaximumHeight(40);
    layout->addWidget(descLabel);

    QLabel *amenitiesLabel = new QLabel("Amenities: " + bus.amenities.join(", "));
    amenitiesLabel->setWordWrap(true);
    amenitiesLabel->setMaximumHeight(30);
    layout->addWidget(amenitiesLabel);

    QLabel *priceLabel = new QLabel(QString("₹%1 per seat").arg(bus.price, 0, 'f', 2));
    priceLabel->setStyleSheet("color: #2e7d32; font-weight: bold; font-size: 16px;");
    layout->addWidget(priceLabel);

    // Book button
    QPushButton *bookButton = new QPushButton("Book Now");
    bookButton->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; padding: 8px; border: none; border-radius: 4px; }"
                              "QPushButton:hover { background-color: #45a049; }");
    connect(bookButton, &QPushButton::clicked, [this, bus]() {
        showBookingDialog(
    double total = price * m_quantitySpinBox->value();
    m_totalPriceLabel->setText(QString("Total Price: $%1").arg(total, 0, 'f', 2));

    m_bookingDialog->exec();
}

void MainWindow::confirmBooking()
{
    QString type = m_bookingDialog->property("bookingType").toString();
    int itemId = m_bookingDialog->property("itemId").toInt();
    double price = m_bookingDialog->property("price").toDouble();

    QString name = m_customerNameEdit->text();
    QString email = m_customerEmailEdit->text();
    QString date = m_bookingDateEdit->date().toString("yyyy-MM-dd");
    int quantity = m_quantitySpinBox->value();

    if (name.isEmpty() || email.isEmpty()) {
        QMessageBox::warning(m_bookingDialog, "Incomplete Information", "Please fill in all fields.");
        return;
    }

    double totalPrice = price * quantity;

    Booking booking;
    booking.type = type;
    booking.itemId = itemId;
    booking.customerName = name;
    booking.customerEmail = email;
    booking.date = date;
    booking.quantity = quantity;
    booking.totalPrice = totalPrice;

    if (m_database->addBooking(booking)) {
        QMessageBox::information(m_bookingDialog, "Booking Confirmed",
                                QString("Your booking has been confirmed!\n\nTotal Price: $%1\nBooking ID: %2")
                                .arg(totalPrice, 0, 'f', 2).arg(booking.id));
        m_bookingDialog->accept();
        populateBookings();
    } else {
        QMessageBox::critical(m_bookingDialog, "Booking Failed", "Failed to save booking. Please try again.");
    }
}

QString MainWindow::getItemName(int id)
{
    QVector<Bus> buses = m_database->getBuses();
    for (const Bus& bus : buses) {
        if (bus.id == id) return bus.name;
    }int itemId, const QString& itemName, double price)
{
    int itemId = m_bookingDialog->property("itemId").toInt();
    double price = m_bookingDialog->property("price").toDouble();

    QString name = m_customerNameEdit->text();
    QString email = m_customerEmailEdit->text();
    QString date = m_bookingDateEdit->date().toString("yyyy-MM-dd");
    int quantity = m_quantitySpinBox->value();

    if (name.isEmpty() || email.isEmpty()) {
        QMessageBox::warning(m_bookingDialog, "Incomplete Information", "Please fill in all fields.");
        return;
    }

    double totalPrice = price * quantity;

    Booking booking;
    booking.type = "bus";
    booking.itemId = itemId;
    booking.customerName = name;
    booking.customerEmail = email;
    booking.date = date;
    booking.quantity = quantity;
    booking.totalPrice = totalPrice;

    if (m_database->addBooking(booking)) {
        QMessageBox::information(m_bookingDialog, "Booking Confirmed",
                                QString("Your bus booking has been confirmed!\n\nTotal Price: ₹
    populateHotels();
    populateBuses();
    populateBookings();
}