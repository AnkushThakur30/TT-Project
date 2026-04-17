# Hotel & Bus Booking Management System

## Overview
This is a comprehensive Hotel & Bus Booking Management System built with modern web technologies. The system allows users to browse, select, and book hotels and buses with detailed information including photos, prices, routes, and amenities.

## Features
- **Hotel Booking**: Browse luxury hotels with photos, prices, locations, and amenities
- **Bus Booking**: Book bus tickets with route information, departure times, and amenities
- **User-Friendly Interface**: Modern, responsive web design
- **Booking Management**: View all bookings with detailed information
- **Data Persistence**: Bookings stored in browser localStorage

## Technologies Used
- **Frontend**: HTML5, CSS3, JavaScript (ES6+)
- **Backend Simulation**: JavaScript (in production, this would be C++ with a web server)
- **Database Simulation**: Browser localStorage (in production, this would be a C++ database system)

## C++ Backend Concept
While this demo uses JavaScript for the frontend and data handling, the backend logic is designed to be easily ported to C++:

```cpp
// Example C++ classes (conceptual)
class Hotel {
    int id;
    std::string name;
    std::string location;
    double price;
    std::vector<std::string> amenities;
    // Methods for CRUD operations
};

class Bus {
    int id;
    std::string name;
    std::string route;
    std::string departureTime;
    double price;
    std::vector<std::string> amenities;
    // Methods for CRUD operations
};

class BookingManager {
    std::vector<Hotel> hotels;
    std::vector<Bus> buses;
    std::vector<Booking> bookings;
    // Database connection and operations
};
```

## Sample Data
The system includes sample hotels and buses with:
- High-quality photos from Unsplash
- Detailed descriptions and amenities
- Competitive pricing in INR
- Route information for buses

## How to Use
1. Open `index.html` in a modern web browser
2. Navigate using the menu buttons
3. Browse hotels or buses
4. Click "Book Now" on any item
5. Fill out the booking form
6. View your bookings in the "My Bookings" section

## Future Enhancements
- User authentication and accounts
- Payment processing integration
- Real-time availability checking
- Admin panel for managing hotels/buses
- Mobile app version
- C++ backend implementation with web API

## Running the Project
Simply open `index.html` in any modern web browser. No server required for this demo version.