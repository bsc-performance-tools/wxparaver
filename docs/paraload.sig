################################################################################
#
#                             paraload.sig (v3.0)
# 
#   Format:
#       1st line: CFG (optional!)
#       2nd line: ZOOM TIME RANGE (begin:end)
#       3rd line: TRACE (optional!)
#       4th line: IMAGE (optional!)
#     Commented lines are not considered.
#     Allowed indentation.
#
#   How to use it:
#     When wxparaver receives a SIGUSRx this file is read and commands queued for:
#     * SIGUSR1 -> the cfg file in the 1st line is loaded and
#                  the zoom defined in the 2nd line is applied to the last
#                  created window by this cfg.
#     * SIGUSR2 -> the zoom defined in the 2nd line is applied to the
#                  selected window in wxparaver.
#     Begin and end time must be expressed in trace time units.
#     In both cases, if 3rd line declaring the trace file to use is:
#     * missing -> the selected trace in wxparaver is used. 
#     * present -> the trace is loaded or not depending on if it isn't
#                  currently loaded or it is.
#     In both cases, if 4th line declaring the output image file exist then the
#     zoomed window image will be saved.
#     Writing only the trace name is allowed, but may lead to confusions if you 
#     loaded different traces from different paths but named them the same way (!?).
#     To properly identify a cfg, trace or image file, please write it with the full path.
#
#   Restrictions:
#     This file must be placed in your $HOME.
#     Only for GNU/Linux (not available for MS Windows).
#
################################################################################
# 1st line: CFG (optional!)
  /opt/bsctools/cfgs/General/views/user_functions.cfg
# 2nd line: ZOOM TIME RANGE (begin:end)
  80000000:120000000     
# 3rd line: TRACE (optional!)
  /home/user/bsctools/traces/trace.chop2.prv    
# 4th line: IMAGE (optional!)
  /home/user/images/timeline_zoom.png
