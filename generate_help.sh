#!/bin/bash

OUTDIR="build/debug"

if [[ "$1" == "release" ]]; then
    OUTDIR="build/release"
elif [[ -n "$1" ]]; then
    echo "Usage: $0 [release]"
    exit 1
fi

cat > "$OUTDIR/help.txt" <<EOF
List of Instructions:
  HELP - Displays a list of all available commands.
	
  STAT - Displays the current state of all registers.

  END - Ends the execution of the program.

  PRINT <reg> - Outputs the value stored in the specified register.
  Example: PRINT RAX

  CLR <reg> - Clears (sets to zero) the specified register.
  Example: CLR RBX

  CHS <reg> - Changes sign of the value stored in the specified register.
  Example: CHS RBX

  MOV <reg>, <number> - Stores a numeric value (floating point supported) in the specified register.
  Example: MOV RAX, 10.5

  MOVR <reg1>, <reg2> - Copies the value from register <reg2> to <reg1>.
  Example: MOVR RAX, RBX

  ADD <reg1>, <reg2> - Adds the values in <reg1> and <reg2> and stores the result in <reg1>.
  Example: ADD RAX, RBX

  SUB <reg1>, <reg2> - Subtracts the value of <reg2> from <reg1> and stores the result in <reg1>.
  Example: SUB RAX, RBX

  MUL <reg1>, <reg2> - Multiplies the values of <reg1> and <reg2>, result is stored in RCX.
  Only for 32-bit registers!
  Example: MUL RAL, RBL

  DIV <reg1>, <reg2> - Divides the value of <reg1> by <reg2>, result is stored in RDL.
  <reg1> must be a 64-bit register, <reg2> must be a 32-bit register.
  Example: DIV RAX, RBL


List of Registers:
  64-bit Registers:
  RAX, RBX, RCX, RDX - these registers can store large integers or fixed-point floating numbers.

  32-bit Parts of 64-bit Registers:
  RAH, RBH, RCH, RDH — upper 32 bits;
  RAL, RBL, RCL, RDL — lower 32 bits.

Usage Restrictions:
  Both operands in arithmetic instructions must be of the same size (both 64-bit or both 32-bit).
  The exceptions are the MUL and DIV instructions (check description above).
EOF
