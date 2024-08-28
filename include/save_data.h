#ifndef SAVE_DATA_H
#define SAVE_DATA_H

#include <common_structs.h>

typedef struct {
    // 6 little endian 3-byte records.
    // When converted to big endian the bottom 5 nibbles can be interpreted as the centisecond count for that record.
    // The top nibble is the character ID of the charcter used to set that record
    // The first 5 records are the top 5 3-lap records
    // The 6th record is the best 1-lap record
    u8 records[6][3];
    // It's unknown what these bytes are used for
    // Byte 1 might be an indicator that there is a ghost available for that course
    u8 unknownBytes[5];
    u8 checksum;
} CourseTimeTrialRecords; // size = 0x18

typedef struct {
    // Each cup is made up of 4 courses
    CourseTimeTrialRecords courseRecords[4];
} CupTimeTrialRecords; // size = 0x60

typedef struct {
    // There are 4 cups total
    CupTimeTrialRecords cupRecords[4];
} AllCourseTimeTrialRecords; // size = 0x180

typedef struct {
    // Records 0 through 3 are for the first cup's courses
    // Records 4 through 7 are for the second cup's courses
    u8 bestThreelaps[8][3];
    u8 bestSinglelaps[8][3];
    // It's unknown what these bytes are used for
    u8 unknownBytes[8];
} OnlyBestTimeTrialRecords; // size = 0x38

typedef struct {
    // GP Points scored for each CC mode
    // 1st place is 3 points, 2nd is 2, etc.
    // Lowest dibit is the Mushroom Cup, 2nd dibit is the Flower Cup, etc
    u8 grandPrixPoints[4];
    u8 soundMode;
} SaveInfo;

typedef struct {
    SaveInfo saveInfo;
    // It's unknown what the first byte is used for
    u8 checksum[3];
} Stuff; // size = 0x8

typedef struct {
    /* 0x000 */ AllCourseTimeTrialRecords allCourseTimeTrialRecords;
    /* 0x180 */ Stuff main;
    /**
     * For some reason there's 2 entries covering 4 cups
     * Instead of 4 entries, one per cup. Or even just one big entry for all 4 cups
     * Its also unknown why these are here when they're identical to the values found
     * in allCourseTimeTrialRecords
     **/
    /* 0x188 */ OnlyBestTimeTrialRecords onlyBestTimeTrialRecords[2];
    /**
     * If checksum[1] or checksum[2] does not match their expected value,
     * the grandPrixPoints and soundMode are reset. Then if the backup data's
     * checksums are valid, copy the backup data to the main data.
     **/
    /* 0x1F8 */ Stuff backup;
} SaveData; // size = 0x200

extern SaveData gSaveData;

#endif
