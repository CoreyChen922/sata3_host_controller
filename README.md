# sata3_host_controller
It is SATA 3 host controller. Using this you can read write to sata3 sdd/hdd from your fpga logic with simple memory like interface.
This is SATA3 host controller is developped over my SATA2 host controller. The main change part is phy layer GTX part. And also have some changes in the wrapper part which encapsulates the sata interface to a simple memory like interface. If you need more details please contact me. 
This controller has a very good throughput. Write: 275MBps Read: 519MBps (Continous Read and Write) (Tested on SAMSUNG 250GB SATA3 SSD)
tested on FPGA: Kintex7 xc7k325t (KC705 board)
