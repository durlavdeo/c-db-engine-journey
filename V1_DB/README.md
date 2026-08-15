# Tiny DB

A small database storage engine built **from scratch in C**.

This project is not intended to compete with PostgreSQL, SQLite, or other production databases. The goal is to understand what actually happens underneath the abstractions of a database — how records are organized, how pages are managed, how free space is tracked, and how data is ultimately written to disk.

It is the result of putting together the C concepts, data structures, memory management, bitwise operations, and file I/O concepts I've been learning into one practical system.

---

## What This Database Does

Tiny DB currently supports:

* Fixed-size records
* Fixed-size pages
* Binary file storage
* Page-based storage
* Slot-based record management
* Bitmap-based free-space tracking
* Multi-page storage
* Record insertion
* Record reading
* Record updating
* Record deletion
* Deleted-slot reuse
* Random access to records
* Persistent file metadata
* Tests for CRUD, multi-page storage, and slot reuse

The current implementation uses a **32-bit bitmap for pages** and a bitmap inside each page for its slots, giving the initial implementation a maximum of 32 pages.

---

## The Storage Model

The database file is organized into pages rather than treating the file as one giant collection of records.

Conceptually:

```text
Database File
│
├── File Header
│   ├── Page Size
│   ├── Record Size
│   ├── Page Count
│   ├── Page Capacity
│   └── Page Bitmap
│
├── Page 0
│   ├── Page Header
│   │   ├── Record Count
│   │   └── Slot Bitmap
│   │
│   └── Record Slots
│
├── Page 1
│   ├── Page Header
│   ├── Slot Bitmap
│   └── Record Slots
│
└── ...
```

Each page contains a fixed number of fixed-size record slots.

The database can therefore calculate where a record physically belongs from its `recordId`:

```text
page number = recordId / page capacity
slot number = recordId % page capacity
```

---

## Bitmap-Based Slot Management

One of the most interesting parts of the project is the use of **bitmaps** to track whether pages and record slots are free or occupied.

For example:

```text
00000001
```

can represent:

```text
Slot 0 → occupied
Slot 1 → free
Slot 2 → free
Slot 3 → free
...
```

Instead of storing an entire integer or boolean for every slot, a single bit represents the state of a slot.

Bitwise operations such as:

* `AND`
* `OR`
* `XOR`
* bit shifting
* bitwise negation

are used to manipulate individual bits.

This also introduced me to fixed-width integer types such as:

```c
uint8_t
uint16_t
uint32_t
```

which are particularly useful when building low-level systems where the exact size of the data matters.

---

## Two Levels of Free-Space Tracking

Tiny DB uses two levels of bitmaps.

### Page bitmap

The file header contains a bitmap describing which pages are full.

```text
Page bitmap:

0 → page has free space
1 → page is full
```

This allows the database to quickly locate a page that can accept another record.

### Slot bitmap

Each page contains its own bitmap describing which record slots are occupied.

```text
Slot bitmap:

0 → slot is free
1 → slot is occupied
```

So inserting a record becomes roughly:

```text
Find page with free space
        ↓
Read page
        ↓
Find free slot
        ↓
Write record into slot
        ↓
Mark slot occupied
        ↓
Update page metadata
```

---

# Abstraction Layers

One of the main goals of the project was to understand how the different parts of a database can be separated into layers.

The user-facing API is intentionally small.

The user interacts primarily with:

```c
insert_record()
read_record()
update_record()
delete_record()
```

The record layer handles the high-level operation and delegates the lower-level work.

Conceptually:

```text
                 USER
                  │
                  ▼
          ┌─────────────────┐
          │  Record Layer   │
          │                 │
          │ insert_record() │
          │ read_record()   │
          │ update_record() │
          │ delete_record() │
          └────────┬────────┘
                   │
                   ▼
          ┌─────────────────┐
          │   Page Layer    │
          │                 │
          │ create_page()   │
          │ read_page()     │
          │ write_page()    │
          │ append_page()   │
          └────────┬────────┘
                   │
                   ▼
          ┌─────────────────┐
          │ Storage Layer  │
          │                 │
          │ File I/O       │
          │ fseek()        │
          │ fread()        │
          │ fwrite()       │
          └────────┬────────┘
                   │
                   ▼
               DISK FILE
```

The idea is that a user shouldn't need to know how a page is laid out or where bytes are located on disk just to insert a record.

For example:

```c
insert_record(&db, &student);
```

may internally perform:

```text
Record operation
      ↓
Find page
      ↓
Read page
      ↓
Find free slot
      ↓
Modify page
      ↓
Write page
      ↓
Persist metadata
      ↓
Disk
```

