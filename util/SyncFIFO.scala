package freechips.rocketchip.util

import chisel3._
import chisel3.util._

class SyncFIFO[T <: Data](ioType: T, depth: Int) extends Module {
  val io = IO(new Bundle{
    val in = Flipped(Decoupled(ioType))
    val out = Decoupled(ioType)
    val full = Output(Bool())
    val empty = Output(Bool())
    val widx = Output(UInt((log2Ceil(depth)).W))
    val ridx = Output(UInt((log2Ceil(depth)).W))
  })
  require(depth>0 && isPow2(depth))

  io.widx := Counter(io.in.valid && !io.full, depth)._1
  io.ridx := Counter(io.out.ready && !io.empty, depth)._1

  io.out <> Queue(io.in, depth)
  io.full := !io.in.ready
  io.empty := !io.out.valid
}