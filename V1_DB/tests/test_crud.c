#include "db.h"
#include "record.h"

#include <stdio.h>
#include <stdint.h>

typedef struct
{
    int id;
    char name[32];
    int age;
} Student;


void print_student(const char *label, Student *s)
{
    printf("%s: ID=%d | Name=%s | Age=%d\n",
           label,
           s->id,
           s->name,
           s->age);
}


int main()
{
    Database db;

    printf("========== INITIALIZE DB ==========\n");

    if (init_db("students.db", &db, sizeof(Student)) != 0)
    {
        printf("Failed to initialize database!\n");
        return 1;
    }

    printf("Database initialized successfully.\n\n");


    // ============================================================
    // INSERT
    // ============================================================

    printf("========== INSERT TEST ==========\n");

    Student s1 = {1, "Alice", 20};
    Student s2 = {2, "Bob", 21};
    Student s3 = {3, "Charlie", 22};

    uint32_t id1 = insert_record(&db, &s1);
    uint32_t id2 = insert_record(&db, &s2);
    uint32_t id3 = insert_record(&db, &s3);

    printf("Inserted Alice -> recordId: %u\n", id1);
    printf("Inserted Bob   -> recordId: %u\n", id2);
    printf("Inserted Charlie -> recordId: %u\n", id3);

    printf("\n");


    // ============================================================
    // READ
    // ============================================================

    printf("========== READ TEST ==========\n");

    Student buffer;

    if (read_record(&db, &buffer, id1) == 0)
    {
        print_student("Read record 1", &buffer);
    }
    else
    {
        printf("Failed to read record %u\n", id1);
    }

    if (read_record(&db, &buffer, id2) == 0)
    {
        print_student("Read record 2", &buffer);
    }
    else
    {
        printf("Failed to read record %u\n", id2);
    }

    printf("\n");


    // ============================================================
    // UPDATE
    // ============================================================

    printf("========== UPDATE TEST ==========\n");

    Student updatedBob = {2, "Bob Updated", 25};

    if (update_record(&db, id2, &updatedBob) == 0)
    {
        printf("Bob updated successfully.\n");
    }
    else
    {
        printf("Failed to update Bob.\n");
    }

    if (read_record(&db, &buffer, id2) == 0)
    {
        print_student("After update", &buffer);
    }

    printf("\n");


    // ============================================================
    // DELETE
    // ============================================================

    printf("========== DELETE TEST ==========\n");

    if (delete_record(&db, id1) == 0)
    {
        printf("Alice deleted successfully.\n");
    }
    else
    {
        printf("Failed to delete Alice.\n");
    }

    // Reading deleted record should fail
    if (read_record(&db, &buffer, id1) != 0)
    {
        printf("Correct: deleted record %u cannot be read.\n", id1);
    }
    else
    {
        printf("ERROR: deleted record can still be read!\n");
    }

    printf("\n");


    // ============================================================
    // SLOT REUSE TEST
    // ============================================================

    printf("========== SLOT REUSE TEST ==========\n");

    Student s4 = {4, "David", 23};

    uint32_t id4 = insert_record(&db, &s4);

    printf("Inserted David -> recordId: %u\n", id4);

    if (id4 == id1)
    {
        printf("Correct: deleted slot was reused!\n");
    }
    else
    {
        printf("Deleted slot was NOT reused.\n");
        printf("Expected: %u | Got: %u\n", id1, id4);
    }

    printf("\n");


    // ============================================================
    // READ REUSED SLOT
    // ============================================================

    printf("========== READ REUSED SLOT ==========\n");

    if (read_record(&db, &buffer, id4) == 0)
    {
        print_student("Reused slot", &buffer);
    }
    else
    {
        printf("Failed to read David.\n");
    }

    printf("\n");


    // ============================================================
    // UPDATE DELETED RECORD TEST
    // ============================================================

    printf("========== INVALID OPERATION TEST ==========\n");

    Student fake = {99, "Fake", 99};

    if (update_record(&db, id1, &fake) != 0)
    {
        printf("Correct: cannot update deleted record %u.\n", id1);
    }
    else
    {
        printf("ERROR: updated deleted record!\n");
    }

    printf("\n");


    // ============================================================
    // INVALID RECORD ID TEST
    // ============================================================

    printf("========== INVALID RECORD ID TEST ==========\n");

    uint32_t invalidId = 9999;

    if (read_record(&db, &buffer, invalidId) != 0)
    {
        printf("Correct: invalid record ID rejected.\n");
    }
    else
    {
        printf("ERROR: invalid record ID was accepted!\n");
    }

    printf("\n");


    // ============================================================
    // CLOSE
    // ============================================================

    close_db(&db);

    printf("========== TEST COMPLETE ==========\n");

    return 0;
}