/*
 * up_link_data_sizes.h
 *
 * Created: 01-06-2022 14:04:37
 *  Author: Sander
 */ 


#ifndef UP_LINK_DATA_SIZES_H_
#define UP_LINK_DATA_SIZES_H_

#define UP_LINK_PORT_NO 1

#define NUMBER_OF_MEASUREMENTS_TO_SEND 5

#define NUMBER_OF_BYTES_FOR_A_MEASURMENT 4

#define SIZE_OF_PAYLOAD_DATA (NUMBER_OF_MEASUREMENTS_TO_SEND*NUMBER_OF_BYTES_FOR_A_MEASURMENT)


#endif /* UP_LINK_DATA_SIZES_H_ */