#
attach(csvd)
#
eq1001 = orig_chart8$equipment_id == 1001
eq1003 = orig_chart8$equipment_id == 1003
eq1009 = orig_chart8$equipment_id == 1009
#
rpu1001 = orig_chart8$r_pickup_count[eq1001]
rpu1003 = orig_chart8$r_pickup_count[eq1003]
rpu1009 = orig_chart8$r_pickup_count[eq1009]
#
rrt1001 = orig_chart8$real_run_time[eq1001]
rrt1003 = orig_chart8$real_run_time[eq1003]
rrt1009 = orig_chart8$real_run_time[eq1009]
#
rrrt1001 = orig_chart8$r_real_run_time[eq1001]
rrrt1003 = orig_chart8$r_real_run_time[eq1003]
rrrt1009 = orig_chart8$r_real_run_time[eq1009]
#
par(mfrow=c(3,1));
plot(pu1001,
     main = "EQ-ID=1001, Summary Pickup Counts")
plot(pu1003,
     main = "EQ-ID=1003, Summary Pickup Counts")
plot(pu1009,
     main = "EQ-ID=1009, Summary Pickup Counts")
#
x11()
par(mfrow=c(3,1));
plot(rpu1001,
     main = "EQ-ID=1001, Raw Pickup Counts")
plot(rpu1003,
     main = "EQ-ID=1003, Raw Pickup Counts")
plot(rpu1009,
     main = "EQ-ID=1009, Raw Pickup Counts")
#
x11()
par(mfrow=c(3,1));
plot(rrt1001,
     main = "EQ-ID=1001, Summary Real Run Time")
plot(rrt1003,
     main = "EQ-ID=1003, Summary Real Run Time")
plot(rrt1009,
     main = "EQ-ID=1009, Summary Real Run Time")
#
x11()
par(mfrow=c(3,1));
plot(rrrt1001,
     main = "EQ-ID=1001, Raw Real Run Time")
plot(rrrt1003,
     main = "EQ-ID=1003, Raw Real Run Time")
plot(rrrt1009,
     main = "EQ-ID=1009, Raw Real Run Time")
#
detach()


