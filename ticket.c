#include <stdio.h>
#include <stdlib.h>

#define MAX_SEATS 50

struct Ticket {
    int ticketID;
    char name[50];
    int age;
    int seatNo;
};

int generateTicketID() {
    static int id = 1000;
    return id++;
}

int isSeatBooked(int seatNo) {
    FILE *fp = fopen("tickets.dat", "rb");
    struct Ticket t;

    if (fp == NULL) return 0;

    while (fread(&t, sizeof(t), 1, fp)) {
        if (t.seatNo == seatNo) {
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

void bookTicket() {
    struct Ticket t;
    FILE *fp = fopen("tickets.dat", "ab");

    if (fp == NULL) {
        printf("File error!\n");
        return;
    }

    t.ticketID = generateTicketID();
    printf("\nGenerated Ticket ID: %d\n", t.ticketID);

    printf("Enter Name: ");
    scanf(" %[^\n]", t.name);

    do {
        printf("Enter Age: ");
        scanf("%d", &t.age);
    } while (t.age <= 0);

    do {
        printf("Enter Seat Number (1-%d): ", MAX_SEATS);
        scanf("%d", &t.seatNo);

        if (t.seatNo < 1 || t.seatNo > MAX_SEATS)
            printf("Invalid seat number!\n");
        else if (isSeatBooked(t.seatNo))
            printf("Seat already booked! Choose another.\n");

    } while (t.seatNo < 1 || t.seatNo > MAX_SEATS || isSeatBooked(t.seatNo));

    fwrite(&t, sizeof(t), 1, fp);
    fclose(fp);

    printf("Ticket booked successfully!\n");
}

void viewTickets() {
    struct Ticket t;
    FILE *fp = fopen("tickets.dat", "rb");

    if (fp == NULL) {
        printf("\nNo tickets found.\n");
        return;
    }

    printf("\n--- Booked Tickets ---\n");

    while (fread(&t, sizeof(t), 1, fp)) {
        printf("\nID: %d | Name: %s | Age: %d | Seat: %d",
               t.ticketID, t.name, t.age, t.seatNo);
    }

    fclose(fp);
}

void searchTicket() {
    int id, found = 0;
    struct Ticket t;
    FILE *fp = fopen("tickets.dat", "rb");

    if (fp == NULL) {
        printf("No records found.\n");
        return;
    }

    printf("Enter Ticket ID to search: ");
    scanf("%d", &id);

    while (fread(&t, sizeof(t), 1, fp)) {
        if (t.ticketID == id) {
            printf("\nFound: %s | Age: %d | Seat: %d\n",
                   t.name, t.age, t.seatNo);
            found = 1;
            break;
        }
    }

    if (!found) printf("Ticket not found.\n");

    fclose(fp);
}

void cancelTicket() {
    FILE *fp = fopen("tickets.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");

    struct Ticket t;
    int id, found = 0;

    if (fp == NULL) {
        printf("No tickets to cancel.\n");
        return;
    }

    printf("Enter Ticket ID to cancel: ");
    scanf("%d", &id);

    while (fread(&t, sizeof(t), 1, fp)) {
        if (t.ticketID == id) {
            found = 1;
        } else {
            fwrite(&t, sizeof(t), 1, temp);
        }
    }

    fclose(fp);
    fclose(temp);

    remove("tickets.dat");
    rename("temp.dat", "tickets.dat");

    if (found)
        printf("Ticket cancelled successfully!\n");
    else
        printf("Ticket not found!\n");
}

void updateTicket() {
    FILE *fp = fopen("tickets.dat", "rb+");
    struct Ticket t;
    int id, found = 0;

    if (fp == NULL) {
        printf("No records found.\n");
        return;
    }

    printf("Enter Ticket ID to update: ");
    scanf("%d", &id);

    while (fread(&t, sizeof(t), 1, fp)) {
        if (t.ticketID == id) {
            printf("Enter new name: ");
            scanf(" %[^\n]", t.name);

            printf("Enter new age: ");
            scanf("%d", &t.age);

            fseek(fp, -sizeof(t), SEEK_CUR);
            fwrite(&t, sizeof(t), 1, fp);

            found = 1;
            break;
        }
    }

    if (found)
        printf("Updated successfully!\n");
    else
        printf("Ticket not found!\n");

    fclose(fp);
}

int main() {
    int choice;

    while (1) {
        printf("\n===== SMART TICKET SYSTEM =====\n");
        printf("1. Book Ticket\n");
        printf("2. View Tickets\n");
        printf("3. Search Ticket\n");
        printf("4. Update Ticket\n");
        printf("5. Cancel Ticket\n");
        printf("6. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: bookTicket(); break;
            case 2: viewTickets(); break;
            case 3: searchTicket(); break;
            case 4: updateTicket(); break;
            case 5: cancelTicket(); break;
            case 6: exit(0);
            default: printf("Invalid choice!\n");
        }
    }
}