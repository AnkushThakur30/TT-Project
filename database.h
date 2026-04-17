#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>
#include <QVector>

struct Bus {
    int id;
    QString name;
    QString route;
    QString departureTime;
    QString duration;
    QString description;
    double price;
    QString imageUrl;
    QStringList amenities;
};

struct Booking {
    int id;
    QString type; // "bus"
    int itemId;
    QString customerName;
    QString customerEmail;
    QString date;
    int quantity;
    double totalPrice;
};

class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = nullptr);
    ~Database();

    bool initialize();
    bool createTables();

    // Hotel operations
    QVector<Hotel> getHotels();
    bool addHotel(const Hotel& hotel);
    bool updateHotel(const Hotel& hotel);
    bool deleteHotel(int id);

    // Bus operations
    QVector<Bus> getBuses();
    bool addBus(const Bus& bus);
    bool updateBus(const Bus& bus);
    boo
    // Utility
    int getNextId(const QString& table);

private:
    QSqlDatabase m_db;
};

#endif // DATABASE_H