// Bus Booking Management System JavaScript

// Sample data (in a real C++ backend, this would be stored in a database)
const buses = [
    {
        id: 1,
        name: "Express Line",
        route: "New York - Boston",
        departureTime: "08:00",
        duration: "4 hours",
        price: 4025,
        image: "https://images.unsplash.com/photo-1544620347-c4fd4a3d5957?w=400",
        description: "High-speed bus service between major cities",
        amenities: ["WiFi", "AC", "Entertainment", "Restroom"]
    },
    {
        id: 2,
        name: "Coastal Route",
        route: "Los Angeles - San Francisco",
        departureTime: "09:30",
        duration: "6 hours",
        price: 665,
        image: "https://images.unsplash.com/photo-1558618666-fcd25c85cd64?w=400",
        description: "Scenic coastal journey with ocean views",
        amenities: ["WiFi", "Snacks", "Charging Ports", "Restroom"]
    },
    {
        id: 3,
        name: "Mountain Express",
        route: "Denver - Aspen",
        departureTime: "07:00",
        duration: "3 hours",
        price: 1035,
        image: "https://images.unsplash.com/photo-1578662996442-48f60103fc96?w=400",
        description: "Mountain route with stunning views",
        amenities: ["Heating", "WiFi", "Storage", "Restroom"]
    },
    {
        id: 4,
        name: "Metro Link",
        route: "Washington DC - Philadelphia",
        departureTime: "10:00",
        duration: "2.5 hours",
        price: 2230,
        image: "https://images.unsplash.com/photo-1558618047-3c8c76ca7d13?w=400",
        description: "Comfortable city-to-city transportation",
        amenities: ["WiFi", "AC", "USB Charging", "Restroom"]
    }
];

// Load bookings from localStorage
let bookings = JSON.parse(localStorage.getItem('bookings')) || [];

function showSection(sectionName) {
    // Hide all sections
    document.querySelectorAll('.section').forEach(section => {
        section.classList.remove('active');
    });

    // Show selected section
    document.getElementById(sectionName + '-section').classList.add('active');

    // Load content for specific sections
    if (sectionName === 'buses') {
        displayBuses();
    } else if (sectionName === 'bookings') {
        displayBookings();
    }
}

function displayBuses() {
    const busesList = document.getElementById('buses-list');
    busesList.innerHTML = '';

    buses.forEach(bus => {
        const busCard = document.createElement('div');
        busCard.className = 'item-card';
        busCard.innerHTML = `
            <img src="${bus.image}" alt="${bus.name}" class="item-image">
            <div class="item-details">
                <h3>${bus.name}</h3>
                <p><strong>Route:</strong> ${bus.route}</p>
                <p><strong>Departure:</strong> ${bus.departureTime} (${bus.duration})</p>
                <p>${bus.description}</p>
                <p><strong>Amenities:</strong> ${bus.amenities.join(', ')}</p>
                <p class="price">₹${bus.price} per seat</p>
                <button class="book-btn" onclick="openBookingModal(${bus.id})">Book Now</button>
            </div>
        `;
        busesList.appendChild(busCard);
    });
}

function displayBookings() {
    const bookingsList = document.getElementById('bookings-list');
    bookingsList.innerHTML = '';

    if (bookings.length === 0) {
        bookingsList.innerHTML = '<p>No bookings found. Start booking buses now!</p>';
        return;
    }

    bookings.forEach(booking => {
        const bookingItem = document.createElement('div');
        bookingItem.className = 'booking-item';
        bookingItem.innerHTML = `
            <h3>Bus Booking</h3>
            <p><strong>Name:</strong> ${booking.name}</p>
            <p><strong>Email:</strong> ${booking.email}</p>
            <p><strong>Bus:</strong> ${getItemName(booking.itemId)}</p>
            <p><strong>Date:</strong> ${booking.date}</p>
            <p><strong>Seats:</strong> ${booking.guests}</p>
            <p><strong>Total Price:</strong> ₹${booking.totalPrice}</p>
            <p><strong>Booking ID:</strong> ${booking.id}</p>
        `;
        bookingsList.appendChild(bookingItem);
    });
}

function getItemName(id) {
    const bus = buses.find(b => b.id === id);
    return bus ? bus.name : 'Unknown';
}

function openBookingModal(itemId) {
    const modal = document.getElementById('booking-modal');
    const form = document.getElementById('booking-form');
    const title = document.getElementById('modal-title');

    document.getElementById('booking-item-id').value = itemId;

    const itemName = getItemName(itemId);
    title.textContent = `Book Bus: ${itemName}`;

    modal.style.display = 'block';

    form.onsubmit = function(e) {
        e.preventDefault();
        makeBooking();
    };
}

function closeModal() {
    document.getElementById('booking-modal').style.display = 'none';
    document.getElementById('booking-form').reset();
}

function makeBooking() {
    const itemId = parseInt(document.getElementById('booking-item-id').value);
    const name = document.getElementById('booking-name').value;
    const email = document.getElementById('booking-email').value;
    const date = document.getElementById('booking-date').value;
    const guests = parseInt(document.getElementById('booking-guests').value);

    // Get price from buses
    const bus = buses.find(b => b.id === itemId);
    const price = bus ? bus.price : 0;
    const totalPrice = price * guests;

    // Create booking
    const booking = {
        id: Date.now().toString(),
        itemId,
        name,
        email,
        date,
        guests,
        totalPrice,
        timestamp: new Date().toISOString()
    };

    bookings.push(booking);
    localStorage.setItem('bookings', JSON.stringify(bookings));

    alert(`Booking confirmed! Total: ₹${totalPrice}. Booking ID: ${booking.id}`);
    closeModal();
    displayBookings();
}

// Initialize the page
document.addEventListener('DOMContentLoaded', function() {
    showSection('home');
});

// Close modal when clicking outside
window.onclick = function(event) {
    const modal = document.getElementById('booking-modal');
    if (event.target === modal) {
        closeModal();
    }
};