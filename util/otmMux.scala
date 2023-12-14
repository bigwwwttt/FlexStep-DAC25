package freechips.rocketchip.util

import chisel3._
import chisel3.util._

class otmMux(num: Int) extends Module{
    val io = IO(new Bundle{
        val in = Flipped(Decoupled(UInt(32.W)))
        val out = Vec(num, Decoupled(UInt(32.W)))
        val sels = Input(Vec(num, Bool()))
    })
    val readyVec = io.out.zip(io.sels).map { case (out, sel) =>
    out.ready && sel
    }
    io.in.ready := PriorityMux(io.sels, readyVec)
    for(i <- 0 until num){
        when(io.sels(i)){
            io.in <> io.out(i)
        }.otherwise{
            io.out(i).valid := false.B
            io.out(i).bits := 0.U
        }
    }
    
}

