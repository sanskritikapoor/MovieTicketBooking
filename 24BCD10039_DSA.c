#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_MOVIES 5
#define MAX_BOOKINGS 100
typedef struct {
    int id;
    char title[100];
    int totalSeats;
    int availableSeats;
} Movie;
typedef struct {
    int movieId;
    char movieTitle[100];
    int seatsBooked;
    int refNumber;  // Unique reference number
} Booking;
// Function declarations
void showMovies(Movie movies[], int n);
void bookTickets(Movie movies[], int n, Booking bookings[], int *bookingCount, int *refCounter);
void viewBookingHistory(Booking bookings[], int bookingCount);
void cancelTickets(Movie movies[], int n, Booking bookings[], int *bookingCount);
int main() {
    Movie movies[MAX_MOVIES] = {
        {1, "Avengers: Endgame", 100, 100},
        {2, "Where the Crawdads Sing", 80, 80},
        {3, "The Residence", 60, 60},
        {4, "The Devil Wears Prada", 50, 50},
        {5, "The Conjuring: Last Rites", 70, 70}
    };
    Booking bookings[MAX_BOOKINGS];
    int bookingCount = 0;
    int refCounter = 1000; // Start of reference number
    int choice;
    while (1) {
        printf("\n===== Movie Ticket Booking System =====\n");
        printf("1. View Movies\n");
        printf("2. Book Tickets\n");
        printf("3. View Booking History\n");
        printf("4. Cancel Tickets\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                showMovies(movies, MAX_MOVIES);
                break;
            case 2:
                bookTickets(movies, MAX_MOVIES, bookings, &bookingCount, &refCounter);
                break;
            case 3:
                viewBookingHistory(bookings, bookingCount);
                break;
            case 4:
                cancelTickets(movies, MAX_MOVIES, bookings, &bookingCount);
                break;
            case 5:
                printf("Thank you! Visit again.\n");
                exit(0);
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
    return 0;
}
// Show available movies
void showMovies(Movie movies[], int n) {
    printf("\nAvailable Movies:\n");
    for (int i = 0; i < n; i++) {
        printf("%d. %s (Available: %d/%d)\n",
               movies[i].id,
               movies[i].title,
               movies[i].availableSeats,
               movies[i].totalSeats);
    }
}
// Book tickets
void bookTickets(Movie movies[], int n, Booking bookings[], int *bookingCount, int *refCounter) {
    int movieId, seats;
    showMovies(movies, n);
    printf("\nEnter Movie ID to book: ");
    scanf("%d", &movieId);
    if (movieId < 1 || movieId > n) {
        printf("Invalid Movie ID.\n");
        return;
    }
    Movie *selected = &movies[movieId - 1];
    printf("Enter number of seats to book: ");
    scanf("%d", &seats);
    if (seats <= 0 || seats > selected->availableSeats) {
        printf("Invalid number of seats. Only %d available.\n", selected->availableSeats);
        return;
    }
    selected->availableSeats -= seats;
    Booking *newBooking = &bookings[*bookingCount];
    newBooking->movieId = selected->id;
    strcpy(newBooking->movieTitle, selected->title);
    newBooking->seatsBooked = seats;
    newBooking->refNumber = (*refCounter)++;  // Assign unique reference number
    (*bookingCount)++;
    printf("\n🎟️  Booking Successful!\n");
    printf("Movie: %s\n", selected->title);
    printf("Seats: %d\n", seats);
    printf("📌 Your Reference Number: #%d\n", newBooking->refNumber);
}
// View booking history
void viewBookingHistory(Booking bookings[], int bookingCount) {
    if (bookingCount == 0) {
        printf("\nNo bookings yet.\n");
        return;
    }
    printf("\nBooking History:\n");
    for (int i = 0; i < bookingCount; i++) {
        printf("%d. %s - %d seat(s) | Ref#: %d\n",
               i + 1,
               bookings[i].movieTitle,
               bookings[i].seatsBooked,
               bookings[i].refNumber);
    }
}
// Cancel tickets
void cancelTickets(Movie movies[], int n, Booking bookings[], int *bookingCount) {
    if (*bookingCount == 0) {
        printf("\nNo bookings to cancel.\n");
        return;
    }
    viewBookingHistory(bookings, *bookingCount);
    int cancelIndex;
    printf("\nEnter the booking number to cancel: ");
    scanf("%d", &cancelIndex);
    if (cancelIndex < 1 || cancelIndex > *bookingCount) {
        printf("Invalid booking number.\n");
        return;
    }
    Booking *cancelBooking = &bookings[cancelIndex - 1];
    // Restore seats to the movie
    for (int i = 0; i < n; i++) {
        if (movies[i].id == cancelBooking->movieId) {
            movies[i].availableSeats += cancelBooking->seatsBooked;
            break;
        }
    }
    printf("\n❌ Cancelled %d seat(s) for '%s'. (Ref#: %d)\n",
           cancelBooking->seatsBooked,
           cancelBooking->movieTitle,
           cancelBooking->refNumber);
    // Shift the bookings array
    for (int i = cancelIndex - 1; i < *bookingCount - 1; i++) {
        bookings[i] = bookings[i + 1];
    }
    (*bookingCount)--;
}
