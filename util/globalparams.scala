package freechips.rocketchip.tile

import chisel3._
import chisel3.util._

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
