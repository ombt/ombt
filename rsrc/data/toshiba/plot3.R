#
opar=par(no.readonly=TRUE)
#
attach(csvd)
#
eq1001 = orig_chart8$equipment_id == 1001
eq1003 = orig_chart8$equipment_id == 1003
eq1009 = orig_chart8$equipment_id == 1009
#
puc1001 = orig_chart8$pickup_count[eq1001]
puc1003 = orig_chart8$pickup_count[eq1003]
puc1009 = orig_chart8$pickup_count[eq1009]
#
plc1001 = orig_chart8$place_count[eq1001]
plc1003 = orig_chart8$place_count[eq1003]
plc1009 = orig_chart8$place_count[eq1009]
#
errc1001 = puc1001 - plc1001
errc1003 = puc1003 - plc1003
errc1009 = puc1009 - plc1009
#
mean_errc1001 = mean(errc1001)
mean_errc1003 = mean(errc1003)
mean_errc1009 = mean(errc1009)
sd_errc1001 = sd(errc1001)
sd_errc1003 = sd(errc1003)
sd_errc1009 = sd(errc1009)
#
# norm_errc1001 = sqrt(((errc1001-mean_errc1001)/sd_errc1001)^2)
# norm_errc1003 = sqrt(((errc1003-mean_errc1003)/sd_errc1003)^2)
# norm_errc1009 = sqrt(((errc1009-mean_errc1009)/sd_errc1009)^2)
#
norm_errc1001 = sqrt((errc1001-mean_errc1001)^2)/sd_errc1001
norm_errc1003 = sqrt((errc1003-mean_errc1003)^2)/sd_errc1003
norm_errc1009 = sqrt((errc1009-mean_errc1009)^2)/sd_errc1009
#
par(mfrow=c(3,2),
    col.lab="blue",
    col.main="red",
    col.axis="black",
    fg="green")
plot(puc1001,
     xlab = "board",
     ylab = "pickup",
     main = "EQ-ID=1001, Summary Pickup Counts")
plot(puc1003,
     xlab = "board",
     ylab = "pickup",
     main = "EQ-ID=1003, Summary Pickup Counts")
plot(puc1009,
     xlab = "board",
     ylab = "pickup",
     main = "EQ-ID=1009, Summary Pickup Counts")
plot(plc1001,
     xlab = "board",
     ylab = "place",
     main = "EQ-ID=1001, Summary Placement Counts")
plot(plc1003,
     xlab = "board",
     ylab = "place",
     main = "EQ-ID=1003, Summary Placement Counts")
plot(plc1009,
     xlab = "board",
     ylab = "place",
     main = "EQ-ID=1009, Summary Placement Counts")
#
x11()
par(mfrow=c(3,2),
    col.lab="blue",
    col.main="red",
    col.axis="black",
    fg="green")
plot(plc1001,
     puc1001,
     xlab = "place",
     ylab = "pickup",
     main = "EQ-ID=1001, Summary Place vs Pickup Counts")
plot(plc1001,
     puc1001,
     log = "xy",
     xlab = "place",
     ylab = "pickup",
     main = "EQ-ID=1001, Summary Place vs Pickup Counts")
plot(plc1003,
     puc1003,
     xlab = "place",
     ylab = "pickup",
     main = "EQ-ID=1003, Summary Place vs Pickup Counts")
plot(plc1003,
     puc1003,
     log = "xy",
     xlab = "place",
     ylab = "pickup",
     main = "EQ-ID=1003, Summary Place vs Pickup Counts")
plot(plc1009,
     puc1009,
     xlab = "place",
     ylab = "pickup",
     main = "EQ-ID=1009, Summary Place vs Pickup Counts")
plot(plc1009,
     puc1009,
     log = "xy",
     xlab = "place",
     ylab = "pickup",
     main = "EQ-ID=1009, Summary Place vs Pickup Counts")
#
x11()
par(mfrow=c(3,1),
    col.lab="blue",
    col.main="red",
    col.axis="black",
    fg="green")
#
plot(errc1001,
     xlab = "board",
     ylab = "pickup - place",
     main = "EQ-ID=1001, Summary Error Counts")
plot(errc1003,
     xlab = "board",
     ylab = "pickup - place",
     main = "EQ-ID=1003, Summary Error Counts")
plot(errc1009,
     xlab = "board",
     ylab = "pickup - place",
     main = "EQ-ID=1009, Summary Error Counts")
#
x11()
par(mfrow=c(3,1),
    col.lab="blue",
    col.main="red",
    col.axis="black",
    fg="green")
#
plot(norm_errc1001,
     xlab = "board",
     ylab = "sqrt(((err-mean)/sd)^2)",
     main = "EQ-ID=1001, Normalized Error Counts")
minx = 1
maxx = length(norm_errc1001)
xvals = seq(from=1,
            to=length(norm_errc1001),
            length.out=50)
yvals = seq(from=4.0*sd_errc1001,
            to=4.0*sd_errc1001,
            length.out=50)
lines(xvals, yvals, col="red", lty=2, lwd=2)
#
plot(norm_errc1003,
     xlab = "board",
     ylab = "sqrt(((err-mean)/sd)^2)",
     main = "EQ-ID=1003, Normalized Error Counts")
minx = 1
maxx = length(norm_errc1003)
xvals = seq(from=1,
            to=length(norm_errc1003),
            length.out=50)
yvals = seq(from=4.0*sd_errc1003,
            to=4.0*sd_errc1003,
            length.out=50)
lines(xvals, yvals, col="red", lty=2, lwd=2)
#
plot(norm_errc1009,
     xlab = "board",
     ylab = "sqrt(((err-mean)/sd)^2)",
     main = "EQ-ID=1009, Normalized Error Counts")
minx = 1
maxx = length(norm_errc1009)
xvals = seq(from=1,
            to=length(norm_errc1009),
            length.out=50)
yvals = seq(from=4.0*sd_errc1009,
            to=4.0*sd_errc1009,
            length.out=50)
lines(xvals, yvals, col="red", lty=2, lwd=2)
#
detach()
#
par(opar)


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
