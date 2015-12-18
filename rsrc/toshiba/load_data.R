#
load_db_data <- function(nrows=-1,at_home=FALSE)
{
    print(paste("Start ...", Sys.time()))
    #
    db_path = "/home/MRumore/analytics/rdata/toshiba"
    db_name = "CSV2DB"
    #
    if ((at_home) ||
        (dirname(getwd()) == "/home/ombt/sandbox/ombt/rsrc"))
    {
        db_path = "./"
        db_name = "CSV2DB"
    }
    #
    db = sqlite_open_db(db_path, db_name)
    #
    print(db$tbls)
    #
    orig_chart8_sql =
        sqlite_load_table_from_db(db,
                                 "orig_chart8_sql",
                                  nrows=nrows)
    #
    sqlite_close_db(db)
    #
    print(paste("End ...", Sys.time()))
    #
    return(list("orig_chart8" = orig_chart8_sql))
}

load_csv_data <- function(separator=",", nrows=-1)
{
    print(paste("Start ...", Sys.time()))
    #
    csv_path = "/home/MRumore/analytics/rdata/toshiba"
    csv_name = "orig_chart8.sql.csv"
    #
    character_col_classes = c(
        "STATUS",
        "report_file",
        "r_report_file",
        "last_col"
    )
    integer_col_classes = c(
        "equipment_id",
        "lane_no",
        "stage_no",
        "pickup_count",
        "pickup_miss_count",
        "recog_miss_count",
        "height_miss_count",
        "place_count",
        "boards_produced",
        "blocks_produced",
        "r_equipment_id",
        "r_lane_no",
        "r_stage_no",
        "r_product_id",
        "r_setup_id",
        "r_pickup_count",
        "r_pickup_miss_count",
        "r_recog_miss_count",
        "r_height_miss_count",
        "r_place_count",
        "r_boards_produced",
        "r_blocks_produced"
    )
    numeric_col_classes = c(
        "total_down_time",
        "prev_proc_wait_time",
        "next_proc_wait_time",
        "singl_cycl_stop_time",
        "chip_supp_wait_time",
        "sldr_ctr_wait_time",
        "sldr_snsr_wait_time",
        "cln_paper_wait_time",
        "real_run_time",
        "total_run_time",
        "r_total_down_time",
        "r_prev_proc_wait_time",
        "r_next_proc_wait_time",
        "r_singl_cycl_stop_time",
        "r_chip_supp_wait_time",
        "r_sldr_ctr_wait_time",
        "r_sldr_snsr_wait_time",
        "r_cln_paper_wait_time",
        "r_real_run_time",
        "r_total_run_time"
    )
    #
    csv_data <- read_csv_file(csv_path = csv_path, 
                              fname = csv_name,
                              character_cols = character_col_classes,
                              integer_cols = integer_col_classes,
                              numeric_cols = numeric_col_classes,
                              separator = separator,
                              nrows=nrows)
    #
    print(paste("End ...", Sys.time()))
    #
    return(list("orig_chart8" = csv_data))
}
