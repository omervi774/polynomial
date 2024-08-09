# Polynomial Multithreading Calculator

## Overview

This project is a C-based program that calculates the value of polynomial expressions using multithreading. Each thread is responsible for computing the contribution of one term of the polynomial to the final result. The program continues to prompt the user for input until the word "done" is entered.

## Prerequisites

To run this project on a Linux machine, you need the following:

A Linux environment with a C compiler.

## How It Works

1. The user is prompted to enter a polynomial expression and a value to evaluate.
2. The program parses the input to identify each term in the polynomial.
3. For each term (involving `x`), a thread is created to compute its value.
4. The main thread waits for all threads to finish, sums their results, and prints the final value.
5. The process repeats until the user types "done".

## Input Format

The expected input format is: 

- **Polynomial:** A polynomial expression in terms of `x`. For example, `3*x^2 + 2*x + 1`.
- **Value:** The integer value for `x` in the polynomial.

Example:

3x^2 + 2x + 1, 5

## how to compile

1. compile:  gcc -pthread -o polynomial  polynomial.c
2. run: ./polynomial

