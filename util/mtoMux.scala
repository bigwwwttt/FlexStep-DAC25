package freechips.rocketchip.util

import chisel3._
import chisel3.util._
import freechips.rocketchip.util.GlobalParams

class mtoMux(num: Int) extends Module{
    val io = IO(new Bundle{
        val out = Decoupled(GlobalParams.Data_type)
        val in = Flipped(Vec(num, Decoupled(GlobalParams.Data_type)))
        val sels = Input(Vec(num, Bool()))
    })
    io.out.bits := Mux1H(io.sels, io.in.map(_.bits))
    io.out.valid := Mux1H(io.sels, io.in.map(_.valid))    
    for(i <- 0 until num){
        when(io.sels(i)){
            io.in(i).ready := io.out.ready     
        }.otherwise{
            io.in(i).ready := false.B
        }
    }
}
