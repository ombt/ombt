select * from epv_admin.sp_usage_log order by 1 desc

epv_admin.prodn_rep_by_mach	@p_hr_expr = NULL, @p_start_time = '1422540000', @p_end_time = '1422626400', @p_mach_id_list = '1012,1014', @p_stage_list = '1,2', @p_setup_id_list = NULL, @p_pareto_order = NULL, @p_pareto_width = NULL, @p_pareto_column = NULL, @p_shift_id_list = NULL, @p_cal_start_time_list = NULL, @p_cal_end_time_list = NULL, @p_cal_legend_list = NULL, @p_group_by_list = '3,44,45', @p_lane_list = '1,2', @p_is_pa_based = 'T', @p_is_current_product = NULL, @p_expr_ids = '124101,124102,124103,124104,124105,124106,124107,124108,124109,124110,124111,124112,124113,124114,124115,124116', @p_sum_across_machines = NULL, @p_dst_value = NULL, @p_time_list = NULL, @p_bias = NULL, @p_job_id_list = NULL

SELECT pr.equipment_id AS EQUIP_ID, pr.lane_no,pr.stage_no,SUM(ISNULL(TOTAL_DOWN_TIME,0)),
SUM(ISNULL(PREV_PROC_WAIT_TIME,0)),SUM(ISNULL(NEXT_PROC_WAIT_TIME,0)),
SUM(ISNULL(SINGL_CYCL_STOP_TIME,0)),SUM(ISNULL(CHIP_SUPP_WAIT_TIME,0)),
SUM(ISNULL(SLDR_CTR_WAIT_TIME,0)),SUM(ISNULL(SLDR_SNSR_WAIT_TIME,0)),SUM(ISNULL(CLN_PAPER_WAIT_TIME,0)),
SUM(ISNULL(brcg_time,0)),SUM(ISNULL(cperr_count,0)),SUM(ISNULL(dataedit,0)),SUM(ISNULL(mente,0)),
SUM(ISNULL(othrstop_count,0)),SUM(ISNULL(prodview,0)),SUM(ISNULL(unitadjust,0)),SUM(ISNULL(REAL_RUN_TIME,0)) 
FROM  production_reports_view PR JOIN machines M  ON M.equipment_id  = PR.equipment_id  
JOIN all_equipment_view E  ON E.equipment_id = M.equipment_id  
JOIN routes R  ON R.route_id = E.route_id  
WHERE  pr.end_time  >  @start_time  AND   pr.end_time <=  @end_time  
AND PR.equipment_id IN ( 1012, 1014 )  AND PR.LANE_NO IN ( 1,2 )  GROUP BY pr.equipment_id,pr.lane_no,pr.stage_no
