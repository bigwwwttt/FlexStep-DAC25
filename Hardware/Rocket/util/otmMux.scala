package freechips.rocketchip.util

import chisel3._
import chisel3.util._
import freechips.rocketchip.util.GlobalParams

class otmMux(num: Int) extends Module{
    val io = IO(new Bundle{
        val in        = Flipped(Decoupled(GlobalParams.Data_type))
        val out       = Vec(num, Decoupled(GlobalParams.Data_type))
        val busy_in   = Vec(num, Input(Bool()))
        val free_out  = Vec(num, Output(Bool()))
        val umode_in  = Vec(num, Input(Bool()))
        val umode_out = Vec(num, Output(Bool()))
        val sels      = Input(Vec(num, Bool()))
    })
    val readyVec = io.out.zip(io.sels).map { case (out, sel) =>
    (!out.ready) && sel
    }
    val NotallFfalse = io.sels.reduce(_ || _)
    when(NotallFfalse){
        io.in.ready := !(readyVec.reduce(_ || _))
    }.otherwise{
        io.in.ready := false.B
    }
    
    for(i <- 0 until num){
        when(io.sels(i)){
            io.free_out(i)  := !io.busy_in(i)
            io.umode_out(i) := io.umode_in(i)
            when(io.in.ready){
                io.out(i).bits  := io.in.bits
                io.out(i).valid := io.in.valid
                // io.in.bits <> io.out(i).bits
                // io.in.valid <> io.out(i).valid
            }.otherwise{
                io.out(i).valid := false.B
                io.out(i).bits  := 0.U
            }                 
        }.otherwise{
            io.out(i).valid := false.B
            io.out(i).bits  := 0.U
            io.free_out(i)  := true.B
            io.umode_out(i) := true.B
        }
    }
        
}


