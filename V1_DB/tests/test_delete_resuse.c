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
    printf(
        "%s: ID=%d | Name=%s | Age=%d\n",
        label,
        s->id,
        s->name,
        s->age
    );
}


int main()
{
    Database db;

    printf("========== DELETE + REUSE TEST ==========\n\n");

    // ============================================================
    // INITIALIZE
    // ============================================================

    if (init_db("students.db", &db, sizeof(Student)) != 0)
    {
        printf("Failed to initialize database!\n");
        return 1;
    }

    printf("Page capacity: %u records\n", db.header.pageCapacity);
    printf("Initial page count: %u\n\n", db.header.pageCount);


    // ============================================================
    // INSERT 10 RECORDS
    //
    // Page 0: A B C D E F
    // Page 1: G H I J
    // ============================================================

    printf("========== INSERT INITIAL RECORDS ==========\n");

    Student students[10] =
    {
        {1,  "Alice",   20},
        {2,  "Bob",     21},
        {3,  "Charlie", 22},
        {4,  "David",   23},
        {5,  "Eve",     24},
        {6,  "Frank",   25},
        {7,  "Grace",   26},
        {8,  "Henry",   27},
        {9,  "Ivy",     28},
        {10, "Jack",    29}
    };

    uint32_t ids[10];

    for (int i = 0; i < 10; i++)
    {
        ids[i] = insert_record(&db, &students[i]);

        printf(
            "Inserted %-8s -> recordId: %u\n",
            students[i].name,
            ids[i]
        );
    }

    printf("\nPage count: %u\n", db.header.pageCount);

    printf("Page bitmap: ");
    print_binary(db.header.pageBitmap);

    printf("\n");


    // ============================================================
    // DELETE FROM BOTH PAGES
    //
    // Delete:
    // C → Page 0 Slot 2
    // G → Page 1 Slot 0
    // J → Page 1 Slot 3
    // ============================================================

    printf("========== DELETE RECORDS ==========\n");

    printf("Deleting Charlie (recordId=%u)...\n", ids[2]);

    if (delete_record(&db, ids[2]) != 0)
    {
        printf("FAILED to delete Charlie!\n");
    }

    printf("Deleting Grace (recordId=%u)...\n", ids[6]);

    if (delete_record(&db, ids[6]) != 0)
    {
        printf("FAILED to delete Grace!\n");
    }

    printf("Deleting Jack (recordId=%u)...\n", ids[9]);

    if (delete_record(&db, ids[9]) != 0)
    {
        printf("FAILED to delete Jack!\n");
    }

    printf("\n");

    printf("Page bitmap after deletion: ");
    print_binary(db.header.pageBitmap);

    printf("\n");


    // ============================================================
    // VERIFY DELETED RECORDS CANNOT BE READ
    // ============================================================

    printf("========== VERIFY DELETIONS ==========\n");

    Student buffer;

    uint32_t deletedIds[3] =
    {
        ids[2], // Charlie
        ids[6], // Grace
        ids[9]  // Jack
    };

    const char *deletedNames[3] =
    {
        "Charlie",
        "Grace",
        "Jack"
    };

    for (int i = 0; i < 3; i++)
    {
        if (read_record(&db, &buffer, deletedIds[i]) != 0)
        {
            printf(
                "Correct: %s (recordId=%u) cannot be read.\n",
                deletedNames[i],
                deletedIds[i]
            );
        }
        else
        {
            printf(
                "ERROR: deleted record %s can still be read!\n",
                deletedNames[i]
            );
        }
    }

    printf("\n");


    // ============================================================
    // INSERT NEW RECORDS
    //
    // Expected:
    //
    // K → Page 0 Slot 2
    // L → Page 1 Slot 0
    // M → Page 1 Slot 3
    // ============================================================

    printf("========== REUSE DELETED SLOTS ==========\n");

    Student replacements[3] =
    {
        {11, "Kevin",  30},
        {12, "Laura",  31},
        {13, "Mike",   32}
    };

    uint32_t newIds[3];

    for (int i = 0; i < 3; i++)
    {
        newIds[i] = insert_record(&db, &replacements[i]);

        printf(
            "Inserted %-8s -> recordId: %u\n",
            replacements[i].name,
            newIds[i]
        );
    }

    printf("\n");


    // ============================================================
    // VERIFY PHYSICAL SLOT REUSE
    // ============================================================

    printf("========== VERIFY SLOT REUSE ==========\n");

    if (newIds[0] == ids[2])
    {
        printf(
            "SUCCESS: Kevin reused Charlie's slot "
            "(recordId=%u).\n",
            newIds[0]
        );
    }
    else
    {
        printf(
            "ERROR: Kevin expected recordId=%u, got %u.\n",
            ids[2],
            newIds[0]
        );
    }


    if (newIds[1] == ids[6])
    {
        printf(
            "SUCCESS: Laura reused Grace's slot "
            "(recordId=%u).\n",
            newIds[1]
        );
    }
    else
    {
        printf(
            "ERROR: Laura expected recordId=%u, got %u.\n",
            ids[6],
            newIds[1]
        );
    }


    if (newIds[2] == ids[9])
    {
        printf(
            "SUCCESS: Mike reused Jack's slot "
            "(recordId=%u).\n",
            newIds[2]
        );
    }
    else
    {
        printf(
            "ERROR: Mike expected recordId=%u, got %u.\n",
            ids[9],
            newIds[2]
        );
    }

    printf("\n");


    // ============================================================
    // READ REUSED RECORDS
    // ============================================================

    printf("========== READ REUSED RECORDS ==========\n");

    for (int i = 0; i < 3; i++)
    {
        if (read_record(&db, &buffer, newIds[i]) == 0)
        {
            uint32_t pageNum =
                newIds[i] / db.header.pageCapacity;

            uint32_t slotNum =
                newIds[i] % db.header.pageCapacity;

            printf(
                "%s -> Page %u Slot %u\n",
                replacements[i].name,
                pageNum,
                slotNum
            );

            print_student("    Record", &buffer);
        }
        else
        {
            printf(
                "FAILED to read reused record %s!\n",
                replacements[i].name
            );
        }
    }

    printf("\n");


    // ============================================================
    // FINAL STATE
    // ============================================================

    printf("========== FINAL DATABASE STATE ==========\n");

    printf("Page count   : %u\n", db.header.pageCount);
    printf("Page capacity: %u\n", db.header.pageCapacity);

    printf("Page bitmap  : ");
    print_binary(db.header.pageBitmap);

    printf("\n");

    close_db(&db);

    printf("========== TEST COMPLETE ==========\n");

    return 0;
}