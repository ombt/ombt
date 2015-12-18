
read_angelo_npm_w1_1 <- function(nrows=-1)
{
	csv_path = "/home/MRumore/analytics/csv/angelo_npm_w1_1/COMBINED"
	#
	print("read_BRecg(csv_path=csv_path,nrows=nrows)")
	data_BRecg = read_BRecg(csv_path=csv_path,nrows=nrows)
	#
	print("read_Count(csv_path=csv_path,nrows=nrows)")
	data_Count = read_Count(csv_path=csv_path,nrows=nrows)
	#
	print("read_CycleTime(csv_path=csv_path,nrows=nrows)")
	data_CycleTime = read_CycleTime(csv_path=csv_path,nrows=nrows)
	#
	print("read_FID_DATA(csv_path=csv_path,nrows=nrows)")
	data_FID_DATA = read_FID_DATA(csv_path=csv_path,nrows=nrows)
	#
	print("read_FILENAME_TO_IDS(csv_path=csv_path,nrows=nrows)")
	data_FILENAME_TO_IDS = read_FILENAME_TO_IDS(csv_path=csv_path,nrows=nrows)
	#
	print("read_HeightCorrect(csv_path=csv_path,nrows=nrows)")
	data_HeightCorrect = read_HeightCorrect(csv_path=csv_path,nrows=nrows)
	#
	print("read_Index(csv_path=csv_path,nrows=nrows)")
	data_Index = read_Index(csv_path=csv_path,nrows=nrows)
	#
	print("read_Information(csv_path=csv_path,nrows=nrows)")
	data_Information = read_Information(csv_path=csv_path,nrows=nrows)
	#
	print("read_InspectionData(csv_path=csv_path,nrows=nrows)")
	data_InspectionData = read_InspectionData(csv_path=csv_path,nrows=nrows)
	#
	print("read_MountExchangeReel(csv_path=csv_path,nrows=nrows)")
	data_MountExchangeReel = read_MountExchangeReel(csv_path=csv_path,nrows=nrows)
	#
	print("read_MountLatestReel(csv_path=csv_path,nrows=nrows)")
	data_MountLatestReel = read_MountLatestReel(csv_path=csv_path,nrows=nrows)
	#
	print("read_MountNormalTrace(csv_path=csv_path,nrows=nrows)")
	data_MountNormalTrace = read_MountNormalTrace(csv_path=csv_path,nrows=nrows)
	#
	print("read_MountPickupFeeder(csv_path=csv_path,nrows=nrows)")
	data_MountPickupFeeder = read_MountPickupFeeder(csv_path=csv_path,nrows=nrows)
	#
	print("read_MountPickupNozzle(csv_path=csv_path,nrows=nrows)")
	data_MountPickupNozzle = read_MountPickupNozzle(csv_path=csv_path,nrows=nrows)
	#
	print("read_MountQualityTrace(csv_path=csv_path,nrows=nrows)")
	data_MountQualityTrace = read_MountQualityTrace(csv_path=csv_path,nrows=nrows)
	#
	print("read_Time(csv_path=csv_path,nrows=nrows)")
	data_Time = read_Time(csv_path=csv_path,nrows=nrows)
	#
	return(list(
	    "BRecg" = data_BRecg,
	    "Count" = data_Count,
	    "CycleTime" = data_CycleTime,
	    "FID_DATA" = data_FID_DATA,
	    "FILENAME_TO_IDS" = data_FILENAME_TO_IDS,
	    "HeightCorrect" = data_HeightCorrect,
	    "Index" = data_Index,
	    "Information" = data_Information,
	    "InspectionData" = data_InspectionData,
	    "MountExchangeReel" = data_MountExchangeReel,
	    "MountLatestReel" = data_MountLatestReel,
	    "MountNormalTrace" = data_MountNormalTrace,
	    "MountPickupFeeder" = data_MountPickupFeeder,
	    "MountPickupNozzle" = data_MountPickupNozzle,
	    "MountQualityTrace" = data_MountQualityTrace,
	    "Time" = data_Time))
}
