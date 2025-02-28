rm -rf $PLATFORM/generators/rocket-chip/src/scala
rm -rf $PLATFORM/generators/chipyard/src/main

cp $FIREGUARD/HARDWARE/Rocket $PLATFORM/generators/rocket-chip/src/scala
cp $FIREGUARD/HARDWARE/Chipyard $PLATFORM/generators/rocket-chip/src/scala
