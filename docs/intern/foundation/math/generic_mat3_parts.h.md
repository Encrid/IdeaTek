# src/foundation/math/generic_mat3_parts.h

This generic text takes a matrix of 3 by 3 apart and puts it together: its columns, its transpose, and its 9 plain numbers.

## The generic names

| Generic name | In 32 bits | In 64 bits |
| --- | --- | --- |
| `mat3_t` | `mat3f_t` | `mat3d_t` |
| `mat3_get_column` | `mat3f_get_column` | `mat3d_get_column` |
| `mat3_make_columns` | `mat3f_make_columns` | `mat3d_make_columns` |
| `mat3_get_transpose` | `mat3f_get_transpose` | `mat3d_get_transpose` |
| `mat3_load` | `mat3f_load` | `mat3d_load` |
| `mat3_store` | `mat3f_store` | `mat3d_store` |

## The columns

A basis holds 1 axis in each column. Thus the other generic texts of a matrix work on columns as vectors of 3.
`mat3_get_column` gives 1 column, and `mat3_make_columns` makes a matrix from 3 columns.

## The plain numbers

`mat3_load` and `mat3_store` move the 9 numbers column by column: the number of row `r` and column `c` is at the index `c * 3 + r`.
That is the order of a basis in a contract of IdeaTek. The functions copy number by number, and write 9 numbers only.

The style refuses the builtins of the compiler for a transpose and a load, because rule A4 does not name them. So the 3 functions are loops.

## Functions

| Function | Result |
| --- | --- |
| `mat3_get_column` | Gives 1 column as a vector of 3. |
| `mat3_make_columns` | Gives the matrix of 3 columns. |
| `mat3_get_transpose` | Gives the matrix with its rows as columns. |
| `mat3_load` | Gives the matrix of 9 numbers at `numbers`. |
| `mat3_store` | Writes the 9 numbers at `numbers`. |
