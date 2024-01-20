     This code adds communicating channels between one master core and multiple slave cores. 
     We add a globalparams.scala file at generators/rocket-chip/src/main/scala/util/globalparams.scala to manage some important parameters:
object GlobalParams {
  val Num_cores = 6;
  val Num_Mastercores = 2
  val Num_Slavecores = Num_cores - Num_Mastercores
  val List_hartid = (0 until Num_cores by 1).toList
  val Data_width = 32
  val Data_type = UInt(Data_width.W)
  val depth = 32
}

object GlobalVariables {
  var List_MasterId = List(0, 1)
  val List_SlaveId = GlobalParams.List_hartid.filterNot(List_MasterId.contains)
  require(List_MasterId.length == GlobalParams.Num_Mastercores)
}

     
     When the number of slave cores changes, the number of sels signals also needs to change. The sels singals are controlled by reg_sels at 
     class Roctet(generators/rocket-chip/src/main/scala/rocket/RocketCore.scala)
