     This code adds communicating channels between one master core and multiple slave cores. 
     We add a globalparams.scala file at generators/rocket-chip/src/main/scala/util/globalparams.scala to manage some important parameters:


     
     When the number of slave cores changes, the number of sels signals also needs to change. The sels singals are controlled by reg_sels at 
     class Roctet(generators/rocket-chip/src/main/scala/rocket/RocketCore.scala)
