package freechips.rocketchip.tile

import chisel3._
import chisel3.util._

object GlobalParams {
  val Num_cores = 8;
  val Num_Slavecores = Num_cores - 1
  var Master_core = 0
  val List_hartid = (0 until Num_cores by 1).toList
  var List_Slaveid = List_hartid.filter(_ != Master_core)
  val Data_width = 32
  val Data_type = UInt(Data_width.W)
  val depth = 32
}