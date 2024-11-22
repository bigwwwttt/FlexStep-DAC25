# FlexStep on Rocket-core and Chipyard platform
FlexStep: Enabling Flexible Error Detection in Multi/Many-core Real-time Systems. 
FlexStep is a homogeneous-core error-detection framework. Using a hardware-software co-design approach,
we implemented FlexStep in a multi-core system by adding lightweight modifications to existing microarchitecture of Rocket and integrating
simple OS-level scheduling codes. Here we will introduce how to implement FlexStep on the Chipyard platform and deploy it on FPGA through Firesim to run some benchmarks.


## Setup
The setup of chipyard can refer to the tutorial https://chipyard.readthedocs.io/en/1.11.0/Chipyard-Basics/index.html 
and the FPGA simulation tool firesim can refer to https://docs.fires.im/en/latest/FireSim-Basics.html according to your FPGA model.

## Hardware 
After the setup, you can replace all files in /your_path/chipyard/generators/rocket-chip/src/main/scala 
and /your_path/chipyard/generators/chipyard/src/main/scala with files in /Hardware/Rocket including the source codes of Rocket-core microarchitecture
and /Hardware/Chipyard including the configuration of Rocket-chip, respectively.

## Software simulation
###  verilator simulation
When you complete the setup of chipyard, you can run any workload you want on verilator to test the hardware. The test.c is an example.
```bash
./build.sh //generate the binary file: test.riscv, if you want to delete it, run ./clean.sh.(you need riscv toolchain in chipyard or other. In chipyard, remember run source env.sh first)
cd /.../chipyard/sims/verilator
make run-binary-debug BINARY=/.../test.riscv //simulate the test.c and generate the waveform.
