-- select * from epv_admin.sp_usage_log order by 1 desc
-- 
-- epv_admin.prodn_rep_by_mach	
--     @p_hr_expr = NULL, 
--     @p_start_time = '1422540000', 
--     @p_end_time = '1422626400', 
--     @p_mach_id_list = '1012,1014', 
--     @p_stage_list = '1,2', 
--     @p_setup_id_list = NULL, 
--     @p_pareto_order = NULL, 
--     @p_pareto_width = NULL, 
--     @p_pareto_column = NULL, 
--     @p_shift_id_list = NULL, 
--     @p_cal_start_time_list = NULL, 
--     @p_cal_end_time_list = NULL, 
--     @p_cal_legend_list = NULL, 
--     @p_group_by_list = '3,44,45', 
--     @p_lane_list = '1,2', 
--     @p_is_pa_based = 'T', 
--     @p_is_current_product = NULL, 
--     @p_expr_ids = '124101,124102,124103,124104,124105,124106,124107,124108,124109,124110,124111,124112,124113,124114,124115,124116', 
--     @p_sum_across_machines = NULL, 
--     @p_dst_value = NULL, 
--     @p_time_list = NULL, 
--     @p_bias = NULL, 
--     @p_job_id_list = NULL

-- + TZ=Asia/Jakarta
-- + export TZ
-- + read dia
-- + cat
-- + date -d '01/29/2015 08:00:00' +%s
-- 1422493200
-- + read dia
-- + date -d '01/30/2015 08:00:00' +%s
-- 1422579600
-- + read dia
-- + date -d '01/30/2015 08:00:00' +%s
-- 1422579600
-- + read dia
-- + date -d '01/30/2015 10:00:00' +%s
-- 1422586800
-- + read dia
-- + cat
-- + read dia
-- + date -d@1422540000
-- Thu Jan 29 21:00:00 WIT 2015
-- + read dia
-- + date -d@1422626400
-- Fri Jan 30 21:00:00 WIT 2015
-- + read dia
-- + exit 0

-- print N'01/29/2015 08:00:00 1422493200 to 01/30/2015 08:00:00 1422579600'
-- select 
--     pr.equipment_id as equip_id, 
--     pr.lane_no as lane_no,
--     pr.stage_no as stage_no,
--     sum(isnull(total_down_time,0)) as total_down_time,
--     sum(isnull(prev_proc_wait_time,0)) as prev_proc_wait_time,
--     sum(isnull(next_proc_wait_time,0)) as next_proc_wait_time,
--     sum(isnull(singl_cycl_stop_time,0)) as singl_cycl_stop_time,
--     sum(isnull(chip_supp_wait_time,0)) as chip_supp_wait_time,
--     sum(isnull(sldr_ctr_wait_time,0)) as sldr_ctr_wait_time,
--     sum(isnull(sldr_snsr_wait_time,0)) as sldr_snsr_wait_time,
--     sum(isnull(cln_paper_wait_time,0)) as cln_paper_wait_time,
--     sum(isnull(brcg_time,0)) as brcg_time,
--     sum(isnull(cperr_count,0)) as cperr_count,
--     sum(isnull(dataedit,0)) as dataedit,
--     sum(isnull(mente,0)) as mente,
--     sum(isnull(othrstop_count,0)) as othrstop_count,
--     sum(isnull(prodview,0)) as prodview,
--     sum(isnull(unitadjust,0)) as unitadjust,
--     sum(isnull(real_run_time,0)) as real_run_time 
-- from  
--     production_reports_view pr 
-- join 
--     machines m  
-- on 
--     m.equipment_id  = pr.equipment_id  
-- join 
--     all_equipment_view e  
-- on 
--     e.equipment_id = m.equipment_id  
-- join 
--     routes r  
-- on 
--     r.route_id = e.route_id  
-- where  
--     pr.end_time  >  1422493200
-- and   
--     pr.end_time <=  1422579600
-- and 
--     pr.equipment_id in ( 1012, 1014 )  
-- and 
--     pr.lane_no in ( 1,2 )  
-- group by 
--     pr.equipment_id,
--     pr.lane_no,
--     pr.stage_no
-- go

