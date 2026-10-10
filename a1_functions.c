/*
 * Name:
 * Student ID:
 * Date:
 * Assignment: CIS*2520 F26 Assignment 1 - Rental Car Linked Lists
 *
 * a1_functions.c: the linked list functions used by main() in a1.c.
 * Do NOT change the names, parameters or return types of the functions below.
 * You may add helper functions. A helper called by any function in this file
 * must be defined (and declared, if needed) in this file, not in a1.c or a1.h.
 * Note: strcasecmp() is not part of C99. To compare plates without regard to
 * case, compare them character by character with toupper() (from ctype.h).
 */

#include "a1.h"


/* Displays the primary user interface. This is already done for you. */
void prompt(void) {

  printf("1. Add a new car to the available-for-rent list,\n");
  printf("2. Add a returned car to the available-for-rent list,\n");
  printf("3. Add a returned car to the repair list,\n");
  printf("4. Transfer a car from the repair list to the available-for-rent list,\n");
  printf("5. Rent the first available car,\n");
  printf("6. Print all the lists,\n");
  printf("7. Quit.\n");
  printf("Enter your choice: ");
}

/**
 * Adds a new car at the END of the list. It does not sort the list and does
 * not check for duplicate plates (the caller does that).
 * @param head Pointer to the head of the list (*head is NULL for an empty list).
 * @param plate Character array containing the car's plate number (copied into the new node).
 * @param mileage Integer representing the car's mileage.
 * @param return_date Integer in yymmdd format, or NO_RETURN_DATE (-1) if the car has none.
 * @return Pointer to the newly added car, or NULL if memory could not be allocated.
 */
struct car * insert_to_list(struct car ** head, char plate[], int mileage, int return_date) {
    struct car *new_car = malloc(sizeof(struct car)); 
    if (new_car == NULL) {
        return NULL;
    }
    strcpy(new_car->plate, plate); 
    new_car->mileage = mileage; 
    new_car->return_date = return_date;
    new_car->next = NULL;

    if (*head == NULL) {
        *head = new_car; 
    } else {
        struct car *current = *head; 
        while (current->next != NULL) {
            current = current->next; 
        }
        current->next = new_car; 
    }
    return new_car;
}
    

/**
 * Prints all the cars in the list, one car per line, in this format:
 *     Plate: ABC123, Mileage: 10000
 *     Plate: JKL890, Mileage: 98000, Return date: 26/10/05
 * The return date (printed with date()) is shown only if the car has one.
 * An empty list prints the single line:  (empty)
 * @param head Pointer to the head of the list.
 */
void print_list(struct car *head) {
    if (head == NULL) {
        printf("(empty)\n");
        return; 
    }
    struct car *current = head; 

    while (current != NULL) {
        printf("Plate: %s, Mileage: %d", current->plate, current->mileage);
            
            if (current->return_date != NO_RETURN_DATE) {
                printf(",Return date: "); 
                date(current->return_date);
            }
            printf("\n");

            current = current->next; 
    }
    return;
}

/**
 * Checks if a plate is already in the list. The comparison ignores case:
 * "abc123" and "ABC123" are the same plate.
 * @param head Pointer to the head of the list.
 * @param plate Character array containing the plate number to look for.
 * @return true if the plate is found, false otherwise (including an empty list).
 */
bool is_plate_in_list(struct car * head, char plate[]) {
    struct car *current = head; //we want to check if a specific plate exists and head points to first car 
    while (current != NULL) { //keeps checking until end is reached 
        if (strlen(current->plate) == strlen(plate)) { //checking if 2 plates have the same number of characters 
        bool same = true; //going to assume that they match 

        for (int i = 0; i < strlen(plate); i++) { //goes throuhg plate by character 
            if (toupper(current->plate[i]) != toupper(plate[i])) { //converts both characters to uppercase before comparing 
                same = false; 
                break; //if the character doesn't match then loop stops 
            }
        }
        if (same) {
            return true; //if every character matches then the plate exists in the list 
        }
    }
    current = current->next; //moves forward to the next car 
    }
    return false; //checked everything and found nothing 
}

/**
 * Swaps the data of two cars in the list.
 * @param a Pointer to the first car.
 * @param b Pointer to the second car.
 * Swaps the plate, mileage and return date of the two cars. The next
 * pointers are not changed.
 */
void swap(struct car *a, struct car *b) {
    char temp_plate[MAX_PLATE_LEN + 1];
    int temp_mileage; 
    int temp_return_date; 

    strcpy(temp_plate, a->plate);
    strcpy(a->plate, b->plate);
    strcpy(b->plate, temp_plate);

    temp_mileage = a->mileage; 
    a->mileage = b->mileage; 
    b->mileage = temp_mileage;

    temp_return_date = a->return_date; 
    a->return_date = b->return_date; 
    b->return_date = temp_return_date;
}

/**
 * Sorts the list in ascending order.
 * @param head Pointer to the head of the list.
 * @param sort_by_mileage If true, sort by mileage (least miles first).
 * @param sort_by_return_date If true (and sort_by_mileage is false), sort by
 *        return date (earliest first).
 * If both are false the list is not changed. Cars with an equal key keep
 * their current order. An empty list or a list with one car is fine.
 */
