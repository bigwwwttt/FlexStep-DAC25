package freechips.rocketchip.util

import chisel3._
import chisel3.util._
import freechips.rocketchip.util.GlobalParams

class otmMux(num: Int) extends Module{
    val io = IO(new Bundle{
        val in = Flipped(Decoupled(GlobalParams.Data_type))
        val out = Vec(num, Decoupled(GlobalParams.Data_type))
        val sels = Input(Vec(num, Bool()))
    })
    val readyVec = io.out.zip(io.sels).map { case (out, sel) =>
    (!out.ready) && sel
    }
    io.in.ready := !(readyVec.reduce(_ || _))
    for(i <- 0 until num){
        when(io.sels(i)){
            when(!(readyVec.reduce(_ || _))){
               io.in.bits <> io.out(i).bits
               io.in.valid <> io.out(i).valid
            }.otherwise{
               io.out(i).valid := false.B
               io.out(i).bits := 0.U
            }        
        }.otherwise{
            io.out(i).valid := false.B
            io.out(i).bits := 0.U
        }
    }
        
}

