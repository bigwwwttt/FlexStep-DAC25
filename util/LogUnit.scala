package freechips.rocketchip.util

import chisel3._
import chisel3.util._
import freechips.rocketchip.rocket._

class LogUnit extends Module {
    val io = IO(new Bundle {
        val valid = Input(Bool())
        val mem = Input(Bool())
        val mem_cmd = Input(UInt(5.W))

        val imm = Input(SInt(64.W))
        val rd = Input(UInt(64.W))
        val rs0 = Input(UInt(64.W))
        val rs1 = Input(UInt(64.W))

        val lhs = Input(UInt(64.W))
        val rhs = Input(UInt(64.W))
        val out = Input(UInt(64.W))

    })
    //Log addr & data of load & store
    //if load
    //addr = rs0 + imm
    //data = rd
    //if store
    //addr = rs0 + imm
    //data = rs1

    val addr = Wire(UInt(64.W))
    val data_l = Wire(UInt(64.W))
    val data_s = Wire(UInt(64.W))
    addr := io.rs0 + io.imm.asUInt
    data_l := io.rd
    data_s := io.rs1
    //dontTouch(addr)
    //dontTouch(data)

    when(io.valid) {
        when(io.mem) {
            when(io.mem_cmd === M_XRD) {
                printf(cf"LOAD:  ADDR:0x$addr%x DATA:0x$data_l%x\n")
            }.elsewhen(io.mem_cmd === M_XWR) {
                printf(cf"STORE: ADDR:0x$addr%x DATA:0x$data_s%x\n")
            }.elsewhen(io.mem_cmd.isOneOf(M_XA_ADD, M_XA_XOR, M_XA_OR, M_XA_AND, M_XA_MIN, M_XA_MAX, M_XA_MINU, M_XA_MAXU)) {
                printf(cf"AMO: ADDR:0x${io.rs0}%x ORI:0x${io.lhs}%x RES:0x${io.out}%x\n")
            }.elsewhen(io.mem_cmd === M_XLR) {
                printf(cf"LR:  ADDR:0x${io.rs0}%x DATA:0x${io.rd}%x\n")
            }.elsewhen(io.mem_cmd === M_XSC) {
                printf(cf"SC:  ADDR:0x${io.rs0}%x DATA:0x${io.rs1}%x SUC:0x${io.rd}%x\n")
            }
        }
    }

}
