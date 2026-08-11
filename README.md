# C DB Engine Journey

So... this is the repo where im dumping all the C programs im writing while learning and practicing different C fundamentals and concepts.

This whole repo is basically my **practice timeline**.

I'm starting from the absolute basics: pointers, structs, memory management, data structures, file I/O, binary files, etc and slowly working my way toward something I've been wanting to build for a while.

## A custom DB engine in C.

**YES. A DB ENGINE. FROM SCRATCH. IN C.** 💀

Is it ambitious? Absolutely.

Do I fully know what I'm getting myself into?

**Not even close.** 😂

But that's kinda the point.

I'm not trying to build the next PostgreSQL or some production-ready database that people are going to deploy on millions of servers.

I'm doing this because I have this itch to actually understand **how this stuff works underneath the abstractions**.

Like...

* How does data actually get stored?
* What really happens when a database reads a record?
* How does it find a particular piece of data?
* How does a DB manage files and memory?
* How do indexes actually work?
* What are pages, buffers, records, and storage engines?
* What the hell is a B-Tree actually doing?
* And eventually... how does a database turn a query into actual operations?

Instead of just learning all of that theoretically, I want to **build my way into understanding it.**

So this repo is basically me figuring that shit out one concept at a time.

```text
C fundamentals
      ↓
Pointers
      ↓
Memory management
      ↓
Data structures
      ↓
File I/O
      ↓
Binary storage
      ↓
Storage layer
      ↓
Pages
      ↓
Indexes
      ↓
Query processing
      ↓
????
      ↓
MY OWN DB ENGINE
```

I have absolutely no idea how long this is going to take.

Maybe I'll get there.

Maybe I'll get destroyed halfway through by some completely insane systems-level concept. 💀

But we'll see.

**One concept at a time. One stupid bug at a time.**

Let's build a DB engine.
