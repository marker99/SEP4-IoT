/*
 * ApplicaitonConfig.h
 *
 * Created: 23-05-2022 11:02:01
 *  Author: Sander
 */ 


#ifndef APPLICAITONCONFIG_H_
#define APPLICAITONCONFIG_H_


// Define the bits used for event groups,
// when 0000 0000 0000 0000 0000 0001 is set the measuring is started
// Temp/hum sensor sets 0000 0000 0000 0000 0000 0010 when done measuring
// Co2 sets 0000 0000 0000 0000 0000 0100 when done measuring.
#define BIT_START_MEASURE (3)

#define BIT_START_TEMPHUM_MEASURE (1<<0)
#define BIT_START_CO2_MEASURE (1<<1)

#define BIT_TEMPHUM_READY_MEASURE (1<<2)
#define BIT_CO2_READY_MEASURE (1<<3)


#endif /* APPLICAITONCONFIG_H_ */