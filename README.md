# Sub-Bit Packed Array

## Table Of Contents

- [Sub-Bit Packed Array](#sub-bit-packed-array)
  - [Table Of Contents](#table-of-contents)
  - [TL;DR](#tldr)
  - [Summary](#summary)
  - [The Problem With Storing States](#the-problem-with-storing-states)
    - [Bitwise Packing](#bitwise-packing)
    - [This Library: Sub-Bit Packing](#this-library-sub-bit-packing)
  - [How Does It Work?](#how-does-it-work)
  - [Space Saved](#space-saved)
  - [How To Use](#how-to-use)
    - [Initialization](#initialization)
    - [Set Values](#set-values)
    - [Retrieve Values](#retrieve-values)
  - [Optimization](#optimization)

## TL;DR

A library to save memory by efficiently saving state arrays with sub-bit compression.

## Summary

There is huge potential to save memory by packing values arithmetically. [This table](#space-saved) lists the amount of saved space in contrast to bitwise packing.
The drawback is a runtime overhead for setting/retrieving the values because calculations need to be done instead of simple addressing.
Therefore if the number of states you want to store has no spacial benefit (e.g. powers of 2) you should stick to bitwise packing because of the faster runtime.
On the other hand, you can fit more values of states that can be tightly packed on platforms where memory is the limiting factor (e.g. embedded systems) utilizing this library.

## The Problem With Storing States

```c++
    enum class MyState
    {
        First,
        Second,
        Third,
    }

    uint8_t a_lot_of_states[10000];
```

A single `MyState` value can be stored in an 8-bit variable.

`sizeof(uint8_t)` will give you 1 byte

`sizeof(a_lot_of_states)` will then give you 10000

In order to store 10000 values we are using 10000 bytes which could store up to 2^(10000 * 8) binary states.

This naive method is a huge waste of memory.

### Bitwise Packing

To store three states we need 2 bits.

So to store 10000 states we would only need 20000 bits or 2500 bytes.

We can already save 75% of the storage (10000 vs. 2500).

### This Library: Sub-Bit Packing

As you may have already figured out, when using 2 bits to represent 3 states there is one extra state too many:

`00` First

`01` Second

`10` Third

`11` _unused_

We can save 25% memory here by packing the values arithmetically.

## How Does It Work?

Internally the values are held in a array of 32-bit data chunks. Instead of bitwise packing, however, a form of [arithmetic coding](https://en.wikipedia.org/wiki/Arithmetic_coding) is used to store the values. The advantage of this is that you can pack (the same or) more values in a given data buffer than with traditional bitwise packing. The overall space saved for values with `X` different states can be calculated with the following formula:

$$ factor = {\lfloor32 / (\log(X) / \log(2))\rfloor \over \lfloor32 / \lceil(\log(X) / \log(2))\rceil \rfloor} $$

Lets say you want to store values with 3 different states:

$$ factor = {\lfloor32 / (\log(3) / \log(2))\rfloor \over \lfloor32 / \lceil(\log(3) / \log(2))\rceil \rfloor} = 1.25$$

This means an arithmetically packed buffer will be able to store 25% more values than with tradtional packing. See [this table](#space-saved) for more values.

## Space Saved

Here's a table for values with different number of states and how much space is saved compared to bitwise packing:

| Number of states per value  | Space advantage |
| -- | ------ |
|3 | 25%|
|5 | 30%|
|6 | 20%|
|7 | 10%|
|9 | 25%|
|10,11 | 12.5%|
|17-23 | 16.66%|
|33-40 | 20%|
|65-84 | 25%|
|1025-1625 | 50%|


The space saved for all other values is 0%. That means it is not better nor worse than bitwise packing. However, the access times are slower. Therefore this library is not optimal for such use cases.

## How To Use

### Initialization

Let's say you have a 3-Color-Display with a resolution of 800x480 (== 384000 values):

```c++
    kt::SubBitPackedArray<3, 384000> displayBuffer{};
```

Or on the heap:

```c++
    auto displayBuffer = new kt::SubBitPackedArray<3, 384000>{};
```

### Set Values

```c++
    enum class Color
    {
        WHITE = 0,
        BLACK
        RED
    }

    size_t pixel = ...

    displayBuffer->setState(pixel, static_cast<uint16_t>(Color::RED));
```

### Retrieve Values

```c++
    Color color = static_cast<Color>(displayBuffer->getState(pixel));
```

## Optimization

You can disable Exceptions by setting the compile flag `SUBBITPACKEDARRAY_NO_EXCEPTIONS`. This disables the array boundary checks altogether and should speed up runtime somewhat.

Additionally you can exclude the class Iterator by setting the compile flag `SUBBITPACKEDARRAY_NO_ITERATOR` if you don't need it.
