-- select * from epv_admin.sp_usage_log order by 1 desc go

-- epv_admin.prodn_rep_by_mach 
--     @p_hr_expr = null, 
--     @p_start_time = '1422540000', 
--     @p_end_time = '1422626400', 
--     @p_mach_id_list = '1012,1014', 
--     @p_stage_list = '1,2', 
--     @p_setup_id_list = null, 
--     @p_pareto_order = null, 
--     @p_pareto_width = null, 
--     @p_pareto_column = null, 
--     @p_shift_id_list = null, 
--     @p_cal_start_time_list = null, 
--     @p_cal_end_time_list = null, 
--     @p_cal_legend_list = null, 
--     @p_group_by_list = '3,44,45', 
--     @p_lane_list = '1,2', 
--     @p_is_pa_based = 't', 
--     @p_is_current_product = null, 
--     @p_expr_ids = '124101,124102,124103,124104,124105,124106,124107,124108,124109,124110,124111,124112,124113,124114,124115,124116', 
--     @p_sum_across_machines = null, 
--     @p_dst_value = null, 
--     @p_time_list = null, 
--     @p_bias = null, 
--     @p_job_id_list = null
-- go
-- 
select 
    pr.equipment_id as equip_id, 
    pr.lane_no as lane_no,
    pr.stage_no as stage_no,
    sum(isnull(total_down_time,0)) as total_down_time,
    sum(isnull(prev_proc_wait_time,0)) as prev_proc_wait_time,
    sum(isnull(next_proc_wait_time,0)) as next_proc_wait_time,
    sum(isnull(singl_cycl_stop_time,0)) as singl_cycl_stop_time,
    sum(isnull(chip_supp_wait_time,0)) as chip_supp_wait_time,
    sum(isnull(sldr_ctr_wait_time,0)) as sldr_ctr_wait_time,
    sum(isnull(sldr_snsr_wait_time,0)) as sldr_snsr_wait_time,
    sum(isnull(cln_paper_wait_time,0)) as cln_paper_wait_time,
    sum(isnull(brcg_time,0)) as brcg_time,
    sum(isnull(cperr_count,0)) as cperr_count,
    sum(isnull(dataedit,0)) as dataedit,
    sum(isnull(mente,0)) as mente,
    sum(isnull(othrstop_count,0)) as othrstop_count,
    sum(isnull(prodview,0)) as prodview,
    sum(isnull(unitadjust,0)) as unitadjust,
    sum(isnull(real_run_time,0)) as real_run_time 
from  
    production_reports_view pr 
join 
    machines m  
on 
    m.equipment_id  = pr.equipment_id  
join 
    all_equipment_view e  
on 
    e.equipment_id = m.equipment_id  
join 
    routes r  
on 
    r.route_id = e.route_id  
where  
    pr.end_time  >  1422540000
and   
    pr.end_time <=  1422626400
and 
    pr.equipment_id in ( 1012, 1014 )  
and 
    pr.lane_no in ( 1,2 )  
group by 
    pr.equipment_id,
    pr.lane_no,
    pr.stage_no
go

