/*
 * temp_hum_sensor.c
 *
 * Created: 10-05-2022 11:25:03
 *  Author: Sander
 */ 

typedef struct temp_hum_sensor{
	
	EventGroupHandle_t data_ready_event_group;
	EventGroupHandle_t measure_event_group;

} temp_hum_sensor_t;



