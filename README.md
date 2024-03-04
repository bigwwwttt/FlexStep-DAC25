     This code adds communicating channels between master core(s) and multiple slave core(s) which can be changed in runtime.
     
     We add a globalparams.scala file at generators/rocket-chip/src/main/scala/util/globalparams.scala to manage some important parameters
     
     The implementation codes are in the following files: HasTiles.scala; BaseTile.scala; RocketTile.scala; RocketCore.scala
     
     When the number and ID of Master cores and slave cores change, the sels signals also needs to change.
     The selection signal requires satisfying some assertion conditions. 
     The sels singals are controlled by reg_sels at class Roctet(generators/rocket-chip/src/main/scala/rocket/RocketCore.scala)

     Update at 2024.3.4：Added custom instruction for changing the number and ID of Master core
