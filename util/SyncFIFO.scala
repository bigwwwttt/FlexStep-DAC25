package freechips.rocketchip.util

import chisel3._
import chisel3.util._

class SyncFIFO[T <: Data](ioType: T, depth: Int) extends Module {
  val io = IO(new Bundle {
    val in = Flipped(Decoupled(ioType))
    val out = Decoupled(ioType)
    val almostfull = Output(Bool())
    val count = Output(UInt((log2Ceil(depth) + 1).W))
    val full = Output(Bool())
    val empty = Output(Bool())
    val widx = Output(UInt((log2Ceil(depth)).W))
    val ridx = Output(UInt((log2Ceil(depth)).W))
  })
  require(depth > 0 && isPow2(depth))

  val queue = Module(new Queue(ioType, depth, false, false, true, false))
  val almostfull = queue.io.count >= depth.U - 5.U
  queue.io.enq.bits  := io.in.bits
  queue.io.enq.valid := io.in.valid
  io.in.ready        := queue.io.enq.ready
  //io.in <> queue.io.enq
  when(queue.io.count <= 1.U){
    queue.io.deq.ready := false.B
    io.out.bits        := 0.U
    io.out.valid       := false.B
  }.otherwise{
    queue.io.deq.ready := io.out.ready
    io.out.bits        := queue.io.deq.bits
    io.out.valid       := queue.io.deq.valid
  }
  
  
  //queue.io.deq <> io.out

  io.widx := Counter(io.in.valid && io.in.ready && !io.full, depth)._1
  io.ridx := Counter(io.out.valid && io.out.ready && !io.empty, depth)._1

  io.full := queue.io.count === depth.U
  io.empty := queue.io.count === 0.U
  io.almostfull := almostfull
  io.count := queue.io.count
}


