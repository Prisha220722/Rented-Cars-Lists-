/*
 * Name:
 * Student ID:
 * Date:
 * Assignment: CIS*2520 F26 Assignment 1 - Rental Car Linked Lists
 *
 * a1.c: main() and the menu loop of the car rental program.
 */

#include "a1.h"

/*
 * A basic menu based structure has been provided.
 */

int main(void) {

    /* Our three linked lists */
    struct car *available_head = NULL;
    struct car *rented_head = NULL;
    struct car *repair_head = NULL;

    int choice = 0;

    read_file_into_list(AVAILABLE_FILE, &available_head); 
    read_file_into_list(RENTED_FILE, &rented_head); 
    read_file_into_list(REPAIR_FILE, &repair_head); 

    /*
     * TODO: restore the three lists from AVAILABLE_FILE, RENTED_FILE and
     * REPAIR_FILE here, before the menu starts.
     */

    do {
        prompt();

        /*
         * Read the menu choice. If the input is not a number, discard the
         * rest of the line so the loop does not repeat forever. End of
         * input (EOF) is treated as quit.
         */
        if (scanf("%d", &choice) != 1) {
            if (feof(stdin)) {
                choice = 7;
            } else {
                choice = 0;
                while (getchar() != '\n' && !feof(stdin)) {
                }
            }
        }

        /*
         * You will need to add the code for each case:
         *  - Call the appropriate functions.
         *  - Check every input (plate, mileage, date) and print an error
         *    message when it is not valid.
         *  - Read the plate number into a buffer larger than 8 characters
         *    and limit the width, e.g.:  char buf[100]; scanf("%99s", buf);
         *    then check its length and characters yourself.
         *  - On quit (7): save the three lists to their files and free them.
         * File format: plate,mileage,return_date
         */
        switch (choice) {
        case 1: {
            char plate [100];  //temp place to store plate that the user would enter 
            int mileage; //creating a variable to put mileage in 
            bool valid_plate = true; 

            printf("Enter plate number (2-8 letters or digits): "); //asking the user for their plate number 
            scanf("%99s", plate); //reading what user put in and storing it in plate
           
            if (strlen(plate) < 2 || strlen(plate) > 8) { //is the plate number shorter than 2? is it longer than 8 - has restrictions 
                printf("Invalid plate number. \n"); //if it is out of boundaries then it would be invalid 
                break; 
            }

            for (int i = 0; i < strlen(plate); i++){ //starting a 0 and keep going until we've checked every character and until whole plate is checked 
                
                if (!isalnum(plate[i])) {//is the character a number or letter? if it's not then the plate is invalid and prints false 
                    valid_plate = false; 
                }
                plate[i] = toupper(plate[i]); //assignment says that "Plate numbers shall be stored and printed in upper case."
            }

            if (!valid_plate) {
                printf("Invalid plate number. \n"); 
                break; 
            }
            
            printf("Enter mileage: "); //asking user for mileage 
            
            if (scanf("%d", &mileage) != 1){
                printf("Invalid mileage. \n");
                
                while (getchar() != '\n' && !feof(stdin)){ //keep readings and throwing characters until reached end of line because user can enter letters 
                }
                break; 
            }

            if (mileage < 0) { // assignment states that the mileage should be 0 or greater 
                printf("Invalid mileage. \n"); 
                break;
            }

            if (is_plate_in_list(available_head, plate) || is_plate_in_list(rented_head, plate) || is_plate_in_list(repair_head, plate)) { //first check: is the plate already in available list? second check: is it already in rented list? third check: is it already in repair list? 
                printf("Error: plate %s is already in the system.\n", plate);
                break; 
            }

            struct car *new_car = insert_to_list(&available_head, plate, mileage, NO_RETURN_DATE); //adding the plate & mileage to available list, and return date is -1 

            if (new_car == NULL) {
                printf("Memory Allocation failed.\n");
                break; //if malloc fails inside then insert_to_list returns NULL 
            }
            sort_list(&available_head, true, false); //true? sort by mileage false? don't sort by return date 
            printf("Car %s (%d km) added to the available-for-rent list.\n", plate, mileage);
        }
            break;

        case 2: {
            char plate [100];
            int mileage;

            printf ("Enter plate number (2-8 letters or digits): ");
            scanf("%99s", plate);

            for (int i = 0; i < strlen(plate); i++) {
                plate[i] = toupper(plate[i]);
            }
            printf("Enter mileage: ");
            if (scanf("%d", &mileage) != 1) {
                printf("Invalid mileage.\n");
                
                while (getchar() != '\n' && !feof(stdin)) {
                }
                
                break; 
        }
            if (!is_plate_in_list(rented_head, plate)) {
                printf("Error: plate %s is not in the rented list.\n", plate);
                break; 
            }
            struct car *car = remove_car_from_list(&rented_head, plate); 
            if (mileage <= car->mileage) {
                printf("Invalid mileage: must be an integer larger than the previous mileage (%d).\n", car->mileage);
                insert_to_list(&rented_head, car->plate, car->mileage, car->return_date);
                free(car);
                break;
            }
        
            double charge = profit_calculator(car->mileage, mileage); 
            printf("Car %s moved from the rented list to the available-for-rent list.\n", car->plate);
            printf("Charge: $%.2f\n", charge);  

            car->mileage = mileage; 
            car->return_date = NO_RETURN_DATE; 

            insert_to_list(&available_head, car->plate, car->mileage, car->return_date); 

            free (car); 

            sort_list(&available_head, true, false); 
            
            break; 
        }
        case 3: {
            char plate[100]; 
            int mileage; 

            printf("Enter plate number (2-8 letters or digits): "); 
            scanf("%99s", plate); 

            for (int i = 0; i < strlen(plate); i++) {
                plate[i] = toupper(plate[i]); 
            }

            if(!is_plate_in_list(rented_head, plate)) {
                printf("Error: plate %s is not in the rented list.\n", plate); 
                break; 
            }
            struct car *car = remove_car_from_list(&rented_head , plate); 
            printf("Enter mileage: ");

            if(scanf("%d", &mileage) != 1) {
                printf("Invalid mileage.\n"); 

                while (getchar() != '\n' && !feof(stdin)) {
                }

                insert_to_list(&rented_head, car->plate, car->mileage, car->return_date);
                free (car); 
                break;
            }
            if (mileage <= car->mileage) { // if the mileage user entered is less than mileage the car had before before then it's invalid
                printf("Invalid mileage: must be an integer larger than the previous mileage (%d).\n", car->mileage);
                insert_to_list(&rented_head, car->plate, car->mileage, car->return_date); 

                free (car); 
                break; 
            }
            double charge = profit_calculator(car->mileage, mileage); 
            printf("Car %s moved from the rented list to the repair list .\n", car->plate); 
            printf("Charge: $%.2f\n", charge);
            car->mileage = mileage;
            car->return_date = NO_RETURN_DATE;

            insert_to_list(&repair_head, car->plate, car->mileage, car->return_date); 

            free(car); 

            break;
        }

        case 4: {
            char plate [100]; 

            printf("Enter plate number (2-8 letters or digits): ");
            scanf("%99s", plate); 

            for (int i = 0; i < strlen(plate); i++) {
                plate[i] = toupper(plate[i]); 
            }

            if(!is_plate_in_list(repair_head, plate)) {
                printf("Error: plate %s is not in the repair list.\n", plate);
                break; 
            }
            struct car *car = remove_car_from_list(&repair_head, plate); 
            car->return_date = NO_RETURN_DATE; 

            insert_to_list(&available_head, car->plate, car->mileage, car->return_date); 
            printf("Car %s moved from the repair list to the available-for-rent list.\n", car->plate);
            free(car); 

            sort_list(&available_head, true, false);

            break;
        }

        case 5: {
            if (available_head == NULL) {
                printf("No cars are available.\n");
                break; 
            }
            int return_date;
            printf("Enter expected return date (yymmdd): ");
            if(scanf("%d", &return_date) != 1) {
                printf("Invalid date: enter six digits yymmdd (month 01-12, day 01-31).\n"); 
                
                while (getchar() != '\n' && !feof(stdin)) {
                }
                break;
        }
        int year = return_date / 10000; 
        int month = (return_date / 100) % 100; 
        int day = return_date % 100; 

        if (return_date < 100000 || return_date > 999999 || month < 1 || month > 12 || day < 1 || day > 31) {
            printf("Invalid date: enter six digits yymmdd (month 01-12, day 01-31).\n"); 
            break;
        }
        struct car *car = remove_first_from_list(&available_head); 
        car->return_date = return_date;
        printf("Car %s moved from the available-for-rent list to the rented list. Expected return date: %02d/%02d/%02d\n", car->plate, year, month, day);
        insert_to_list(&rented_head, car->plate, car->mileage, car->return_date);
        free(car); 
        sort_list(&rented_head, false, true);
        break; 

    }

        case 6: {
            printf("\nAvailable-for-rent list:\n"); 
            print_list(available_head); 

            printf("\nRented list:\n"); 
            print_list(rented_head); 

            printf("\nIn-repair list:\n"); 
            print_list(repair_head); 

            break; 
        }

        case 7: {
            write_list_to_file(AVAILABLE_FILE, available_head); 
            write_list_to_file(RENTED_FILE, rented_head); 
            write_list_to_file(REPAIR_FILE, repair_head); 

            free_list(&available_head);
            free_list(&rented_head);
            free_list(&repair_head);
            
            printf("Lists saved to available.txt, rented.txt and repair.txt. Goodbye!\n");
            
            break;
        }
        default:
            printf("Invalid choice: enter a number from 1 to 7.\n"); 
            break;
        }

    } while (choice != 7);

    return 0;
}
