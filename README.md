# EcoSim-Default
EcoSim, an individual-based evolving predator-prey ecosystem simulation

To start a new EcoSim run, do the following:
1. copy the source files into a directory
2. set the parameter "Restore" in Parameters1.txt to "0"
3. make the working directory the one with the EcoSim source files in it
4. type "make" and hit enter, which builds EcoSim using the file "makefile"
5. ./EcoSim will then run EcoSim, starting a new run because Restore was set to 0

To continue an EcoSim run, do the following:
1. -ensure that there is a MaxSave file in the current working directory (MaxSave files are EcoSim restore points)
2. -set the parameter "Restore" in Parameters1.txt to "1"
3. -if EcoSim is built, ./EcoSim with then run EcoSim, continuing the run at the restore point held in the MaxSave
4. -otherwise, type "make" and hit enter, which builds EcoSim using the file "makefile" and subsequently run EcoSim with ./EcoSim

Refer to "EcoSim ODD.docx" for a full explanation of EcoSim according to the "Overview, Design concepts, and Details" protocol of Grimm et al. (2006) which was updated in 2010.
