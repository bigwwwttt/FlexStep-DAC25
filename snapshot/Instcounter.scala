package freechips.rocketchip.snapshot

import chisel3._
import chisel3.util._


class Instcounter extends Module{
  val io = IO(new Bundle{
     val start = Input(Bool())
     val wb_valid               =    Input(Bool())
     val copy_valid             =    Output(Bool())
     val copy_done              =    Input(Bool())
     val next_check             =    Input(Bool())
     val check_done             =    Output(Bool())
     val instnum                =    Output(UInt(64.W))
     val inst_left              =    Input(UInt(64.W))
     val score_check_complete   =    Output(Bool())
     val mcore_check_complete   =    Output(Bool())
     val CP_end                 =    Input(Bool())
     val Mchecke_call           =    Input(Bool())
     val instnum_threshold      =    Input(UInt(64.W))
     val exception              =    Input(Bool())
})
  val instnumber     =    RegInit(0.U(64.W))
   
  io.instnum := instnumber
  //copy request
  when(io.start){
     when(io.wb_valid){
        instnumber := instnumber + 1.U
      }.elsewhen(io.next_check){
        instnumber := 0.U
      }.otherwise{
        instnumber := instnumber
      }
     when(instnumber >= io.instnum_threshold){
        io.copy_valid := true.B
        //io.check_done := true.B
     }
     .otherwise{
        io.copy_valid := false.B
        //io.check_done := false.B
     }

     when(io.Mchecke_call || io.exception){
       io.mcore_check_complete := true.B
     }.otherwise{
       io.mcore_check_complete := false.B
     }
     
     when(instnumber === (io.instnum_threshold - 1.U) && io.wb_valid){
       io.check_done := true.B
     }.otherwise{
       io.check_done := false.B
     }

     when((instnumber === io.inst_left - 1.U) && io.wb_valid && io.CP_end){
       io.score_check_complete := true.B
     }.otherwise{
       io.score_check_complete := false.B
     }
     
     dontTouch(instnumber)
  }.otherwise{
   io.score_check_complete := false.B
   io.mcore_check_complete := false.B
   when(io.next_check){
      instnumber := 0.U
   }
   io.copy_valid := false.B

   io.check_done := false.B
   
    
  }
  
}
