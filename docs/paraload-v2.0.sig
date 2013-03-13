################################################################################
#
#                             paraload.sig (v2.0)
# 
#   Format:
#     Only first two or three useful lines are used.
#       1st line: CFG
#       2nd line: ZOOM TIME RANGE
#       3rd line: TRACE (optional!)
#     Non useful lines: empty, commented or only spaces or tabs.
#     Allowed indentation.
#
#   How to use it:
#     When wxparaver receives a SIGUSRx this file is read and for:
#     * SIGUSR1 -> the cfg file in the 1st line is loaded and
#                  the zoom defined in the 2nd line is applied to the last
#                  created window by this cfg.
#     * SIGUSR2 -> the zoom defined in the 2nd line is applied to the
#                  selected window in wxparaver.
#     In both cases, if 3rd line declaring the trace file to use is:
#     * missing -> the selected trace in wxparaver is used. 
#     * present -> the trace is loaded or not depending on if it isn't
#                  currently loaded or it is.
#     Writing only the trace name is allowed, but may lead to confusions if you 
#     loaded different traces from different paths but named them the same way (!?).
#     To properly identify a trace, please write it with the full path.
#
#   Restrictions:
#     This file must be placed in your $HOME.
#     Only for wxparaver > 3.99 (not for paraver 3.0).
#     Only for GNU/Linux (not available for MS Windows).
#
################################################################################
 
# 1st line: CFG
  /opt/cepbatools/cfgs/General/views/user_functions.cfg
     
# 2nd line: ZOOM TIME RANGE
  80000000:120000000     
          
# 3rd line: TRACE (optional!)
  /home/pgonzalez/bsc/traces/bugs/2d/inclusive-exclusive/trace.prv.CPUDurBurst.chop2.prv    
#trace.prv.CPUDurBurst.chop2.prv

