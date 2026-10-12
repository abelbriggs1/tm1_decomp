# QUIRKS.md

This is a file for compiler quirks and other issues that may prevent
code and data from matching.

(Much of this will be uploaded to `decomp.wiki` in the future if it's determined
to be applicable to other games.)

## Small data must be decompiled early

The toolchain that we use for this repo treats the following variables
differently when attempting to load/store to them.

```
extern s16 my_var;
static s16 my_var2;
```

Data in small-data sections (`.sdata`, `.sbss`) must be decompiled
into C variables before any C code that accesses them can be matched;
`extern` cannot be used for them.


## Inlining results in a variable reload

If you have two functions which use the same variable:

```c
uaSelectOpponents(gVehicleID, gCurrentLevel, &gNumOpponents, opponents);
screenInitCardSprites(gCurrentLevel, gNumOpponents, opponents);
```

By default, PsyQ will load `opponents` into a local variable, and keep it across
the two calls.

However, if these calls are placed as part of an inline function:

```c
inline void shellSelectCards(void)
{
    uaSelectOpponents(gVehicleID, gCurrentLevel, &gNumOpponents, opponents);
    screenInitCardSprites(gCurrentLevel, gNumOpponents, opponents);
}

void shellBeginLevelTransition(void)
{
    // ...
    shellSelectCards(); // inlined
    // ...
}
```

When inlined, PsyQ will treat each call as if it were in a separate context.
In this case, `opponents` will be reloaded across the two calls.

## Inlined struct/array operations

Many cross-member struct and array operations will be converted to
function calls. As detailed [above](#inlining-results-in-a-variable-reload),
these often can't be matched by using the raw/substituted equivalent explicitly
because they typically result in abnormal variable behavior (such as reloads).

### `memcpy()` (constant struct assignment)

Struct assignments such as the following:

```c
// all are `MATRIX`
cs->mat = unitMatrix;
cs->wmat = unitMatrix;
```

PsyQ accepts this syntax, and converts this to a call to the BIOS (not the SDK!)'s
`memcpy()`:

```c
memcpy(&cs->mat, &unitMatrix, sizeof(MATRIX));
memcpy(&cs->wmat, &unitMatrix, sizeof(MATRIX));
```

### `memset()` (partial struct/array assignment)

`memset()` behavior varies greatly depending on how the assignment is performed.
Similarly to `memcpy()`, the inlined calls here go to the BIOS `memcpy()`, not the SDK's.
Here is a table showing the general behavior:

| Source                                                     | Generated code |
|------------------------------------------------------------|----------------|
| `char buf[30] = "";`                                       | Copies the string literal (`lbu`/`sb`), then calls `memset(buf + 1, 0, 29)` |
| `char buf[30] = "ab";`                                     | Copies 3 bytes, then calls `memset(buf + 3, 0, 27)` |
| `int a[20] = {v};` (any non-constant element)              | Calls `memset(a, 0, 80)` on the whole array, then stores the elements |
| `int a[20] = {0};`, `{1}`, `Big b = {{0}};` (all constant) | No `memset`; a copy loop from `.rodata` |
