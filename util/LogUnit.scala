package freechips.rocketchip.util

import chisel3._
import chisel3.util._
import freechips.rocketchip.rocket._
import freechips.rocketchip.tile._
import org.chipsalliance.cde.config.Parameters

class LogUnit(implicit p: Parameters) extends CoreModule()(p) 
    with HasRocketCoreParameters
{
    val io = IO(new Bundle {
        val wb_valid = Input(Bool())
        val wb_ctrl = Input(new IntCtrlSigs(aluFn))
        val inst = Input(UInt(32.W))

        // for addr 
        val imm = Input(SInt(64.W))
        val rd0val = Input(UInt(64.W))
        val wrdata = Input(UInt(64.W))

        // for data
        val dmem_resp_data = Input(UInt(64.W))
        val rd1val = Input(UInt(64.W))
        val dmem_s2_data = Input(UInt(64.W))
        val csr_rw_rdata = Input(UInt(64.W))

        val fifo_valid = Output(Bool())
        val fifo_data = Output(UInt(256.W))

    })

    val addr = Wire(UInt(64.W))
    val data = Wire(UInt(64.W))
    val more = Wire(UInt(64.W))
    
    when(io.wb_ctrl.mem && io.wb_ctrl.mem_cmd.isOneOf(M_XRD, M_XWR)) {
        addr := io.rd0val + io.imm.asUInt
    }.elsewhen(io.wb_ctrl.mem && (io.wb_ctrl.mem_cmd.isOneOf(M_XSC, M_XLR) || isAMO(io.wb_ctrl.mem_cmd))) {
        addr := io.rd0val
    }.otherwise {
        addr := 0.U
    }

    when(io.wb_ctrl.mem && (io.wb_ctrl.mem_cmd.isOneOf(M_XRD, M_XLR, M_XSC) || isAMO(io.wb_ctrl.mem_cmd))) {
        data := io.dmem_resp_data
    }.elsewhen(io.wb_ctrl.csr =/= CSR.N && io.wb_ctrl.wxd){
        data := io.csr_rw_rdata
    }.otherwise {
        data := 0.U
    }

    when(io.wb_ctrl.mem && (io.wb_ctrl.mem_cmd === M_XWR || io.wb_ctrl.mem_cmd === M_XSC || isAMO(io.wb_ctrl.mem_cmd))) {
        more := io.dmem_s2_data
    }.otherwise {
        more := 0.U
    }


    // valid: when wb_valid and wb_ctrl.mem
    // for the time being,
    // we assume that only LR/SC/AMO, integer LD/ST, fp LD_ST set wb_ctrl.mem but in fact there are more.

    // for int and fp LD:       addr(rd0val+imm);   data loaded(dmem_resp_data);      
    // for LR:                  addr(rd0val);       data loaded(dmem_resp_data);          
    // for SC:                  addr(rd0val);       data loaded(dmem_resp_data);    more(dmem_s2_data).              
    // for AMO:                 addr(rd0val);       data loaded(dmem_resp_data);    more(dmem_s2_data).    
    
    // for int and fp ST:       add(rd0val+imm);                                    more(dmem_s2_data);

    
    // from the slave's perspective:
    // scenarios affecting the architectural state:
    // sigs involved:       rf_wdata:               int ld; lr; sc; amo
    //                      io.fpu.dmem_resp_data:  fp ld;

    // need to be checked:
    //                           


    val fifo_valid = io.wb_valid && (io.wb_ctrl.mem || (io.wb_ctrl.csr =/= CSR.N && io.wb_ctrl.wxd))
    val fifo_data = Cat("h_dead_beef".U, io.inst, 
                        addr, 
                        data,
                        more)

    io.fifo_data := fifo_data
    io.fifo_valid := fifo_valid
}
