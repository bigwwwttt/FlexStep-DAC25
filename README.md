# FlexStep on Rocket-core and Chipyard platform
FlexStep: Enabling Flexible Error Detection in Multi/Many-core Real-time Systems. 
FlexStep is a homogeneous-core error-detection framework. Using a hardware-software co-design approach,
we implemented FlexStep in a multi-core system by adding lightweight modifications to existing microarchitecture of Rocket and integrating
simple OS-level scheduling codes. Here we will introduce how to implement FlexStep on the Chipyard platform and deploy it on FPGA through Firesim to run some benchmarks.


## Setup
The setup of chipyard can refer to the tutorial https://chipyard.readthedocs.io/en/1.11.0/Chipyard-Basics/index.html 
and the FPGA simulation tool firesim can refer to https://docs.fires.im/en/latest/FireSim-Basics.html according to your FPGA model.