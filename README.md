# EcoSim-Default
EcoSim, an individual-based evolving predator-prey ecosystem simulation

To start a new EcoSim run, do the following:
-copy the source files into a directory
-set the parameter "Restore" in Parameters1.txt to "0"
-make the working directory the one with the EcoSim source files in it
-type "make" and hit enter, which builds EcoSim using the file "makefile"
-./EcoSim will then run EcoSim, starting a new run because Restore was set to 0

To continue an EcoSim run, do the following:
-ensure that there is a MaxSave file in the current working directory (MaxSave files are EcoSim restore points)
-set the parameter "Restore" in Parameters1.txt to "1"
-if EcoSim is built, ./EcoSim with then run EcoSim, continuing the run at the restore point held in the MaxSave
-otherwise, type "make" and hit enter, which builds EcoSim using the file "makefile" and subsequently run EcoSim with ./EcoSim

Refer to "EcoSim ODD.doc" for a full explanation of EcoSim according to the "Overview, Design concepts, and Details" protocol of Grimm et al. (2006) which was updated in 2010.
