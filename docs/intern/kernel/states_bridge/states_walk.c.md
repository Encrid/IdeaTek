# src/kernel/states_bridge/states_walk.c

This file walks the query of a system, and reads the cells of a row.

## The walk

Bridge writes a span for each table that a query matches, at each barrier.
`walk_query` reads the spans of the query of a system, then the rows of each table. It calls the visitor with the id of each row.
The visitor reads the cells of the row with `cell_of`. `cell_of` gives the address of a component of the row, or a null pointer when the table lacks it.

```pseudo
PROCEDURE walk_query(api : U64, query : U32, visit : U64)
   q <- query_of(api, query)
   FOR s <- 0 TO q.spans - 1
      table <- span_of(api, q, s)
      ids <- block_of(api, table)
      FOR r <- 0 TO rows_of(api, table) - 1
         CALL visit(api, ids[r], table, r)
      NEXT r
   NEXT s
ENDPROCEDURE
```

`rows_of` reads the row count of the table in the directory of the shared memory, and `block_of` the ids of its rows.
The walk reads what the barrier wrote. A visitor that changes the world queues its changes for the next barrier, so the spans do not move.

## Functions

| Function | Result |
| --- | --- |
| `walk_query` | Calls the visitor with each row of each table that the query matches. |
| `cell_of` | Gives the cell of a slot in a row of a table, or a null pointer. |
