command to run :

eventShow.exe -G mCAPTAIN -tubdaq -n 1 -s 1 input_file.ubdaq.gz

-G must be used to upload geometry file

-tudaq must be used to read daq output files

-n tells how many events one wants to read (I suggest 1-3 at the time, but can be any)

-s tells how many events need to be skipped (do not need to be used at all)


For each event the output is 3 *.png files for x, u, and v wire planes which show ads values on each wire in the event
