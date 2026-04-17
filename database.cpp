#include "database.h"

Database::Database(QObject *parent) : QObject(parent)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("booking_system.db");
}

Database::~Database()
{
    if (m_db.isOpen()) {
        m_db.close();
    }
}

bool Database::initialize()
{
    if (!m_db.open()) {
        qDebug() << "Error opening database:" << m_db.lastError().text();
        return false;
    }

    return createTables();
}

bool Database::createTables()
{
    QSqlQuery query;

    // Create buses table
    QString createBusesTable = R"(
        CREATE TABLE IF NOT EXISTS buses (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            route TEXT NOT NULL,
            departure_time TEXT NOT NULL,
            duration TEXT,
            description TEXT,
            price REAL NOT NULL,
            image_url TEXT,
            amenities TEXT
        )
    )";

    if (!query.exec(createBusesTable)) {
        qDebug() << "Error creating buses table:" << query.lastError().text();
        return false;
    }

    // Create bookings table
    QString createBookingsTable = R"(
        CREATE TABLE IF NOT EXISTS bookings (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            type TEXT NOT NULL,
            item_id INTEGER NOT NULL,
            customer_name TEXT NOT NULL,
            customer_email TEXT NOT NULL,
            date TEXT NOT NULL,
            quantity INTEGER NOT NULL,
            total_price REAL NOT NULL
        )
    )";

    if (!query.exec(createBookingsTable)) {
        qDebug() << "Error creating bookings table:" << query.lastError().text();
        return false;
    }

    // Insert sample data if tables are empty
    insertSampleData();

    return true;
}

void Database::insertSampleData()
{
    // Check if buses table is empty
    QSqlQuery checkQuery("SELECT COUNT(*) FROM buses");
    if (checkQuery.next() && checkQuery.value(0).toInt() == 0) {
        // Insert sample buses
        QVector<Bus> sampleBuses = {
            {0, "Express Line", "New York - Boston", "08:00", "4 hours", "High-speed bus service between major cities", 45.0,
             "https://images.unsplash.com/photo-1544620347-c4fd4a3d5957?w=400", {"WiFi", "AC", "Entertainment", "Restroom"}},
            {0, "Coastal Route", "Los Angeles - San Francisco", "09:30", "6 hours", "Scenic coastal journey with ocean views", 65.0,
             "https://images.unsplash.com/photo-1558618666-fcd25c85cd64?w=400", {"WiFi", "Snacks", "Charging Ports", "Restroom"}},
            {0, "Mountain Express", "Denver - Aspen", "07:00", "3 hours", "Mountain route with stunning views", 35.0,
             "https://images.unsplash.com/photo-1578662996442-48f60103fc96?w=400", {"Heating", "WiFi", "Storage", "Restroom"}},
            {0, "Metro Link", "Washington DC - Philadelphia", "10:00", "2.5 hours", "Comfortable city-to-city transportation", 30.0,
             "https://images.unsplash.com/photo-1558618047-3c8c76ca7d13?w=400", {"WiFi", "AC", "USB Charging", "Restroom"}}
        };

        for (const Bus& bus : sampleBuses) {
            addBus(bus);
        }
    }
}

QVector<Hotel> Database::getHotels()
{
    QVector<Hotel> hotels;
    QSqlQuery query("SELECT * FROM hotels ORDER BY id");

    while (query.next()) {
        Hotel hotel;
        hotel.id = query.value("id").toInt();
        hotel.name = query.value("name").toString();
        hotel.location = query.value("location").toString();
        hotel.description = query.value("description").toString();
        hotel.price = query.value("price").toDouble();
        hotel.imageUrl = query.value("image_url").toString();
        hotel.amenities = query.value("amenities").toString().split(",");

        hotels.append(hotel);
    }

    return hotels;
}
rn buses;
}

bool Database::addBus(const Bus& bus)
{
    QSqlQuery query;
    query.prepare("INSERT INTO buses (name, route, departure_time, duration, description, price, image_url, amenities) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(bus.name);
    query.addBindValue(bus.route);
    query.addBindValue(bus.departureTime);
    query.addBindValue(bus.duration);
    query.addBindValue(bus.description);
    query.addBindValue(bus.price);
    query.addBindValue(bus.imageUrl);
    query.addBindValue(bus.amenities.join(","));

    if (!query.exec()) {
        qDebug() << "Error adding bus:" << query.lastError().text();
        return false;
    }

    return true;
}

QVector<Booking> Database::getBookings()
{
    QVector<Booking> bookings;
    QSqlQuery query("SELECT * FROM bookings ORDER BY id DESC");

    while (query.next()) {
        Booking booking;
        booking.id = query.value("id").toInt();
        booking.type = query.value("type").toString();
        booking.itemId = query.value("item_id").toInt();
        booking.customerName = query.value("customer_name").toString();
        booking.customerEmail = query.value("customer_email").toString();
        booking.date = query.value("date").toString();
        booking.quantity = query.value("quantity").toInt();
        booking.totalPrice = query.value("total_price").toDouble();

        bookings.append(booking);
    }

    return bookings;
}

bool Database::addBooking(const Booking& booking)
{
    QSqlQuery query;
    query.prepare("INSERT INTO bookings (type, item_id, customer_name, customer_email, date, quantity, total_price) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(booking.type);
    query.addBindValue(booking.itemId);
    query.addBindValue(booking.customerName);
    query.addBindValue(booking.customerEmail);
    query.addBindValue(booking.date);
    query.addBindValue(booking.quantity);
    query.addBindValue(booking.totalPrice);

    if (!query.exec()) {
        qDebug() << "Error adding booking:" << query.lastError().text();
        return false;
    }

    return true;
}

bool Database::updateHotel(const Hotel& hotel)
{
    QSqlQuery query;
    query.prepare("UPDATE hotels SET name=?, location=?, description=?, price=?, image_url=?, amenities=? WHERE id=?");
    query.addBindValue(hotel.name);
    query.addBindValue(hotel.location);
    query.addBindValue(hotel.description);
    query.addBindValue(hotel.price);
    query.addBindValue(hotel.imageUrl);
    query.addBindValue(hotel.amenities.join(","));
    query.addBindValue(hotel.id);

    return query.exec();
}

bool Database::deleteHotel(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM hotels WHERE id=?");
    query.addBindValue(id);
    return query.exec();
}

bool Database::updateBus(const Bus& bus)
{
    QSqlQuery query;
    query.prepare("UPDATE buses SET name=?, route=?, departure_time=?, duration=?, description=?, price=?, image_url=?, amenities=? WHERE id=?");
    query.addBindValue(bus.name);
    query.addBindValue(bus.route);
    query.addBindValue(bus.departureTime);
    query.addBindValue(bus.duration);
    query.addBindValue(bus.description);
    query.addBindValue(bus.price);
    query.addBindValue(bus.imageUrl);
    query.addBindValue(bus.amenities.join(","));
    query.addBindValue(bus.id);

    return query.exec();
}

bool Database::deleteBus(int id)
{