-- SELECT 
--     PRH.equipment_id,
--     PRH.lane_no,
--     PR.stage_no,
--     sum(PR.total_down_time) as total_down_time,
--     sum(PR.prev_proc_wait_time) as prev_proc_wait_time,
--     sum(PR.next_proc_wait_time) as next_proc_wait_time,
--     sum(PR.singl_cycl_stop_time) as singl_cycl_stop_time,
--     sum(PR.chip_supp_wait_time) as chip_supp_wait_time,
--     sum(PR.sldr_ctr_wait_time) as sldr_ctr_wait_time,
--     sum(PR.sldr_snsr_wait_time) as sldr_snsr_wait_time,
--     sum(PR.cln_paper_wait_time) as cln_paper_wait_time,
--     0 as brcg_time,
--     0 as cperr_count,
--     0 as dataedit,
--     0 as mente,
--     0 as othrstop_count,
--     0 as prodview,
--     0 as unitadjust,
--     sum(PR.real_run_time) as real_run_time,
--     (sum(PR.total_down_time) + 
--      sum(PR.prev_proc_wait_time) + 
--      sum(PR.next_proc_wait_time) + 
--      sum(PR.singl_cycl_stop_time) + 
--      sum(PR.chip_supp_wait_time) + 
--      sum(PR.sldr_ctr_wait_time) + 
--      sum(PR.sldr_snsr_wait_time) + 
--      sum(PR.cln_paper_wait_time) + 
--      sum(PR.real_run_time))/(60*60) as grand_total_in_hours,
--     NULL as last_col
-- FROM 
--     dbo.production_reports_kx_hdr PRH
-- JOIN 
--     dbo.production_reports_kx PR
-- ON 
--     PR.report_id  = PRH.report_id
-- join 
--     machines m  
-- on 
--     m.equipment_id  = PRH.equipment_id  
-- join 
--     all_equipment_view e  
-- on 
--     e.equipment_id = m.equipment_id  
-- join 
--     routes r  
-- on 
--     r.route_id = e.route_id  
-- where  
--     PRH.end_time  >  1422493200
-- and   
--     PRH.end_time <=  1422579600
-- and 
--     PRH.equipment_id in ( 1012, 1014 )  
-- and 
--     PRH.lane_no in ( 1,2 )  
-- and
--     PR.real_run_time <= 500
-- group by 
--     PRH.equipment_id,
--     PRH.lane_no,
--     PR.stage_no
-- order by 
--     PRH.equipment_id,
--     PRH.lane_no,
--     PR.stage_no
-- go
-- 
-- SELECT 
--     PRH.equipment_id,
--     PRH.lane_no,
--     PR.stage_no,
--     PR.total_down_time,
--     PR.prev_proc_wait_time,
--     PR.next_proc_wait_time,
--     PR.singl_cycl_stop_time,
--     PR.chip_supp_wait_time,
--     PR.sldr_ctr_wait_time,
--     PR.sldr_snsr_wait_time,
--     PR.cln_paper_wait_time,
--     0 as brcg_time,
--     0 as cperr_count,
--     0 as dataedit,
--     0 as mente,
--     0 as othrstop_count,
--     0 as prodview,
--     0 as unitadjust,
--     PR.real_run_time,
--     NULL as last_col
-- FROM 
--     dbo.production_reports_kx_hdr PRH
-- JOIN 
--     dbo.production_reports_kx PR
-- ON 
--     PR.report_id  = PRH.report_id
-- join 
--     machines m  
-- on 
--     m.equipment_id  = PRH.equipment_id  
-- join 
--     all_equipment_view e  
-- on 
--     e.equipment_id = m.equipment_id  
-- join 
--     routes r  
-- on 
--     r.route_id = e.route_id  
-- where  
--     PRH.end_time  >  1422493200
-- and   
--     PRH.end_time <=  1422579600
-- and 
--     PRH.equipment_id in ( 1012, 1014 )  
-- and 
--     PRH.lane_no in ( 1,2 )  
-- and
--     PR.real_run_time > 500
-- order by 
--     PRH.equipment_id,
--     PRH.lane_no,
--     PR.stage_no
-- go

SELECT 
    PRH.equipment_id,
    PRH.lane_no,
    PR.stage_no,
    PR.total_down_time,
    PR.prev_proc_wait_time,
    PR.next_proc_wait_time,
    PR.singl_cycl_stop_time,
    PR.chip_supp_wait_time,
    PR.sldr_ctr_wait_time,
    PR.sldr_snsr_wait_time,
    PR.cln_paper_wait_time,
    PR.real_run_time,
    case when PR.real_run_time > 500
    then
        'NOT_OK'
    else
        'OK'
    end as STATUS,
    PRH.report_file,
    RAWPRH.equipment_id,
    RAWPRH.lane_no,
    RAWPR.stage_no,
    RAWPRH.product_id,
    RAWPRH.setup_id,
    RAWPR.total_down_time,
    RAWPR.prev_proc_wait_time,
    RAWPR.next_proc_wait_time,
    RAWPR.singl_cycl_stop_time,
    RAWPR.chip_supp_wait_time,
    RAWPR.sldr_ctr_wait_time,
    RAWPR.sldr_snsr_wait_time,
    RAWPR.cln_paper_wait_time,
    RAWPR.real_run_time,
    RAWPRH.report_file,
    NULL as last_col
FROM 
    dbo.production_reports_kx_hdr PRH
JOIN 
    dbo.production_reports_kx PR
ON 
    PR.report_id  = PRH.report_id
join 
    machines m  
on 
    m.equipment_id  = PRH.equipment_id  
join 
    all_equipment_view e  
on 
    e.equipment_id = m.equipment_id  
join 
    routes r  
on 
    r.route_id = e.route_id  
left join
    dbo.production_reports_kx_hdr_raw RAWPRH
on
    RAWPRH.equipment_id = PRH.equipment_id
and
    RAWPRH.lane_no = PRH.lane_no
left join 
    dbo.production_reports_kx_raw RAWPR
ON 
    RAWPR.report_id  = RAWPRH.report_id
and
    RAWPR.stage_no  = PR.stage_no
where  
    PRH.end_time  >  1422493200
and   
    PRH.end_time <=  1422579600
and 
    PRH.equipment_id in ( 1012, 1014 )  
and 
    PRH.lane_no in ( 1,2 )  
-- and
    -- PR.real_run_time > 300
and
    RAWPRH.end_time  >  PRH.start_time
and   
    RAWPRH.end_time <=  PRH.end_time
order by 
    PRH.equipment_id,
    PRH.lane_no,
    PR.stage_no,
    PRH.end_time
go