This separation makes the system easier to reason about and gives each layer a specific responsibility.

---

# Delete Without Shifting Records

One of the early questions that motivated this project was:

> What does deleting a record actually mean at the byte level?

A naive implementation could physically shift every record after the deleted record.

That works for a simple file, but becomes increasingly expensive as the amount of data grows.

Tiny DB instead marks the slot as free using the bitmap.

For example:

```text
Before:

111111

Delete slot 2:

110111
```

The existing records don't move.

The freed slot can later be reused by another insertion.

This means normal CRUD operations do **not** require shifting records or truncating the file.

---

# Record IDs

The current implementation uses a physical record location as the `recordId`.

For example, with a page capacity of 6:

```text
recordId 0 → Page 0, Slot 0
recordId 1 → Page 0, Slot 1
...
recordId 5 → Page 0, Slot 5

recordId 6 → Page 1, Slot 0
recordId 7 → Page 1, Slot 1
...
```

This is intentionally simple.

It also means that a deleted slot can later be reused:

```text
Alice → recordId 0

delete Alice

David → recordId 0
```

Therefore the current `recordId` represents a **physical location**, not a permanent identity.

A future version could introduce stable logical IDs or an indirection layer.

---

# Project Structure

```text
Tiny-DB/
│
├── include/
│   ├── db.h
│   ├── db_types.h
│   ├── record.h
│   ├── page.h
│   ├── bitmap.h
│   └── storage.h
│
├── src/
│   ├── db.c
│   ├── record.c
│   ├── page.c
│   ├── bitmap.c
│   └── storage.c
│
└── tests/
    ├── test_crud.c
    ├── test_multipage.c
    └── test_delete_reuse.c
```

### `record`

Contains the high-level CRUD operations exposed to the user.

### `page`

Handles page creation, reading, writing, and page-level metadata.

### `bitmap`

Contains the bitwise operations used for page and slot management.

### `storage`

Handles the low-level interaction with the database file.

### `db`

Handles database initialization and the overall database state.

### `tests`

Contains experiments and tests used to validate the different parts of the storage engine.

---

# What I Learned Building This

This project started as a way to learn C more deeply and gradually turned into a practical exercise in systems programming.

Along the way, I worked with:

* C fundamentals
* Pointers
* Memory management
* `malloc()` / `realloc()`
* `memcpy()` / `memmove()`
* Structs
* Data structures
* File I/O
* Binary files
* `fseek()`
* `fread()` / `fwrite()`
* Random file access
* Fixed-size records
* Fixed-width integer types
* Bitwise operators
* Bit masks
* Bitmaps
* Page layouts
* Metadata
* Free-space management
* Multi-page storage
* CRUD operations
* Modular C projects
* Header/source separation
* Debugging low-level storage logic

More importantly, it forced me to think about questions that are easy to ignore when using a database:

* Where exactly does a record live?
* How do we find it?
* How does the database know which space is available?
* What does deletion actually do?
* How can deleted space be reused?
* How does a file grow?
* How do pages and records relate to each other?
* How do high-level database operations eventually become disk operations?

---

# Current Limitations

This is intentionally a small learning project.

Current limitations include:

* Maximum of 32 pages
* Fixed-size records
* One record type per database
* Physical record IDs
* No indexing
* No query language
* No transactions
* No concurrency
* No crash recovery
* No WAL
* No buffer pool
* No variable-length records
* No B-Tree / hash indexes
* No sophisticated free-space management

These aren't bugs I'm trying to hide — they're boundaries of the current implementation and opportunities for future iterations.

---

# What's Next?

The project will continue evolving as I learn more about database internals.

Potential next steps include:

```text
Current
   │
   ├── Page-based storage
   ├── Bitmap free-space management
   └── CRUD
        │
        ▼
   Stable record IDs
        │
        ▼
   Searching / indexing
        │
        ▼
   B-Tree / B+Tree
        │
        ▼
   Buffer pool
        │
        ▼
   Variable-length records
        │
        ▼
   Transactions
        │
        ▼
   WAL / crash recovery
```

The goal isn't simply to end up with a "database."

The goal is to understand **why databases are designed the way they are** by building the pieces myself.

---

## Why This Project Exists

Tiny DB is essentially a collection of everything I've been learning about C and low-level programming, brought together into one system.

Instead of learning pointers, memory, structs, file I/O, bitwise operations, and data structures as isolated concepts, this project gives each of them a reason to exist.

Every abstraction exists because something underneath it needs to work.

And every bug is another opportunity to understand what is actually happening at the byte level.

**One concept at a time.
One bug at a time.
One layer at a time.**

Eventually, a tiny database engine.
