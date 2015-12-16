#
jein_load_db_data <- function(nrows=-1,at_home=FALSE)
{
    print(paste("Start ...", Sys.time()))
    #
    db_path = "/home/MRumore/analytics/db"
    db_name = "jein"
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
    TBL_201501290800_201501300800 =
        sqlite_load_table_from_db(db,
                                 "201501290800_201501300800",
                                  nrows=nrows)

    TBL_201501292100_201501302100 =
        sqlite_load_table_from_db(db,
                                 "201501292100_201501302100",
                                  nrows=nrows)

    TBL_TR201501290800_201501300800 =
        sqlite_load_table_from_db(db,
                                 "TR201501290800_201501300800",
                                  nrows=nrows)

    TBL_TR201501292100_201501302100 =
        sqlite_load_table_from_db(db,
                                 "TR201501292100_201501302100",
                                  nrows=nrows)
    #
    sqlite_close_db(db)
    #
    print(paste("End ...", Sys.time()))
    #
    return(list(
    "01290800_01300800" = TBL_201501290800_201501300800,
    "01292100_01302100" = TBL_201501292100_201501302100,
    "TR01290800_01300800" = TBL_TR201501290800_201501300800,
    "TR01292100_01302100" = TBL_TR201501292100_201501302100))
}
