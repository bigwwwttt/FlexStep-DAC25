package freechips.rocketchip.snapshot

import chisel3._
import chisel3.util._


class ISS extends Module{
  val io = IO(new Bundle{
    val copy_valid    =    Input(Bool())
    val sent_valid    =    Input(Bool())

    val copy_done     =    Output(Bool())
    val sent_done     =    Output(Bool())
    
    val intreg_input  =    Input(Vec(32, UInt(64.W)))
    val fpreg_input   =    Input(Vec(32, UInt(64.W)))
    val intpc_input   =    Input(UInt(40.W))
    val fpcsr_input   =    Input(UInt(8.W))

    val sign     =    Output(UInt(2.W))
     
    val sent_output  =    Output(UInt(256.W))

})
  val intreg           =    Reg(Vec(32,UInt(64.W)))
  val fpreg            =    Reg(Vec(32,UInt(64.W)))
  val intpcreg         =    RegInit(0.U(40.W))
  val fpcsrreg         =    RegInit(0.U(8.W))
   
  val copydone_reg     =    RegInit(0.U(1.W))

  //copy request
  when(io.copy_valid){
     for(i <- 0 until 32){
        intreg(i) := io.intreg_input(i)
        fpreg(i) := io.fpreg_input(i)
     }  
     intpcreg := io.intpc_input
     fpcsrreg := io.fpcsr_input
     copydone_reg := 1.U
  } 
  .otherwise{
        for(i <-0 until 32){
        intreg(i) := intreg(i)
        fpreg(i) := fpreg(i)   
     }
     intpcreg := intpcreg
     fpcsrreg := fpcsrreg
  }
  
  when(copydone_reg === 1.U){
     io.copy_done := true.B
     copydone_reg := 0.U
  }
  .otherwise{
     io.copy_done := false.B
  }
  
  for(i <-0 until 32){
     dontTouch(intreg(31-i))
     dontTouch(fpreg(i))
     }  
     dontTouch(intpcreg)
     dontTouch(fpcsrreg)
  
  //sent request
  
  //output test coun 
  val testcoun =RegInit(0.U(8.W))
  testcoun := Mux(io.sent_valid,testcoun+1.U,0.U)
  dontTouch(testcoun)
  
  val seq_signal = RegInit(0.U(2.W))

  val data_sign = RegInit("b101".U(3.W))
  dontTouch(data_sign)
  
  val int_coun = RegInit(0.U(5.W))
  val q_int_coun = Wire(UInt())
  val int_codo = Wire(Bool())
  
  val fp_coun = RegInit(0.U(5.W))
  val q_fp_coun = Wire(UInt())
  val fp_codo = Wire(Bool())
  
  val codo = Wire(Bool())
  
  q_int_coun := int_coun
  q_fp_coun := fp_coun
  int_codo := (int_coun===31.U).asBool && (seq_signal === 0.U).asBool
  fp_codo := (fp_coun===31.U).asBool && (seq_signal === 1.U).asBool
  codo := (seq_signal === 2.U).asBool

  io.sign := seq_signal
  when(io.sent_valid){
     seq_signal := Mux(int_codo || fp_codo || codo ,seq_signal+1.U , seq_signal)
     when(seq_signal === 0.U){
        int_coun := Mux(int_codo , 0.U,int_coun+1.U)
        io.sent_output := Cat(data_sign, seq_signal, int_coun, 0.U(182.W), intreg(q_int_coun))
     }
     .elsewhen(seq_signal === 1.U){
        fp_coun := Mux(fp_codo , 0.U,fp_coun+1.U)
        io.sent_output := Cat(data_sign, seq_signal, fp_coun, 0.U(182.W), fpreg(q_fp_coun))
     }
     .elsewhen(seq_signal === 2.U){
        io.sent_output := Cat(data_sign, seq_signal, 0.U(203.W), intpcreg,fpcsrreg)
     }
     .otherwise{
        seq_signal := 0.U
        io.sent_output := 0.U 
     }
  }
  .otherwise{
     io.sent_output := 0.U
     seq_signal := 0.U
  }
  
  when(codo){
     io.sent_done := true.B
  }
  .otherwise{
     io.sent_done := false.B
  }                       
}


