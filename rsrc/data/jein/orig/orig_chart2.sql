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
-- @p_start_time = '1422540000', 
--    pr.end_time  >  @start_time 
    pr.end_time  >  1422540000
and   
--     @p_end_time = '1422626400', 
--     pr.end_time <=  @end_time 
    pr.end_time <=  1422626400 
and 
    pr.equipment_id in ( 1012, 1014 )  
and 
    pr.lane_no in ( 1,2 )  
group by 
    pr.equipment_id,
    pr.lane_no,
    pr.stage_no
