# nmea_message_parser_project
Supported NMEA sentences:GPRMC and GPROT.

Message examples:

$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A
$GPROT,35.6,A*4E

You can also add multiple messages:

$GPROT,35.6,A*4E<CR><LF>$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A
$GPROT,55.5,A*4<CR><LF>$GPROT,35.6,A*4E<CR><LF>$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A