# plot(csvd$orig_chart8$pickup_count)
# plot(csvd$orig_chart8$real_run_time)
# plot(csvd$orig_chart8$pickup_count)
# plot(csvd$orig_chart8$real_run_time)
# plot(csvd$orig_chart8$r_pickup_count)
# plot(csvd$orig_chart8$real_run_time)
# plot(csvd$orig_chart8$r_pickup_count)
# plot(csvd$orig_chart8$pickup_count)
# plot(csvd$orig_chart8$r_pickup_miss_count)
# plot(csvd$orig_chart8$pickup_miss_count)
# plot(csvd$orig_chart8$r_place_count)
# plot(csvd$orig_chart8$r_place_count)
# detach()
# plot(csvd$orig_chart8$r_pickup_count-csvd$orig_chart8$r_place_count)
# plot(csvd$orig_chart8$pickup_count-csvd$orig_chart8$place_count)
# plot(csvd$orig_chart8$pickup_count)
# plot(csvd$orig_chart8$r_pickup_count)
# plot(csvd$orig_chart8$real_run_time)
# plot(csvd$orig_chart8$total_run_time-csvd$orig_chart8$real_run_time)
# plot(csvd$orig_chart8$r_total_run_time-csvd$orig_chart8$r_real_run_time)
# plot(csvd$orig_chart8$total_run_time-csvd$orig_chart8$real_run_time)
# plot(csvd$orig_chart8$r_pickup_count)
# plot(csvd$orig_chart8$pickup_count)
# plot(csvd$orig_chart8$pickup_count-csvd$orig_chart8$place_count)
# plot(csvd$orig_chart8$r_pickup_count-csvd$orig_chart8$r_place_count)
# 
# 
# ls()
# td = load_db_data()
# ts
# names(td)
# names(td$orig_chart8")
# names("td$orig_chart8")
# names(td$orig_chart8)
# td$orig_chart8[[pick_count>20000]]
# td$orig_chart8[[td$orig_chart8$pick_count>20000]]
# td$orig_chart8[[td$orig_chart8$pick_count>1000]]
# td$orig_chart8[td$orig_chart8$pick_count>1000]
# td$orig_chart8
# names(td$orig_chart8)
# attach(td)
# orig_chart8[orig_chart8$pickup_count>1000,]
# orig_chart8[orig_chart8$pickup_count>2000,]
# orig_chart8[orig_chart8$pickup_count>1000000,]
# orig_chart8$pickup_count
# typeof(orig_chart8)
# typeof(orig_chart8[1,1:10])
# names(orig_chart8)
# typeof(orig_chart8[["place_count"]]
# typeof(orig_chart8[["place_count"]])
# c=orig_chart8[["place_count"]]
# typeof(c)
# c=as.numeric(c)
# typeof(c)
# summary(orig_chart8)
# q()
# ls()
# rm(list=ls())
# q()
# ls()
# q()
# ls()
# csvd=load_csv_data()
# csvd
# names(csvd)
# summary(csvd)
# summary(csvd$orig_chart8)
# q(0
# ;
# q()
# summary(csvd$orig_chart8)
# names(csvd$orig_chart8)
# attach(csvd$orig_chart8)
# lane_no
# ls()
# names(csvd$orig_chart8)
# typeif(pickup_counts)
# typeof(pickup_counts)
# typeof(pickup_count)
# pickup_count[pickup_count>1000]
# pickup_count[pickup_count>5000]
# pickup_count>5000
# pickup_count[pickup_count>5000]
# ls()
# pickup_count[pickup_count>5000]
# STATUS[STATUS == "NOT OK"]
# class(STATUS
# STATUS[STATUS == 0]
# detach()
# STATUS[STATUS == 0]
# ls()
# names(csvd)
# names(csvd$orig_chart8)
# plot(pickup_counts)
# plot(csvd$orig_chart8$pickup_count)
# plot(csvd$orig_chart8$r_pickup_count)
# plot(csvd$orig_chart8$pickup_count)
# plot(csvd$orig_chart8$real_run_time)
# plot(csvd$orig_chart8$pickup_count)
# plot(csvd$orig_chart8$real_run_time)
# plot(csvd$orig_chart8$r_pickup_count)
# plot(csvd$orig_chart8$real_run_time)
# plot(csvd$orig_chart8$r_pickup_count)
# plot(csvd$orig_chart8$pickup_count)
# plot(csvd$orig_chart8$r_pickup_miss_count)
# plot(csvd$orig_chart8$pickup_miss_count)
# plot(csvd$orig_chart8$r_place_count)
# plot(csvd$orig_chart8$r_place_count)
# detach()
# plot(csvd$orig_chart8$r_pickup_count-csvd$orig_chart8$r_place_count)
# plot(csvd$orig_chart8$pickup_count-csvd$orig_chart8$place_count)
# plot(csvd$orig_chart8$pickup_count)
# plot(csvd$orig_chart8$r_pickup_count)
# plot(csvd$orig_chart8$real_run_time)
# plot(csvd$orig_chart8$total_run_time-csvd$orig_chart8$real_run_time)
# plot(csvd$orig_chart8$r_total_run_time-csvd$orig_chart8$r_real_run_time)
# plot(csvd$orig_chart8$total_run_time-csvd$orig_chart8$real_run_time)
# plot(csvd$orig_chart8$r_pickup_count)
# plot(csvd$orig_chart8$pickup_count)
# plot(csvd$orig_chart8$pickup_count-csvd$orig_chart8$place_count)
# plot(csvd$orig_chart8$r_pickup_count-csvd$orig_chart8$r_place_count)
# plot((csvd$orig_chart8$r_pickup_count-csvd$orig_chart8$r_place_count)/csvd$orig_chart8$r_pickup_count)
# plot((csvd$orig_chart8$pickup_count-csvd$orig_chart8$place_count)/csvd$orig_chart8$pickup_count)
# plot((csvd$orig_chart8$r_pickup_count-csvd$orig_chart8$r_place_count)/csvd$orig_chart8$r_pickup_count)
# help(savehistory)
# savehistory(file="toshiba.plot.cmds")
# q(0
# q()
# history()
# ls()
# names(csvd)
# names(csvd$orig_chart8)
# plot((csvd$orig_chart8$r_pickup_count-csvd$orig_chart8$r_place_count)/csvd$orig_chart8$r_pickup_count)
# plot(csvd$orig_chart8$r_pickup_count-csvd$orig_chart8$r_place_count)
# plot(csvd$orig_chart8$pickup_count)
# plot(csvd$orig_chart8$real_run_time)
# plot(csvd$orig_chart8$r_pickup_count)
# plot((csvd$orig_chart8$r_pickup_count-csvd$orig_chart8$r_place_count)/csvd$orig_chart8$r_pickup_count)
# plot((csvd$orig_chart8$r_pickup_count-csvd$orig_chart8$r_place_count))
# plot((csvd$orig_chart8$r_pickup_count-csvd$orig_chart8$r_place_count)/csvd$orig_chart8$r_pickup_count)
# plot((csvd$orig_chart8$r_pickup_count[csvd$orig_chart8$equiment_id==1003])
# plot((csvd$orig_chart8$r_pickup_count[csvd$orig_chart8$equiment_id==1003))
# plot((csvd$orig_chart8$r_pickup_count[csvd$orig_chart8$equiment_id==1003)
# plot((csvd$orig_chart8$r_pickup_count[csvd$orig_chart8$equiment_id==1003]))
# plot((csvd$orig_chart8$r_pickup_count[csvd$orig_chart8$r_equiment_id==1003]))
# csvd$orig_chart8$r_equiment_id
# csvd$orig_chart8$r_equipment_id
# plot((csvd$orig_chart8$r_pickup_count[csvd$orig_chart8$r_equipment_id==1003]))
# plot((csvd$orig_chart8$r_pickup_count[csvd$orig_chart8$r_equipment_id==1009]))
# plot((csvd$orig_chart8$pickup_count[csvd$orig_chart8$equipment_id==1009]))
# plot((csvd$orig_chart8$r_pickup_count[csvd$orig_chart8$r_equipment_id==1009]))
# plot((csvd$orig_chart8$r_pickup_count[csvd$orig_chart8$r_equipment_id==1006]))
# plot((csvd$orig_chart8$r_pickup_count[csvd$orig_chart8$r_equipment_id==1007]))
# plot((csvd$orig_chart8$r_pickup_count[csvd$orig_chart8$r_equipment_id==1011]))
# factors(csvd$orig_chart8$r_equipment_id)
# factor(csvd$orig_chart8$r_equipment_id)
# plot((csvd$orig_chart8$r_pickup_count-csvd$orig_chart8$r_place_count)/csvd$orig_chart8$r_pickup_count)
# plot((csvd$orig_chart8$r_pickup_count-csvd$orig_chart8$r_place_count)/csvd$orig_chart8$r_pickup_count)
# plot(csvd$orig_chart8$real_run_time)
# plot(csvd$orig_chart8$real_run_time[csvd$orig_chart8:$real_run_time>500])
# plot(csvd$orig_chart8$real_run_time[csvd$orig_chart8$real_run_time>500])
# plot(csvd$orig_chart8$real_run_time[csvd$orig_chart8$real_run_time>1000])
# plot(csvd$orig_chart8$real_run_time)
# plot(csvd$orig_chart8$real_run_time[csvd$orig_chart8$real_run_time>1000])
# plot(csvd$orig_chart8$real_run_time)
# plot(csvd$orig_chart8$real_run_time[csvd$orig_chart8$real_run_time>1000])
# plot(csvd$orig_chart8$real_run_time-1000)
# plot(csvd$orig_chart8$real_run_time-500)
# plot(csvd$orig_chart8$real_run_time-300)
# plot(csvd$orig_chart8$real_run_time[csvd$orig_chart8$real_run_time>1000])
# plot(csvd$orig_chart8$real_run_time)
# plot(csvd$orig_chart8$real_run_time[csvd$orig_chart8$real_run_time>1000])
# plot((csvd$orig_chart8$r_pickup_count-csvd$orig_chart8$r_place_count)/csvd$orig_chart8$r_pickup_count)
# plot((csvd$orig_chart8$r_pickup_count-csvd$orig_chart8$r_place_count)
# plot((csvd$orig_chart8$r_pickup_count-csvd$orig_chart8$r_place_count)
# plot((csvd$orig_chart8$r_pickup_count-csvd$orig_chart8$r_place_count)/csvd$orig_chart8$r_pickup_count)
# plot((csvd$orig_chart8$r_pickup_count-csvd$orig_chart8$r_place_count))
# plot((csvd$orig_chart8$r_pickup_count-csvd$orig_chart8$r_place_count)/csvd$orig_chart8$r_pickup_count)
# plot((csvd$orig_chart8$r_pickup_count)
# )
# plot((csvd$orig_chart8$pickup_count)
# )
# plot((csvd$orig_chart8$real_place_counts)
# )
# plot((csvd$orig_chart8$real_place_count))
# plot(csvd$orig_chart8$real_run_time)
# plot((csvd$orig_chart8$pickup_count)
# )
# q()
# ls()
# sink("out")
# names(csvd$orig_chart8)
# sink()
# q()
# ls()
# td = load_db_data()
# ts
# names(td)
# names(td$orig_chart8")
# names("td$orig_chart8")
# names(td$orig_chart8)
# td$orig_chart8[[pick_count>20000]]
# td$orig_chart8[[td$orig_chart8$pick_count>20000]]
# td$orig_chart8[[td$orig_chart8$pick_count>1000]]
# td$orig_chart8[td$orig_chart8$pick_count>1000]
# td$orig_chart8
# names(td$orig_chart8)
# attach(td)
# orig_chart8[orig_chart8$pickup_count>1000,]
# orig_chart8[orig_chart8$pickup_count>2000,]
# orig_chart8[orig_chart8$pickup_count>1000000,]
# orig_chart8$pickup_count
# typeof(orig_chart8)
# typeof(orig_chart8[1,1:10])
# names(orig_chart8)
# typeof(orig_chart8[["place_count"]]
# typeof(orig_chart8[["place_count"]])
# c=orig_chart8[["place_count"]]
# typeof(c)
# c=as.numeric(c)
# typeof(c)
# summary(orig_chart8)
# q()
# ls()
# rm(list=ls())
# q()
# ls()
# q()
# ls()
# csvd=load_csv_data()
# csvd
# names(csvd)
# summary(csvd)
# summary(csvd$orig_chart8)
# q(0
# ;
# q()
# summary(csvd$orig_chart8)
# names(csvd$orig_chart8)
# attach(csvd$orig_chart8)
# lane_no
# ls()
# names(csvd$orig_chart8)
# typeif(pickup_counts)
# typeof(pickup_counts)
# typeof(pickup_count)
# pickup_count[pickup_count>1000]
# pickup_count[pickup_count>5000]
# pickup_count>5000
# pickup_count[pickup_count>5000]
# ls()
# pickup_count[pickup_count>5000]
# STATUS[STATUS == "NOT OK"]
# class(STATUS
# STATUS[STATUS == 0]
# detach()
# STATUS[STATUS == 0]
# ls()
# names(csvd)
# names(csvd$orig_chart8)
# plot(pickup_counts)
# 
# plot(csvd$orig_chart8$pickup_count)
# plot(csvd$orig_chart8$r_pickup_count)
# plot(csvd$orig_chart8$pickup_count)
# plot(csvd$orig_chart8$real_run_time)
# plot(csvd$orig_chart8$pickup_count)
# plot(csvd$orig_chart8$real_run_time)
# plot(csvd$orig_chart8$r_pickup_count)
# plot(csvd$orig_chart8$real_run_time)
# plot(csvd$orig_chart8$r_pickup_count)
# plot(csvd$orig_chart8$pickup_count)
# plot(csvd$orig_chart8$r_pickup_miss_count)
# plot(csvd$orig_chart8$pickup_miss_count)
# plot(csvd$orig_chart8$r_place_count)
# plot(csvd$orig_chart8$r_place_count)
# detach()
# plot(csvd$orig_chart8$r_pickup_count-csvd$orig_chart8$r_place_count)
# plot(csvd$orig_chart8$pickup_count-csvd$orig_chart8$place_count)
# plot(csvd$orig_chart8$pickup_count)
# plot(csvd$orig_chart8$r_pickup_count)
# plot(csvd$orig_chart8$real_run_time)
# plot(csvd$orig_chart8$total_run_time-csvd$orig_chart8$real_run_time)
# plot(csvd$orig_chart8$r_total_run_time-csvd$orig_chart8$r_real_run_time)
# plot(csvd$orig_chart8$total_run_time-csvd$orig_chart8$real_run_time)
# plot(csvd$orig_chart8$r_pickup_count)
# plot(csvd$orig_chart8$pickup_count)
# plot(csvd$orig_chart8$pickup_count-csvd$orig_chart8$place_count)
# plot(csvd$orig_chart8$r_pickup_count-csvd$orig_chart8$r_place_count)
# plot((csvd$orig_chart8$r_pickup_count-csvd$orig_chart8$r_place_count)/csvd$orig_chart8$r_pickup_count)
# plot((csvd$orig_chart8$pickup_count-csvd$orig_chart8$place_count)/csvd$orig_chart8$pickup_count)
# plot((csvd$orig_chart8$r_pickup_count-csvd$orig_chart8$r_place_count)/csvd$orig_chart8$r_pickup_count)
# help(savehistory)
# savehistory(file="toshiba.plot.cmds")
#  [1] "equipment_id"           "lane_no"                "stage_no"              
#  [4] "pickup_count"           "pickup_miss_count"      "recog_miss_count"      
#  [7] "height_miss_count"      "place_count"            "boards_produced"       
# [10] "blocks_produced"        "total_down_time"        "prev_proc_wait_time"   
# [13] "next_proc_wait_time"    "singl_cycl_stop_time"   "chip_supp_wait_time"   
# [16] "sldr_ctr_wait_time"     "sldr_snsr_wait_time"    "cln_paper_wait_time"   
# [19] "real_run_time"          "total_run_time"         "STATUS"                
# [22] "report_file"            "r_equipment_id"         "r_lane_no"             
# [25] "r_stage_no"             "r_product_id"           "r_setup_id"            
# [28] "r_total_down_time"      "r_pickup_count"         "r_pickup_miss_count"   
# [31] "r_recog_miss_count"     "r_height_miss_count"    "r_place_count"         
# [34] "r_boards_produced"      "r_blocks_produced"      "r_prev_proc_wait_time" 
# [37] "r_next_proc_wait_time"  "r_singl_cycl_stop_time" "r_chip_supp_wait_time" 
# [40] "r_sldr_ctr_wait_time"   "r_sldr_snsr_wait_time"  "r_cln_paper_wait_time" 
# [43] "r_real_run_time"        "r_total_run_time"       "r_report_file"         
# [46] "last_col"              
