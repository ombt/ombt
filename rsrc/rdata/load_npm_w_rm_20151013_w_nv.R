#
sqlite_load_npm_w_rm_w_nv <- function(nrows=-1)
{
    print(paste("Start ...", Sys.time()))
    #
    db_path = "/home/MRumore/analytics/db"
    db_name = "npm-w-rm"
    #
    db = sqlite_open_db(db_path, db_name)
    #
    print(db$tbls)
    #
    data_BRecg = 
        sqlite_load_table_from_db(db,"BRecg",nrows=nrows)
    data_Count = 
        sqlite_load_table_from_db(db,"Count",nrows=nrows)
    data_CycleTime = 
        sqlite_load_table_from_db(db,"CycleTime",nrows=nrows)
    data_FID_DATA = 
        sqlite_load_table_from_db(db,"FID_DATA",nrows=nrows)
    data_FILENAME_TO_IDS = 
        sqlite_load_table_from_db(db,"FILENAME_TO_IDS",nrows=nrows)
    data_Index = 
        sqlite_load_table_from_db(db,"Index",nrows=nrows)
    data_Information = 
        sqlite_load_table_from_db(db,"Information",nrows=nrows)
    data_InspectionData = 
        sqlite_load_table_from_db(db,"InspectionData",nrows=nrows)
    data_MountLatestReel = 
        sqlite_load_table_from_db(db,"MountLatestReel",nrows=nrows)
    data_MountNormalTrace = 
        sqlite_load_table_from_db(db,"MountNormalTrace",nrows=nrows)
    data_MountPickupFeeder = 
        sqlite_load_table_from_db(db,"MountPickupFeeder",nrows=nrows)
    data_MountPickupNozzle = 
        sqlite_load_table_from_db(db,"MountPickupNozzle",nrows=nrows)
    data_MountQualityTrace = 
        sqlite_load_table_from_db(db,"MountQualityTrace",nrows=nrows)
    data_Time = 
        sqlite_load_table_from_db(db,"Time",nrows=nrows)
    #
    sqlite_close_db(db)
    #
    print(paste("End ...", Sys.time()))
    #
    return(list(
           "BRecg" = data_BRecg,
           "Count" = data_Count,
           "CycleTime" = data_CycleTime,
           "FID_DATA" = data_FID_DATA,
           "FILENAME_TO_IDS" = data_FILENAME_TO_IDS,
           "Index" = data_Index,
           "Information" = data_Information,
           "InspectionData" = data_InspectionData,
           "MountLatestReel" = data_MountLatestReel,
           "MountNormalTrace" = data_MountNormalTrace,
           "MountPickupFeeder" = data_MountPickupFeeder,
           "MountPickupNozzle" = data_MountPickupNozzle,
           "MountQualityTrace" = data_MountQualityTrace,
           "Time" = data_Time))
}