void sort_list(struct car ** head, bool sort_by_mileage, bool sort_by_return_date) {
    if (*head == NULL || (*head)->next == NULL) { //if there are no cars or there is one car then there is nothing to sort 
        //head->next that means that if there is no next car and the only car is head 
        return; 
    }
    struct car *current = *head; 
    while (current->next != NULL) {
        struct car *next_car = current->next; //we're comparing those cars 

        while (next_car != NULL) {
            if (sort_by_mileage && current->mileage > next_car->mileage) { //if the current one has more mileage that we swap their data using the swap() function we just made
                swap(current,current->next);
            }
            else if (sort_by_return_date && current->return_date > next_car->return_date){
                swap(current, current->next);
            }
            next_car = next_car->next; 
        }
        current = current->next; 
    }
    
}

/**
 * Removes a car from the list by its plate number (case is ignored).
 * The car is unlinked from the list but NOT freed: the caller becomes the
 * owner of the returned node and must free it when done.
 * @param head Pointer to the head of the list.
 * @param plate Character array containing the plate number.
 * @return Pointer to the removed car, or NULL if the plate is not in the list.
 */
struct car * remove_car_from_list(struct car **head, char plate[]) {
    struct car *current = *head; //current is the car we are at 
    struct car *previous = NULL; //previous keeps track of the car before current 

    while (current != NULL) {
        bool same = true;
        
        if (strlen(current->plate) != strlen(plate)) {
            same = false;
        } else {
            for (int i = 0; i < strlen(plate); i++){ 
                if (toupper(current->plate[i]) != toupper(plate[i])) {
                    same = false; 
                    break; 
                }
            }
        }
        if (same) {
            if (previous == NULL) {
                *head = current->next; 
            } else {
                previous->next = current->next;
            }
            current->next = NULL; 
            return current; 
        }
        previous = current; 
        current = current->next;
    }
    return NULL; 
}

/**
 * Removes the first car from the list. The car is unlinked but NOT freed:
 * the caller must free it.
 * @param head Pointer to the head of the list.
 * @return Pointer to the removed car, or NULL if the list is empty.
 */
struct car * remove_first_from_list(struct car **head) {
    if (*head == NULL) {
        return NULL;
    }
        struct car *removed = *head; 
        *head = (*head)->next; 
        removed->next = NULL; 
        return removed; 

    }


/**
 * Calculates the rental charge for a returned car.
 * The distance driven is final_mileage - initial_mileage (in km).
 * The charge is $80.00 for up to 200 km, plus $0.15 for each km beyond 200.
 * (The function name says "profit"; it returns the charge for the customer.)
 * @param initial_mileage Integer representing the car's mileage before the rental.
 * @param final_mileage Integer representing the car's mileage when it is returned.
 * @return Double value representing the charge in dollars.
 */
double profit_calculator(int initial_mileage, int final_mileage) {
    int distance = final_mileage - initial_mileage; 

    if (distance <= 200) {
        return 80.00; 
    }

    return 80.00 + (distance - 200) *0.15; 
}

/**
 * Writes the list of cars to a text file, one car per line, in the format:
 *     plate,mileage,return_date
 * The return date is written as six digits (e.g. 050101) or as -1 if the car
 * has none. An existing file is overwritten. An empty list gives an empty file.
 * @param filename Character array with the file name.
 * @param head Pointer to the head of the list.
 */
void write_list_to_file(char *filename, struct car *head) {
    FILE *file = fopen(filename, "w");

    if (file == NULL) {
        return;
    }
    struct car *current = head; 
    while (current != NULL) {
        fprintf(file, "%s, %d, %d\n", current->plate, current->mileage, current->return_date);
        current = current->next; 
    }
    fclose(file);
}

/**
 * Reads cars from a text file (format: plate,mileage,return_date) and adds
 * each one at the END of the list, in the order they appear in the file (no
 * sorting). If the file cannot be opened the list is left unchanged.
 * @param filename Character array with the file name.
 * @param head Pointer to the head of the list.
 */
void read_file_into_list(char *filename, struct car **head) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return;
}
    char plate [MAX_PLATE_LEN + 1]; 
    int mileage; 
    int return_date; 

    while (fscanf(file, " %8[^,],%d,%d", plate, &mileage, &return_date) == 3) {
        insert_to_list(head, plate, mileage, return_date);
    }

    fclose(file);
}

/**
 * Prints a date in the format yy/mm/dd, without a newline. For example,
 * 261005 prints 26/10/05 and 050101 prints 05/01/01.
 * @param yymmdd Integer representing the date in yymmdd format.
 */
void date(int yymmdd) {
    int year = yymmdd / 10000; 
    int month = (yymmdd /100) % 100; 
    int day = yymmdd % 100; 

    printf("%02d/%02d/%02d", year, month,day);
}

/**
 * Frees all the memory allocated for the cars in the list and sets *head
 * to NULL.
 * @param head Pointer to the head of the list.
 */
void free_list(struct car ** head) {
    struct car *current = (*head); 

    while (current != NULL) {
        struct car *next = current->next; 
        free(current);

        current = next; 
    }
    *head = NULL; 
}
