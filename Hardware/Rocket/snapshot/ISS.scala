package freechips.rocketchip.snapshot

import chisel3._
import chisel3.util._
import freechips.rocketchip.rocket._
import freechips.rocketchip.tile._
import org.chipsalliance.cde.config.Parameters


class ISS (implicit p: Parameters) extends CoreModule()(p) 
    with HasRocketCoreParameters{
  val io = IO(new Bundle{
   //  val copy_valid      =    Input(Bool())
   val sent_valid      =    Input(Bool())
  
   //  val copy_done       =    Output(Bool())
    val sent_done       =    Output(Bool())

    val copy_valid_test =    Input(Bool())
    val copy_done_test  =    Output(Bool())
    val cansent         =    Output(Bool())
    
   //  val intreg_input    =    Input(Vec(32, UInt(64.W)))
   //  val fpreg_input     =    Input(Vec(32, UInt(64.W)))
    val intreg_input_test     =    Input(Vec(4, UInt(64.W)))
    val fpreg_input_test      =    Input(Vec(4, UInt(64.W)))
    val intpc_input     =    Input(UInt(40.W))
    val fpcsr_input     =    Input(UInt(8.W))

    val sign            =    Output(UInt(2.W))
    val copyidx         =    Output(UInt(3.W))
      
    val sent_output     =    Output(UInt(256.W))

})

//   val intrf_mem        =    SyncReadMem(32, UInt(xLen.W))
//   val fprf_mem         =    SyncReadMem(32, UInt(xLen.W))

  val intrf_mem1       =    SyncReadMem(8, UInt(xLen.W))
  val fprf_mem1        =    SyncReadMem(8, UInt(xLen.W))
  val intrf_mem2       =    SyncReadMem(8, UInt(xLen.W))
  val fprf_mem2        =    SyncReadMem(8, UInt(xLen.W))
  val intrf_mem3       =    SyncReadMem(8, UInt(xLen.W))
  val fprf_mem3        =    SyncReadMem(8, UInt(xLen.W))
  val intrf_mem4       =    SyncReadMem(8, UInt(xLen.W))
  val fprf_mem4        =    SyncReadMem(8, UInt(xLen.W))

   // val intreg           =    Reg(Vec(32,UInt(64.W)))
   // val fpreg            =    Reg(Vec(32,UInt(64.W)))
   // val intreg_test      =    Reg(Vec(32,UInt(64.W)))
   // val fpreg_test       =    Reg(Vec(32,UInt(64.W)))
   val intpcreg         =    RegInit(0.U(40.W))
   val fpcsrreg         =    RegInit(0.U(8.W))
    
   val copydone_reg      =    RegInit(0.U(1.W))
   val copydone_reg_test =    RegInit(0.U(1.W))
   val cansent           =    RegInit(0.U(1.W))
   
   val copyidx          =    Counter(32/4)

   dontTouch(copyidx.value)
   dontTouch(copydone_reg_test)
   dontTouch(cansent)
 
   io.copyidx := copyidx.value
   //copy request
   when(io.copy_valid_test){
      intpcreg := io.intpc_input
      fpcsrreg := io.fpcsr_input
      copyidx.inc()
      intrf_mem1(copyidx.value) := io.intreg_input_test(0)
      intrf_mem2(copyidx.value) := io.intreg_input_test(1)
      intrf_mem3(copyidx.value) := io.intreg_input_test(2)
      intrf_mem4(copyidx.value) := io.intreg_input_test(3)
      fprf_mem1(copyidx.value)  := io.fpreg_input_test(0)
      fprf_mem2(copyidx.value)  := io.fpreg_input_test(1)
      fprf_mem3(copyidx.value)  := io.fpreg_input_test(2)
      fprf_mem4(copyidx.value)  := io.fpreg_input_test(3)
      // for(i <- 0 until 4){
      // //   intreg_test(copyidx.value * 4.U + i.U) := io.intreg_input_test(i)
      // //   fpreg_test(copyidx.value * 4.U + i.U)  := io.fpreg_input_test(i)
      //   intrf_mem(copyidx.value * 4.U + i.U) := io.intreg_input_test(i)
      //   fprf_mem(copyidx.value * 4.U + i.U)  := io.fpreg_input_test(i)
      // }  
      when(copyidx.value === 7.U){
         copydone_reg_test := 1.U
      }
      cansent := 1.U
   }.otherwise{
      copyidx.reset()
   }

   // when(io.copy_valid){
   //    for(i <- 0 until 32){
   //      intreg(i) := io.intreg_input(i)
   //      fpreg(i)  := io.fpreg_input(i)
   //      // intrf_mem(i) := io.intreg_input(i)
   //      // fprf_mem(i)  := io.fpreg_input(i)
   //    }  
      
   //    copydone_reg := 1.U
   // }

   when(copydone_reg_test === 1.U){
      io.copy_done_test := true.B
      copydone_reg_test := 0.U
   }
   .otherwise{
      io.copy_done_test := false.B
   }

   when(cansent === 1.U){
      io.cansent := true.B
      cansent := 0.U
   }
   .otherwise{
      io.cansent := false.B
   }
   
   // when(copydone_reg === 1.U){
   //    io.copy_done := true.B
   //    copydone_reg := 0.U
   // }
   // .otherwise{
   //    io.copy_done := false.B
   // }
   
   // for(i <-0 until 32){
   //    dontTouch(intreg(i))
   //    dontTouch(fpreg(i))
   //    dontTouch(intreg_test(i))
   //    dontTouch(fpreg_test(i))
   // }
   
      dontTouch(intpcreg)
      dontTouch(fpcsrreg)
   
   //sent request
   
   //output test coun 
   // val testcoun =RegInit(0.U(8.W))
   // testcoun := Mux(io.sent_valid,testcoun+1.U,0.U)
   // dontTouch(testcoun)
   val testout = WireInit(0.U(256.W))
   dontTouch(testout)
   
   val seq_signal = RegInit(0.U(2.W))
    
   val data_sign = RegInit("b101".U(3.W))
   dontTouch(data_sign)
 
    val rf_idx    = Counter(32)
   // val idx_next  = Mux(rf_idx.value === 31.U, 0.U, rf_idx.value + 1.U)
   // val raddr     = WireDefault(Mux(io.sent_valid && seq_signal === 1.U, idx_next, rf_idx.value))
   // val fprf_rdata = fprf_mem.read(raddr)
   // val intrf_rdata   = intrf_mem.read(raddr)

   val mem_idx        = Counter(4)
   val cell_idx       = Counter(8)
   val cell_next      = Mux(cell_idx.value === 7.U, 0.U, cell_idx.value + 1.U)
   val cell_addr      = WireDefault(Mux(mem_idx.value === 3.U && io.sent_valid, cell_next, cell_idx.value))
   val fprf_rdata1    = fprf_mem1.read(cell_addr, io.sent_valid)
   val intrf_rdata1   = intrf_mem1.read(cell_addr, io.sent_valid)
   val fprf_rdata2    = fprf_mem2.read(cell_addr, io.sent_valid)
   val intrf_rdata2   = intrf_mem2.read(cell_addr, io.sent_valid)
   val fprf_rdata3    = fprf_mem3.read(cell_addr, io.sent_valid)
   val intrf_rdata3   = intrf_mem3.read(cell_addr, io.sent_valid)
   val fprf_rdata4    = fprf_mem4.read(cell_addr, io.sent_valid)
   val intrf_rdata4   = intrf_mem4.read(cell_addr, io.sent_valid)

   dontTouch(fprf_rdata1)
   dontTouch(intrf_rdata1)
   dontTouch(fprf_rdata2)
   dontTouch(intrf_rdata2)
   dontTouch(fprf_rdata3)
   dontTouch(intrf_rdata3)
   dontTouch(fprf_rdata4)
   dontTouch(intrf_rdata4)
 
   val rf_coun   = RegInit(0.U(5.W))
   val q_rf_coun = Wire(UInt())
   val rf_codo   = Wire(Bool())
   //val rf_testdo = Wire(Bool())
 
   val past_coun = RegInit(0.U(5.W))
   val past_codo = Wire(Bool())
   //val codo = Wire(Bool())
   
 
   q_rf_coun := rf_coun
   rf_codo   := (rf_coun === 31.U) && (seq_signal === 1.U) && io.sent_valid
   //rf_testdo := (rf_idx.value === 31.U) && (seq_signal === 1.U) && io.sent_valid
   past_codo := (past_coun === 1.U) && (seq_signal === 2.U) && io.sent_valid
   //codo := (seq_signal === 0.U).asBool
   //dontTouch(rf_testdo)
   //dontTouch(idx_next)
   //dontTouch(raddr)
   dontTouch(testout)
   dontTouch(cell_next)
   dontTouch(cell_addr)
 
   io.sign := seq_signal
   when(io.sent_valid){
      seq_signal := Mux(past_codo, 0.U, seq_signal)
      when(seq_signal === 0.U){
         io.sent_output := Cat(data_sign, seq_signal, 0.U(203.W), intpcreg, fpcsrreg)
         seq_signal     := seq_signal + 1.U
      }.elsewhen(seq_signal === 1.U){
         mem_idx.inc()
         rf_idx.inc()
         rf_coun        := Mux(rf_codo, 0.U, rf_coun + 1.U)
         //int_coun := Mux(int_codo , 0.U,int_coun+1.U)
         //io.sent_output := Cat(data_sign, seq_signal, rf_idx.value, 0.U(118.W), fprf_rdata, intrf_rdata)
         //testout        := Cat(data_sign, seq_signal, rf_idx.value, 0.U(118.W), fprf_rdata, intrf_rdata)
         seq_signal     := Mux(rf_codo, seq_signal + 1.U, seq_signal)
         when(mem_idx.value === 0.U){
            io.sent_output     := Cat(data_sign, seq_signal, rf_idx.value, 0.U(118.W), fprf_rdata1, intrf_rdata1)
         }.elsewhen(mem_idx.value === 1.U){
            io.sent_output     := Cat(data_sign, seq_signal, rf_idx.value, 0.U(118.W), fprf_rdata2, intrf_rdata2)
         }.elsewhen(mem_idx.value === 2.U){
            io.sent_output     := Cat(data_sign, seq_signal, rf_idx.value, 0.U(118.W), fprf_rdata3, intrf_rdata3)
         }.elsewhen(mem_idx.value === 3.U){
            io.sent_output     := Cat(data_sign, seq_signal, rf_idx.value, 0.U(118.W), fprf_rdata4, intrf_rdata4)
            cell_idx.inc()
         }
      }.elsewhen(seq_signal === 2.U){
          io.sent_output := Cat(data_sign, seq_signal, 0.U(251.W))
          past_coun      := Mux(past_codo, 0.U, past_coun + 1.U)
      }.otherwise{
         seq_signal := 0.U
         io.sent_output := 0.U 
      }
   }.otherwise{
      io.sent_output := 0.U
   }
   
   when(past_codo){
      io.sent_done := true.B
      seq_signal := 0.U
   }.otherwise{
      io.sent_done := false.B
   }                       
} 


