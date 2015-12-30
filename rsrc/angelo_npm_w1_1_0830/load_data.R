#
sqlite_load_angelo_npm_w1_1_0830 <- function(nrows=0, at.home=FALSE)
{
    Sys.time()
    #
    db_path = "/home/MRumore/analytics/db"
    db_name = "angelo_npm_w1_1_0830"
    #
    if ((at.home) ||
        (getwd() == "/home/ombt/analytics/rdata/angelo_npm_w1_1_0830"))
    {
        db_path = "/home/ombt/analytics/db"
        db_name = "angelo_npm_w1_1_0830"
    }
    #
    Sys.time()
    db = sqlite_open_db(db_path, db_name)
    #
    tbls = c("BRecg",
             "FILENAME_TO_IDS",
             "InspectionData",
             "MountPickupFeeder",
             "Count",
             "HeightCorrect",
             "MountExchangeReel",
             "MountPickupNozzle",
             "CycleTime",
             "Index",
             "MountLatestReel",
             "MountQualityTrace",
             "FID_DATA",
             "Information",
             "MountNormalTrace",
             "Time")
    #
    data = list()
    #
    Sys.time()
    for (tbl in tbls)
    {
        Sys.time()
        print(paste("reading table", tbl))
        data[[tbl]] = sqlite_load_table_from_db(db,
                                                tbl,
                                                nrows=nrows)
    }
    #
    Sys.time()
    sqlite_close_db(db)
    #
    Sys.time()
    return(data)
}
