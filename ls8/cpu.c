#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DATA_LEN 6

unsigned char cpu_ram_read(struct cpu *cpu)
{
  return cpu->ram[cpu->pc];
}

void cpu_ram_write(struct cpu *cpu, int address, unsigned char input)
{
  cpu->ram[address] = input;
}
/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
  FILE *fp;
  char line[1024];
  int address = 0;

  fp = fopen(filename, "r");

  if (fp == NULL)
  {
    fprintf(stderr, "file not found\n");
    exit(1);
  }

  while (fgets(line, 1024, fp) != NULL)
  {
    char *endptr;
    unsigned char v = strtoul(line, &endptr, 2);

    if (endptr == line)
    {
      continue;
    }

    cpu_ram_write(cpu, address, v);
    address++;
  }

  // TODO: Replace this with something less hard-coded
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op)
  {
  case ALU_MUL:
    cpu->registers[regA] = cpu->registers[regA] * cpu->registers[regB];
    break;

    // TODO: implement more ALU ops
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  unsigned char instruction;

  unsigned char operandA;
  unsigned char operandB;

  while (running)
  {
    instruction = cpu->ram[cpu->pc];
    if (instruction > 0b01111111)
    {
      operandA = cpu->ram[cpu->pc + 1];
      operandB = cpu->ram[cpu->pc + 2];
    }
    else if (instruction > 0b00111111)
    {
      operandA = cpu->ram[cpu->pc + 1];
    }
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. Figure out how many operands this next instruction requires
    // 3. Get the appropriate value(s) of the operands following this instruction
    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.
    switch (instruction)
    {
    case LDI:
      cpu->registers[operandA] = operandB;
      cpu->pc += 3;
      break;
    case MUL:
      alu(cpu, ALU_MUL, operandA, operandB);
      cpu->pc += 3;
      break;
    case PRN:
      printf("%d\n", cpu->registers[operandA]);
      cpu->pc += 2;
      break;
    default:
      exit(1);
    }
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->pc = 0;
  memset(cpu->registers, 0, 8);
  memset(cpu->ram, 0, 256);
}
