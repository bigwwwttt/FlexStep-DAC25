package freechips.rocketchip.util

import chisel3._

object GlobalParams {
  val Num_Groups = 1
  val Num_Groupcores = 4
  val Num_cores = Num_Groups * Num_Groupcores
  val hartid    = (0 until Num_cores by 1).toList
  val List_hartid1 = (0 until Num_Groupcores by 1).toList
  val List_hartid = hartid.grouped(Num_Groupcores).toList

  var List_MasterId = List(0)
  var Num_Mastercores = List_MasterId.length
  var Num_Slavecores = Num_Groupcores - Num_Mastercores
  var List_SlaveId = List_hartid1.filterNot(List_MasterId.contains)
  
  val Data_width = 256
  val Data_type = UInt(Data_width.W)
  val depth = 32
}