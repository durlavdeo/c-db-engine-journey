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

void print_binary(uint32_t bitmap)
{
    for (int i = 32; i >= 0; i--)
    {
        printf("%d", (bitmap >> i) & 1); // moves the ith bit all the way to 0th position and after & with 1 we get the exact bit it is;
        if (i % 4 == 0)
        {
            printf(" ");
        }
    }
    printf("\n");
}

int main()
{
    Database db;

    printf("========== MULTI-PAGE TEST ==========\n\n");

    // Fresh database
    if (init_db("students.db", &db, sizeof(Student)) != 0)
    {
        printf("Failed to initialize database!\n");
        return 1;
    }

    printf("Page capacity: %u records\n",
           db.header.pageCapacity);

    printf("Initial page count: %u\n",
           db.header.pageCount);

    printf("\n");

    // ============================================================
    // TEST 1: FILL PAGE 0
    // ============================================================

    printf("========== FILLING PAGE 0 ==========\n");

    Student students[10] =
        {
            {1, "Alice", 20},
            {2, "Bob", 21},
            {3, "Charlie", 22},
            {4, "David", 23},
            {5, "Eve", 24},
            {6, "Frank", 25},
            {7, "Grace", 26},
            {8, "Henry", 27},
            {9, "Ivy", 28},
            {10, "Jack", 29}};

    uint32_t ids[10];

    for (int i = 0; i < 10; i++)
    {
        ids[i] = insert_record(&db, &students[i]);

        printf(
            "Inserted %-8s -> recordId: %u\n",
            students[i].name,
            ids[i]);
    }

    printf("\n");

    printf("Page count after inserts: %u\n",
           db.header.pageCount);

    printf("Page bitmap: ");
    print_binary(db.header.pageBitmap);

    printf("\n");

    // ============================================================
    // TEST 2: READ RECORDS FROM DIFFERENT PAGES
    // ============================================================

    printf("========== READ ACROSS PAGES ==========\n");

    Student buffer;

    for (int i = 0; i < 10; i++)
    {
        if (read_record(&db, &buffer, ids[i]) == 0)
        {
            uint32_t pageNum =
                ids[i] / db.header.pageCapacity;

            uint32_t slotNum =
                ids[i] % db.header.pageCapacity;

            printf(
                "recordId=%u -> Page=%u Slot=%u -> ",
                ids[i],
                pageNum,
                slotNum);

            printf(
                "%s (%d)\n",
                buffer.name,
                buffer.age);
        }
        else
        {
            printf(
                "FAILED to read recordId=%u\n",
                ids[i]);
        }
    }

    printf("\n");

    // ============================================================
    // TEST 3: VERIFY PAGE BOUNDARY
    // ============================================================

    printf("========== PAGE BOUNDARY ==========\n");

    printf(
        "Record 0 -> Page %u, Slot %u\n",
        ids[0] / db.header.pageCapacity,
        ids[0] % db.header.pageCapacity);

    printf(
        "Record %u -> Page %u, Slot %u\n",
        ids[5],
        ids[5] / db.header.pageCapacity,
        ids[5] % db.header.pageCapacity);

    printf(
        "Record %u -> Page %u, Slot %u\n",
        ids[6],
        ids[6] / db.header.pageCapacity,
        ids[6] % db.header.pageCapacity);

    printf("\n");

    // ============================================================
    // TEST 4: DELETE FROM PAGE 0
    // ============================================================

    printf("========== DELETE FROM PAGE 0 ==========\n");

    if (delete_record(&db, ids[0]) == 0)
    {
        printf(
            "Deleted recordId=%u (%s)\n",
            ids[0],
            students[0].name);
    }

    printf("Page bitmap after deletion: ");
    print_binary(db.header.pageBitmap);

    printf("\n");

    // ============================================================
    // TEST 5: REUSE SLOT ON PAGE 0
    // ============================================================

    printf("========== REUSE PAGE 0 SLOT ==========\n");

    Student replacement =
        {
            11,
            "Replacement",
            30};

    uint32_t replacementId =
        insert_record(&db, &replacement);

    printf(
        "Inserted %s -> recordId=%u\n",
        replacement.name,
        replacementId);

    if (replacementId == ids[0])
    {
        printf(
            "SUCCESS: deleted slot on Page 0 was reused.\n");
    }
    else
    {
        printf(
            "WARNING: expected recordId=%u, got %u\n",
            ids[0],
            replacementId);
    }

    printf("\n");

    // ============================================================
    // FINAL STATE
    // ============================================================

    printf("========== FINAL DATABASE STATE ==========\n");

    printf(
        "Page count   : %u\n",
        db.header.pageCount);

    printf(
        "Page capacity: %u\n",
        db.header.pageCapacity);

    printf("Page bitmap  : ");
    print_binary(db.header.pageBitmap);

    printf("\n");

    close_db(&db);

    printf("========== TEST COMPLETE ==========\n");

    return 0;
}