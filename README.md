     This code adds communicating channels between one master core and multiple slave cores. We add a globalparams.scala file at generators/rocket-chip/src/main/scala/util/globalparams.scala to manage some important parameters:
          object GlobalParams {
            val Num_cores = 8;
            val Num_Slavecores = Num_cores - 1
            var Master_core = 0
            val List_hartid = (0 until Num_cores by 1).toList
            var List_Slaveid = List_hartid.filter(_ != Master_core)
            val Data_width = 32
            val Data_type = UInt(Data_width.W)
            val depth = 32
          }

     
     When the number of slave cores changes, the number of sels signals also needs to change. The sels singals are controlled by reg_sels at class Roctet(generators/rocket-chip/src/main/scala/rocket/RocketCore.scala)
