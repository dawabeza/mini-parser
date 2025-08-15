# Mini Arithmetic Parser

A small parser/interpreter built from scratch in C that evaluates **arithmetic expressions** and supports **variables, constants, and functions**. This project was designed as a personal learning experiment to explore parsing, expression evaluation, and building interpreters from the ground up.

---

## Features

- **Variables:** Supports flexible variable names, including spaces
- **Constants:** Includes mathematical constants like `pi` and `e`
- **Arithmetic Operations:** `+`, `-`, `*`, `/` with support for **implicit multiplication**
- **Functions:** Supports `sin`, `cos`, `log` (base 10), `ln`, and more
- **Interactive REPL:** Accepts statements one by one and prints updated variables
- **Error Handling:** Detects invalid operations, such as assigning to constants
- **Custom Data Structures:** Uses linked lists for dynamic variable and expression storage

---

## Sample Usage

```c
x = 1
y = 2
z = x + y
theta = pi / 2
r = 10
total = ((x + 2)*(y - 3) + 4.5*z) / (r*(x + y)) + sin(theta)*cos(pi/4) + log(10) + ln(e) + 2*x*y - pi*r*r
