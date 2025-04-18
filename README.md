# FlexStep on Rocket-core and Chipyard platform
FlexStep: Enabling Flexible Error Detection in Multi/Many-core Real-time Systems. 
FlexStep is a homogeneous-core error-detection framework. Using a hardware-software co-design approach,
we implemented FlexStep in a multi-core system by adding lightweight modifications to existing microarchitecture of Rocket and integrating
simple OS-level scheduling codes. Here we will introduce how to implement FlexStep on the Chipyard platform and deploy it on FPGA through Firesim to run some benchmarks.


## Setup
The setup of chipyard can refer to the tutorial https://chipyard.readthedocs.io/en/1.11.0/Chipyard-Basics/Initial-Repo-Setup.html 
```bash
git clone https://github.com/ucb-bar/chipyard.git
cd chipyard
git checkout 1.10.0
./build-setup.sh
```
and the FPGA simulation tool firesim can refer to https://docs.fires.im/en/latest/FireSim-Basics.html according to your FPGA model.
```bash
git clone https://github.com/firesim/firesim
 cd firesim
 git checkout 1.17.1
./scripts/machine-launch-script.sh --prefix REPLACE_ME_USER_CONDA_LOCATION
conda env list
conda activate firesim
./build-setup.sh
```

## Hardware 
After the setup, you can replace all files in /your_path/chipyard/generators/rocket-chip/src/main/scala 
and /your_path/chipyard/generators/chipyard/src/main/scala with files in /Hardware/Rocket including the source codes of Rocket-core microarchitecture
and /Hardware/Chipyard including the configuration of Rocket-chip, respectively.
```bash
export PLATFORM=$(pwd)
./update_src.sh
```


## Software simulation
### verilator simulation
When you complete the setup of chipyard, you can run any workload you want on verilator to test the hardware. The test.c is an example. The specific simulation details can be referred to https://chipyard.readthedocs.io/en/1.11.0/Simulation/index.html.
```bash
./build.sh //generate the binary file: test.riscv, if you want to delete it, run ./clean.sh.(you need riscv toolchain in chipyard or other. In chipyard, remember running source env.sh first)
cd /.../chipyard/sims/verilator
make run-binary-debug BINARY=/.../test.riscv //simulate the test.c and generate the waveform.
```
### FPGA simulation
After completing the simulation on verilator, you can deploy FlexStep on FPGA and boot Linux and run some large workloads like Parsec and SPEC06 (In the "Benchmark" folder, the basic tutorials are in their respective README.md). The simulation method can refer to https://docs.fires.im/en/latest/Getting-Started-Guides/On-Premises-FPGA-Getting-Started/Running-Simulations/Running-Single-Node-Simulation-Xilinx-Alveo-U280.html, and the custom workloads running on FPGA are defined through FireMarshal referring to https://docs.fires.im/en/latest/Advanced-Usage/Workloads/FireMarshal.html.

When you try to add the verification function of FlexStep to run Parsec and SPEC06, you need not change the source codes like simulation on verilator. Instead, you can just link the link.c to the binary files of those workloads without any modifications to the source codes. In the link.c, we apply the constructor attribute to myStartupFun() so that it is executed before main(), and apply the destructor attribute to myCleanupFun() so that it is executed after main(). Besides, we use P-Threads to create threads and employ pthread_setaffinity_np to bind each thread to a specific CPU core.

To do that, you need change the configuration files to link the link.o generated by link.c to the origin binary. As an example, for Parsec, you need add the link.o in line 46 - 49 of /your_path/FlexStep-DAC25/Benchmark/Parsec/config/gcc.bldconf, \ie, export CFLAGS=" $CFLAGS -O3 -static -DRISCV /your_path/link.o". For SPEC06, the configuration file is /your_path/FlexStep-DAC25/Benchmark/SPEC06/riscv.cfg.
